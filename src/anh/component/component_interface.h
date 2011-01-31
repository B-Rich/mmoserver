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

#ifndef LIBANH_COMPONENT_COMPONENT_INTERFACE_H_
#define LIBANH_COMPONENT_COMPONENT_INTERFACE_H_

#include <boost/property_tree/ptree.hpp>
#include <anh/component/component_info.h>
#include <anh/event_dispatcher/basic_event.h>

namespace anh {
namespace component {

typedef	unsigned long long										ObjectId;
typedef std::shared_ptr<anh::event_dispatcher::IEvent>			Message;
typedef anh::event_dispatcher::IEvent							IMessage;
typedef anh::event_dispatcher::EventType						MessageType;
typedef anh::event_dispatcher::SimpleEvent						SimpleMessage;
typedef std::shared_ptr<anh::event_dispatcher::IEvent>			MessagePtr;

/**
 * \brief
 */
class ComponentInterface
{
public:
	/**
	 * \brief Initializes the component.
	 *
	 * \param pt Template properties passed by the ObjectBuilder.
	 */
	virtual void Init(boost::property_tree::ptree& pt) = 0;

	/**
	 * \brief Releases any resources the component holds.
	 */
	virtual void Deinit(void) = 0;

	/**
	 * \brief Called every tick if the option is enabled in the ComponentType.
	 * \see ComponentType
	 */
	virtual void Update(const float timeout) = 0;

	/**
	 * \breif Called to handle a message passed to this component by the Object Manager.
	 * Messages are derived from the event dispatchers IEvent.
	 * \see IEvent
	 *
	 * \param Message The message being passed to this component.
	 */
	virtual void HandleMessage(const Message message) = 0;

	/**
	 * \returns The type of component this is in the form of a hashed string.
	 * \see HashString
	 */
	virtual const ComponentInfo& component_info(void) = 0;

	/**
	 * \returns The Object Id of the entity that owns this component.
	 */
	virtual const ObjectId& object_id(void) = 0;

};

}  // namespace component
}  // namespace anh

#endif  // LIBANH_COMPONENT_COMPONENT_INTERFACE_H_
