#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>
#include <string>

using namespace cv;

class Frame
{
public:
	Mat image; // rgb
	Mat mask; // alpha
public:
	Frame(std::string const& path);
	bool isValid() const;
};

void drawFrame(Frame const& src, Mat & dst, Point const& TL);


using namespace std;

class Animation
{
private:
	vector<Frame> _frames;

public:
	Animation(string const& folder);
	Frame const& getFrame(size_t idx);
	size_t numFrames() const;

	void resizeImg();
};

typedef shared_ptr<Animation> AnimationPtr;