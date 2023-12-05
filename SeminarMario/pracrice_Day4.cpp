#include <Windows.h>
#include <opencv2/opencv.hpp>
#include "Day2/SlimeEntity.h"
#include "Day4/Timer.h"
#include "Day4/HeroEntity.h"
#include <iostream>
#include "Day6/LiveEntity.h"
#include "Day6/ScoreEntity.h"
#include "Day6/LiveScoreEntity.h"
#include "Day5/EntitiesPool.h"

#include <ctime>
#include <chrono> // Include this header for high-resolution clocks



using namespace cv;
using namespace std;
using namespace chrono;


int main()
{
	Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	resize(background, background, cv::Size(GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN)));
	cout << background.size() << endl;

	//auto slimeState = createSlime(R"(../Animations/SlimeOrange)");
	//slimeState->reset(Point(background.size().width * 2 / 3, background.size().height * 5 / 5.9));

	auto slimesPool = new EntitiesPool(R"(../Animations/SlimeOrange)");
	auto slime = (slimesPool->getNext());
	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 5 / 5.9));

	EntityPtr hero = createHero(R"(../Animations/Hero)");
	hero->reset(Point(background.size().width / 2, background.size().height * 2 / 3.1));

	Frame block(R"(../Animations/wall.png)");
	//resize(block.image, block.image, Size(180, 60));
	//resize(block.mask, block.mask, Size(180, 60));
	drawFrame(block, background, Point(600, 693));

	EntityPtr heartesPtr(new LivesEntity());
	hero->Register(heartesPtr);

	EntityPtr scorePtr(new ScoresEntity(1, 1, FONT_HERSHEY_DUPLEX));
	scorePtr->reset(Point(20, 40));
	hero->Register(scorePtr);

	//EntityPtr slime(new Entity(slimeState));
	Timer timer(100);
	timer.Register(slime);
	timer.Register(hero);

	timer.Register(scorePtr);


	auto apple(new AppleEntity());
	//hero->Register(EntityPtr(apple));
	timer.Register(EntityPtr(apple));

	// ------>> 
	apple->getState()->Register((EntityPtr)apple);

	apple->Register(heartesPtr);

	const double minSpawnInterval = 2.0; // Minimum interval (in seconds) between slime spawns
	const double maxSpawnInterval = 5.0; // Maximum interval (in seconds) between slime spawns
	std::chrono::high_resolution_clock::time_point lastSpawnTime = std::chrono::high_resolution_clock::now();


	bool isToExit = false;
	while (false == isToExit)
	{
		Mat canvas = background.clone();
		timer.tick();

		hero->draw(canvas);
		//hero->checkCollision(slime);

		/*if (!apple->_isActive) {
			apple->reset(Point(hero->getState()->getPhysics()->getTL().x + 60, hero->getState()->getPhysics()->getTL().y));
		}

		if (apple->_isActive) {
			apple->getState()->getPhysics()->update(apple->getState()->getPhysics()->getCollisionMask());
			apple->draw(canvas);
		}

		for (auto i = 0; i < slimesPool->_pool.size(); i++)
		{
			if (slimesPool->_isInUse[i]) {
				slimesPool->_pool[i]->draw(canvas);

				hero->checkCollision(slimesPool->_pool[i]);
				if (apple->checkCollision(slimesPool->_pool[i]))
					slimesPool->_isInUse[i] = false;
			}
		}*/

		high_resolution_clock::time_point currentTime = high_resolution_clock::now();
		double timeSinceLastSpawn = duration<double>(currentTime - lastSpawnTime).count();
		if (slimesPool->getCount() < slimesPool->_pool.size())
		{
			// Randomly decide to spawn a slime
			if ((rand() % 100) < 10) // Adjust the probability (here 10%) as desired
			{
				auto slime = slimesPool->getNext();
				int xSpawnPoint = rand() % (background.size().width);
				slime->reset(Point(xSpawnPoint, background.size().height * 5 / 5.9));
			}
		}

		// ...

		//for (auto i = 0; i < slimesPool->_pool.size(); i++)
		//{
		//	if (slimesPool->_isInUse[i]) {
		//		slimesPool->_pool[i]->draw(canvas);

		//		hero->checkCollision(slimesPool->_pool[i]);
		//		if (apple->checkCollision(slimesPool->_pool[i]))
		//		{
		//			// Handle the case when a slime is killed by the apple
		//			slimesPool->_isInUse[i] = false;
		//			// Add code to remove the slime from the pool or handle its state as desired.
		//			// For example, you can reset its position or mark it as inactive.
		//		}
		//	}
		//}


				// Code for apple update and collision check for the new slime
		if (!apple->_isActive) {
			apple->reset(Point(hero->getState()->getPhysics()->getTL().x + 60, hero->getState()->getPhysics()->getTL().y));
		}

		if (apple->_isActive) {
			apple->getState()->getPhysics()->update(apple->getState()->getPhysics()->getCollisionMask());
			apple->draw(canvas);
		}

		/*for (auto i = 0; i < slimesPool->_pool.size(); i++)
		{
			if (slimesPool->_isInUse[i]) {
				slimesPool->_pool[i]->draw(canvas);

				hero->checkCollision(slimesPool->_pool[i]);
				if (apple->checkCollision(slimesPool->_pool[i]))
					slimesPool->_isInUse[i] = false;
			}
		}*/
		for (auto i = 0; i < slimesPool->_pool.size(); i++)
		{
			if (slimesPool->_isInUse[i]) {
				slimesPool->_pool[i]->draw(canvas);

				hero->checkCollision(slimesPool->_pool[i]);
				if (apple->checkCollision(slimesPool->_pool[i]))
				{
					// Handle the case when a slime is killed by the apple
					slimesPool->_isInUse[i] = false;
					// Reset the slime's position for re-spawning
					int xSpawnPoint = rand() % (background.size().width);
					slimesPool->_pool[i]->reset(Point(xSpawnPoint, background.size().height * 5 / 5.9));
				}
			}
		}


		scorePtr->draw(canvas);
		heartesPtr->draw(canvas);


		imshow("test", canvas);

	}



	return 0;
}

// 1.
// draws an animation of our hero that ducks in place, and stays down till you 
// press the 'w' key. (if (key == 'w'))
void duck();

// 2.
// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
void walkRight();

// 3.
// draw our hero jumps up and right, and stays standing after jump finishes.
void jumpRight();

// 4.
// write a main() that:
// when user presses 'd' - hero walks right
// when user presses 's' - hero ducks
// when user presses 'w' - hero jumps
// when user presses 'a' - hero stops (if it was walking right before)

// 5.
// in walkRight - when hero reaches the canvas boundery - don't let it go outside of image.
// force the hero x coordinate to be such that we'll see her.
// hint: you can get hero image Mat with frame.image

// 6.
// do the same with jumpRight