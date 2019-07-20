#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include "mappy_A5.h"
#include "Player.h"
#include "Enemy.h"

int collided(int x, int y);  //Tile Collision
bool endValue(int x, int y); //End Block with the User Value = l

int main()
{
	const int MAXWIDTH = 1589;
	const int MAXHEIGHT = 1568;
	const int WIDTH = 900;
	const int PLAYHEIGHT = 520; //Gameplay height
	const int SCREENHEIGHT = 600; //Extra for status
	const int FPS = 60;

	bool keys[] = { false, false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };

	bool start = false; //Turn true to begin the game
	bool gameOver = false;
	bool update = false;
	bool levelUp = false;
	bool spaceDown = false;

	const int numEnemy = 6; //Number of enemies to spawn per level. Likes to crash at higher numbers
	int level = 1; //Starting level
	int xOff = 0;
	int yOff = 0;
	int gameTime = 0.0;
	int totalTime = 0;


	//---------------------------------------STARTUP START
	//Allegro startup
	if (!al_init()) { std::cout << "error starting allegro"; return -1; }


	//Addons
	al_install_keyboard();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();


	//Display setup
	ALLEGRO_DISPLAY* display = al_create_display(WIDTH, SCREENHEIGHT);
	if (!display) { std::cout << "error creating display"; return -1; }
	al_set_window_title(display, "Assignment 5 - Justin Adams");


	//Map load
	if (MapLoad("test.fmp", 1)) { std::cout << "error loading map"; return -5; }
	ALLEGRO_BITMAP* intro = al_load_bitmap("intro.png"); //Intro card with instructions
	ALLEGRO_BITMAP* endWin = al_load_bitmap("endWin.png"); //End card if the player wins
	ALLEGRO_BITMAP* endLose = al_load_bitmap("endLose.png"); //End card if the player runs out of time
	ALLEGRO_BITMAP* star = al_load_bitmap("star.png"); //Star that engulfs the player if they win

	//Timer setup
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);


	//Music setup
	ALLEGRO_SAMPLE* level1 = al_load_sample("level1.wav"); //Background music
	ALLEGRO_SAMPLE* introMusic = al_load_sample("intro.wav"); //Instructions intro music
	ALLEGRO_SAMPLE* win = al_load_sample("win.wav"); //Winning noise
	al_reserve_samples(10);


	//Font setup
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT* gameFont = al_load_font("gameFont.ttf", 12, 0);
	ALLEGRO_FONT* creditFont = al_load_font("creditFont.ttf", 24, 0);

	//Event setup
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Player setup
	Player player;
	player.load(); //Load player assets
	player.spawn(64, 1504); //Spawn the player
	Enemy enemy[numEnemy];

	//--------------------------------------STARTUP END


	//Play intro and instructions
	while (!start)
	{
		al_play_sample(introMusic, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);
		xOff = player.getX() - WIDTH / 4;
		yOff = player.getY() - PLAYHEIGHT / 4;

		if (xOff < 0) xOff = 0;

		if (xOff > (mapwidth * mapblockwidth - WIDTH))
			xOff = mapwidth * mapblockwidth - WIDTH;
		if (yOff < 0)
			yOff = 0;
		if (yOff > (mapheight * mapblockheight - PLAYHEIGHT))
			yOff = mapheight * mapblockheight - PLAYHEIGHT;

		MapUpdateAnims();
		MapDrawBG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT);
		MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 0);
		al_draw_tinted_bitmap(intro, al_map_rgba_f(1, 1, 1, .75), 200, 50, 0); //Draw the instruction card
		al_flip_display();

		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			start = true;
			al_stop_samples();
		}
	}

	//Start the timer and begin background music
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_start_timer(timer);
	al_play_sample(level1, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, 0);


	//--------------------------------------GAME LOOP
	while (!gameOver)
	{
		gameTime = al_get_timer_count(timer) / 60; //Converts timer into seconds
		al_wait_for_event(event_queue, &ev);

		//If the player can't beat the level in the set time
		if (gameTime > 180)
		{
			gameOver = true;
		}

		//Provides program input from queue based on timer
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (keys[SPACE] && spaceDown) {
				player.attack(enemy, numEnemy);
				spaceDown = false;
			}
			else {
				if (keys[UP]) {
					player.move('u', xOff, yOff, enemy, numEnemy);
				}
				if (keys[DOWN]) {
					player.move('d', xOff, yOff, enemy, numEnemy);
				}
				if (keys[LEFT]) {
					player.move('l', xOff, yOff, enemy, numEnemy);
				}
				if (keys[RIGHT]) {
					player.move('r', xOff, yOff, enemy, numEnemy);
				}
			}

			for (int i = 0; i < numEnemy; i++)
			{
				enemy[i].spawn(player.getLevel());
				enemy[i].move(WIDTH, PLAYHEIGHT);
			}

			update = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { gameOver = true; }

		//Get key inputs and add to event queue
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				gameOver = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				if (!spaceDown) {
					keys[SPACE] = true;
					spaceDown = true;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		//Draw all changes
		if (update && al_event_queue_is_empty(event_queue))
		{
			update = false;

			xOff = player.getX() - WIDTH / 4;
			yOff = player.getY() - PLAYHEIGHT / 4;
			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - PLAYHEIGHT))
				yOff = mapheight * mapblockheight - PLAYHEIGHT;

			MapUpdateAnims();
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT);
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 0);
			for (int i = 0; i < numEnemy; i++) {
				enemy[i].update(xOff, yOff, player.getLevel());
			}
			if (!player.update(xOff, yOff)) { gameOver = true; }
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 1);
			player.statusDraw(gameTime, 0);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		if (player.getWin()) { //Player reaches the end of the game
			gameOver = true;
		}
	}


	// ----------------- Game over stuff starts here --------------------------- //
	MapDrawBG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT);
	MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 0);
	player.update(xOff, yOff);
	player.statusDraw(gameTime, 1);
	MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 1);
	//If player is a winner
	if (gameOver && player.getWin())
	{
		al_play_sample(win, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
		double j = 0;
		int i = 0;
		while (j < 5) {
			al_clear_to_color(al_map_rgb(0, 0, 0));
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT);
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 0);
			player.update(xOff, yOff);
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 1);
			al_draw_scaled_rotated_bitmap(star, 150, 150, player.getX() - xOff, player.getY(), j, j, i, 0); //Draws rotating growing star based on player
			al_flip_display();
			al_rest(.1);
			j += .5;
			i += 1;
		}
		al_draw_tinted_bitmap(endWin, al_map_rgba_f(1, 1, 1, .75), player.getX() - xOff, PLAYHEIGHT / 4, 0);
	}
	//If player quits early or runs out of time
	else
	{
		MapDrawBG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT);
		MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 0);
		player.update(xOff, yOff);
		MapDrawFG(xOff, yOff, 0, 0, WIDTH, PLAYHEIGHT, 1);
		al_draw_tinted_bitmap(endLose, al_map_rgba_f(1, 1, 1, .75), player.getX() - xOff, PLAYHEIGHT / 4, 0);
	}
	al_flip_display();
	al_rest(3);
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();
	//End Credits
	al_draw_text(creditFont, al_map_rgb(255, 255, 255), 350, 100, 0, "GAME RESULTS");
	al_draw_textf(creditFont, al_map_rgb(255, 255, 255), 100, 200, 0, "TOTAL TIME: %d", gameTime);
	al_draw_textf(creditFont, al_map_rgb(255, 255, 255), 500, 200, 0, "RUPEES: %d", player.getRupees());
	al_flip_display();
	al_rest(5);

	//Destroy all the pointers
	al_uninstall_keyboard();
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_sample(level1);
	al_destroy_sample(introMusic);
	al_destroy_sample(win);
	al_destroy_font(gameFont);
	al_destroy_display(display);
	return 0;
	// ----------------- Game over stuff ends ------------------------------ //
}

int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

//End block check -> User1 = 1
bool endValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 99)
	{
		return true;
	}
	else
		return false;
}