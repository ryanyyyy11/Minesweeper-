#pragma once
#include <SFML/Graphics.hpp>

class Objects
{
public:
	Objects();
	Objects(const char* imageName);
	sf::Sprite &GetSprite();
	void SetPosition(float x, float y);

protected:
	sf::Sprite sprite;
	sf::Texture texture;
	float x;
	float y;
};

class Image : public Objects
{
public:
	Image(const char *imageName, float _x, float _y);
	void update();
	virtual float GetX();
	virtual float GetY();
};

class Digits : public Objects
{
public:
	Digits(const char *imageName, float _x, float _y);
	void update();
	void Intact(int num);
};

