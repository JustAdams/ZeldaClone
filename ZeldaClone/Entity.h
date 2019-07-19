#pragma once

class Entity {

public:
	virtual int getX() = 0; //Forced abstraction method
	int getY();
	void damage(Entity* source, int damage);

protected:
	int x;
	int y;
	int health;
	int speed;

	bool alive;


};