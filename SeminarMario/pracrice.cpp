//#include <opencv2/opencv.hpp>
//#include "Day1/Animation.h"
//#include "Day2/SlimeEntity.h"
//
//
//using namespace cv;
//
////4 state to the girl
//void walkRight(int);
//void duck(int);
//void standAfterDuck(int);
//void jumpRight(int);
//
////show the img
//bool showAnimation(Animation, bool);
//
////show the background
//Mat background = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
//
//// before you start: open SeminarMario project properties, go to 
//// Debugging -> Environment
//// set the following value:
//// PATH=$(PATH);../OpenCV_451/bin
//// hit Ctrl + F5 and see a walking lady. that's our hero!
//// press Esc to exit.
//// read carefully the comments below - it contains explanations and instructions.
//// and do the exercises.
//
//
//Point walkRight(Point topLeft)
//{
//	topLeft.x += 10;
//	return topLeft;
//}
//int main()
//{
//	// Animation is a class we wrote for saving you the handling with the 
//	// details of image processing and focusing on code design. iyH we'll get 
//	// to it in the image processing course.
//	Animation animation(R"(../Animations/Hero/idle)");
//
//	// OpenCV is a popular image processing and computer vision library.
//	// it is not part of the C++ language, but it's written in C++.
//	// in this project we integrated it for you.
//	// The class that represents an image in OpenCV is Mat (i.e. Matrix),
//	// and it's defined inside namespace cv. So the full name is cv::Mat.
//
//	//size to the screen
//	resize(background, background, Size(1280, 720));
//
//	//slime
//	auto slime = CreateSlimeEnemy(R"(../Animations/SlimeOrange)");
//	slime->reset(Point(background.size().width * 2 / 3, background.size().height * 4 / 5));
//
//	//life & score
//	auto score = ScoresGraphics(1, 150, FONT_HERSHEY_DUPLEX);
//	score.draw(background, Point(50, 50));
//	auto heartes = LivesGraphics();
//	heartes.draw(background, Point(1200, 20));
//
//
//	// Draw
//	bool isToExit = false;
//	while (false == isToExit)
//	{
//
//		isToExit = showAnimation(animation, false);
//
//		for (int i = 0; i < animation.numFrames(); i++)
//		{
//			Frame const& frame = animation.getFrame(i);
//			// create a copy of the original backgrount - not to "dirty" it.
//			Mat canvas = background.clone();
//			// the point on the canvas where we would like to place the top left corner 
//			// of our frame. NOTE: the y axis on the screen goes from top to bottom.
//			Point topLeft(canvas.size().width / 2, canvas.size().height * 2 / 3.1);
//			drawFrame(frame, canvas, topLeft);
//
//
//			// @2: slime is an "EntityState" and knows how to take care of itself:
//			slime->update();
//			slime->draw(canvas);
//
//
//			// show what we've got (use BOTH of the following two commands):
//			imshow("test", canvas);
//			// wait 100 milliseconds for a key to be pressed. if not pressed, returns -1:
//			// (you MUST call it after imshow. we'll discuss why in image processing).
//			int key = waitKey(100);
//		}
//		int key = waitKey(100);
//		if (key == 27) // if you pressed ESC key
//		{
//			isToExit = true;
//		}
//
//
//	}
//
//	return 0;
//}
//
////to walk
//void walkRight(int key) {
//	Animation animationWalk(R"(../Animations/Hero/runRight)");
//
//	bool isToExit = false;
//	while (false == isToExit)
//	{
//		isToExit = showAnimation(animationWalk, false);
//
//		int key = waitKey(100);
//		if (key == 27) // if you pressed ESC key
//		{
//			isToExit = true;
//		}
//
//
//	}
//}
//
//
//
//// 1.
//// draws an animation of our hero that ducks in place, and stays down till you 
//// press the 'w' key. (if (key == 'w'))
//
////to duck
//void duck(int key) {
//
//	Animation animationDuck(R"(../Animations/Hero/duckDown)");
//
//	bool isExit = showAnimation(animationDuck, false);
//	key = waitKey(100);
//	bool isToExit = false;
//	while (false == isToExit)
//	{
//		key = waitKey(100);
//		if (key == 'w')
//		{
//			isToExit = true;
//		}
//	}
//	standAfterDuck(key);
//}
//
////to stand
//void standAfterDuck(int key) {
//	Animation animationStand(R"(../Animations/Hero/standAfterDuck)");
//
//	key = waitKey(100);
//	bool isExit = showAnimation(animationStand, false);
//}
//
//
//
//// 2.
//// draws an animation of our hero that walks to the right, and ACTUALLLY MOVES RIGHT
//// with a constant speed, of (1/10)*(image width) per 100 milliseconds (=0.1 second).
//
//
//// 3.
//// draw our hero jumps up and right, and stays standing after jump finishes.
//
////to jump
//void jumpRight(int key) {
//	Animation animationJump(R"(../Animations/Hero/jump)");
//	key = waitKey(100);
//	bool isExit = showAnimation(animationJump, true);
//}
//
//// 4.
//// write a main() that:
//// when user presses 'd' - hero walks right
//// when user presses 's' - hero ducks
//// when user presses 'w' - hero jumps
//// when user presses 'a' - hero stops (if it was walking right before)
//
//// 5.
//// in walkRight - when hero reaches the canvas boundery - don't let it go outside of image.
//// force the hero x coordinate to be such that we'll see her.
//// hint: you can get hero image cv::Mat with frame.image
//
//// 6.
//// do the same with jumpRight
//
////move the girl up
//void goUp(int i, Point* topLeft) {
//	if (i >= 3 && i <= 5) {
//		topLeft->y -= 50;
//	}
//	if (i >= 6 && i <= 8) {
//		topLeft->y += 20;
//	}
//}
//
//
//
////display the girl
//bool showAnimation(Animation animation, bool isGoUp) {
//	for (int i = 0; i < animation.numFrames(); i++)
//	{
//		Frame const& frame = animation.getFrame(i);
//		Mat canvas = background.clone();
//		Point topLeft(canvas.size().width / 2, canvas.size().height * 2 / 3.1);
//		topLeft = walkRight(topLeft);
//		if (isGoUp) {
//			goUp(i, &topLeft);
//		}
//		drawFrame(frame, canvas, topLeft);
//		imshow("test", canvas);
//		int key = waitKey(100);
//
//		if (key == 27) // if you pressed ESC key
//		{
//			return true;
//		}
//		
//		if (key == 'd') {
//			walkRight(key);
//		}
//		if (key == 's') {
//			duck(key);
//		}
//
//		if (key == 'w') {
//			jumpRight(key);
//		}
//
//		if (key == 'a') {
//			return true;
//		}
//	}
//	return false;
//}
//
//
//
//
////#include <Windows.h>
////if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
////	walkRight();
////}
////if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
////	duck();
////}
////if (GetAsyncKeyState(VK_UP) & 0x8000) {
////	jumpRight();
////}	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
////	walkRight();
////}
////if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
////	duck();
////}
////if (GetAsyncKeyState(VK_UP) & 0x8000) {
////	jumpRight();
////}