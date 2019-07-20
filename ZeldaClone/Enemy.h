#pragma once
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "mappy_A5.h"
#include "Entity.h"
#include <allegro5/allegro_primitives.h>

class Enemy : public Entity
{

	friend int collided(int x, int y);

public:
	Enemy();
	~Enemy();
	void spawn(int level);
	void attack();
	void die();
	void move(int WIDTH, int HEIGHT);
	void update(int xOff, int yOff, int layer);
	void setAlive(bool value);
	void setCurrBlock();
	void damage(Entity* source, int weaponDamage);
	int getX() { return x; }
	int getY() { return y; }
	int getBoundX() { return x + frameWidth; }
	int getBoundY() { return y + frameHeight; }


private:
	int x;
	int y;
	int speed;
	int frameWidth;
	int frameHeight;
	int animColumn; //Walking animation
	int animRow; //Direction sprite
	int currBlock; //Current tile the entity is on
	int currFrame; //Currently drawn sprite frame
	int level; //Level the enemy is spawned on
	int maxFrame; //Max frame for current frame
	int frameCount; //Current frame delay
	int frameDelay; //Set delay for next current frame
	int maxHealth;
	int health;
	int currLayer;
	bool alive = false;
	char dir;

	ALLEGRO_BITMAP* image;
	ALLEGRO_SAMPLE* dieNoise;
};