#pragma once
#include "Common/MessageDispatchCallback.h"
#include "ObjectFactory.h"
#include "WorldManager.h"
#include "ZoneOpcodes.h"
#include "Item.h"
#include "PlayerObject.h"
#include <vector>


#define gScoutManager ScoutManager::getSingletonPtr()

class Message;
class Database;
class MessageDispatch;
class PlayerObject;
class ObjectControllerCommandMap;

class ScoutManager
{
public:
	static ScoutManager*	getSingletonPtr() { return mSingleton; }
	
	static ScoutManager*	Instance()
	{
		if (!mSingleton)
		{
			mSingleton = new ScoutManager();
		}
		return mSingleton;
	}

	static inline void deleteManager(void)    
	{ 
		if (mSingleton)
		{
			delete mSingleton;
			mSingleton = 0;
		}
	}


	void createCamp(uint32 typeId,uint64 parentId,Anh_Math::Vector3 position,string customName, PlayerObject* player);

protected:
	~ScoutManager(void);

private:
	static ScoutManager*	mSingleton;
	
	ScoutManager(){}
	
};