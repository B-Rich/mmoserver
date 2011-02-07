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

#include "login/login_server.h"

using namespace anh::event_dispatcher;
using namespace std;

using login::LoginServer;

LoginServer::LoginServer(std::shared_ptr<anh::event_dispatcher::EventDispatcherInterface> event_dispatcher) 
    : event_dispatcher_(event_dispatcher) 
{
    event_dispatcher_->subscribe(
        "LoginClientId", 
        bind(&LoginServer::handleLoginClientId, this, std::placeholders::_1));

    event_dispatcher_->subscribe(
        "DeleteCharacterMessage",
        bind(&LoginServer::handleLoginClientId, this, std::placeholders::_1));
}

bool LoginServer::handleLoginClientId(shared_ptr<EventInterface> incoming_event) {
    return false;
}

bool LoginServer::handleDeleteCharacterMessage(shared_ptr<EventInterface> incoming_event) {
    return false;
}
