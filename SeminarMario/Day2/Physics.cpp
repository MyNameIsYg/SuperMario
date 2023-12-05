#include "Physics.h"
#include <algorithm>
#include <Windows.h>

using namespace cv;
using namespace std;
/// //////////////////////////////////////////

bool checkPixelLevelCollision(IPhysicsComponent const* first, IPhysicsComponentPtr const& other)
{
	Mat const& mask_First = first->getCollisionMask();
	Mat const& mask_Other = other->getCollisionMask();
	if (mask_Other.empty() || mask_First.empty())
		return false;

	Point TL_first = first->getTL();
	Point TL_other = other->getTL();

	// ROI = Reagion of Interest
	Rect firstROI(TL_first, TL_first + Point(mask_First.size()));
	Rect othersROI(TL_other, TL_other + Point(mask_Other.size()));
	Rect intersection = firstROI & othersROI;
	if (intersection.empty())
		return false;

	Rect intersection_RelativeToMe(
		intersection.tl() - TL_first, intersection.br() - TL_first);

	Rect intersection_RelativeToOther(
		intersection.tl() - TL_other, intersection.br() - TL_other);

	Mat myMaskROI = mask_First(intersection_RelativeToMe);
	Mat othersMaskROI = mask_Other(intersection_RelativeToOther);
	Mat whereMeAndOtherIntersect = myMaskROI.mul(othersMaskROI);

	return countNonZero(whereMeAndOtherIntersect);
}

/// //////////////////////////////////////////

FixedWidgetPhysics::FixedWidgetPhysics()
	:_topLeft(0, 0), _mask()
{
}

void FixedWidgetPhysics::reset(Point const& tl)
{
	cout << "TL in FixedWidgetPhysics: " << tl << endl;
	_topLeft = tl;
}

bool FixedWidgetPhysics::update(Mat const& collisionMask)
{
	_mask = collisionMask;
	return false;
}

bool FixedWidgetPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

Point const& FixedWidgetPhysics::getTL() const
{
	return _topLeft;
}

Point const& FixedWidgetPhysics::getVelocity() const
{
	return Point(0, 0);
}

Mat const& FixedWidgetPhysics::getCollisionMask() const
{
	return _mask;
}


/////////////////////////////////////////////

ConstVelocityPhysics::ConstVelocityPhysics(Point const& velocity)
	:_velocity(velocity), _currTL(0, 0), _mask()
{
}

void ConstVelocityPhysics::reset(Point const& tl)
{
	_currTL = tl;
}

bool ConstVelocityPhysics::update(Mat const& collisionMask)
{
	_mask = collisionMask;
	_currTL.x += _velocity.x;
	_currTL.y += _velocity.y;
	return false;
}

Mat const& ConstVelocityPhysics::getCollisionMask() const
{
	// TODO: insert return statement here
	return _mask;
}

bool ConstVelocityPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

Point const& ConstVelocityPhysics::getTL() const
{
	return _currTL;
}

Point const& ConstVelocityPhysics::getVelocity() const
{
	return _velocity;
}


///////////////////////////////////////////////////////

JumpPhysics::JumpPhysics(int horizontalVelocity, int initialVerticalVelocity, int gravity, int finishHight)
	:_initialJumpVelocity(horizontalVelocity, -(initialVerticalVelocity)), _gravity(gravity)
{
	_finishJump = finishHight;
}

void JumpPhysics::reset(Point const& tl)
{
	_jumpStartTL = tl;
	_currTL = tl;
	_currVelocity = _initialJumpVelocity;
}

bool JumpPhysics::update(Mat const& collisionMask)
{
	/*_mask = collisionMask;
	_currTL += _currVelocity;
	_currVelocity.y += _gravity;
	if (_currTL.y > _jumpStartTL.y)
		_currTL.y = _jumpStartTL.y;
	return _currTL.y >= _jumpStartTL.y;*/
	_mask = collisionMask;
	_currTL += _currVelocity;
	_currVelocity.y += _gravity;
	if (_currTL.y > _finishJump) {
		_currTL.y = _finishJump;
		return true;
	}

	return false;
}

Mat const& JumpPhysics::getCollisionMask() const
{
	return _mask;
}

bool JumpPhysics::checkCollision(IPhysicsComponentPtr const& other) const
{
	return checkPixelLevelCollision(this, other);
}

Point const& JumpPhysics::getTL() const
{
	return _currTL;
}

Point const& JumpPhysics::getVelocity() const
{
	return _currVelocity;
}

///////////////////////////////////////////

NonCollidingPhysicsDecorator::NonCollidingPhysicsDecorator(IPhysicsComponentPtr base)
	:_base(base)
{
}

void NonCollidingPhysicsDecorator::reset(Point const& tl)
{
	_base->reset(tl);
}

bool NonCollidingPhysicsDecorator::update(Mat const& collisionMask)
{
	return _base->update(collisionMask);
}

Mat const& NonCollidingPhysicsDecorator::getCollisionMask() const
{
	return Mat();
}

bool NonCollidingPhysicsDecorator::checkCollision(shared_ptr<IPhysicsComponent> const& other) const
{
	return false;
}

Point const& NonCollidingPhysicsDecorator::getTL() const
{
	return _base->getTL();
}

Point const& NonCollidingPhysicsDecorator::getVelocity() const
{
	return Point(0, 0);
}

/////////////////////////////////////////////////////////

void BoundedPhysicsDecorator::test()
{
	//_currTL = _base->getTL();
	//cout << "A curr --> " << _currTL << endl;
	//cout << "A bounds --> " << _bounds << endl;
	//cout << "A base --> " << _base << endl;

	//if (_base->getTL().x > (_bounds.x + _bounds.width - _base->getCollisionMask().size().width))
	//{
	//	_currTL.x = _base->getTL().x % _bounds.width;
	//	_base->reset(Point(getTL().x % _bounds.width, getTL().y));

	//	cout << "B curr --> " << _currTL << endl;
	//	cout << "B bounds --> " << _bounds << endl;
	//	cout << "B base --> " << _base << endl;

	//	return;
	//}

	//if (_base->getTL().x <= 0)
	//{
	//	//_currTL.x = _base->getTL().x + _bounds.width;
	//	_currTL.x = _bounds.width + (_base->getCollisionMask().size().width + (-_base->getTL().x % _bounds.width));
	//	_base->reset(Point(_bounds.width, getTL().y));

	//	cout << "C curr --> " << _currTL << endl;
	//	cout << "C bounds --> " << _bounds << endl;
	//	cout << "C base --> " << _base << endl;

	//	return;
	//}
	_currTL = _base->getTL();
	//right side
	if (_base->getTL().x > _bounds.x + _bounds.width - _base->getCollisionMask().size().width)
		_currTL.x = _base->getTL().x % _bounds.width;
	//left side
	if (_base->getTL().x <= 0)
		_currTL.x = _bounds.width + _base->getTL().x % _bounds.width;
}


BoundedPhysicsDecorator::BoundedPhysicsDecorator(IPhysicsComponentPtr base)
{
	_base = base;
	Size size = Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));
	_bounds.width = size.width;
	_bounds.height = size.height;
	cout << "Ctor in BoundedPhysicsDecorator : " << _bounds << endl;
}

void BoundedPhysicsDecorator::reset(Point const& tl)
{
	_currTL = tl;
	_base->reset(tl);
}

bool BoundedPhysicsDecorator::update(Mat const& collisionMask)
{
	bool flag = _base->update(collisionMask);
	test();
	return flag;
}

Mat const& BoundedPhysicsDecorator::getCollisionMask() const
{
	return _base->getCollisionMask();
}

bool BoundedPhysicsDecorator::checkCollision(shared_ptr<IPhysicsComponent> const& other) const
{
	return _base->checkCollision(other);
}

Point const& BoundedPhysicsDecorator::getTL() const
{
	//return _base->getTL();
	return _currTL;
}

Point const& BoundedPhysicsDecorator::getVelocity() const
{
	return Point(0, 0);
}
