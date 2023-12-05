#include "Entities.h"
#include "Config.h"
#include <memory>
#include "../Day2/Graphics.h"
#include "../Day2/Physics.h"

using namespace std;
using namespace cv;

EntityState::EntityState(IGraphicsComponentPtr graphics, IPhysicsComponentPtr physics)
	:_graphicsPtr(graphics), _physicsPtr(physics)
{
}

void EntityState::update()
{
	bool graphicsFinished = _graphicsPtr->update();
	bool physicsFinished = _physicsPtr->update(_graphicsPtr->getCollisionMask());

	if (physicsFinished)
		Notify(Event{EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED});
	if (graphicsFinished) {
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION });
	}
}

void EntityState::addState(Event const& e, std::shared_ptr<EntityState> toWhere)
{
	_adjecentStates[e] = toWhere;
}

std::shared_ptr<EntityState> EntityState::tryModifyState(Event const& e) const
{
	bool isEventMapped = _adjecentStates.count(e);
	if (isEventMapped)
		return _adjecentStates.at(e);

	return nullptr;
}

IPhysicsComponentPtr const& EntityState::getPhysics() const
{
	return _physicsPtr;
}

IGraphicsComponentPtr const& EntityState::getGraphics() const
{
	return _graphicsPtr;
}

void EntityState::reset(cv::Point const& TL)
{
	_graphicsPtr->reset();
	_physicsPtr->reset(TL);
}

void EntityState::draw(Mat& canvas)
{
 	_graphicsPtr->draw(canvas, _physicsPtr->getTL());
}


////////////////////////////////////////////////////////

Entity::Entity(EntityStatePtr state)
	:_state(state)
{
}

void Entity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_TIMER
		&&
		e.code == EventCodes::TIME_TICK)
	{
		_state->update();
	}

	if (e.sender == EventSenders::SENDER_KEYBOARD
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::KEY_THROW)
	{
		cout << "this is On Notify in Entity in KEY_THROW" << endl;
		Notify(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_THROW });
	}

	auto newStateCandidate = _state->tryModifyState(e);
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_WITH_ENEMY && newStateCandidate)
	{
		int pos_x = _state->getPhysics()->getTL().x;
		int pos_y = newStateCandidate->getPhysics()->getTL().y;
		if (_state->getPhysics()->getVelocity().x < 0)
			newStateCandidate->reset(Point(pos_x + 200, pos_y));
		else
			_state->reset(Point(pos_x - 50, pos_y));
		cout << "this is On Notify in Entity in COLLISION_WITH_ENEMY" << endl;
		_state = newStateCandidate;
		_state->reset(Point(pos_x - 50, pos_y));
	}
	if (newStateCandidate)
	{
		newStateCandidate->reset(_state->getPhysics()->getTL());
		_state = newStateCandidate;
	}
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_APPLE_ENEMY)
	{
		;
	}

}

void Entity::reset(Point const& TL)
{
	_state->reset(TL);
}

void Entity::draw(Mat& canvas)
{
	_state->draw(canvas);
}
EntityStatePtr Entity::getState()
{
	return _state;
}

bool Entity::checkCollision(std::shared_ptr<Entity> slime)
{
	if (this->_state->getPhysics()->checkCollision(slime->_state->getPhysics())) {
		this->_state->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY });
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY });
	}
	return false;
}