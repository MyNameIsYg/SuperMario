#include "ScoreEntity.h"


using namespace std;
using namespace cv;

EntityStatePtr CreateScoreState(float fontScale, int score, int fontFace)
{
	IGraphicsComponentPtr graphicsPtr(
		new ScoresGraphics(fontScale, score, fontFace));


	IPhysicsComponentPtr physicsPtr(
		make_shared<NonCollidingPhysicsDecorator>(
			make_shared<FixedWidgetPhysics>()
		)
	);

	return make_shared<EntityState>(graphicsPtr, physicsPtr);

}

EntityPtr createScore(float fontScale, int score, int fontFace)
{
	EntityPtr ScoreEntity(new Entity(CreateScoreState(fontScale, score, fontFace)));
	return ScoreEntity;
}