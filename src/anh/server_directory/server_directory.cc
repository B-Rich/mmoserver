/*
 This file is part of MMOServer. For more information, visit http://swganh.com
 
 Copyright (c) 2006 - 2010 The SWG:ANH Team

 MMOServer is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 MMOServer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with MMOServer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "anh/server_directory/server_directory.h"

#include "anh/server_directory/datastore.h"

using namespace anh::server_directory;
using namespace std;

ServerDirectory::ServerDirectory(shared_ptr<DatastoreInterface> datastore, const string& cluster_name, bool create_cluster) 
    : datastore_(datastore)
    , active_cluster_(nullptr)
    , active_process_(nullptr)
{
    active_cluster_ = datastore->findClusterByName(cluster_name);
    
    if (!active_cluster_) {
        // if no cluster was found and no request to create it was made, fail now
        if (! create_cluster) {
            throw InvalidClusterError(std::string("Attempted to join an invalid cluster: ").append(cluster_name));
        }

        if (!(active_cluster_ = datastore->createCluster(cluster_name))) {
            throw InvalidClusterError(std::string("Attempt to create cluster failed: ").append(cluster_name));
        }
    }
}

Cluster ServerDirectory::cluster() const {
    return *active_cluster_;
}

Process ServerDirectory::process() const {
    return *active_process_;
}

bool ServerDirectory::registerProcess(const std::string& name, const std::string& process_type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port) {
    if (active_process_ = datastore_->createProcess(active_cluster_, name, process_type, version, address, tcp_port, udp_port)) {
        return true;
    }

    return false;
}    

bool ServerDirectory::makePrimaryProcess(const Process& process) {
    active_cluster_->primary_id(process.id());
    return true;
}

void ServerDirectory::pulse() {
    active_process_->last_pulse(datastore_->getClusterTimestamp(active_cluster_));
}
