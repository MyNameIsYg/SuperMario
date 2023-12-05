#include "LiveEntity.h"

using namespace std;
using namespace cv;

EntityStatePtr CreateLivesState()
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

EntityPtr createLive()
{
	EntityPtr LiveEntity(new Entity(CreateLivesState()));
	return LiveEntity;
}


