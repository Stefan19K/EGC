#pragma once

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/Macros.h"
#include "utils/glm_utils.h"

using namespace std;

enum GameState {
	MENU,
	RUNNING,
	RETRY
};

enum DuckState {
	STATIC,
	FLYING,
	FALLING,
	ESCAPING
};

enum Gamemode {
	RIFLE,
	SNIPER
};

struct EventManager
{
	bool speedIncrease;
	bool headshot;
	bool randomBounce;
	bool changeSize;
	bool changeColor;

	short speedIncreaseChance;
	short randomBounceChance;
	short changeSizeChance;
	short changeColorChance;

	EventManager() : speedIncrease(false), headshot(false), randomBounce(false), changeSize(false),
		changeColor(false), speedIncreaseChance(0), randomBounceChance(0), changeSizeChance(0),
		changeColorChance(0) {}
};

struct Component
{
	string name;
	float offsetX;
	float offsetY;
	float offsetAngle;

	glm::mat3 matrix;
};

struct Duck 
{
	glm::vec2 direction;
	glm::vec2 speed;
	glm::vec2 offset;

	DuckState state;
	vector<Component> components;
	vector<Component> hitboxes;

	glm::vec3 center;

	float translateX;
	float translateY;
	float scaleX;
	float scaleY;
	float angularStep;
	float lifetime;
	short rotation;

	float wingScale;
	float factor;
	float deathAcceleration;
	float escapingAcceleration;

	short wallHit;
};

struct Heart
{
	short heartCount;

	vector<Component> components;
};

struct Bullet
{
	short bulletCount;

	vector<Component> components;
};

void InitializeStructs(
	GameState &gamestate, 
	Gamemode &gameMode, 
	EventManager &eventManager, 
	Duck &duck, 
	Heart &hearts, 
	Bullet &bullets);

void InitializeDuckComponents(Duck &duck);

glm::mat3 GetTransformationMatrix(Duck duck);

glm::mat3 GetReverseTransformationMatrix(Duck duck);

void TriggerEvents(EventManager &eventManager);

void increaseChances(EventManager &eventManager);