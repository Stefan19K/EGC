#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    Mesh* CreateEllipse(const std::string& name, glm::vec3 leftBottomCorner, float width, float height, int triangles, glm::vec3 color, bool fill = false);

    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float height, glm::vec3 color, bool fill = false);

    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, glm::vec3 corner2, glm::vec3 corner3, glm::vec3 color, bool fill = false);
}
