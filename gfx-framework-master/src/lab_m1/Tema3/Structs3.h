#pragma once

#include <vector>
#include <iostream>

#include <core/gpu/vertex_format.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/Macros.h"
#include "utils/glm_utils.h"

using namespace std;
using namespace glm;

struct Component3
{
	string name;
	string texture;
	vec3 color;
	vec3 center;
	vec3 translation;
	vec3 rotation;
	vec3 scalation;
	mat4 matrix;


	Component3(string nam, vec3 col, vec3 t, vec3 r, vec3 s, string text = "")
		: name(nam), color(col), center(t), translation(t), rotation(r), scalation(s), texture(text)
	{
		SetTransformationMatrix();
	}

private:
	void SetTransformationMatrix() {
		matrix = mat4(1);
		matrix = translate(matrix, translation);
		matrix = rotate(matrix, RADIANS(rotation.x), vec3(1, 0, 0));
		matrix = rotate(matrix, RADIANS(rotation.y), vec3(0, 1, 0));
		matrix = rotate(matrix, RADIANS(rotation.z), vec3(0, 0, 1));
		matrix = scale(matrix, scalation);		
	}
};

struct Obj
{
	vector<Component3> components;
	vec3 center;
	vec3 translation;
	vec3 rotation;
	vec3 scalation;
	mat4 matrix;

	Obj(vec3 t, vec3 r, vec3 s) : translation(t), scalation(s), rotation(r), center(t), matrix(mat4(1)) {}

	void SetTransformationMatrix() {
		matrix = mat4(1);
		matrix = translate(matrix, translation);
		matrix = rotate(matrix, RADIANS(rotation.x), vec3(1, 0, 0));
		matrix = rotate(matrix, RADIANS(rotation.y), vec3(0, 1, 0));
		matrix = rotate(matrix, RADIANS(rotation.z), vec3(0, 0, 1));
		matrix = scale(matrix, scalation);
	}

	void Translate(vec3 vec)
	{
		translation += vec;
		center += vec;
	}
};

struct Skier : public Obj
{
	Skier(vec3 t, vec3 r, vec3 s) : Obj(t, r, s)
	{
		SetComponents();
	}

	Skier() : Obj(ORIGIN, ORIGIN, vec3(1)) 
	{
		SetComponents();
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("box", vec3(0, 0, 1), vec3(-0.645995, 0.00457428, 0.789098), vec3(0, 0, 0), vec3(0.900796, 0.0984749, 3.33838), "board"));
		components.push_back(Component3("box", vec3(0, 0, 1), vec3(0.650919, 0.00457428, 0.766301), vec3(0, 0, 0), vec3(0.900796, 0.0984749, 3.33838), "board"));
		components.push_back(Component3("box", vec3(1, 1, 0), vec3(-0.62514, 0.136173, 0.545167), vec3(0, 0, 0), vec3(0.370208, 0.200076, 0.651745), "shoe"));
		components.push_back(Component3("box", vec3(1, 1, 0), vec3(0.617192, 0.136173, 0.542502), vec3(0, 0, 0), vec3(0.370208, 0.200076, 0.651745), "shoe"));
		components.push_back(Component3("box", vec3(0.4, 0.4, 0.4), vec3(-0.649319, 1.22046, -0.020553), vec3(0, 0, 0), vec3(-0.698069, 2.36665, 0.48321), "denim"));
		components.push_back(Component3("box", vec3(0.4, 0.4, 0.4), vec3(0.654706, 1.22046, -0.0238019), vec3(0, 0, 0), vec3(-0.698069, 2.36665, 0.48321), "denim"));
		components.push_back(Component3("box", vec3(0.4, 0.4, 0.4), vec3(-0.0129122, 2.00006, -0.0198227), vec3(-0.294426, -0.531764, -0.209365), vec3(-0.690105, 0.775461, 0.493699), "denim"));
		components.push_back(Component3("box", vec3(0.5, 1, 1), vec3(0.0430092, 3.63452, 0.0487067), vec3(0, 0, 0), vec3(-2.59927, 2.45822, 0.815181), "shirt"));
		components.push_back(Component3("box", vec3(1, 0.8, 0.5), vec3(-1.74258, 4.13552, -0.199015), vec3(123.747, 0, 0), vec3(-0.978825, 0.46483, -1.12259), "skin"));
		components.push_back(Component3("box", vec3(1, 0.8, 0.5), vec3(1.82335, 4.11847, -0.218314), vec3(123.747, 0, 0), vec3(-0.978825, 0.46483, -1.12259), "skin"));
		components.push_back(Component3("box", vec3(1, 0.8, 0.5), vec3(1.81882, 3.41024, 0.261726), vec3(33.4619, 0, 0), vec3(-0.962496, 0.46483, -1.12259), "skin"));
		components.push_back(Component3("box", vec3(1, 0.8, 0.5), vec3(-1.73855, 3.4107, 0.272813), vec3(34.0366, 0, 0), vec3(-0.969454, 0.46483, -1.12259), "skin"));
		components.push_back(Component3("box", vec3(0, 1, 0), vec3(-1.75008, 2.93146, 1.01798), vec3(31.4581, 0, 0), vec3(-1.12738, 0.698742, -0.726918), "glove"));
		components.push_back(Component3("box", vec3(0, 1, 0), vec3(1.82828, 2.90966, 1.00757), vec3(31.4581, 0, 0), vec3(-1.12789, 0.698742, -0.726918), "glove"));
		components.push_back(Component3("box", vec3(0, 0, 1), vec3(1.82961, 2.09478, 0.458031), vec3(29.2781, 90.4311, 4.77417), vec3(-0.271728, -3.39258, -0.276298), "board"));
		components.push_back(Component3("cyllinder", vec3(1, 0, 0), vec3(1.82391, 0.597109, -0.573013), vec3(123.345, 177.858, 440.783), vec3(-0.271728, -0.240305, -0.264314), "red"));
		components.push_back(Component3("cyllinder", vec3(1, 0, 0), vec3(-1.84481, 0.6409, -0.570647), vec3(123.319, 177.858, 440.783), vec3(-0.271728, -0.240305, -0.264314), "red"));
		components.push_back(Component3("box", vec3(0, 0, 1), vec3(-1.785, 2.07639, 0.40228), vec3(124.423, 177.858, 450.652), vec3(-0.2691, -0.265054, -3.39918), "board"));
		components.push_back(Component3("box", vec3(1, 0.8, 0.5), vec3(-0.0238551, 5.36414, 0.130291), vec3(180.943, 181.482, 630.515), vec3(-1.10696, -1.41572, -1.1512), "skin"));
	}
};

struct Ground : public Obj
{
	vec2 textureTranslation;

	Ground(vec3 t, vec3 r, vec3 s) : Obj(t, r, s), textureTranslation(vec2(0))
	{
		SetComponents();
	}

	Ground() : Obj(ORIGIN, ORIGIN, vec3(1)), textureTranslation(vec2(0))
	{
		SetComponents();
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("quad", vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "snow"));
	}
};

struct TextTree : public Obj
{
	vec3 lightColor;

	TextTree(vec3 t, vec3 r, vec3 s) : Obj(t, r, s)
	{
		SetComponents();
		lightColor = vec3(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);
	}

	TextTree() : Obj(ORIGIN, ORIGIN, vec3(1))
	{
		SetComponents();
		lightColor = vec3(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);
	}

	bool CheckCollision(TextTree tree, Skier skier) 
	{
		float width = tree.components[0].scalation.x * tree.scalation.x;
		vec3 point = skier.center + vec3(0, 0, skier.components[7].scalation.z / 2.0f);
		float x1 = tree.center.x - width / 2.0f;
		float x2 = tree.center.x + width / 2.0f;
		float z1 = tree.center.z - width / 2.0f;
		float z2 = tree.center.z + width / 2.0f;
		if (point.x > x1 && point.x < x2 && point.z > z1 && point.z < z2)
			return true;

		return false;
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("box", vec3(1, 1, 1), vec3(0.00423019, 1.36539, -0.0035467), vec3(0, 0, 0), vec3(1, 2.6973, 1), "wood"));
		components.push_back(Component3("cone", vec3(1, 1, 1), vec3(0.00423019, 4.04965, -0.0354803), vec3(0, 0, 0), vec3(1.8639, 1.34891, 2.31433), "leaves"));
		components.push_back(Component3("cone", vec3(1, 1, 1), vec3(-0.0868578, 5.7281, -0.0187438), vec3(0, 0, 0), vec3(1.24475, 1.34891, 1.24927), "leaves"));
		components.push_back(Component3("cone", vec3(1, 1, 1), vec3(-0.0868578, 4.98149, -0.0187438), vec3(0, 0, 0), vec3(1.44283, 1.34891, 1.63347), "leaves"));
	}
};

struct Rock : public Obj
{
	Rock(vec3 t, vec3 r, vec3 s) : Obj(t, r, s)
	{
		SetComponents();
	}

	Rock() : Obj(ORIGIN, ORIGIN, vec3(1))
	{
		SetComponents();
	}

	bool CheckCollision(Rock rock, Skier skier)
	{
		vec3 point = skier.center;
		float dist = distance(point, rock.translation);
		if (dist < 1 * rock.scalation.x)
			return true;
		
		return false;
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("sphere", vec3(1, 1, 1), vec3(-0.467034, 0, 0), vec3(0, 0, 0), vec3(1.04927, 0.61417, 1.34875), "stone"));
		components.push_back(Component3("sphere", vec3(1, 1, 1), vec3(0.269176, 0, -0.398526), vec3(0, 0, 0), vec3(1.10334, 0.903631, 1.36699), "stone"));
		components.push_back(Component3("sphere", vec3(1, 1, 1), vec3(0.153277, -0.0196596, 0.382983), vec3(0, 0, 0), vec3(0.551177, 0.421924, 0.568705), "stone"));
		components.push_back(Component3("sphere", vec3(1, 1, 1), vec3(-0.00431675, 0.280522, 0.182981), vec3(0, 0, 0), vec3(0.551177, 0.421924, 0.568705), "stone"));
	}
};

struct Lamp : public Obj
{
	Lamp(vec3 t, vec3 r, vec3 s) : Obj(t, r, s)
	{
		SetComponents();
	}

	Lamp() : Obj(ORIGIN, ORIGIN, vec3(1))
	{
		SetComponents();
	}

	bool CheckCollision(Lamp lamp, Skier skier)
	{
		float width = lamp.components[0].scalation.x * lamp.scalation.x;
		vec3 point = skier.center + vec3(0, 0, skier.components[7].scalation.z / 2.0f);
		float x1 = lamp.center.x - width / 2.0f;
		float x2 = lamp.center.x + width / 2.0f;
		float z1 = lamp.center.z - width / 2.0f;
		float z2 = lamp.center.z + width / 2.0f;
		if (point.x > x1 && point.x < x2 && point.z > z1 && point.z < z2)
			return true;

		return false;
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("box", vec3(1, 1, 1), vec3(-0.0021408, 1.76672, -0.0299062), vec3(0, 0, 0), vec3(0.332382, 3.5826, 0.350489), "iron"));
		components.push_back(Component3("box", vec3(1, 1, 1), vec3(-0.0021408, 3.65422, -0.0326844), vec3(0, 0, 0), vec3(3.0796, 0.196385, 0.868545), "iron"));
		components.push_back(Component3("square", vec3(0.4, 0.4, 0.4), vec3(1.19685, 3.53808, -0.0165129), vec3(-91.8964, 0, 0), vec3(0.530432, 0.574586, 0.566385), "grey"));
		components.push_back(Component3("square", vec3(0.4, 0.4, 0.4), vec3(-1.18346, 3.53808, -0.0182042), vec3(-91.8964, 0, 0), vec3(0.537767, 0.574586, 0.566385), "grey"));
	}
};

struct Present : public Obj
{
	vec3 lightColor;

	Present(vec3 t, vec3 r, vec3 s) : Obj(t, r, s)
	{
		SetComponents();
		lightColor = vec3(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);
	}

	Present() : Obj(ORIGIN, ORIGIN, vec3(1))
	{
		SetComponents();
		lightColor = vec3(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f);
	}

	bool CheckCollision(Present present, Skier skier)
	{
		float widthX = present.components[0].scalation.x * present.scalation.x;
		float widthZ = present.components[0].scalation.z * present.scalation.z * cos(RADIANS(present.rotation.x));
		vec3 point = skier.center;
		float x1 = present.center.x - widthX / 2.0f;
		float x2 = present.center.x + widthX / 2.0f;
		float z1 = present.center.z - widthZ / 2.0f;
		float z2 = present.center.z + widthZ / 2.0f;
		if (point.x > x1 && point.x < x2 && point.z > z1 && point.z < z2)
			return true;

		return false;
	}

	void SetTexture(string text)
	{
		components[0].texture = text;
	}

	string GetTexture()
	{
		return components[0].texture;
	}

private:
	void SetComponents()
	{
		components.push_back(Component3("box", vec3(1, 1, 1), vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "present"));
	}
};

struct Bulbs
{
	vector<vec3> lights;
	vector<vec3> colors;
	vector<unsigned short> resetCounters;
	vector<unsigned short> colIDs;

	Bulbs()
	{
		lights.push_back(ORIGIN);
		colors.push_back(WHITE);
		resetCounters.push_back(0);
		colIDs.push_back(0);
	}

	void PushBack(vec3 light)
	{
		int r = rand() % 5;
		vec3 lightColor;
		unsigned short colID;
		unsigned short resetCounter;
		switch (r)
		{
		case 0:
			lightColor = BLUE;
			colID = 0;
			break;

		case 1:
			lightColor = RED;
			colID = 1;
			break;

		case 2:
			lightColor = YELLOW;
			colID = 2;
			break;

		case 3:
			lightColor = GREEN;
			colID = 3;
			break;

		case 4:
			lightColor = PINK;
			colID = 4;
			break;

		default:
			lightColor = YELLOW;
			colID = 2;
		}

		resetCounter = 0;

		this->lights.push_back(light);
		this->colors.push_back(lightColor);
		this->colIDs.push_back(colID);
		this->resetCounters.push_back(resetCounter);
	}

	void Erase(int pos)
	{
		this->lights.erase(this->lights.begin() + pos);
		this->colors.erase(this->colors.begin() + pos);
		this->colIDs.erase(this->colIDs.begin() + pos);
		this->resetCounters.erase(this->resetCounters.begin() + pos);
	}

	void Clear() {
		this->lights.erase(this->lights.begin() + 1, this->lights.end());
		this->colors.erase(this->colors.begin() + 1, this->colors.end());
		this->colIDs.erase(this->colIDs.begin() + 1, this->colIDs.end());
		this->resetCounters.erase(this->resetCounters.begin() + 1, this->resetCounters.end());
	}

	void CycleColors(int pos)
	{
		this->colIDs[pos] = this->colIDs[pos] < 4 ? this->colIDs[pos] + 1 : 0;
		switch (this->colIDs[pos])
		{
		case 0:
			this->colors[pos] = BLUE;
			break;

		case 1:
			this->colors[pos] = RED;
			break;

		case 2:
			this->colors[pos] = YELLOW;
			break;

		case 3:
			this->colors[pos] = GREEN;
			break;

		case 4:
			this->colors[pos] = PINK;
			break;
		}
	}
};

bool IsOnGround(Obj object, Ground ground);
