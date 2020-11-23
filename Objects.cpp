#include "Objects.h"
using namespace std;

Objects::Objects(const char* imageName)
{
	texture.loadFromFile(imageName);
	sprite.setTexture(texture);
}

sf::Sprite & Objects::GetSprite()
{
	return sprite;
}

void Objects::SetPosition(float x, float y)
{
	sprite.setPosition(x, y);
}

//Image===============================================
Image::Image(const char * imageName, float _x, float _y) : Objects(imageName)
{
	x = _x;
	y = _y;
}

void Image::update()
{
	sprite.setPosition(x, y);
}

float Image::GetX()
{
	return x;
}

float Image::GetY()
{
	return y;
}


//Digits=======================================================
Digits::Digits(const char * imageName, float _x, float _y) : Objects(imageName)
{
	x = _x;
	y = _y;
}

void Digits::update()
{
	sprite.setPosition(x, y);
}

void Digits::Intact(int num)
{
	if (num == 0)
		sprite.setTextureRect(sf::IntRect(0, 0, 21, 32));
	if (num == 1)
		sprite.setTextureRect(sf::IntRect(21, 0, 21, 32));
	if (num == 2)
		sprite.setTextureRect(sf::IntRect(42, 0, 21, 32));
	if (num == 3)
		sprite.setTextureRect(sf::IntRect(63, 0, 21, 32));
	if (num == 4)
		sprite.setTextureRect(sf::IntRect(84, 0, 21, 32));
	if (num == 5)
		sprite.setTextureRect(sf::IntRect(105, 0, 21, 32));
	if (num == 6)
		sprite.setTextureRect(sf::IntRect(126, 0, 21, 32));
	if (num == 7)
		sprite.setTextureRect(sf::IntRect(147, 0, 21, 32));
	if (num == 8)
		sprite.setTextureRect(sf::IntRect(168, 0, 21, 32));
	if (num == 9)
		sprite.setTextureRect(sf::IntRect(189, 0, 21, 32));
	if (num == -1)
	{
		sprite.setTextureRect(sf::IntRect(210, 0, 21, 32));
	}


}
