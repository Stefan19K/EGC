#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!
    position.x = 5;
    position.y = 0;
    position.z = 0;

    clrScr.x = 0;
    clrScr.y = 0;
    clrScr.z = 0;

    // bunnyPosition = glm::vec3(0, 0, 0);
    bunnyPosition.x = 5;
    bunnyPosition.y = 0;
    bunnyPosition.z = 0;

    counter = 0;

    acceleration = 0.0f;
    gravity = -0.1f;
    bunnyState = 0;
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

    {
        Mesh* mesh = new Mesh("model1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("model2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("model3");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("model4");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(clrScr.x, clrScr.y, clrScr.z, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties
    RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(meshes["model2"], bunnyPosition, glm::vec3(0.1f, 0.1f, 0.1f));

    if (counter == 0)
        RenderMesh(meshes["box"], glm::vec3(-10, 0, 0));

    if (counter == 1)
        RenderMesh(meshes["model3"], glm::vec3(-10, 0, 0));

    if (counter == 2)
        RenderMesh(meshes["model4"], glm::vec3(-10, 0, 0));

    if (acceleration != 0 && bunnyState == 1) {
        bunnyPosition.y += acceleration;
        acceleration += gravity;
        
    }

    if (acceleration < 0 && bunnyState == 1) {
        bunnyState = 2;
        gravity *= -1;
    }

    if (acceleration <= 1 && bunnyState == 2) {
        bunnyPosition.y -= acceleration;
        acceleration += gravity;
        
    }

    if (acceleration > 1) {
        acceleration = 0;
        bunnyState = 0;
        gravity *= -1;
    }
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    float speed = 2;

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

    if (window->KeyHold(GLFW_KEY_I)) {
        bunnyPosition.y += deltaTime * speed;
    }

    if (window->KeyHold(GLFW_KEY_J)) {
        bunnyPosition.x -= deltaTime * speed;
    }

    if (window->KeyHold(GLFW_KEY_K)) {
        bunnyPosition.y -= deltaTime * speed;
    }

    if (window->KeyHold(GLFW_KEY_L)) {
        bunnyPosition.x += deltaTime * speed;
    } 
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    //if (key == GLFW_KEY_F) {
        // TODO(student): Change the values of the color components.

    //}

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if (key == GLFW_KEY_Z) {
        clrScr.x = 1;
        clrScr.y = 1;
        clrScr.z = 0;
    }

    if (key == GLFW_KEY_X) {
        clrScr.x = 0;
        clrScr.y = 0;
        clrScr.z = 0;
    }

    if (key == GLFW_KEY_C) {
        if (counter == 2)
            counter = 0;
        else
            ++counter;
    }

    if (key == GLFW_KEY_SPACE && bunnyState == 0) {
        acceleration = 1;
        bunnyState = 1;
    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
