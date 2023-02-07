#include "lab_m1/Spline/Spline.h"

#include <vector>
#include <string>
#include <iostream>
#include <lab_m1/Tema1/Macros.h>
#include <lab_m1/Tema2/obj3D.h>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Spline::Spline()
{
}


Spline::~Spline()
{
}


void Spline::Init()
{
    renderCameraTarget = false;
    // ToggleGroundPlane();

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters

    left = -10.0f;
    right = 10.0f;
    bottom = -10.0f;
    top = 10.0f;
    zNear = 0.01f;
    zFar = 200.0f;

    fov = RADIANS(60);
    aspect = window->props.aspectRatio;

    projection = 0;

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    float measure = FULL_CIRCLE / 32;
    for (int i{}; i < 32; ++i) {
        controlPoints.push_back(glm::vec3(3 * sin(i * measure * PI / 180), 3 * cos(i * measure * PI / 180), 0));
    }

    selected = 0;

    Mesh* window = obj3D::CreateSquare("window", DARK_GREY - glm::vec3(0.2f));
    AddMeshToList(window);
}


void Spline::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Spline::Update(float deltaTimeSeconds)
{
    for (auto point : controlPoints) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, point);
        RenderMesh(meshes["window"], shaders["VertexColor"], glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f)));
    }

    for (float t{ 0.0f }; t < (float)controlPoints.size(); t += 0.01f) {
        glm::vec3 p = GetSplinePoint(controlPoints, t, true);
        glm::vec3 g = GetSplineGradient(controlPoints, t, true);

        glm::vec3 pp = GetSplinePoint(controlPoints, t, true);
        glm::vec3 gg = GetSplineGradient(controlPoints, t, true);

        float r = atan2(-gg.y, gg.x);
        glm::vec3 p1 = glm::vec3(1.0f * sin(r) + pp.x, 1.0f * cos(r) + pp.y, 0);
        glm::vec3 p2 = glm::vec3(-1.0f * sin(r) + pp.x, -1.0f * cos(r) + pp.y, 0);

        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), p1);
        RenderMesh(meshes["window"], shaders["VertexColor"], glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f)));

        modelMatrix = glm::translate(glm::mat4(1), p2);
        RenderMesh(meshes["window"], shaders["VertexColor"], glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f)));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, p);
        RenderMesh(meshes["window"], shaders["VertexColor"], glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f)));
    }
}


void Spline::FrameEnd()
{
    if (projection)
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    else
        projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Spline::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Spline::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            camera->TranslateForward(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            // TODO(student): Translate the camera downward
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            // TODO(student): Translate the camera upward
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
    else {
        if (window->KeyHold(GLFW_KEY_W)) {
            // TODO(student): Translate the camera forward
            controlPoints[selected].y += 2 * deltaTime;
            // car.translation += glm::vec3(30.0f * deltaTime, 0, 0);
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            // TODO(student): Translate the camera to the left
            controlPoints[selected].x -= 2 * deltaTime;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            // TODO(student): Translate the camera backward
            controlPoints[selected].y -= 2 * deltaTime;
            // car.translation -= glm::vec3(30.0f * deltaTime, 0, 0);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            // TODO(student): Translate the camera to the right
            controlPoints[selected].x += 2 * deltaTime;
        }
    }

    // TODO(student): Change projection parameters. Declare any extra
    // variables you might need in the class header. Inspect this file
    // for any hardcoded projection arguments (can you find any?) and
    // replace them with those extra variables.
    if (window->KeyHold(GLFW_KEY_U) && projection == 0) {
        // TODO(student): Translate the camera upward
        fov += 0.5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_I) && projection == 0) {
        // TODO(student): Translate the camera upward
        fov -= 0.5 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Y) && projection == 1) {
        // TODO(student): Translate the camera upward
        right += 50 * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_T) && projection == 1) {
        // TODO(student): Translate the camera upward
        right -= 50 * deltaTime;
    }
}


void Spline::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_P)
    {
        projection = 0;
    }

    if (key == GLFW_KEY_O)
    {
        projection = 1;
    }

    if (key == GLFW_KEY_2) {
        // TODO(student): Translate the camera upward
        if (selected == controlPoints.size() - 1)
            selected = 0;
        else
            selected += 1;
    }

    if (key == GLFW_KEY_1) {
        // TODO(student): Translate the camera upward
        if (selected == 0)
            selected = controlPoints.size() - 1;
        else
            selected -= 1;
    }

    if (key == GLFW_KEY_ENTER) {
        cout << "std::vector<glm::vec3> res = {\n";
        for (auto point : controlPoints) {
            cout << "    glm::vec3(" << point.x << "f, " << point.y << "f, " << 0.0f << "),\n";
        }

        cout << "\n};";
    }
}


void Spline::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Spline::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            float angleX = (-deltaX) * sensivityOY;
            float angleY = (-deltaY) * sensivityOX;
            camera->RotateFirstPerson_OY(angleX);
            camera->RotateFirstPerson_OX(angleY);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            float angleX = (-deltaX) * sensivityOY;
            float angleY = (-deltaY) * sensivityOX;
            camera->RotateThirdPerson_OY(angleX);
            camera->RotateFirstPerson_OX(angleY);
        }
    }
}


void Spline::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Spline::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Spline::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Spline::OnWindowResize(int width, int height)
{
}

glm::vec3 Spline::GetSplinePoint(std::vector<glm::vec3> points, float t, bool loop)
{
    unsigned int p0, p1, p2, p3;
    if (!loop) {
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
    }
    else {
        p1 = (int)t;
        p2 = (p1 + 1) % points.size();
        p3 = (p2 + 1) % points.size();
        p0 = p1 > 1 ? p1 - 1 : points.size() - 1;

    }

    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -ttt + 2.0f * tt - t;
    float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
    float q3 = -3.0f * ttt + 4.0f * tt + t;
    float q4 = ttt - tt;

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

    return glm::vec3(tx, ty, 0);
}

glm::vec3 Spline::GetSplineGradient(std::vector<glm::vec3> points, float t, bool loop)
{
    unsigned int p0, p1, p2, p3;
    if (!loop) {
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
    }
    else {
        p1 = (int)t;
        p2 = (p1 + 1) % points.size();
        p3 = (p2 + 1) % points.size();
        p0 = p1 > 1 ? p1 - 1 : points.size() - 1;

    }

    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -3.0f * tt + 4.0f * t - 1.0f;
    float q2 = 9.0f * tt - 10.0f * t;
    float q3 = -9.0f * tt + 8.0f * t + 1.0f;
    float q4 = 3.0f * tt - 2.0f * t;

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float ty = 0.5f * (points[p0].y * q1 + points[p1].y * q2 + points[p2].y * q3 + points[p3].y * q4);

    return glm::vec3(tx, ty, 0);
}
