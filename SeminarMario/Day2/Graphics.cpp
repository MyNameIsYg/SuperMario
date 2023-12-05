#include "Graphics.h"
#include <algorithm>

using namespace cv;
using namespace std;

//////////////////////////////////////////////////

SingleAnimationGraphics::SingleAnimationGraphics(AnimationPtr animPtr, bool isCyclic)
	:_animation(animPtr), _currFrameIdx(0)
	, _isCyclic(isCyclic)
{
}


Mat SingleAnimationGraphics::getCollisionMask()
{
	return _animation->getFrame(_currFrameIdx).mask;
}

void SingleAnimationGraphics::draw(Mat& canvas, cv::Point const& topLeft)
{
	drawFrame(_animation->getFrame(_currFrameIdx), canvas, topLeft);
}

void SingleAnimationGraphics::reset(int code)
{
	_currFrameIdx = 0;
}

bool SingleAnimationGraphics::update()
{
	bool hasFinished = (_currFrameIdx == _animation->numFrames() - 1);
	if (_isCyclic)
		_currFrameIdx = (_currFrameIdx + 1) % _animation->numFrames();
	else
		_currFrameIdx = min<int>(_currFrameIdx + 1, (int)_animation->numFrames() - 1);

	return hasFinished;
}


//////////////////////////////////////////////////

/*void exampleOfPutText()
{
	Mat image = imread(R"(../Animations/background.png)", IMREAD_UNCHANGED);
	string text = "score: ";
	int score = 465;
	string moreText = to_string(score);
	text = text + moreText;
	Point topLeft(50, 50);
	int fontFace = FONT_HERSHEY_SIMPLEX; // normal size sans - serif font
	// other options for fontFace:
	FONT_HERSHEY_PLAIN;// small size sans - serif font
	FONT_HERSHEY_DUPLEX;// normal size sans - serif font(more complex than FONT_HERSHEY_SIMPLEX)
	FONT_HERSHEY_COMPLEX; // normal size serif font
	FONT_HERSHEY_TRIPLEX; //normal size serif font(more complex than FONT_HERSHEY_COMPLEX)
	FONT_HERSHEY_COMPLEX_SMALL; //smaller version of FONT_HERSHEY_COMPLEX
	FONT_HERSHEY_SCRIPT_SIMPLEX; //hand - writing style font
	FONT_HERSHEY_SCRIPT_COMPLEX;// more complex variant of FONT_HERSHEY_SCRIPT_SIMPLEX
	double fontScale = 2; // Font scale factor that is multiplied by the font-specific base size.
	Scalar color(Blue between 0...255 =0,Green between 0...255 =0,Red between 0...255 =255);
	int thickness = 2;
	int lineType = FILLED;
	// other options for line type:
	LINE_4; LINE_8; LINE_AA;

	// finally:
	putText(image, text, topLeft, fontFace, fontScale, color, thickness, lineType);

	imshow("test", image);
	waitKey();//waiting forever till some key is hit.
}*/

//////////////////////////////////////////////////

Mat LivesGraphics::getCollisionMask()
{
	return _singleLife.mask;
}

void LivesGraphics::draw(Mat& canvas, Point const& topLeft)
{
	Point tempPoint = Point(1300, 20);/////////////////////////////////////
	for (int i = 0; i < _livesCount; i++) {
		drawFrame(_singleLife, canvas, tempPoint);
		tempPoint.x -= 35;
	}

	if (_livesCount <= 0) {
		string text = "Game Over :(";
		int fontFace = FONT_HERSHEY_COMPLEX;
		double fontScale = 6;
		Scalar color(0, 0, 254);
		int thickness = 20;
		int lineType = LINE_AA;
		putText(canvas, text, Point(10, 600), fontFace, fontScale, color, thickness, lineType);

	}
}

void LivesGraphics::reset(int code) {
	if (code == 0) {
		--_livesCount;
		return;
	}
	if(_livesCount <4)
		_livesCount += code;
}

bool LivesGraphics::update()
{
	return false;
}

//////////////////////////////////////////////////

Mat ScoresGraphics::getCollisionMask()
{
	return Mat();
}

void ScoresGraphics::draw(Mat& canvas, Point const& topLeft)
{
	string text;
	text = "score: ";
	string score = to_string(_score);
	text = text + score;
	Scalar color(/*Blue between 0...255 =*/31,/*Green between 0...255 =*/41,/*Red between 0...255 =*/255);
	int thickness = 2;
	int lineType = FILLED;
	putText(canvas, text, /*Point(20, 40)*/ topLeft, _fontFace, _fontScale, color, thickness, lineType);
}

void ScoresGraphics::reset(int code) {
	if (code == 0) {
		if (_score > 50) {
			this->_score -= 50;
			return;
		}
		this->_score = 0;
		return;
	}
	this->_score += code;
}

bool ScoresGraphics::update()
{
	return false;
}

////////////////////////////////////////////////

Mat AppleGraphics::getCollisionMask()
{
	/////////////////////////////////////////
	return _singalApple.mask;
}

void AppleGraphics::draw(Mat& canvas, Point const& topLeft)
{
	drawFrame(_singalApple, canvas, topLeft);
}

void AppleGraphics::reset(int code)
{

}

bool AppleGraphics::update()
{
	return false;
}


///////////////////////////////////////////////

Mat BlocksGraphics::getCollisionMask()
{
	return _singleBlock.mask;
}

void BlocksGraphics::draw(Mat& canvas, Point const& topLeft)
{
	drawFrame(_singleBlock, canvas, topLeft);
}

void BlocksGraphics::reset(int code) {
	
}

bool BlocksGraphics::update()
{
	return false;
}