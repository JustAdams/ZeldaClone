#include "Player.h"

#include <iostream>

Player::Player() {
	dir = 'd';
	speed = 3;
	health = 5;
	level = 1;
	animColumn = 0;
	animRows = 2;
	maxFrame = 4;
	maxMoveFrame = 2;
	currFrame = 1;
	frameCount = 0;
	frameDelay = 4;
	frameWidth = 25;
	frameHeight = 25;
	attackDelay = 0;
	attacking = false;
	alive = true;
	win = false;
	currBlock = NULL;
	weapon = "unarmed";
	weaponDamage = 0;
	tunic = "green";
	statusFont = al_load_font("status.ttf", 12, 0);
	stairs = al_load_sample("stairs.wav"); //Stairs noise
	beep = al_load_sample("beep.wav"); //Secret button sound
	splash = al_load_sample("splash.wav"); //Water event sound
	boom = al_load_sample("boom.wav"); //Boom sound
	hurt = al_load_sample("hurt.wav"); //Sound if player is hurt
	sizzle = al_load_sample("sizzle.wav"); //When player falls in lava
	sword = al_load_sample("sword.wav"); //Sword stab noise
	superSword = al_load_sample("superSword.wav"); //Super sword noise
	pickup = al_load_sample("pickup.wav"); //Pickup noise
	dude = al_load_bitmap("link.png"); //Player sprite sheet
	x = 64;
	y = 1504;
}

Player::~Player() {
	al_destroy_font(statusFont);
	al_destroy_bitmap(dude);
	al_destroy_sample(beep);
	al_destroy_sample(splash);
	al_destroy_sample(boom);
	al_destroy_sample(hurt);
	al_destroy_sample(sizzle);
	al_destroy_sample(sword);
	al_destroy_sample(pickup);
}

//Move the player in a given direction by speed, checks for collision against tiles and enemies
void Player::move(char moveDir, int xOff, int yOff, Enemy enemy[], int numEnemy)
{

	int oldX = x;
	int oldY = y;
	dir = moveDir;

	if (++frameCount > frameDelay)
	{
		frameCount = 0;
		if (++currFrame > maxMoveFrame)
		{
			currFrame = 0;
		}
	}

	switch (dir) {
	case 'u':
		animColumn = 2;
		y -= speed;
		break;
	case 'd':
		y += speed;
		animColumn = 0;
		break;
	case 'l':
		animColumn = 1;
		x += -speed;
		break;
	case 'r':
		animColumn = 3;
		x += speed;
		break;

	}

	//Stops movement while attacking
	if (attacking)
	{
		x = oldX;
		y = oldY;
	}

	//Collision checks
	if (dir == 'u') {
		if (collided(x, y) || collided(x + 20, y)) {
			y = oldY;
		}
	}
	if (dir == 'd') {
		if (collided(x, y + 20) || collided(x + 20, y + 20)) {
			y = oldY;
		}
	}
	if (dir == 'l') {
		if (collided(x, y) || collided(x, y + 20)) {
			x = oldX;
		}
	}
	if (dir == 'r') {
		if (collided(x + 20, y) || collided(x + 20, y + 20)) {
			x = oldX;
		}
	}


	for (int i = 0; i < numEnemy; i++)
	{

		if (x >= enemy[i].getX() && x <= enemy[i].getBoundX() &&
			y >= enemy[i].getY() && y <= enemy[i].getBoundY() ||
			x + frameWidth >= enemy[i].getX() && x + frameWidth <= enemy[i].getBoundX() &&
			y + frameHeight >= enemy[i].getY() && y + frameHeight <= enemy[i].getBoundY())
		{
			if (frameCount == 0 || frameCount == 3) {
				health--;
				al_play_sample(hurt, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			switch (dir) {
			case 'u':
				x = oldX;
				y = oldY + 10;
				break;
			case 'd':
				x = oldX;
				y = oldY - 10;
				break;
			case 'l':
				x = oldX + 10;
				y = oldY;
				break;
			case 'r':
				x = oldX - 10;
				y = oldY;
			}

		}
	}

	setCurrBlock(); //Update the players current block
	if (currBlock == 50 && tunic != "red") //Check if player walks into lava
	{
		al_play_sample(sizzle, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		al_play_sample(hurt, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		health--; //Take damage
	}

}

void Player::attack(Enemy enemy[], int numEnemy)
{
	attacking = true;
	attackDelay = 10;
	if (weapon == "sword") {
		al_play_sample(sword, .5, 0, 2, ALLEGRO_PLAYMODE_ONCE, 0);
		for (int i = 0; i < numEnemy; i++) {
			if (dir == 'u') {
				if (x + 10 > enemy[i].getX() && x + 10 < enemy[i].getBoundX() &&
					y > enemy[i].getY() && y - 20 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'd') {
				if (x + 10 > enemy[i].getX() && x + 10 < enemy[i].getBoundX() &&
					y + 40 > enemy[i].getY() && y + 40 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'l') {
				if (x - 15 > enemy[i].getX() && x - 15 < enemy[i].getBoundX() &&
					y > enemy[i].getY() && y - frameHeight < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'r') {
				if (x + 40 > enemy[i].getX() && x + 40 < enemy[i].getBoundX() &&
					y + 5 > enemy[i].getY() && y - frameHeight - 5 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			}
		}
	}
	if (weapon == "super sword") {
		al_play_sample(superSword, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, 0);
		for (int i = 0; i < numEnemy; i++) {
			if (dir == 'u') {
				if (x + 10 > enemy[i].getX() && x + 10 < enemy[i].getBoundX() &&
					y > enemy[i].getY() && y - 40 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'd') {
				if (x + 10 > enemy[i].getX() && x + 10 < enemy[i].getBoundX() &&
					y + 50 > enemy[i].getY() && y + 50 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'l') {
				if (x - 10 > enemy[i].getX() && x - 35 < enemy[i].getBoundX() &&
					y > enemy[i].getY() && y - frameHeight < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			} if (dir == 'r') {
				if (x + 60 > enemy[i].getX() && x + 10 < enemy[i].getBoundX() &&
					y + 5 > enemy[i].getY() && y - frameHeight - 5 < enemy[i].getBoundY()) {
					enemy[i].damage(weaponDamage);
				}
			}
		}
	}

	blockValueCheck(); //Interacts with current block if possible
}

//Draw player facing current direction, or draw player attacking. Frames change depending on variables.
bool Player::update(int xOff, int yOff)
{
	if (health <= 0) {
		alive = false;
	}

	int fy = (currFrame % animRows) * frameHeight;
	int fx = animColumn * frameWidth;
	if (tunic == "green") {
		fx = animColumn * frameWidth;
	}
	else if (tunic == "red") {
		fx = (animColumn * frameWidth) + 161;
	}
	int attackFX = 0; //Changes depending on tunic color
	if (tunic == "red") {
		attackFX += 163;
	}
	if (attacking) {
		if (weapon == "sword") {
			switch (dir)
			{
			case 'u':
				al_draw_bitmap_region(dude, attackFX + 74, 73, 25, 42, x - xOff, y - 25 - yOff, 0);
				break;
			case 'd':
				al_draw_bitmap_region(dude, attackFX, 90, 25, 40, x - xOff, y - 5 - yOff, 0);

				break;
			case 'l':
				al_draw_bitmap_region(dude, attackFX + 28, 90, 40, 23, x - 20 - xOff, y - 5 - yOff, 0);
				break;
			case 'r':
				al_draw_bitmap_region(dude, attackFX + 101, 90, 40, 24, x - xOff, y - 5 - yOff, 0);
				break;
			}

		}
		else if (weapon == "super sword") {
			//Super sword attack draw with energy wave animation
			superSwordAttack(dir, xOff, yOff, attackFX);
		}
		else {
			fy = 2 * frameHeight;
			fx = animColumn * frameWidth;
			switch (dir)
			{
			case 'u':
				al_draw_bitmap_region(dude, fx, fy, frameWidth, frameHeight, x - xOff, y - yOff, 0);
				break;
			case 'd':
				al_draw_bitmap_region(dude, fx, fy, frameWidth, frameHeight, x - xOff, y - yOff, 0);
				break;
			case 'l':
				al_draw_bitmap_region(dude, fx, fy, frameWidth, frameHeight, x - xOff, y - yOff, 0);
				break;
			case 'r':
				al_draw_bitmap_region(dude, fx, fy, frameWidth, frameHeight, x - xOff, y - yOff, 0);
				break;
			}
		}
		//Reduce attack delay for next animation and set attacking to false when it reaches 0
		attackDelay--;
		if (attackDelay <= 0) {
			attacking = false;
		}
	}
	else {
		al_draw_bitmap_region(dude, fx, fy, frameWidth, frameHeight, x - xOff, y - yOff - 5, 0);
	}

	pickUp(currBlock);

	//Should player die, return false and set gameOver to true
	if (!alive) {
		return false;
	}
	return true;
}

bool Player::CollisionEndBlock()
{
	if (endValue(x + 5, y) || endValue(x - 5, y))
		return true;
	else
		return false;
}

//The value of the current block
void Player::setCurrBlock()
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	currBlock = data->user1;
}

//Interactions with game world dependant on attacking atop tiles user1 values
int Player::blockValueCheck()
{

	switch (currBlock)
	{
	case 0:
		return 0;
	case 1: //Use diamond tile to Unlock firewall level 1
		MapSetBlock(4, 46, 622);
		MapSetBlock(3, 44, 163);
		MapSetBlock(3, 45, 163);
		al_play_sample(beep, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		return 1;
	case 2: //Use bowl by waterfall to open and expose stairs to level 2
		MapSetBlock(43, 35, 711); //Open waterfall
		MapSetBlock(41, 36, 773); //Change bowl
		al_play_sample(splash, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		return 2;
	case 3: //Use tile in level 2 to destroy stones blocking path
		MapSetBlock(40, 20, 590); //Change tile
		MapSetBlock(44, 19, 608); //Destroy stones
		MapSetBlock(45, 19, 608);
		MapSetBlock(44, 20, 608);
		MapSetBlock(45, 20, 608);
		MapSetBlock(44, 21, 608);
		MapSetBlock(45, 21, 608);
		MapSetBlock(44, 22, 608);
		MapSetBlock(45, 22, 608); //End destroy stones
		al_play_sample(boom, 2, -.5, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		return 3;
	case 4: //Use bowl in level 2 to move lava and allow access to super sword
		MapSetBlock(48, 5, 743);
		MapSetBlock(45, 4, 175);
		MapSetBlock(46, 4, 175);
		MapSetBlock(45, 5, 175);
		al_play_sample(splash, 1, .5, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		break;
	case 6: //Use floor plate in level 3 to open lava path
		MapSetBlock(35, 13, 592); //Floor plate to activated plate
		MapSetBlock(22, 7, 670); //Lava to stone floor
		MapSetBlock(23, 7, 670); //Lava to stone floor
		al_play_sample(beep, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		break;
	}


}

void Player::statusDraw(int gameTime, int flag)
{
	//Player info draw updates
	//Draw health
	al_draw_textf(statusFont, al_map_rgb(255, 255, 255), 689, 525, 0, "HEALTH:"); //Health hearts drawn in Player.cpp
	for (int i = 0; i < health; i++) {
		al_draw_bitmap_region(dude, 367, 300, 11, 11, 745 + (i * 15), 527, 0);
	}
	//Draw item rectangle boundary
	al_draw_rectangle(540, 530, 570, 570, al_map_rgb(0, 0, 255), 5);
	al_draw_text(statusFont, al_map_rgb(255, 255, 255), 535, 575, 0, "TUNIC");
	if (tunic == "green") {
		al_draw_bitmap_region(dude, 185, 202, 15, 10, 547, 545, 0);
	}
	if (tunic == "red") {
		al_draw_bitmap_region(dude, 228, 202, 15, 10, 547, 545, 0);
	}
	al_draw_rectangle(590, 530, 620, 570, al_map_rgb(0, 0, 255), 5);
	al_draw_text(statusFont, al_map_rgb(255, 255, 255), 580, 575, 0, "WEAPON");
	if (weapon == "sword") {
		al_draw_bitmap_region(dude, 96, 294, 11, 23, 600, 540, 0);
	}
	if (weapon == "super sword") {
		al_draw_bitmap_region(dude, 96, 339, 11, 24, 600, 540, 0);
	}
	al_draw_textf(statusFont, al_map_rgb(255, 255, 255), 10, 525, 0, "CURRENT LEVEL: %d", level);
	al_draw_textf(statusFont, al_map_rgb(255, 255, 255), 10, 540, 0, "TIME REMAINING: %d", 180 - gameTime);

	//End of game flag
	if (flag == 1) {
		al_draw_textf(statusFont, al_map_rgb(255, 255, 255), 200, 540, 0, "TOTAL GAME TIME: %d", gameTime);
	}

}


//Check current block value and pickup items or change layers
void Player::pickUp(int currBlock)
{
	switch (currBlock)
	{
	case 12: //Change layer 1 to layer 2 (inside cave)
		MapChangeLayer(1);
		level++;
		al_play_sample(stairs, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, 0);
		break;
	case 23: //Change layer 2 to layer 3
		MapChangeLayer(2);
		level++;
		al_play_sample(stairs, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, 0);
		break;
	case 61: //Sword pickup
		MapSetBlock(x / 32, y / 32, 354);
		al_play_sample(pickup, 1, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
		weapon = "sword";
		weaponDamage = 3;
		break;
	case 62: //Heart pickup grass
		al_play_sample(pickup, 1, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
		MapSetBlock(x / 32, y / 32, 354);
		health++;
		break;
	case 63: //Super sword pickup
		MapSetBlock(x / 32, y / 32, 165); //Change sword tile into ground
		al_play_sample(pickup, 1, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
		weapon = "super sword";
		weaponDamage = 6;
		break;
	case 64: //Red tunic pickup
		MapSetBlock(x / 32, y / 32, 165); //Change tunic tile to ground
		al_play_sample(pickup, 1, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
		tunic = "red";
		break;
	case 65: //Heart pickup dirt
		MapSetBlock(x / 32, y / 32, 165);
		al_play_sample(pickup, 1, 0, 1.5, ALLEGRO_PLAYMODE_ONCE, 0);
		health++;
		break;
	case 99: //Trophy pickup
		MapSetBlock(x / 32, y / 32, 670); //Change trophy to ground
		win = true;
		break;
	}

}



void Player::superSwordAttack(char dir, int xOff, int yOff, int attackFX)
{

	switch (dir)
	{
	case 'u':
		al_draw_bitmap_region(dude, attackFX + 74, 132, 25, 43, x - xOff, y - 25 - yOff, 0);
		al_draw_bitmap_region(dude, 298, 246, 15, 5, x + 3 - xOff, y - 30 - yOff, 0);
		if (attackDelay < 7) {
			al_draw_bitmap_region(dude, 297, 241, 17, 6, x + 4 - xOff, y - 35 - yOff, 0);
		}
		if (attackDelay < 4) {
			al_draw_bitmap_region(dude, 295, 235, 21, 7, x - xOff, y - 40 - yOff, 0);
		}
		if (attackDelay < 2) {
			al_draw_bitmap_region(dude, 294, 226, 23, 9, x - xOff, y - 45 - yOff, 0);
		}
		break;
	case 'd':
		al_draw_bitmap_region(dude, attackFX, 150, 24, 42, x - xOff, y - 5 - yOff, 0);
		al_draw_bitmap_region(dude, 298, 255, 15, 5, x + 5 - xOff, y + 40 - yOff, 0);
		if (attackDelay < 7) {
			al_draw_bitmap_region(dude, 297, 259, 17, 6, x + 4 - xOff, y + 45 - yOff, 0);
		}
		if (attackDelay < 4) {
			al_draw_bitmap_region(dude, 295, 264, 21, 7, x + 2 - xOff, y + 50 - yOff, 0);
		}
		if (attackDelay < 2) {
			al_draw_bitmap_region(dude, 294, 271, 23, 8, x + 2 - xOff, y + 55 - yOff, 0);
		}
		break;
	case 'l':
		al_draw_bitmap_region(dude, attackFX + 28, 148, 41, 23, x - 20 - xOff, y - 5 - yOff, 0);
		al_draw_bitmap_region(dude, 289, 246, 6, 14, x - 25 - xOff, y + 5 - yOff, 0);
		if (attackDelay < 7) {
			al_draw_bitmap_region(dude, 284, 245, 5, 16, x - 30 - xOff, y - yOff, 0);
		}
		if (attackDelay < 4) {
			al_draw_bitmap_region(dude, 276, 242, 8, 21, x - 35 - xOff, y - yOff, 0);
		}
		if (attackDelay < 2) {
			al_draw_bitmap_region(dude, 268, 242, 8, 22, x - 40 - xOff, y - yOff, 0);
		}
		break;
	case 'r':
		al_draw_bitmap_region(dude, attackFX + 101, 149, 41, 23, x - xOff, y - 5 - yOff, 0);
		al_draw_bitmap_region(dude, 316, 246, 6, 14, x + 45 - xOff, y - yOff, 0);
		if (attackDelay < 7) {
			al_draw_bitmap_region(dude, 322, 245, 5, 16, x + 50 - xOff, y - yOff, 0);
		}
		if (attackDelay < 4) {
			al_draw_bitmap_region(dude, 327, 242, 8, 21, x + 55 - xOff, y - 3 - yOff, 0);
		}
		if (attackDelay < 2) {
			al_draw_bitmap_region(dude, 335, 242, 8, 22, x + 60 - xOff, y - 5 - yOff, 0);
		}
		break;
	}
}