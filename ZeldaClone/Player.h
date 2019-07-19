#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include "Enemy.h"
#include "mappy_A5.h"

class Player {

	friend int collided(int x, int y);
	friend bool endValue(int x, int y);

public:
	Player();
	~Player();
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	int getX() { return x; }
	int getY() { return y; }
	int blockValueCheck();
	void move(char moveDir, int xOff, int yOff, Enemy enemy[], int numEnemy);
	bool update(int xOff, int yOff);
	void attack(Enemy enemy[], int numEnemy);
	bool CollisionEndBlock();
	void setCurrBlock();
	int getCurrBlock() { return currBlock; }
	int getHealth() { return health; }
	int getLevel() { return level; }
	void setLevel(int num) { level = num; }
	void pickUp(int currBlock);
	void statusDraw(int gameTime, int flag);
	void levelUp();
	void superSwordAttack(char dir, int xOff, int yOff, int attackFX);
	bool getWin() { return win; }

private:
	int x;
	int y;
	int speed; //Movement speed
	int health; //Players health
	int level; //Player level
	int currFrame; //Currently drawn sprite frame
	int maxFrame; //Max frame for current frame
	int maxMoveFrame; //Max frame for movement
	int frameCount; //Current frame delay
	int frameDelay; //Set delay for next current frame
	int frameWidth; //Width of the sprite frame
	int frameHeight; //Sprite frame height
	int animColumn; //Number of columns for animation
	int animRows; //Number of possible animations
	int currBlock; //Value of current block
	int weaponDamage; //How much damage current weapon does
	int attackDelay; //How long to animate the attack action
	char dir; //Facing direction
	bool alive;
	bool attacking;
	bool win; //If the player makes it to the end
	std::string weapon; //Current equipped weapon
	std::string tunic; //Currently worn clothing

	ALLEGRO_FONT* statusFont;
	ALLEGRO_BITMAP* dude;
	ALLEGRO_SAMPLE* stairs;
	ALLEGRO_SAMPLE* beep;
	ALLEGRO_SAMPLE* splash;
	ALLEGRO_SAMPLE* boom;
	ALLEGRO_SAMPLE* sword;
	ALLEGRO_SAMPLE* hurt;
	ALLEGRO_SAMPLE* sizzle;
	ALLEGRO_SAMPLE* pickup;
	ALLEGRO_SAMPLE* superSword;
};