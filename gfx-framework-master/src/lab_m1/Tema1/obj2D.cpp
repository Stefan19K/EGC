#include "obj2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

#define FULL_CIRCLE 360
#define PI 3.141592

Mesh* obj2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* obj2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* obj2D::CreateEllipse(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    int triangles,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices;
    float measure{ 0 };
    float step{ (float)FULL_CIRCLE / triangles };

    vertices.push_back(VertexFormat(corner, color));
    for (int i{ }; i < triangles; ++i) {
        int x = corner.x + width * cos(measure * PI / 180);
        int y = corner.y + height * sin(measure * PI / 180);

        vertices.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
        measure += step;
    }

    Mesh* ellipse = new Mesh(name);
    std::vector<unsigned int> indices;
    for (int i{ }; i <= triangles; ++i) {
        indices.push_back(i);
    }

    if (!fill) {
        indices.erase(indices.begin());
        ellipse->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(1);
        ellipse->SetDrawMode(GL_TRIANGLE_FAN);
    }
    
    ellipse ->InitFromData(vertices, indices);
    return ellipse;
}

Mesh* obj2D::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    glm::vec3 corner2,
    glm::vec3 corner3,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),
        VertexFormat(corner2, color),
        VertexFormat(corner3, color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = {0, 1, 2};

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        triangle->SetDrawMode(GL_TRIANGLES);
    }
    
    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* obj2D::CreateLine(
    const std::string& name, 
    glm::vec3 corner1, 
    glm::vec3 corner2, 
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner1, color),
        VertexFormat(corner2, color),
    };

    Mesh* line = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1 };

    line->SetDrawMode(GL_LINES);

    line->InitFromData(vertices, indices);
    return line;
}
