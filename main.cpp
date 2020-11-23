#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <iostream>
#include <vector>
#include <fstream>
#include <string.h> 
#include "Objects.h"
using namespace std;

const int row = 25;
const int col = 16;
const int numMine = 40;
int mineRemain = numMine;
int flagRemain = mineRemain;
bool gameNotOver = true;
bool mineSet = false;
bool win = false;

vector<Image *> vTiles;
vector<Image *> vTilesRevealed;
vector<Image *> vNum;
vector<Image *> vFlag;
vector<Image *> vMine;

void RandMine(int map[][col+2]);
void Adjacent(int map[][col + 2]);
void SetGraph(int map[][col + 2]);
void SetFlag(int map[][col + 2]);
void SetMine(int map[][col + 2]);
void LeftMouseClick(int x, int y, int map[][col + 2]);
void RightMouseClick(int x, int y, int map[][col + 2]); 
void Spread(int x, int y, int map[][col + 2]);
bool checkWinRight(int x, int y, int map[][col + 2]);
bool checkWinLeft(int map[][col + 2]);
void readFile(string fileName, int map[][col + 2]);
void UpdateDigits(Digits &digit1, Digits &digit2, Digits &digit3);
void restart(int map[][col + 2]);

int main()
{
	int map[row + 2][col + 2] = { 0 };
	RandMine(map);
	Adjacent(map); 
	SetGraph(map);


	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");


	Image face_happy("images/face_happy.png", 368, 512);
	Image face_win("images/face_win.png", 368, 512);
	Image face_lose("images/face_lose.png", 368, 512);
	Image debug("images/debug.png", 496, 512);
	Image test1("images/test_1.png", 560, 512);
	Image test2("images/test_2.png", 624, 512);
	Image test3("images/test_3.png", 688, 512);
	Digits digit1("images/digits.png", 0, 512);
	Digits digit2("images/digits.png", 21, 512);
	Digits digit3("images/digits.png", 42, 512);

	UpdateDigits(digit1, digit2, digit3);

	//do this until we tell you to stop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;

					if (y < 512 && gameNotOver)
					{
						x = x / 32 + 1;
						y = y / 32 + 1;
						LeftMouseClick(x, y, map);
						SetGraph(map);
						win = checkWinLeft(map);
					}
					if (y >= 512 && y <= 576)
					{
						//restart the game=======================
						if (x >= 368 && x < 432)
						{
							restart(map);
							RandMine(map);
							Adjacent(map);
							SetGraph(map);
						}

						//Debug=========================================
						if (x >= 496 && x < 560 && gameNotOver)
						{
							//If mine is not already set, set the mine
							if (!mineSet)
							{
								SetMine(map);

							}
							else //If mine is already set, delete them.
							{
								vMine.clear();
								mineSet = false;
							}
						}

						//test1 =======================================
						if (x >= 560 && x < 624 && gameNotOver)
						{
							restart(map);
							readFile("boards/testboard1.brd", map);
							Adjacent(map);
							SetGraph(map);
						}
						if (x >= 624 && x < 688 && gameNotOver)
						{
							restart(map);
							readFile("boards/testboard2.brd", map);
							Adjacent(map);
							SetGraph(map);
						}
						//test2 ====================================
						if (x >= 688 && x < 752 && gameNotOver)
						{
							restart(map);
							readFile("boards/testboard3.brd", map);
							Adjacent(map);
							SetGraph(map);
						}
					}
					UpdateDigits(digit1, digit2, digit3);
				}
				if (event.mouseButton.button == sf::Mouse::Right && gameNotOver)
				{
					int x = event.mouseButton.x / 32 + 1;
					int y = event.mouseButton.y / 32 + 1;

					//add 20 to where you click and -20 if excees 20
					RightMouseClick(x, y, map);
					SetFlag(map);

					//set up digits on the bottom left... 
					UpdateDigits(digit1, digit2, digit3);

					//if all mine are placed with flags, you win, stop everything.
					win = checkWinRight(x, y, map);
				}
			}


			//1.update
			for (int i = 0; i < vTiles.size(); i++)
			{
				vTiles[i]->update();
			}
			for (int i = 0; i < vTilesRevealed.size(); i++)
			{
				vTilesRevealed[i]->update();
			}
			for (int i = 0; i < vNum.size(); i++)
			{
				vNum[i]->update();
			}
			for (int i = 0; i < vFlag.size(); i++)
			{
				vFlag[i]->update();
			}
			for (int i = 0; i < vMine.size(); i++)
			{
				vMine[i]->update();
			}
			face_happy.update();
			face_win.update();
			face_lose.update();
			debug.update();
			test1.update();
			test2.update();
			test3.update();
			digit1.update();
			digit2.update();
			digit3.update();

			//2.draw
			window.clear();

			for (unsigned int i = 0; i < vTiles.size(); i++)
			{
				window.draw(vTiles[i]->GetSprite());
			}
			for (unsigned int i = 0; i < vFlag.size(); i++)
			{
				window.draw(vFlag[i]->GetSprite());
			}
			for (unsigned int i = 0; i < vTilesRevealed.size(); i++)
			{
				window.draw(vTilesRevealed[i]->GetSprite());
			}
			for (unsigned int i = 0; i < vNum.size(); i++)
			{
				window.draw(vNum[i]->GetSprite());
			}
			if (mineSet)
			{
				for (unsigned int i = 0; i < vMine.size(); i++)
				{
					window.draw(vMine[i]->GetSprite());
				}
			}
			if (gameNotOver)
				window.draw(face_happy.GetSprite());
			if (!gameNotOver)
				window.draw(face_lose.GetSprite());
			if (win)
				window.draw(face_win.GetSprite());

			window.draw(debug.GetSprite());
			window.draw(test1.GetSprite());
			window.draw(test2.GetSprite());
			window.draw(test3.GetSprite());
			window.draw(digit1.GetSprite());
			window.draw(digit2.GetSprite());
			window.draw(digit3.GetSprite());


			window.display();
		}
	}

	//delete
	for (unsigned int i = 0; i < vTiles.size(); i++)
	{
		delete vTiles[i];
	}
	for (unsigned int i = 0; i < vTilesRevealed.size(); i++)
	{
		delete vTilesRevealed[i];
	}
	for (unsigned int i = 0; i < vNum.size(); i++)
	{
		delete vNum[i];
	}
	for (unsigned int i = 0; i < vFlag.size(); i++)
	{
		delete vFlag[i];
	}
	for (unsigned int i = 0; i < vMine.size(); i++)
	{
		delete vMine[i];
	}

	return 0;
}

void RandMine(int map[][col+2])
{
	std::mt19937 mt(time(0)); 
	int x, y;
	std::uniform_int_distribution<int> dist(0, 100);
	
	//Set the random mines first
	for (int i = 0; i < numMine;)
	{
		x = dist(mt) % row+1;
		y = dist(mt) % col+1;
		if (map[x][y] != -1)
		{
			map[x][y] = -1;
			i++;
		}
	}
}

void Adjacent(int map[][col + 2])
{
	//then set the adjacent tiles. 
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			if (map[i][j] == -1)
			{
				for (int p = i - 1; p <= i + 1; p++)
				{
					for (int k = j - 1; k <= j + 1; k++)
					{
						if (map[p][k] != -1)
						{
							map[p][k] ++;
						}
					}
				}
			}
		}
	}
}

//load all the images according to each number on the map.
void SetGraph(int map[][col + 2])
{
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			//load the hidden tiles
			Image *t = new Image("images/tile_hidden.png", (i - 1) * 32, (j - 1) * 32);
			vTiles.push_back(t);

			//load the revealed tiles and number
			if (map[i][j] >= 10 && map[i][j] < 18)
			{
				Image *t2 = new Image("images/tile_revealed.png", (i - 1) * 32, (j - 1) * 32);
				vTilesRevealed.push_back(t2);
			}

			if (map[i][j] == 11)
			{
				Image *t2 = new Image("images/number_1.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 12)
			{
				Image *t2 = new Image("images/number_2.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 13)
			{
				Image *t2 = new Image("images/number_3.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 14)
			{
				Image *t2 = new Image("images/number_4.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 15)
			{
				Image *t2 = new Image("images/number_5.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 16)
			{
				Image *t2 = new Image("images/number_6.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 17)
			{
				Image *t2 = new Image("images/number_7.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
			if (map[i][j] == 18)
			{
				Image *t2 = new Image("images/number_8.png", (i - 1) * 32, (j - 1) * 32);
				vNum.push_back(t2);
			}
		}
	}
}

void SetFlag(int map[][col + 2])
{

	for (int i = 1; gameNotOver && i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			if (map[i][j] >= 19)
			{
				bool pushBack = true;
				Image *t2 = new Image("images/flag.png", (i - 1) * 32, (j - 1) * 32);
				for (unsigned k = 0; k < vFlag.size(); k++)
				{
					//if there is already a flag, do not push back.
					if (((int)vFlag[k]->GetX()) / 32 + 1 == i && ((int)vFlag[k]->GetY()) / 32 + 1 == j)
						pushBack = false;
				}
				if (pushBack)
					vFlag.push_back(t2);
			}
		}
	}
}

void SetMine(int map[][col + 2])
{
	for (int i = 1;i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			//load mine
			if (map[i][j] == -1)
			{
				Image *t2 = new Image("images/mine.png", (i - 1) * 32, (j - 1) * 32);
				vMine.push_back(t2);
			}
		}
	}
	mineSet = true;

}

void LeftMouseClick(int x, int y, int map[][col + 2])
{
	//if click on a mine, die
	if (map[x][y] == -1)
	{
		gameNotOver = false;
		SetMine(map);
	}
	else if (map[x][y] == 0)
	{
		Spread(x, y, map);
	}
	else if(map[x][y] > 0 && map[x][y] <= 8)
	{
		map[x][y] += 10;
	}
}

void RightMouseClick(int x, int y, int map[][col + 2])
{
	if (map[x][y] < 9)
	{
		map[x][y] += 20;
		flagRemain -= 1;
	}
	else if (map[x][y] >= 19)
	{
		map[x][y] -= 20;
		flagRemain += 1;
		//if where you click is already a flag, loop through the vector and delete the object, so that it wont appear anymore. 
		for (unsigned int i = 0; i < vFlag.size(); i++)
		{
			if (((int)vFlag[i]->GetX())/32 +1 == x && ((int)vFlag[i]->GetY())/32+1 == y)
			{
				vFlag.erase(vFlag.begin() + i);
			}
		}
	}
}

void Spread(int x, int y, int map[][col + 2])
{
	map[x][y] += 10;
	for (int i = x-1; i <= x+1; i++)
	{
		if (i >= 1 && i <= 25)
		{
			for (int j = y - 1; j <= y+1; j++)
			{
				if (j >= 1 && j <= 16 )
				{
					if (map[i][j] == 0)
					{
						Spread(i, j, map);
					}
					if (map[i][j] > 0 && map[i][j] <= 8)
					{
						map[i][j] += 10;
					}
				}
			}
		}
	}
}

bool checkWinRight(int x, int y, int map[][col + 2])
{
	if (map[x][y] == 19)
	{
		mineRemain -= 1;
	}
	if (mineRemain != 0 || flagRemain != 0)
	{
		return false;
	}
	gameNotOver = false;
	return true;
}

bool checkWinLeft(int map[][col + 2])
{
	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			if (map[i][j] >= 0 && map[i][j] <= 8 || map[i][j] >= 20)
			{
				return false;
			}
		}
	}
	gameNotOver = false;
	return true;
}

void readFile(string fileName, int map[][col + 2])
{
	ifstream file(fileName, ios_base::in);

	int x = 1;
	int y = 1;
	int count = 0;
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
		{
			char char_array[25];

			for (int i = 0; i < sizeof(char_array); i++)
			{
				char_array[i] = line[i];
				if ((int)char_array[i] - 48 == 1)
				{
					char_array[i] = 47;
					count += 1;
				}

				if (x <= 25)
				{
					map[x][y] = (int)char_array[i] - 48;
					x++;
				}
				if (x > 25)
				{
					x = 1;
					y += 1;
				}
			}
		}
		file.close();
	}
	mineRemain = count;
	flagRemain = mineRemain;
}

void UpdateDigits(Digits & digit1, Digits & digit2, Digits & digit3)
{
	int temp = flagRemain / 10;
	int remainder = flagRemain % 10;
	if (temp >= 0 && remainder >= 0)
	{
		digit1.Intact(0);
		digit2.Intact(temp);
		digit3.Intact(remainder);
	}
	else if (temp <= 0 && remainder <= 0)
	{
		digit1.Intact(-1);
		digit2.Intact(0 - temp);
		digit3.Intact(0 - remainder);
	}
}

void restart(int map[][col + 2])
{

	vTiles.clear();
	vTilesRevealed.clear();
	vNum.clear();
	vFlag.clear();
	vMine.clear();

	gameNotOver = true;
	win = false;
	mineSet = false;
	mineRemain = numMine;
	flagRemain = mineRemain;

	for (int i = 1; i <= row; i++)
	{
		for (int j = 1; j <= col; j++)
		{
			map[i][j] = 0;
		}
	}
}



