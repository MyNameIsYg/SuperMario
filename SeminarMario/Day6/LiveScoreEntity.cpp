#include <Windows.h>
#include "LiveScoreEntity.h"
#include "../Day4/Config.h"
#include "LiveEntity.h"
#include "ScoreEntity.h"
#include "AppleEntity.h"
#include "../Day5/EntitiesPool.h"



using namespace cv;
using namespace std;

LivesEntity::LivesEntity() : Entity(CreateLivesState())
{

}


void LivesEntity::onNotify(Event const& e)
{
	auto newStateCandidate = _state->tryModifyState(e);
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_WITH_ENEMY)
	{
		int pos_x = _state->getPhysics()->getTL().x;
		int pos_y = _state->getPhysics()->getTL().y;
		this->reset(Point(pos_x - 200, pos_y));
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
		this->getState()->getGraphics()->reset(1);
	}
}

void LivesEntity::reset(Point const& TL)
{
	_state->reset(TL);
}

bool LivesEntity::checkCollision(shared_ptr<Entity> slime) {
	return false;
}

void LivesEntity::draw(Mat& canvas)
{
	_state->draw(canvas);
}

ScoresEntity::ScoresEntity(float fontScale, int score, int fontFace) :
	Entity(CreateScoreState(fontScale, score, fontFace))
{
}

void ScoresEntity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_WITH_ENEMY) {

		_state->getGraphics()->reset();
	}
	if (e.sender == EventSenders::SENDER_TIMER
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::COLLISION_WITH_FOOD_SCORE) {

		_state->getGraphics()->reset(1);
	}
}

void ScoresEntity::reset(Point const& TL)
{
	_state->reset(TL);
}

bool ScoresEntity::checkCollision(shared_ptr<Entity> slime)
{
	return false;
}

void ScoresEntity::draw(Mat& canvas)
{
	_state->draw(canvas);
}

AppleEntity::AppleEntity() :
	Entity(CreateAppleState(R"(../Animations/Apple)")), _isActive(false)
{
	_size = Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
}

void AppleEntity::onNotify(Event const& e)
{
	if (e.sender == EventSenders::SENDER_KEYBOARD
		&&
		e.type == EventTypes::EVENT_KEY_PRESSED
		&&
		e.code == EventCodes::KEY_THROW)
	{
		_isActive = true;
		cout << "this is On Notify in AppleEntity KEY_THROW" << endl;
		_state->getPhysics()->getTL();

	}
	if ((e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::APPLE_OUT_RANGE)
		||
		(e.sender == EventSenders::SENDER_ENTITY_STATE
		&&
		e.type == EventTypes::EVENT_PHYSICS
		&&
		e.code == EventCodes::COLLISION_APPLE_ENEMY))
	{
		cout << "this is On Notify in AppleEntity ENTITY_PHYSICS_FINISHED" << endl;
		_isActive = false;
	}
}

void AppleEntity::reset(Point const& TL)
{
	Entity::reset(TL);
}

bool AppleEntity::checkCollision(shared_ptr<Entity> slime)
{
	if (this->_state->getPhysics()->checkCollision(slime->getState()->getPhysics())) {
		cout << "this is check Collision in Apple Entity" << endl;
		_state->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_APPLE_ENEMY });
		Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_APPLE_ENEMY });
		return true;
	}
	cout << "This is GetTL of _state in Apple: " << this->_state->getPhysics()->getTL() << endl;
	cout << "This is _Size of scrren in Apple : " << _size << endl;
	if (this->_state->getPhysics()->getTL().y >= _size.height - 30) {
		_state->Notify(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::APPLE_OUT_RANGE });
	}
	return false;

}

void AppleEntity::draw(Mat& canvas)
{
	_state->draw(canvas);
}
