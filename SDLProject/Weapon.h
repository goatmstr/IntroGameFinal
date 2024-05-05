#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#include <string>
#endif

#define GL_GLEXT_PROTOTYPES 1

class Weapon {
public: 
	Weapon(std::string name, int damage, float accuracy, int durability) : name(name), damage(damage), accuracy(accuracy), durability(durability) {}

	std::string get_name() const { return name; }
	int get_dmg() const { return damage; }
	int get_acc() const { return accuracy; }
	int get_dur() const { return durability; }

	void use() {
		durability--;
	}
private:
	std::string name;
	int damage;
	int accuracy;
	int durability;
};


