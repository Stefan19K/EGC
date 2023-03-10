#include "lab_m1/lab5/lab5.h"

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


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    renderCameraTarget = false;
    // ToggleGroundPlane();

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 0, 3.5f), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    Mesh* ground = obj3D::CreateGround("ground", GREEN);
    AddMeshToList(ground);

    Mesh* speedMeter = new Mesh("speedMeter");
    speedMeter->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/speed"), "speed.fbx");
    AddMeshToList(speedMeter);

    Mesh* needle = new Mesh("needle");
    needle->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/needle"), "needle.fbx");
    AddMeshToList(needle);

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters

    left = -2.0f;
    right = 2.0f;
    bottom = -2.0f;
    top = 2.0f;
    zNear = 0.01f;
    zFar = 200.0f;

    fov = RADIANS(60);
    aspect = window->props.aspectRatio;

    projection = 0;

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab5::Update(float deltaTimeSeconds)
{
    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));

    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
    //    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    RenderMesh(meshes["ground"], shaders["VertexColor"], modelMatrix);
    //}

    //// TODO(student): Draw more objects with different model matrices.
    //// Attention! The `RenderMesh()` function overrides the usual
    //// `RenderMesh()` that we've been using up until now. This new
    //// function uses the view matrix from the camera that you just
    //// implemented, and the local projection matrix.

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 1, 0));

    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1, 0));
    //    modelMatrix = glm::rotate(modelMatrix, RADIANS(137.0f), glm::vec3(0, 1, 0));

    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    //{
    //    glm::mat4 modelMatrix = glm::mat4(1);
    //    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    //    modelMatrix = glm::translate(modelMatrix, glm::vec3(15, 1, 0));
    //    

    //    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    //}

    {
        RenderMesh(meshes["speedMeter"], shaders["Simple"], glm::mat4(1));
    }

    {
        glm::mat4 matrix = glm::mat4(1);
        matrix = glm::rotate(matrix, RADIANS(-150), glm::vec3(0, 0, 1));
        matrix = glm::translate(matrix, glm::vec3(-0.01f, 0.12f, 0.1f));
        matrix = glm::scale(matrix, glm::vec3(0.5f, 0.5f, 1));
        RenderMesh(meshes["needle"], shaders["Simple"], matrix);
    }
    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


void Lab5::FrameEnd()
{
    if (projection)
        projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    else
        projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Lab5::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
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


void Lab5::OnInputUpdate(float deltaTime, int mods)
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


void Lab5::OnKeyPress(int key, int mods)
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
}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
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


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
