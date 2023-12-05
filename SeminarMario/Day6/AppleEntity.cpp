#include "../Day4/Entities.h"
#include "AppleEntity.h"


using namespace cv;
using namespace std;

EntityStatePtr CreateAppleState(string const& animationFolder)
{
	AnimationPtr animation(new Animation(animationFolder));
	animation->resizeImg();

	IGraphicsComponentPtr graphicsPtr(
		new SingleAnimationGraphics(animation, false));


	/*Frame animation(animationFolder);

	IGraphicsComponentPtr graphicsPtr(
		new AppleGraphics(animation));*/

	IPhysicsComponentPtr physicsPtr(
		make_shared<BoundedPhysicsDecorator>(
			make_shared<JumpPhysics>(15, -10, 6, 690))
	);



	auto s = make_shared<EntityState>(graphicsPtr, physicsPtr);
	int count = animation->numFrames();
	for (int i = 0; i < count; i++)
	{
		s->getPhysics()->update(animation->getFrame(i).mask);
	}
	
	// 
	/*EntityStatePtr s = make_shared<EntityState>(graphicsPtr, physicsPtr);
	s->getPhysics()->update(Frame(R"(../Animations/Apple)").mask);*/
	return s;

}

EntityPtr createApple(string const& animationFolder)
{
	EntityPtr AppleEntity(new Entity(CreateAppleState(animationFolder)));
	return AppleEntity;
}

EntityStatePtr CreateAppleState()
{
	return EntityStatePtr();
}
