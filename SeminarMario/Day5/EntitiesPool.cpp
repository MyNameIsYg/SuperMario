#include "EntitiesPool.h"
#include "../Day2/SlimeEntity.h"
#include "../Day4/Config.h"

using namespace std;
using namespace cv;

EntitiesPool::EntitiesPool(string const& animationsPath) :_pool(10), _isInUse(10), _count(10) {
	for (size_t i = 0; i < _count; i++)
	{
		_pool[i] = createSlime(animationsPath);
		_isInUse[i] = false;
	}
}

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);
}

EntityPtr EntitiesPool::getNext()
{
	if (!_pool.size())
		throw std::exception("dont have entities in the pool");

	for (int i = 0; i < _isInUse.size(); i++)
	{
		if (!_isInUse[i]) {
			_isInUse[i] = true;
			return _pool[i];
		}
	}
	throw exception("all the Entities in use");

}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	for (int i = 0; i < _pool.size(); i++)
	{
		if (toFree == _pool[i]) {
			_isInUse[i] = false;
			return;
		}
	}
}

int EntitiesPool::getCount()
{
	return _count;
}

bool EntitiesPool::isFree(int idx)
{
	return _isInUse[idx];
}



void EntitiesPool::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_APPLE_ENEMY)
	{
		for (int i = 0; i < _count; i++)
		{
			if (isFree(i))
				_isInUse[i] = false;
		}
	}
}
