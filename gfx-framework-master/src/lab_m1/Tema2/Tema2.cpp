#include "lab_m1/Tema2/Tema2.h"
#include "lab_m1/Tema1/Macros.h"
#include "lab_m1/Tema2/obj3D.h"
#include "lab_m1/Tema1/obj2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    polygonMode = GL_FILL;

    renderCameraTarget = false;

    left = -7.5f;
    right = 7.5f;
    bottom = -7.5f;
    top = 7.5f;
    zNear = 0.01f;
    zFar = 200.0f;

    speedometerAngle = MINANGLE;

    fov = RADIANS(60);
    aspect = window->props.aspectRatio;

    projection = 0;

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    skyColor = CYAN;

    CreateMeshes();
    GenerateTreeLocations(treeLocations, meshes["track1"]->vertices);
    car = Car();
    car.position = glm::vec3(0, 0, -1);
    car.rotation = FULL_CIRCLE / 2;
    car.hitbox = glm::vec2(9.0f, 4.0f);
    InitializeCarComponents(car);
    track = Track(GenerateTrackSkeleton());
    SetTrackLength(track);

    camera = new implemented::Camera();
    camera->Set(glm::vec3(car.center.x + 0.8f, car.center.y + 0.45f, 0), glm::vec3(0, 0.3f, 0), glm::vec3(0, 1, 0));

    for (int i{}; i < CAR_ENEMIES; ++i) {
        enemies.push_back(EnemyCar(car));
    }

    SetEnemyPositions(enemies, track);

    scaleFactor = 0.0f;
    nightMode = 0;
    frontCamera = 0;
    faze = 0;
    breakOn = 0;
    backOn = 0;

    materialShininess = 30;
    materialKd = 0.5f;
    materialKs = 5;

    /*Set up the minimap used for the car game.*/
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(resolution.x / 25.6f, resolution.y / 14.4f, resolution.x / 5.0f, resolution.y / 5.0f);
    miniViewportArea2 = ViewportArea(resolution.x * 0.775f, resolution.y * 0.6f, resolution.y / 3.0f, resolution.y / 3.0f);

    SetLights(lights, track);
    for (Light& light : lights) {
        lightPositions.push_back(light.pos);
    }
    int i = 0;
    for (auto component : car.components) {
        if (component.name == "stop1" || component.name == "stop2") {
            glm::mat4 auxMatrix = glm::mat4(1);

            /*Set parameters to render car in scene at determined position.*/
                
            auxMatrix = glm::translate(auxMatrix, car.position);
            auxMatrix = glm::rotate(auxMatrix, RADIANS(car.rotation), glm::vec3(0, 1, 0));
            auxMatrix = glm::scale(auxMatrix, glm::vec3(0.06f, 0.03f, 0.06f));

            /*Translate the car in the scene origin so that it's center is in the origin.*/
            auxMatrix = glm::translate(auxMatrix, glm::vec3(-2, 0.9f, -1.48f));

            /*Render the car in a place in the scene.*/
            auxMatrix *= component.matrix;
            glm::vec4 tmp = auxMatrix * glm::vec4(ORIGIN, 1);
            lightPositions.push_back(glm::vec3(tmp.x, tmp.y, tmp.z));
        }
        
    }

    /*Set up the shaders used for the car game.*/
    Shader* shader = new Shader("SplineShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Spline", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Spline", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    shader = new Shader("ColorShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Color", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "Color", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);
}


void Tema2::Update(float deltaTimeSeconds)
{
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
    if (frontCamera == 0)
        camera->Set(car.position - car.direction + glm::vec3(0, 0.45f, 0), car.position + OFFSET, glm::vec3(0, 1, 0));
    else
        camera->Set(car.position + glm::vec3(0, 0.15f, 0), car.position + car.direction + OFFSET - glm::vec3(0, 0.2f, 0), glm::vec3(0, 1, 0));

    ConvertLengthToPoint(enemies, track);
    RenderTrack();
    RenderCar(car);
    RenderGround();
    RenderTrees();
    RenderLights();
    for (Car& enemyCar : enemies) {
        RenderCar(enemyCar);
    }

    car.position += car.direction * car.acceleration * deltaTimeSeconds;

    CheckWallCollision();
    if (car.collision) {
        car.position -= car.direction * car.acceleration * deltaTimeSeconds;
        car.acceleration = 0;
    }
    car.collision = false;

    for (EnemyCar& enemy : enemies) {
        CheckEnemyCollision(car, enemy);
    }

    if (car.collision) {
        car.position -= car.direction * car.acceleration * deltaTimeSeconds;
        car.acceleration = 0;
    }
    car.collision = false;

    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    camera->Set(car.position + glm::vec3(0.0f, 6.0f, 0.0f), car.position + glm::vec3(0, 0.25f, 0), glm::vec3(1, 0, 0));
    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
    RenderTrack();
    RenderCar(car);
    RenderGround();
    RenderTrees();
    RenderLights();
    for (Car& enemyCar : enemies) {
        RenderCar(enemyCar);
    }

    for (EnemyCar& car : enemies) {
        car.posLength -= deltaTimeSeconds * SPEED;
        if (car.posLength < 0)
            car.posLength += (int)track.length;
    }

    int i = 0;
    for (auto component : car.components) {
        if (component.name == "stop1" || component.name == "stop2") {
            glm::mat4 auxMatrix = glm::mat4(1);

            /*Set parameters to render car in scene at determined position.*/

            auxMatrix = glm::translate(auxMatrix, car.position);
            auxMatrix = glm::rotate(auxMatrix, RADIANS(car.rotation), glm::vec3(0, 1, 0));
            auxMatrix = glm::scale(auxMatrix, glm::vec3(0.06f, 0.03f, 0.06f));

            /*Translate the car in the scene origin so that it's center is in the origin.*/
            auxMatrix = glm::translate(auxMatrix, glm::vec3(-2, 0.9f, -1.48f));

            /*Render the car in a place in the scene.*/
            auxMatrix *= component.matrix;
            glm::vec4 tmp = auxMatrix * glm::vec4(ORIGIN, 1);
            lightPositions[LIGHTS * 2 + i] = glm::vec3(tmp.x, tmp.y, tmp.z);
            ++i;
        }

    }

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea2.x, miniViewportArea2.y, miniViewportArea2.width, miniViewportArea2.height);
    glClearColor(BLUE.x, BLUE.y, BLUE.z, 1);
    camera->Set(ORIGIN + glm::vec3(0, 1, 2), ORIGIN, glm::vec3(0, 1, 0));
    projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, zNear, zFar);

    RenderSpeedMeter();
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(loc_light_position, lightPositions.size(), glm::value_ptr(lightPositions[0]));

    int count = glGetUniformLocation(shader->program, "lightCount");
    glUniform1i(count, lightPositions.size());

    glm::vec3 eyePosition = camera->position;
    int loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    int spotDir = glGetUniformLocation(shader->program, "spotDirection");
    glUniform3fv(spotDir, 1, glm::value_ptr(car.direction - glm::vec3(0, 0.8f, 0)));

    int kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(kd, materialKd);

    int ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(ks, materialKs);

    int shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(shininess, materialShininess);

    int obj_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(obj_color, 1, glm::value_ptr(color));

    int night = glGetUniformLocation(shader->program, "nightMode");
    glUniform1i(night, nightMode);

    int location = glGetUniformLocation(shader->program, "Model");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "View");

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    location = glGetUniformLocation(shader->program, "Projection");

    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    location = glGetUniformLocation(shader->program, "carPos");

    glUniform3fv(location, 1, glm::value_ptr(car.position));

    location = glGetUniformLocation(shader->program, "scaleFactor");

    glUniform1f(location, scaleFactor);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        car.acceleration += car.acceleration < 0 ? deltaTime * 5 : deltaTime * 2;
        car.acceleration = car.acceleration > 5 ? 5 : car.acceleration;
        speedometerAngle = MINANGLE - car.acceleration / 10.0f * (MINANGLE - MAXANGLE);
        
    }
    if (!window->KeyHold(GLFW_KEY_W) && !window->KeyHold(GLFW_KEY_S)) {
        if (abs(car.acceleration) < 0.2f)
            car.acceleration = 0;
        else
            car.acceleration -= deltaTime * 4 * (car.acceleration < 0 ? -1 : 1);

        if (car.acceleration >= 0)
            speedometerAngle = MINANGLE - car.acceleration / 10.0f * (MINANGLE - MAXANGLE);
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        car.acceleration -= car.acceleration > 0 ? deltaTime * 5 : deltaTime * 2;
        if (car.acceleration >= 0)
            speedometerAngle = MINANGLE - car.acceleration / 10.0f * (MINANGLE - MAXANGLE);

        car.acceleration = car.acceleration < -3 ? -3 : car.acceleration;
       
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        if (car.acceleration > 0) {
            car.rotation += deltaTime * FULL_CIRCLE / 4;
        }

        if (car.acceleration < 0) {
            car.rotation -= deltaTime * FULL_CIRCLE / 4;
        }

        car.direction = glm::vec3(cos(car.rotation * PI / 180 + PI), 0, -sin(car.rotation * PI / 180 + PI));
    }
    if (window->KeyHold(GLFW_KEY_A) && car.wheelRotation < 45) {
        if (car.wheelRotation + deltaTime * FULL_CIRCLE < 45)
            car.wheelRotation += deltaTime * FULL_CIRCLE;
        else
            car.wheelRotation = 45;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        if (car.acceleration > 0) {
            car.rotation -= deltaTime * FULL_CIRCLE / 4;
        }

        if (car.acceleration < 0) {
            car.rotation += deltaTime * FULL_CIRCLE / 4;
        }

        car.direction = glm::vec3(cos(car.rotation * PI / 180 + PI), 0, -sin(car.rotation * PI / 180 + PI));
    }
    if (window->KeyHold(GLFW_KEY_D) && car.wheelRotation > -45) {
        if (car.wheelRotation - deltaTime * FULL_CIRCLE > -45)
            car.wheelRotation -= deltaTime * FULL_CIRCLE;
        else
            car.wheelRotation = -45;
    }
    if (!window->KeyHold(GLFW_KEY_A) && !window->KeyHold(GLFW_KEY_D)) {
        car.wheelRotation = 0;
    }

    if (window->KeyHold(GLFW_KEY_M)) {
        scaleFactor += deltaTime / 100.0f;
    }

    if (window->KeyHold(GLFW_KEY_N)) {
        scaleFactor -= deltaTime / 100.0f;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }

    if (key == GLFW_KEY_1)
    {
        nightMode ^= 1;
        skyColor = CYAN - (float)nightMode *glm::vec3(0.5f, 1, 0.9f);
    }

    if (key == GLFW_KEY_Q)
    {
        frontCamera ^= 1;
    }

    // debug
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
    miniViewportArea = ViewportArea(width / 25.6f, height / 14.4f, width / 5.0f, height / 5.0f);
    miniViewportArea2 = ViewportArea(width * 0.775f, height * 0.6f, height / 3.0f, height / 3.0f);
}

void Tema2::CreateMeshes()
{
    // These are the meshes used to create the car geometry.
    Mesh* frontWheel = obj3D::CreateCyllinder("frontWheel", glm::vec3(0, 0, -0.5f), 1, 1, 1, 180, BLACK);
    AddMeshToList(frontWheel);

    Mesh* backWheel = obj3D::CreateCyllinder("backWheel", glm::vec3(0, 0, -0.5f), 1, 1, 1, 180, BLACK);
    AddMeshToList(backWheel);

    Mesh* body1 = obj3D::CreateBox("body1", BLUE - glm::vec3(0, 0, 0.8f));
    AddMeshToList(body1);

    Mesh* body11 = obj3D::CreateBox("body11", BLUE - glm::vec3(0, 0, 0.5f));
    AddMeshToList(body11);

    Mesh* stop1 = obj3D::CreateBox("stop1", WHITE - glm::vec3(0.4f));
    AddMeshToList(stop1);

    Mesh* stop2 = obj3D::CreateBox("stop2", RED);
    AddMeshToList(stop2);

    Mesh* body2 = obj3D::CreatePrism("body2", BLUE - glm::vec3(0, 0, 0.5f));
    AddMeshToList(body2);

    Mesh* body22 = obj3D::CreatePrism("body22", BLUE - glm::vec3(0, 0, 0.8f));
    AddMeshToList(body22);

    Mesh* window1 = obj3D::CreateSquare("window", DARK_GREY - glm::vec3(0.2f));
    AddMeshToList(window1);

    Mesh* window2 = obj3D::CreateTriangle("window2", DARK_GREY - glm::vec3(0.2f));
    AddMeshToList(window2);

    // These are the meshes used to create the track geometry.
    Mesh* track = obj3D::CreateTrack("track1", DARK_GREY);
    AddMeshToList(track);

    Mesh* ground = obj3D::CreateGround("ground", GREEN);
    AddMeshToList(ground);

    Mesh* log = obj3D::CreateBox("log", LIGHT_BROWN);
    AddMeshToList(log);

    Mesh* leaf = obj3D::CreateBox("leaf", DARK_GREEN);
    AddMeshToList(leaf);

    Mesh* light = obj3D::CreateSquare("light", WHITE);
    AddMeshToList(light);

    Mesh* circle = obj3D::CreateEllipse("circle1", ORIGIN, 5, 5, 180, RED);
    AddMeshToList(circle);

    // These are the meshes used to create the speedometer geometry.
    Mesh* speedMeter = new Mesh("speedMeter");
    speedMeter->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/speed"), "speed.fbx");
    AddMeshToList(speedMeter);

    Mesh* needle = new Mesh("needle");
    needle->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/needle"), "needle.fbx");
    AddMeshToList(needle);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
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

void Tema2::RenderCar(Car car)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    for (auto component : car.components) {
        glm::mat4 auxMatrix = modelMatrix;

        /*Set parameters to render car in scene at determined position.*/
        auxMatrix = glm::translate(auxMatrix, car.position);
        auxMatrix = glm::rotate(auxMatrix, RADIANS(car.rotation), glm::vec3(0, 1, 0));
        auxMatrix = glm::scale(auxMatrix, glm::vec3(0.06f, 0.03f, 0.06f));

        /*Translate the car in the scene origin so that it's center is in the origin.*/
        auxMatrix = glm::translate(auxMatrix, glm::vec3(-2, 0.9f, -1.48f));

        /*Render the car in a place in the scene.*/
        auxMatrix *= component.matrix;
        if (component.name == "frontWheel") {
            auxMatrix = glm::rotate(auxMatrix, RADIANS(car.wheelRotation), glm::vec3(0, 1, 0));
        }
        RenderSimpleMesh(meshes[component.name], shaders["SplineShader"], auxMatrix, meshes[component.name]->vertices[0].color);
    }
}

void Tema2::RenderTrack()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["track1"], shaders["SplineShader"], modelMatrix, meshes["track1"]->vertices[0].color);
}

void Tema2::RenderGround()
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.02f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 0, 150));
    RenderSimpleMesh(meshes["ground"], shaders["SplineShader"], modelMatrix, meshes["ground"]->vertices[0].color);
}

void Tema2::RenderTrees()
{
    for (Tree& tree : treeLocations) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tree.location);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.02f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, tree.height, 0.5f));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
        RenderSimpleMesh(meshes["log"], shaders["SplineShader"], modelMatrix, meshes["log"]->vertices[0].color);

        for (auto leaf : tree.leaves) {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, tree.location);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1, 0.5f));
            modelMatrix = glm::translate(modelMatrix, -tree.location);
            modelMatrix = glm::translate(modelMatrix, leaf);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
            RenderSimpleMesh(meshes["leaf"], shaders["SplineShader"], modelMatrix, meshes["leaf"]->vertices[0].color);
        }
    }
}

void Tema2::RenderLights()
{
    for (Light& light : lights) {
        glm::mat4 auxMatrix = glm::mat4(1);
        auxMatrix = glm::translate(auxMatrix, light.pos);
        auxMatrix = glm::rotate(auxMatrix, RADIANS(light.rotation), glm::vec3(0, 1, 0));
        auxMatrix = glm::rotate(auxMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
        auxMatrix = glm::scale(auxMatrix, glm::vec3(0.2f, 0.2f, 1));
        RenderSimpleMesh(meshes["light"], shaders["SplineShader"], auxMatrix, meshes["light"]->vertices[0].color);
    }
}

void Tema2::RenderSpeedMeter()
{
    glm::mat4 matrix = glm::mat4(1);
    matrix = glm::scale(matrix, glm::vec3(2, 2, 1));
    RenderMesh(meshes["speedMeter"], shaders["Simple"], matrix);

    matrix = glm::mat4(1);
    matrix = glm::rotate(matrix, RADIANS(speedometerAngle), glm::vec3(0, 0, 1));
    matrix = glm::translate(matrix, glm::vec3(-0.025f, 0.2f, 0.1f));
    matrix = glm::scale(matrix, glm::vec3(1, 1, 1));
    RenderMesh(meshes["needle"], shaders["Simple"], matrix);
}

void Tema2::CheckWallCollision()
{
    glm::mat4 auxMatrix = glm::mat4(1);
    auxMatrix = glm::scale(auxMatrix, glm::vec3(1.0f / 0.06f, 1.0f / 0.03f, 1.0f / 0.06f));
    auxMatrix = glm::rotate(auxMatrix, RADIANS(-car.rotation), glm::vec3(0, 1, 0));
    auxMatrix = glm::translate(auxMatrix, -car.position);

    for (VertexFormat point : meshes["track1"]->vertices) {
        glm::vec4 newPoint = auxMatrix * glm::vec4(point.position, 1);
        if (newPoint.x > -car.hitbox.x / 2.0f && newPoint.x < car.hitbox.x / 2.0f
            && newPoint.z > -car.hitbox.y / 2.0f && newPoint.z < car.hitbox.y / 2.0f) {
            car.collision = true;
            return;
        }
    }
}

void Tema2::CheckEnemyCollision(Car& car, EnemyCar& enemy)
{
    float dist = glm::distance(car.position, enemy.position);
    if (dist < 4.5f * 0.09f) {
        car.collision = true;
        return;
    }
}