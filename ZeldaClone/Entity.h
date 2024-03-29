#pragma once

class Entity {

public:
	virtual int getX() = 0; //Forced abstraction method
	int getY();
	void damage(Entity* source, int damage);

protected:
	int x;
	int y;
	int maxHealth;
	int health;
	int currBlock;
	int speed;
	bool alive;


};