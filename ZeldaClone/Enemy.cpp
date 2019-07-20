#include "Enemy.h"
#include <iostream>

Enemy::Enemy()
{
	alive = false;
	speed = 20;
	maxHealth = 5;
	image = al_load_bitmap("enemy.png");
	frameWidth = 26;
	frameHeight = 26;
	dieNoise = al_load_sample("die.wav"); //Death noise
}

Enemy::~Enemy()
{
	al_destroy_sample(dieNoise);
	al_destroy_bitmap(image);
}

//Create the enemy with given location based on the players level
void Enemy::spawn(int level)
{
	this->level = level;
	//Only spawn if not alive and haven't already been on the current layer
	if (!alive && currLayer != level) {
		currLayer = level;
		if (level == 1) {
			//Pick random X,Y in selected level
			x = (rand() % 1536) + 32;
			y = (rand() % 384) + 1152;
			//Randomize X,Y until not colliding with collide tiles
			while (collided(x + 15, y + 15)) {
				x = (rand() % 1536) + 32;
				y = (rand() % 384) + 1152;
			}
		}
		else if (level == 2) {
			x = (rand() % 480) + 1088;
			y = (rand() % 1024) + 32;
			while (collided(x + 15, y + 15)) {
				x = (rand() % 480) + 1088;
				y = (rand() % 1024) + 32;
			}
		}
		else if (level == 3) {
			x = (rand() % 416) + 160;
			y = (rand() % 192) + 256;
			while (collided(x + 15, y + 15)) {
				x = (rand() % 416) + 160;
				y = (rand() % 192) + 256;
			}
		}
		health = maxHealth;
		alive = true; //Set enemy to alive
	}
}


//The value of the current block
void Enemy::setCurrBlock()
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	currBlock = data->user1;
}

//When enough damage is taken, the enemy dies and drops a rupee
void Enemy::die()
{
	if (level == 1) { MapSetBlock(x / 32, y / 32, 595); }
	if (level == 2) { MapSetBlock(x / 32, y / 32, 627); }
	if (level == 3) { MapSetBlock(x / 32, y / 32, 659); }
	al_play_sample(dieNoise, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	alive = false;
}

//The player attacks the enemy
void Enemy::damage(Entity* source, int weaponDamage)
{
	al_play_sample(dieNoise, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
	health -= weaponDamage;
}

//Random chance for enemy to move in any direction
void Enemy::move(int WIDTH, int HEIGHT)
{
	if (alive)
	{
		int oldX = x;
		int oldY = y;
		if (rand() % 50 == 0) {
			animColumn = rand() % 2;
			int randMove = rand() % 5; //Random number for movement 0 - No move / 1 or 2 - X move / 3 or 4 - Y move
			int randDir = rand() % 2; //Random direction, 0 - Positive / 1 - Negative
			if (randDir == 1) { speed = -speed; }
			if (randMove == 0) {

			}
			else if (randMove == 1 || randMove == 2) {
				x += speed;
				if (speed >= 0) {
					dir = 'r';
					animRow = 0;
				}
				else {
					dir = 'l';
					animRow = 1;
				}
			}
			else if (randMove == 3 || randMove == 4) {
				y += speed;
				if (speed >= 0) {
					dir = 'd';
					animRow = 3;
				}
				else {
					dir = 'u';
					animRow = 2;
				}
			}
		}

		if (x < 5 || x + frameWidth > WIDTH - 5) {
			x = oldX;
		}
		if (y < 5 || y + frameHeight > WIDTH - 5) {
			y = oldY;
		}

		if (dir == 'u') {
			if (collided(x, y) || collided(x + frameWidth, y)) {
				y = oldY;
			}
		}
		if (dir == 'd') {
			if (collided(x, y + frameHeight) || collided(x + frameWidth, y + frameHeight)) {
				y = oldY;
			}
		}
		if (dir == 'l') {
			if (collided(x, y) || collided(x, y + frameHeight)) {
				x = oldX;
			}
		}
		if (dir == 'r') {
			if (collided(x + frameWidth, y) || collided(x + frameWidth, y + frameHeight)) {
				x = oldX;
			}
		}
	}
	else
	{
		x = 0;
		y = 0;
	}
	setCurrBlock();
}

//Draws current location if still alive
void Enemy::update(int xOff, int yOff, int layer)
{
	if (currLayer != layer) {
		alive = false;
	}
	if (alive)
	{
		al_draw_bitmap_region(image, animColumn * frameWidth, animRow * frameHeight, frameWidth, frameHeight, x - xOff, y - yOff, 0);
		if (currBlock == 50) {
			health--;
		}
		if (health <= 0) { die(); }
	}
}

void Enemy::setAlive(bool value) {
	alive = value;
	if (!alive) {
		al_play_sample(dieNoise, 1, 0, .7, ALLEGRO_PLAYMODE_ONCE, 0);
	}
}