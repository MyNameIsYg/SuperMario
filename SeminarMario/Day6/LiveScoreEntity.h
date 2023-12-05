#pragma once
#include "../Day4/Entities.h"

class LivesEntity : public Entity
{
private:
	int _liveCount;

public:
	LivesEntity();

	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);
	bool checkCollision(std::shared_ptr<Entity> slime);
	void draw(cv::Mat& canvas);
};


class ScoresEntity : public Entity
{
private:


public:
	ScoresEntity(float fontScale, int score, int fontFace);

	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);
	bool checkCollision(std::shared_ptr< Entity> slime);
	void draw(cv::Mat& canvas);
};


class AppleEntity : public Entity
{
private:
	Size _size;

public:
	bool _isActive;

	AppleEntity(/*int liveCount*/);

	virtual void onNotify(Event const& e) override;
	virtual void reset(cv::Point const& TL);
	bool checkCollision(std::shared_ptr<Entity> slime);
	void draw(cv::Mat& canvas);
};

