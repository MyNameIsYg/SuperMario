#include "SlimeEntity.h"

using namespace cv;
using namespace std;

EntityStatePtr CreateSlimeEnemy(string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));

	animation->resizeImg();
	
	//resize(animation->getFrame())
	//resize(animation, animation, Size(40, 30));
	//resize(_singleLife.mask, _singleLife.mask, Size(40, 30));
	bool isCyclic = true;

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, isCyclic));

	IPhysicsComponentPtr nonCollidingPhysicsPtr(
			make_shared<FixedWidgetPhysics>()
	);

	return make_shared<EntityState>(graphicsPtr, nonCollidingPhysicsPtr);

}

EntityPtr createSlime(string const& animationFolder)
{
	//auto slime = CreateSlimeEnemy(animationFolder);
	EntityPtr slimeEntity(new Entity(CreateSlimeEnemy(animationFolder)));
	return slimeEntity;
}
