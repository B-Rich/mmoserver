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

#ifndef ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_
#define ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_

#include <memory>
#include <stdexcept>
#include <string>

#include "anh/server_directory/cluster.h"
#include "anh/server_directory/process.h"

namespace anh {
namespace server_directory {

class DatastoreInterface;

class InvalidClusterError : public std::runtime_error {
public:
    InvalidClusterError(const std::string& message = "") 
        : std::runtime_error(message) {}
};

class InvalidProcessError : public std::runtime_error {
public:
    InvalidProcessError(const std::string& message = "") 
        : std::runtime_error(message) {}
};

/*! \brief ServerDirectory is a utility class intended to assist processes in
* registering themselves and participating in a clustered environment.
*/
class ServerDirectory {
public:
    ServerDirectory(std::shared_ptr<DatastoreInterface> datastore, const std::string& cluster_name, bool create_cluster = false);

    Cluster cluster() const;
    Process process() const;

    bool registerProcess(const std::string& name, const std::string& process_type, const std::string& version, const std::string& address, uint16_t tcp_port, uint16_t udp_port);
    
    bool makePrimaryProcess(const Process& process);

    void pulse();

private:
    std::shared_ptr<DatastoreInterface> datastore_;
    std::shared_ptr<Cluster> active_cluster_;
    std::shared_ptr<Process> active_process_;
};

}  // namespace server_directory
}  // namespace anh

#endif  // ANH_SERVER_DIRECTORY_SERVER_DIRECTORY_H_
