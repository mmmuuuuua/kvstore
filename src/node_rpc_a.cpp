#include "node.h"

void RaftNode::send_heartbeat(std::lock_guard<std::mutex> & guard) {
    // TODO: optimize for every peer:
    // If already sent logs, don't do heartbeat this time.
    do_append_entries(guard, true);
}

void RaftNode::do_append_entries(std::lock_guard<std::mutex> & guard, bool heartbeat) {
    // Send `AppendEntriesRequest` to all peer.
    // When heartbeat is set to true, this function will NOT send logs, even if there are some updates.
    // If you believe the logs are updated and need to be notified to peers, then you must set `heartbeat` to false.
    for (auto & pp : this->peers) {
        NodePeer & peer = *pp.second;
        if(!peer.send_enabled) continue;
        if(peer.next_index <= get_base_index() && get_base_index() > 0){
            // If lag behind too much
            do_send_install_snapshot(guard, peer);
        }else{
            // We copy log entries to peer, from `prev_log_index`.
            // This may fail when "an existing entry conflicts with a new one (same index but different terms)"
            IndexID prev_log_index = peer.next_index - 1;
            // What if entry prev_log_index not exist? 
            // We add rules in `on_append_entries_response`, now peer.next_index can't be set GT last_log_index() + 1
            TermID prev_log_term = prev_log_index > default_index_cursor ? gl(prev_log_index).term() : default_term_cursor;
            raft_messages::AppendEntriesRequest request;
            request.set_name(name);
            request.set_term(current_term);
            request.set_prev_log_index(prev_log_index);
            request.set_prev_log_term(prev_log_term);
            request.set_leader_commit(commit_index);
            request.set_time(get_current_ms());
            // Add entries to request
            // NOTICE Even this is a heartbeat RPC, we still need to check entries.
            // Because some nodes may suffer from network failure etc., 
            // and fail to update their log when we firstly sent entries in `do_log`.
            if(peer.next_index < last_log_index() + 1){
                if(heartbeat){}else
                debug_node("Copy to peer %s LogEntries[%lld, %lld]\n", peer.name.c_str(), peer.next_index, last_log_index());
            }else{
                if(heartbeat){}else
                debug_node("Peer %s next_index = %lld, me last %lld, No Append.\n", peer.name.c_str(), peer.next_index, last_log_index());
            }
            // TODO Handle when logs is compacted
            assert(peer.next_index >= get_base_index());
            for (IndexID i = std::max((IndexID)0, peer.next_index); i <= last_log_index() ; i++) {
                raft_messages::LogEntry & entry = *(request.add_entries());
                entry = gl(i);
            }

            set_seq_nr(peer, request);

            #if defined(_HIDE_HEARTBEAT_NOTICE)
            if(heartbeat){}else
            #endif
            debug_node("Send %s AppendEntriesRequest to %s, size %u, term %llu, seq %llu.\n", 
                    heartbeat ? "heartbeat": "normal", peer.name.c_str(), request.entries_size(),
                    current_term, request.seq());
            peer.raft_message_client->AsyncAppendEntries(request, heartbeat);
        }
    }
}

int RaftNode::on_append_entries_request(raft_messages::AppendEntriesResponse * response_ptr, const raft_messages::AppendEntriesRequest & request) {
    GUARD
    std::string peer_name = request.name();
    if((!is_running(guard)) || (!is_peer_receive_enabled(guard, peer_name))){
        #if !defined(_HIDE_NOEMPTY_REPEATED_APPENDENTRY_REQUEST) && !defined(_HIDE_PAUSED_NODE_NOTICE)
        #if defined(_HIDE_HEARTBEAT_NOTICE)
        if(request.entries().size() == 0){}else
        #endif
        debug_node("Ignore AppendEntriesRequest from %s, I state %d paused %d, Peer running %d.\n", request.name().c_str(), 
                state, paused, is_peer_receive_enabled(guard, peer_name));
        #endif
        return -1;
    }

    if((uint64_t)request.time() < (uint64_t)start_timepoint){
        debug_node("Out-dated AppendEntriesRequest\n");
        return -1;
    }
    if(!Nuke::contains(peers, peer_name)){
        debug_node("Ignore AppendEntriesRequest from %s. I don't have this peer.\n", request.name().c_str());
        return -1;
    }
    raft_messages::AppendEntriesResponse & response = *response_ptr;
    response.set_name(name);
    response.set_success(false);
    response.set_seq(request.seq());
    IndexID prev_i = request.prev_log_index(), prev_j = 0;

    if(handle_request_routine(guard, request)){
        response.set_term(current_term);
    }else{
        goto end;
    }

    if(!valid_seq(request.seq(), request.initial())){
        debug_node("Out-of-ordered AppendEntriesRequest from %s last_seq %llu, request.seq() = %llu\n", request.name().c_str(), last_seq, request.seq());
        return -1;
    }

    if(request.prev_log_index() >= 0 && request.prev_log_index() > last_log_index()) {
        // I don't have the `prev_log_index()` entry.
        debug_node("AppendEntries fail. I don't have prev_log_index = %lld, my last_log_index = %lld.\n", request.prev_log_index(), last_log_index());
        goto end;
    }
    if(request.prev_log_index() >= 0 && request.prev_log_index() < last_log_index()){

    }
    if(request.prev_log_index() == default_index_cursor){
        // If Leader has no entry, then we definitely not match.
        goto NO_CONFLICT;
    }
    if(request.prev_log_index() > get_base_index()){
        TermID local_term = gl(request.prev_log_index()).term();
        if(request.prev_log_term() != local_term){
            // If my last logs term mismatch the Leader's. Erase to maintain Log Matching Principle.
            for(IndexID i = request.prev_log_index() - 1; i >= default_index_cursor; i--){
                if(i == default_index_cursor){
                    debug_node("Revoke last_log_index to %lld, remove all wrong entries of term %llu.\n", default_index_cursor, local_term);
                    response.set_last_log_index(default_index_cursor);
                    response.set_last_log_term(default_term_cursor);
                    // NOTICE We need to IMMEDIATELY remove! See seq.concurrent.log !!!
                    // Otherwise, `commit_index` can later be advanced to a wrong index, which should have already be erased.
                    logs.erase(logs.begin(), logs.end());
                } else if(gl(i).term() != local_term){
                    // Now we found non conflict entries.
                    debug_node("Revoke last_log_index to %lld, remove all wrong entries of term %llu.\n", i, local_term);
                    response.set_last_log_index(i);
                    response.set_last_log_term(gl(i).term());
                    // NOTICE We need to IMMEDIATELY remove! See seq.concurrent.log !!!
                    logs.erase(logs.begin() + i + 1 - get_base_index(), logs.end());
		            break; 
                }
                // Otherwise continue loop
            }
            debug_node("Revoke last_log_index finished, request.prev_log_index() = %lld, request.prev_log_term() = %llu, response.prev_log_index() = %lld, response.prev_log_term() = %llu, wrong_term %llu, request.seq() %llu.\n", 
                request.prev_log_index(), request.prev_log_term(), response.last_log_index(), response.last_log_term(), local_term, request.seq());
            // Term mismatch, can't move forward.
            goto end2;
        }
    }
NO_CONFLICT:
    if(request.prev_log_index() >= get_base_index()){

        #ifdef USE_MORE_REMOVE
        if(request.prev_log_index() + 1 <= last_log_index()){  ///考虑写成request.prev_log_index()<last_log_index()更直观一些
            IndexID overlap_length = last_log_index() - request.prev_log_index();
            debug_node("Erase [%lld, ). Before, last_log_index %lld, on Leader's side, prev_log_index %lld. logs = %s, overlap_length %lld\n", 
                request.prev_log_index() + 1, last_log_index(), request.prev_log_index(), print_logs().c_str(), overlap_length);
            logs.erase(logs.begin() + request.prev_log_index() + 1 - get_base_index(), logs.end());
            debug_node("Insert size %u\n", request.entries_size());
        }

        logs.insert(logs.end(), request.entries().begin(), request.entries().end());

        #else
        IndexID local_logs_index_start = find_entry(request.prev_log_index(), request.prev_log_term());
        int leader_logs_index = 0;
        IndexID local_logs_index = local_logs_index_start + 1;

        if(local_logs_index_start == default_index_cursor){
            logs.insert(logs.end(), request.entries().begin() + leader_logs_index, request.entries().end());
        }
        else{
            for(; local_logs_index < logs.size() && leader_logs_index < request.entries_size(); local_logs_index++, leader_logs_index++){
                if(logs[local_logs_index].term() != request.entries(leader_logs_index).term()){
                    break;
                }
            }
            // Find smallest dismatching pair (local_logs_index, leader_logs_index).
            if(local_logs_index < logs.size() && leader_logs_index < request.entries_size()){
                // If there is conflict.
                debug_node("Erase [%lld, ). Before, last_log_index %lld, on Leader's side, prev_log_index %lld. logs = %s\n", 
                    logs[local_logs_index].index(), last_log_index(), request.prev_log_index(), print_logs().c_str());
                logs.erase(logs.begin() + local_logs_index, logs.end());
            }else if(local_logs_index < logs.size()){
                // If leader's log has fewer entries.
                // Remove extra entries appended by prev leader.
                for(; local_logs_index < logs.size(); local_logs_index++){
                    if(logs[local_logs_index].term() != request.term()){
                        logs.erase(logs.begin() + local_logs_index, logs.end());
                        break;
                    }
                }
            }else if(leader_logs_index < request.entries_size()){
                // If local log has fewer entries.
            }
            if(leader_logs_index < request.entries_size()){
                // debug_node("Insert from %d to %d\n", leader_logs_index, request.entries_size());
                logs.insert(logs.end(), request.entries().begin() + leader_logs_index, request.entries().end());
            }else{
                // debug_node("Can't Insert leader_logs_index = %d, entries.size() = %d, local_logs_index = %d, logs.size() = %d, logs = %s\n", 
                    // leader_logs_index, request.entries_size(), local_logs_index, logs.size(), print_logs().c_str());
            }
        }
        #endif
    }

    if (request.leader_commit() > commit_index) {
        IndexID old_commit_index = commit_index;
        commit_index = std::min(request.leader_commit(), last_log_index());
        debug_node("Leader %s ask me to advance commit_index from %lld to %lld, seq %llu, request.prev_log_index() %lld, last_log_index() = %lld.\n", 
            request.name().c_str(), old_commit_index, commit_index, request.seq(), request.prev_log_index(), last_log_index());
        do_apply(guard);
    }else{
        if(request.entries_size()){
            debug_node("My commit_index remain %lld, because it's GE than Leader's commit %lld, entries_size = %u. last_log_index = %lld, seq %llu\n", 
                commit_index, request.leader_commit(), request.entries_size(), last_log_index(), last_seq);
        }
    }

    // std::atomic_signal_fence(std::memory_order_acq_rel);
    // When received Configuration entry 
    for(IndexID i = last_log_index(); i >= std::max((IndexID)0, prev_i) && i >= get_base_index(); i--){
        if(gl(i).command() == NUFT_CMD_TRANS){
            on_update_configuration_joint(guard, gl(i));
            break;
        } else if(gl(i).command() == NUFT_CMD_TRANS_NEW){
            on_update_configuration_new(guard, gl(i));
        }
    }
    // When Configuration entry is committed
    if(trans_conf){
        if(trans_conf->state == Configuration::State::OLD_JOINT && trans_conf->index <= commit_index){
            trans_conf->state == Configuration::State::JOINT;
            persister->Dump(guard);
            debug_node("Advance index to %lld >= trans_conf->index %lld, Joint consensus committed.\n", commit_index, trans_conf->index);
        } else if(trans_conf->state == Configuration::State::JOINT_NEW && trans_conf->index2 <= commit_index){
            on_update_configuration_finish(guard);
        }
    }
succeed:
    response.set_success(true);
end:
    // Must set term here, because `become_follower` may change it
    response.set_term(current_term);
    response.set_last_log_index(last_log_index());
    response.set_last_log_term(last_log_term());
end2:
    response.set_time(get_current_ms());
    if(persister) persister->Dump(guard);
    return 0;
}

void RaftNode::on_append_entries_response(const raft_messages::AppendEntriesResponse & response, bool heartbeat) {
    // When Leader receive `AppendEntriesResponse` from others
    
    GUARD
    
    IndexID new_commit;
    if((uint64_t)response.time() < (uint64_t)start_timepoint){
        return;
    }
    // debug_node("Receive response from %s\n", response.name().c_str());
    if (state != NodeState::Leader) {
        return;
    }

    if (response.term() > current_term) {
        debug_node("Become Follower because find newer term %llu from Peer %s.\n", response.term(), response.name().c_str());
        become_follower(guard, response.term());
        leader_name = "";
        if(persister) persister->Dump(guard);
    } else if (response.term() != current_term) {
        // Reject. Invalid term
        return;
    }        
    if(!Nuke::contains(peers, response.name())){
        // From a removed peer(by conf trans)
        return;
    }

    NodePeer & peer = *(peers[response.name()]);

    if (!response.success()) {
        // A failed `AppendEntriesRequest` can certainly not lead to `commit_index` updating
        peer.next_index = response.last_log_index() + 1;
        peer.match_index = response.last_log_index();
        debug_node("Peer %s returns AppendEntriesResponse: FAILED. Switch next_index to %lld, match_index to %lld, response.seq() = %llu\n", 
            response.name().c_str(), peer.next_index, peer.match_index, response.seq());
        return;
    }

    // NOW WE SEE IF WE CAN COMMIT
    bool finish_staging_flag = false;
    bool committed_joint_flag = false;
    bool committed_new_flag = false;
    if(!peer.voting_rights){
        if(response.last_log_index() == last_log_index()){
            // Now this node has catched up with me.
            debug_node("Node %s has now catched up with me, grant right for vote.\n", response.name().c_str());
            peer.voting_rights = true;
        }
        // If there are no staging nodes, staging finished.
        if(std::find_if(peers.begin(), peers.end(), [](auto & pp){
            NodePeer & peer2 = *pp.second;
            return peer2.voting_rights == false;
        }) == peers.end()){
            finish_staging_flag = true;
        }
    }

    if(response.last_log_index() > last_log_index()){
        peer.next_index = last_log_index() + 1;
        peer.match_index = last_log_index();
        goto CANT_COMMIT;
    }else{
        peer.next_index = response.last_log_index() + 1;
        peer.match_index = response.last_log_index();
    }
    
    // Until now peer.next_index is valid.
    new_commit = response.last_log_index();

    // Update commit
    if (commit_index >= new_commit) {
        goto CANT_COMMIT;
    }

    if(gl(response.last_log_index()).term() != response.last_log_term()){
        // This may possibly happen. see the Raft Paper Chapter 5.4.2.
        debug_node("Peer(%s) have conflict log at last index. My (index %lld, term %llu), Peer (index %lld, term %llu) = %lld.\n",
                response.name().c_str(), response.last_log_index(), gl(response.last_log_index()).term(), response.last_log_index(), response.last_log_term());
    }
    if (current_term != gl(response.last_log_index()).term()) {
        debug_node("Peer(%s) can't commit logs replicated by previous Leaders. Term disagree at commit_index(%lld), (me:%llu, peer:%llu). \n", 
                    response.name().c_str(), commit_index, current_term, gl(response.last_log_index()).term());
        goto CANT_COMMIT;
    }
    
    // Now we can try to advance `commit_index` to `new_commit = response.last_log_index()` by voting.
    if(trans_conf){
        int ans = enough_votes_trans_conf([&](const NodePeer & peer){
            return peer.match_index >= new_commit;
        });
        if(ans > 0){
            if(trans_conf && trans_conf->state == Configuration::State::JOINT_NEW && new_commit >= trans_conf->index2){
                debug_node("Advance commit_index from %lld to %lld. New configuration committed with support new %u(req %u)\n", 
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres);
                commit_index = new_commit;
                trans_conf->state = Configuration::State::NEW;
                committed_new_flag = true;
            }else if (trans_conf->state == Configuration::State::OLD_JOINT && new_commit >= trans_conf->index){
                debug_node("Advance commit_index from %lld to %lld. Joint consensus committed with support new %u(req %u), old %u(req %u)\n", 
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres, old_vote, trans_conf->oldvote_thres);
                commit_index = new_commit;
                trans_conf->state = Configuration::State::JOINT;
                // Some work must be done when we unlock the mutex at the end of the function. We mark here.
                committed_joint_flag = true;
            }else{
                commit_index = new_commit;
                debug_node("Advance commit_index from %lld to %lld. With support new %u(req %u), old %u(req %u)\n", 
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres, old_vote, trans_conf->oldvote_thres);
            }
        }else if(ans < 0){
            if(trans_conf && trans_conf->state == Configuration::State::JOINT_NEW && new_commit >= trans_conf->index2){
                debug_node("CAN'T advance commit_index from %lld to %lld. New configuration CAN'T committed with support new %u(req %u)\n",
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres);
            }else if (trans_conf->state == Configuration::State::OLD_JOINT && new_commit >= trans_conf->index){
                debug_node("CAN'T advance commit_index from %lld to %lld. Joint consensus CAN'T committed with support new %u(req %u), old %u(req %u)\n", 
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres, old_vote, trans_conf->oldvote_thres);
            }else{
                debug_node("CAN'T advance commit_index from %lld to %lld. With support new %u(req %u), old %u(req %u)\n", 
                        commit_index, new_commit, new_vote, trans_conf->newvote_thres, old_vote, trans_conf->oldvote_thres);
            }
        }else{
            goto NORMAL_TEST_COMMIT;
        }
    }
    else{
NORMAL_TEST_COMMIT:
        size_t commit_vote = 1; // This one is from myself.
        std::string mstr;
        for (auto & pp : peers) {
            NodePeer & peer2 = *pp.second;
            if (peer2.voting_rights && peer2.match_index >= new_commit) {
                mstr += (peer2.name + ":" + std::to_string(peer2.match_index) + ";");
                commit_vote++;
            }
        }
        if (enough_votes(commit_vote)) {
            // if(!heartbeat) 
            debug_node("Advance commit_index from %lld to %lld with vote %d. match_index {%s}.\n", commit_index, new_commit, commit_vote, mstr.c_str());

            commit_index = new_commit;
            do_apply(guard);
        }else{
            //if(!heartbeat) 
                debug_node("Can't advance commit_index to %lld because of inadequate votes of %u.\n", new_commit, commit_vote);
        }
    }
    
CANT_COMMIT:
    if(finish_staging_flag){
        // Requires lock.
        update_configuration_joint(guard);
    }
    
    if(trans_conf){
        if(committed_joint_flag){
            on_update_configuration_joint_committed(guard);
        }
        if(committed_new_flag){
            update_configuration_finish(guard);
        }
    }
}

NuftResult RaftNode::do_log(std::lock_guard<std::mutex> & guard, ::raft_messages::LogEntry entry, std::function<void(RaftNode*)> f, int command){
    if(state != NodeState::Leader){
        // No, I am not the Leader, so please find the Leader first of all.
        debug_node("Not Leader!!!!!\n");
        return -NUFT_NOT_LEADER;
    }
    if(!is_running(guard)){
        debug_node("Not Running!!!!\n");
        return -NUFT_FAIL;
    }
    IndexID index = last_log_index() + 1;
    entry.set_term(this->current_term);
    entry.set_index(index);
    entry.set_command(command);
    logs.push_back(entry);
    f(this);
    if(persister) persister->Dump(guard);
    
    debug_node("Append LOCAL log Index %lld, Term %llu, commit_index %lld. Now copy to peers.\n", index, current_term, commit_index);
    // "The leader appends the command to its log as a new entry, then issues AppendEntries RPCs in parallel..."
    do_append_entries(guard, false);
    return index;
}


/* 
NuftResult RaftNode::do_data_log(std::lock_guard<std::mutex> & guard, ::raft_messages::LogEntry entry,std::string key,std::string value){
    if(state != NodeState::Leader){
        // No, I am not the Leader, so please find the Leader first of all.
        debug_node("Not Leader!!!!!\n");
        return -NUFT_NOT_LEADER;
    }
    if(!is_running(guard)){
        debug_node("Not Running!!!!\n");
        return -NUFT_FAIL;
    }
    IndexID index = last_log_index() + 1;
    entry.set_term(this->current_term);
    entry.set_index(index);
    entry.set_key(key);
    entry.set_value(value);
    logs.push_back(entry);

    if(persister) persister->Dump(guard);
    
    debug_node("Append LOCAL log Index %lld, Term %llu, commit_index %lld. Now copy to peers.\n", index, current_term, commit_index);
    // "The leader appends the command to its log as a new entry, then issues AppendEntries RPCs in parallel..."
    do_append_entries(guard, false);
    return index;
}*/