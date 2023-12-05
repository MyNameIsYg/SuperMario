#include "BlockEntity.h"

EntityStatePtr CreateBlockState(std::string const& animationFolder)
{


	IGraphicsComponentPtr graphicsPtr(
		new LivesGraphics());


	IPhysicsComponentPtr physicsPtr(
		make_shared<NonCollidingPhysicsDecorator>(
			make_shared<FixedWidgetPhysics>()
		)
	);

	return make_shared<EntityState>(graphicsPtr, physicsPtr);
}

EntityPtr createBlock(std::string const& animationFolder)
{
    return EntityPtr();
}
 