#include "Timmy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Timmy::Timmy() : Entity(EntityType::TIMMY)
{
	name.Create("Timmy");
	level = 6;
	health = 25;
	maxHealth = 25;
	attack = 13;
	defense = 7;
	magic = 0;
	resistance = 4;
	speed = 13;
	skill = 15;
	stamina = 15;
	maxStamina = 15;
}

Timmy::~Timmy() {

}

bool Timmy::Awake()
{
	return true;
}

bool Timmy::Start()
{
	return true;
}

bool Timmy::Update()
{
	return true;
}

bool Timmy::PostUpdate()
{
	return true;
}

bool Timmy::CleanUp()
{
	return true;
}

int Timmy::Attack()
{
	stamina -= 1;
	return attack;

}

int Timmy::Ability(int id)
{
	//If ability 1 is selected
	if (id == 1) {
		stamina -= 3;
		if (Ab1Type == 1) {
			
			return attack * Ab1Power;

		}
		if (Ab1Type == 2) {

			return magic * Ab1Power;

		}
		if (Ab1Type == 3) {

			return healingpower;

		}
	}
	//If ability 2 is selected
	if (id == 2) {
		stamina -= 5;
		if (Ab1Type == 1) {

			return attack * Ab1Power;

		}
		if (Ab1Type == 2) {

			return magic * Ab1Power;

		}
		if (Ab1Type == 3) {

			return healingpower * 1.5;

		}
	}
}

void Timmy::Movement() {

}
