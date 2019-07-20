#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include "Entity.h"
#include "mappy_A5.h"
#include "Enemy.h"
#include <vector>

class Player : public Entity 
{

	friend int collided(int x, int y);
	friend bool endValue(int x, int y);

public:
	Player();
	~Player();
	int getX() { return x; }
	int getY() { return y; }
	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }
	bool getWin() { return win; }
	int getCurrBlock() { return currBlock; }
	int getHealth() { return health; }
	int getLevel() { return level; }
	int getRupees() { return rupees; }
	std::string getWeaponName() { return weapon; }
	int getWeaponDamage() { return weaponDamage; }

	void attack(Enemy enemy[], int numEnemy);
	int blockValueCheck();
	bool CollisionEndBlock();
	void damage(Entity* source, int weaponDamage);
	void load(); //Load all of the player assets
	void move(char moveDir, int xOff, int yOff, Enemy enemy[], int numEnemy);
	void pickUp(int currBlock);
	void setCurrBlock();
	void setLevel(int num) { level = num; }
	void spawn(int x, int y); //Spawns the player on a new game
	void statusDraw(int gameTime, int flag);
	void superSwordAttack(char dir, int xOff, int yOff, int attackFX);
	bool update(int xOff, int yOff);

private:
	bool alive;
	int animColumn; //Number of columns for animation
	int animRows; //Number of possible animations
	int attackDelay; //How long to animate the attack action
	bool attacking;
	int currBlock; //Value of current block
	int currFrame; //Currently drawn sprite frame
	char dir; //Facing direction
	int frameCount; //Current frame delay
	int frameDelay; //Set delay for next current frame
	int frameWidth; //Width of the sprite frame
	int frameHeight; //Sprite frame height
	int maxHealth; //Limit to player health
	int health; //Players health
	int level; //Player level
	int maxFrame; //Max frame for current frame
	int maxMoveFrame; //Max frame for movement
	int rupees;
	int speed; //Movement speed
	int weaponDamage; //How much damage current weapon does
	int numKeys; //Number of unused keys the player has
	bool win; //If the player makes it to the end
	int x;
	int y;
	std::string weapon; //Current equipped weapon
	std::string tunic; //Currently worn clothing
	std::vector<std::string> treasureList;

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