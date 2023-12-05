#include "HeroEntity.h"
#include "opencv2/opencv.hpp"
#include "Config.h"

#include <memory>
#include <filesystem>
#include "../Day1/Animation.h"
#include "../Day2/Physics.h"
#include "../Day2/Graphics.h"

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

enum HeroStates {
	HERO_IDLE,
	HERO_RUN_RIGHT,
	HERO_RUN_LEFT,
	HERO_DUCK, 
	HERO_STAY_DUCK, 
	HERO_STAND_AFTER_DUCK, 
	HERO_JUMP,
	HERO_THROW
};

EntityStatePtr createHeroState(fs::path const& animationFolder,HeroStates state)
{
	AnimationPtr animation(new Animation(animationFolder.string()));
	bool isNotCyclic =
		state == HeroStates::HERO_DUCK ||
		state == HeroStates::HERO_STAND_AFTER_DUCK ||
		state == HeroStates::HERO_JUMP;
	bool isCyclic = !isNotCyclic;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr physicsPtr = nullptr;
	Size frameSize = graphicsPtr->getCollisionMask().size();
	switch (state)
	{
	case HERO_IDLE:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_RUN_RIGHT:
		physicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<ConstVelocityPhysics>(Point(frameSize.width / 10, 0)));
		//physicsPtr.reset(new ConstVelocityPhysics(Point(frameSize.width / 10, 0)));
		break;
	case HERO_RUN_LEFT:
		physicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<ConstVelocityPhysics>(Point(-(frameSize.width / 10), 0)));
		//physicsPtr.reset(new ConstVelocityPhysics(Point(-(frameSize.width / 10), 0)));
		break;
	case HERO_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAY_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_STAND_AFTER_DUCK:
		physicsPtr.reset(new FixedWidgetPhysics());
		break;
	case HERO_JUMP:
		physicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<JumpPhysics>(18, 50, 10, 454));
		//physicsPtr.reset(new JumpPhysics(5,50,10));
		break;
	case HERO_THROW:
		//physicsPtr = make_shared<BoundedPhysicsDecorator>(make_shared<JumpPhysics>(18, 50, 10));
		//physicsPtr.reset(new JumpPhysics(5,50,10));
		break;
	default:
		throw exception("Unknown physics state!");
	}

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createHero(std::string const & rootAnimationsFolder)
{
	fs::path root = rootAnimationsFolder;
	auto idleFolder = root / "idle";

	auto idle = createHeroState(root / "idle", HeroStates::HERO_IDLE);
	auto runRight = createHeroState(root / "runRight", HeroStates::HERO_RUN_RIGHT);
	auto runLeft = createHeroState(root / "runLeft", HeroStates::HERO_RUN_LEFT);
	auto jump = createHeroState(root / "jump", HeroStates::HERO_JUMP);
	auto duck = createHeroState(root / "duckDown", HeroStates::HERO_DUCK);
	auto duckStay = createHeroState(root / "duckStay", HeroStates::HERO_STAY_DUCK);
	auto standAfterDuck = createHeroState(root / "standAfterDuck", HeroStates::HERO_STAND_AFTER_DUCK);


	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, runRight);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, runLeft);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	idle->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);

	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_LEFT }, idle);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, jump);
	runRight->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_DOWN }, duck);
	runRight->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);

	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::ENTITY_PHYSICS_FINISHED }, idle);
	jump->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);

	runLeft->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_RIGHT }, idle);
	runLeft->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_PHYSICS, EventCodes::COLLISION_WITH_ENEMY }, idle);

	duck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, duckStay);
	duckStay->addState(Event{ EventSenders::SENDER_KEYBOARD, EventTypes::EVENT_KEY_PRESSED, EventCodes::KEY_UP }, standAfterDuck);
	standAfterDuck->addState(Event{ EventSenders::SENDER_ENTITY_STATE, EventTypes::EVENT_GRAPHICS, EventCodes::ENTITY_FINISHED_ANIMATION }, idle);

	
	EntityPtr hero(new Entity(idle));

	idle->Register(hero);
	runRight->Register(hero);
	jump->Register(hero);
	runLeft->Register(hero);
	duckStay->Register(hero);
	duck->Register(hero);
	standAfterDuck->Register(hero);

	//hero->Register(heartes);



	return hero;
}