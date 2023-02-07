#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace obj3D
{
	Mesh* CreateCyllinder(
		const std::string& name, 
		glm::vec3 leftBottomCorner, 
		float width, float length, float height, 
		int triangles, 
		glm::vec3 color);

	Mesh* CreateBox(
		const std::string& name,
		glm::vec3 color);

	Mesh* CreatePrism(
		const std::string& name,
		glm::vec3 color);

	Mesh* CreateSquare(
		const std::string& name,
		glm::vec3 color);

	Mesh* CreateQuad(
		const std::string& name);

	Mesh* CreateEllipse(
		const std::string& name,
		glm::vec3 leftBottomCorner,
		float width,
		float height,
		int triangles,
		glm::vec3 color,
		bool fill = false);

	Mesh* CreateTriangle(
		const std::string& name,
		glm::vec3 color);

	Mesh* CreateTrack(
		const std::string& name,
		glm::vec3 color);

	Mesh* CreateGround(
		const std::string& name,
		glm::vec3 color);
}
