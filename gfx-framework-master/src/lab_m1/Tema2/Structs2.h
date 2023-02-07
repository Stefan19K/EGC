#pragma once

#include <vector>
#include <iostream>

#include <core/gpu/vertex_format.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/Macros.h"
#include "utils/glm_utils.h"

using namespace std;

//enum GameState {
//	MENU,
//	RUNNING,
//	RETRY
//};

struct Component2
{
	string name;
	glm::mat4 matrix;
};

struct Tree
{
	float height;
	glm::vec3 location;
	std::vector<glm::vec3> leaves;
};

struct Light {
	glm::vec3 pos;
	float rotation;
};

struct Track {
	std::vector<glm::vec3> trackSk;
	std::vector<float> distances;
	float length;

	Track() : length(0.0f) {}
	Track(std::vector<glm::vec3> points) : trackSk(points), distances(std::vector<float>(trackSk.size())), length(0.0f) {}
};

struct Car
{
	vector<Component2> components;
	vector<Light> lights;
	glm::vec3 center;
	glm::vec3 direction;
	float acceleration;
	float rotation;
	float wheelRotation;
	glm::vec3 position;
	glm::vec2 hitbox;
	bool collision;

	Car() : center(glm::vec3(0.0f)), direction(glm::vec3(1, 0, 0)), acceleration(0), rotation(0), wheelRotation(0), position(glm::vec3(0)), collision(false) {}
	Car(Car& car) :
		center(glm::vec3(-1, 0, 0)),
		components(car.components),
		direction(glm::vec3(0)),
		acceleration(car.acceleration), 
		rotation(car.rotation), 
		wheelRotation(car.wheelRotation), 
		position(car.position),
		hitbox(car.hitbox),
		collision(false) {}
};

struct EnemyCar : Car {
	float posLength;
	float offset;

	EnemyCar(Car& car) : Car(car), posLength(0.0f), offset(0.0f) {}
};

void InitializeCarComponents(Car& car);
void InitializeWheels(Car& car);
void InitializeBottomBody(Car& car);
void InitializeTopBody(Car& car);
void InitializeBack(Car& car);
void InitializeFront(Car& car);

std::vector<glm::vec3> GenerateTrackSkeleton();
glm::vec3 GetSplinePoint(std::vector<glm::vec3> points, float t, bool loop = true);
glm::vec3 GetSplineGradient(std::vector<glm::vec3> points, float t, bool loop = true);

void GenerateTreeLocations(std::vector<Tree>& treeLocations, std::vector<VertexFormat> vertices);
Tree GenerateTree(glm::vec3 location);

void SetTrackLength(Track& track);
void SetEnemyPositions(std::vector<EnemyCar> &enemies, Track track);
void ConvertLengthToPoint(std::vector<EnemyCar>& enemies, Track track);
std::pair<glm::vec3, float> ConvertLengthToPoint(float length, Track track);

void SetLights(std::vector<Light>& lights, Track track);

