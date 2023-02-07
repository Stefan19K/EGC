#pragma once

#include <vector>
#include "lab_m1/Tema3/Structs3.h"
#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include <core/gpu/mesh.h>
#include <core/gpu/shader.h>

using namespace std;
using namespace glm;

class Obj
{
public:
	Obj();
	~Obj();

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture);

protected:
	vector<Component3> components;
	vec3 translation;
	vec3 rotation;
	vec3 scalation;
};