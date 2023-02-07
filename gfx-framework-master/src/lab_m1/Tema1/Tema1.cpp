#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <cstdlib>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/obj2D.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
    InitializeStructs(gameState, gameMode, eventManager, duck, hearts, bullets);

    modelMatrix = glm::mat3(1);
    skyColor = CYAN;
    cursorX = 0;
    cursorY = 0;
    playTime = 180;
    score = 0;
    highscore = 0;
    level = 1;
    killCount = 0;

    camLeft = 0.0f;
    camRight = (float)window->GetResolution().x;
    camBottom = 0.0f;
    camTop = (float)window->GetResolution().y;

    scopeOn = false;
    scopeSpeed = 0.0f;

    renderX = 0;
    renderY = 0;
    renderTime = 0;

    killStreak = 0;
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    GetCameraInput()->SetActive(false);
    window->HidePointer();

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);

    CreateMeshes();
    InitializeDuckComponents(duck);
}


void Tema1::FrameStart()
{
    auto camera = GetSceneCamera();
    camera->SetOrthographic(camLeft, camRight, camBottom, camTop, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    CheckConditions(deltaTimeSeconds);
    CheckScreenCollision();
    if (RenderScene(deltaTimeSeconds))
        return;
    SetNextFrameDuck(deltaTimeSeconds);
    playTime -= deltaTimeSeconds;
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}

/*Here we tell the game what weapon the player wants to equip.*/
void Tema1::OnKeyPress(int key, int mods)
{    
    if (duck.state == ESCAPING || duck.state == FALLING
        || gameState == MENU || gameState == RETRY) {
        if (key == GLFW_KEY_1) {
            gameMode = RIFLE;
            scopeOn = 0;

            camLeft = 0.0f;
            camRight = (float)window->GetResolution().x;
            camBottom = 0.0f;
            camTop = (float)window->GetResolution().y;
        }

        if (key == GLFW_KEY_2) {
            gameMode = SNIPER;
        }
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
}

/*Here we get the mouse position that will be used mostly when
the player is scope-ing.*/
void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    glm::vec2 resolution = window->GetResolution();
    cursorX = mouseX;
    cursorY = resolution.y - mouseY;

    if (scopeOn) {
        if (cursorX > resolution.x / 2
            && cursorX < resolution.x - resolution.x / 2.5f) {
            camRight += deltaX;
            camLeft += deltaX;
        }

        if (cursorY > resolution.y / 2
            && cursorY < resolution.y - resolution.y / 4) {
            camBottom -= deltaY;
            camTop -= deltaY;
        }
    }
}

/*Here we need to take input for the left mouse button for shooting
and right mosue button for scope-ing while having the awp equiped.
Also input should work only the game is in a MENU mode or when 
the duck is flying on screen.*/
void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (gameState == RUNNING && duck.state == FLYING && button == GLFW_MOUSE_BUTTON_RIGHT) {
        bullets.bulletCount -= 1;

        glm::mat3 modelMatrix = GetReverseTransformationMatrix(duck);
        for (Component elem : duck.hitboxes) {
            glm::mat3 auxMatrix = transform2D::Translate(-elem.offsetX, -elem.offsetY) * modelMatrix;

            glm::vec3 tmp = auxMatrix * glm::vec3(cursorX, cursorY, 1);
            if (cursorY >= meshes["grass2"]->vertices[3].position.y
                && tmp.x >= meshes[elem.name]->vertices[0].position.x
                && tmp.x <= meshes[elem.name]->vertices[1].position.x
                && tmp.y >= meshes[elem.name]->vertices[0].position.y
                && tmp.y <= meshes[elem.name]->vertices[3].position.y) {
                if (elem.name == "hitbox2")
                    eventManager.headshot = true;

                if (gameMode == SNIPER) {
                    renderX = mouseX;
                    renderY = mouseY;
                    renderTime = 1;
                }

                scopeOn = false;
                duck.deathAcceleration = 300;
                skyColor = !eventManager.changeColor ? CYAN : PINK;
                duck.state = FALLING;
                break;
            }
        }

        if (duck.state == FLYING && bullets.bulletCount == 0) {
            scopeOn = false;
            killStreak = 0;
            skyColor = !eventManager.changeColor ? PINK : CYAN;
            duck.state = ESCAPING;
        }
    }
    else if (gameState == MENU && button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (cursorX >= 600 && cursorX <= 700 && cursorY >= 450 && cursorY <= 550) {
            playTime = 180;
            if (gameMode == RIFLE)
                bullets.bulletCount = 3;
            else
                bullets.bulletCount = 1;
            gameState = RUNNING;
        }
    }
    else if (gameState == RETRY && button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (cursorX >= 600 && cursorX <= 700 && cursorY >= 450 && cursorY <= 550) {
            hearts.heartCount = 3;
            score = 0;
            level = 1;
            killCount = 0;
            duck.speed = INITIAL_SPEED;
            duck.scaleX = 1;
            duck.scaleY = 1;
            eventManager = EventManager();
            playTime = 180;
            if (gameMode == RIFLE)
                bullets.bulletCount = 3;
            else
                bullets.bulletCount = 1;
            gameState = RUNNING;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_3 && gameMode == SNIPER
        && duck.state == FLYING) {
        scopeOn ^= 1;
        if (!scopeOn) {
            camLeft = 0.0f;
            camRight = (float)window->GetResolution().x;
            camBottom = 0.0f;
            camTop = (float)window->GetResolution().y;
        }
        else {
            camLeft = (float)window->GetResolution().x / 10;
            camRight = (float)window->GetResolution().x - camLeft;
            camBottom = (float)window->GetResolution().y  / 10;
            camTop = (float)window->GetResolution().y - camBottom;
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

/*Here we create all the meshes that will be used by the RenderScene method.*/
void Tema1::CreateMeshes()
{
    Mesh* head = obj2D::CreateEllipse("head", glm::vec3(0, 0, -1), 30, 20, 90, DARK_GREEN, true);
    AddMeshToList(head);

    Mesh* body = obj2D::CreateEllipse("body", glm::vec3(0, 0, -1), 50, 30, 90, LIGHT_BROWN, true);
    AddMeshToList(body);

    Mesh* eye = obj2D::CreateEllipse("eye", glm::vec3(0, 0, -1), 7, 7, 60, BLACK, true);
    AddMeshToList(eye);

    Mesh* pupil = obj2D::CreateEllipse("pupil", glm::vec3(0, 0, -1), 3, 3, 8, WHITE, true);
    AddMeshToList(pupil);

    Mesh* nose = obj2D::CreateTriangle("nose", glm::vec3(0, 0, -1), glm::vec3(22.5f, 5, 0), glm::vec3(0, 20, 0), YELLOW, true);
    AddMeshToList(nose);

    Mesh* wing1 = obj2D::CreateTriangle("wing1", glm::vec3(0, 0, -1), glm::vec3(60, 0, 0), glm::vec3(0, 60, 0), LIGHT_GREY, true);
    AddMeshToList(wing1);

    Mesh* wing2 = obj2D::CreateTriangle("wing2", glm::vec3(0, 0, -1), glm::vec3(60, 0, 0), glm::vec3(60, 60, 0), DARK_GREY, true);
    AddMeshToList(wing2);

    Mesh* tail = obj2D::CreateTriangle("tail", glm::vec3(0, 0, -1), glm::vec3(25, 0, 0), glm::vec3(25, 25, 0), LIGHT_BROWN, true);
    AddMeshToList(tail);

    Mesh* feet = obj2D::CreateRectangle("feet", glm::vec3(0, 0, -1), 118, 9, ORANGE, true);
    AddMeshToList(feet);

    Mesh* grass1 = obj2D::CreateTriangle("grass1", glm::vec3(0, 100, 0), glm::vec3(50, 100, 0), glm::vec3(50, 300, 0), GREEN, true);
    AddMeshToList(grass1);

    Mesh* grass2 = obj2D::CreateRectangle("grass2", glm::vec3(0, 0, 0), window->GetResolution().x, 100, GREEN, true);
    AddMeshToList(grass2);

    Mesh* grass3 = obj2D::CreateTriangle("grass3", glm::vec3(0, 100, -2), glm::vec3(50, 100, -1), glm::vec3(50, 250, -1), DARK_GREEN, true);
    AddMeshToList(grass3);

    Mesh* circle1 = obj2D::CreateEllipse("circle1", glm::vec3(0, 0, 0), 10, 10, 45, RED, true);
    AddMeshToList(circle1);

    Mesh* base1 = obj2D::CreateTriangle("base1", glm::vec3(0, 0, 0), glm::vec3(17.5f, -20, 0), glm::vec3(35, 0, 0), RED, true);
    AddMeshToList(base1);

    Mesh* bullet1 = obj2D::CreateRectangle("bullet1", glm::vec3(0, 0, 0), 10, 30, GREEN, true);
    AddMeshToList(bullet1);

    Mesh* cursor1 = obj2D::CreateRectangle("cursor1", glm::vec3(-20, -3, 1), 13, 6, VIOLET, true);
    AddMeshToList(cursor1);

    Mesh* hitbox1 = obj2D::CreateRectangle("hitbox1", glm::vec3(-50, -30, 0), 100, 60, BLACK);
    AddMeshToList(hitbox1);

    Mesh* hitbox2 = obj2D::CreateRectangle("hitbox2", glm::vec3(-30, -20, 0), 60, 40, BLACK);
    AddMeshToList(hitbox2);

    Mesh* scope1 = obj2D::CreateRectangle("scope1", glm::vec3(-300, -15, 1), 100, 30, BLACK, true);
    AddMeshToList(scope1);

    Mesh* scope2 = obj2D::CreateEllipse("scope2", glm::vec3(0, 0, 1), 300, 300, 180, BLACK);
    AddMeshToList(scope2);

    Mesh* scope3 = obj2D::CreateLine("scope3", glm::vec3(-200, 0, 1), glm::vec3(200, 0, 0), BLACK);
    AddMeshToList(scope3);

    Mesh* scope4 = obj2D::CreateEllipse("scope4", glm::vec3(0, 0, 1), 5, 5, 30, RED, true);
    AddMeshToList(scope4);

    Mesh* playBtn = new Mesh("playBtn");
    playBtn->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/playButton"), "playButton.fbx");
    AddMeshToList(playBtn);

    Mesh* retryBtn = new Mesh("retryBtn");
    retryBtn->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/retryButton"), "retryButton.fbx");
    AddMeshToList(retryBtn);

    Mesh* ak = new Mesh("ak");
    ak->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/ak"), "ak.fbx");
    AddMeshToList(ak);

    Mesh* awp = new Mesh("awp");
    awp->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models/awp"), "awp.fbx");
    AddMeshToList(awp);
}

/*Here we render objects in scene. This function returns 0 if the 
game is running. Otherwise the game will be in a menu where the player
can choose to play the game or restart the game depending on gameState.*/
int Tema1::RenderScene(float deltaTimeSeconds)
{
    RenderIdleBackground();

    if (gameState == RETRY) {
        RenderMesh(meshes["retryBtn"], glm::vec3(650, 500, 0),glm::vec3(300, 300, 0));
        return 1;
    }

    if (gameState == MENU) {
        textRenderer->RenderText("Duck Hunt", window->GetResolution().x / 2 - 230, 50, 5, BLACK);
        RenderMesh(meshes["playBtn"], glm::vec3(650, 500, 0), glm::vec3(200, 200, 0));
        return 1;
    }

    RenderRunningBackground();
    RenderDuck();

    return 0;
}

/*Here we render stuff across all states of the game : grass and crosshairs
depending on mode : rifle crosshair or scope crosshair.*/
void Tema1::RenderIdleBackground()
{
    glm::mat3 modelMatrix = transform2D::Translate(window->GetResolution().x, 0);
    modelMatrix *= transform2D::Scale(0.5f, 0.2f);
    RenderMesh2D(meshes["scope2"], shaders["VertexColor"], modelMatrix);

    if (!scopeOn) {
        if (gameMode == RIFLE)
            RenderMesh(meshes["ak"], glm::vec3(window->GetResolution().x - 112.5f, 35, 0), glm::vec3(75, 62.5f, 0));
        else if (gameMode == SNIPER)
            RenderMesh(meshes["awp"], glm::vec3(window->GetResolution().x - 112.5f, 35, 0), glm::vec3(75, 62.5f, 0));
    }
    
    RenderMesh2D(meshes["grass2"], shaders["VertexColor"], glm::mat3(1));
    for (int i{}; i <= window->GetResolution().x / 50; ++i) {
        RenderMesh2D(meshes["grass1"], shaders["VertexColor"], transform2D::Translate(i * 50, 0));

    }

    for (int i{ -1 }; i < window->GetResolution().x / 50; ++i) {
        RenderMesh2D(meshes["grass3"], shaders["VertexColor"], transform2D::Translate(i * 50 + 25, 0));
    }

    if (!scopeOn) {
        glm::mat3 auxMatrix;
        for (int i{}; i < 4; ++i) {
            auxMatrix = transform2D::Translate(cursorX, cursorY);
            auxMatrix *= transform2D::Rotate(90 * i * PI / 180);
            RenderMesh2D(meshes["cursor1"], shaders["VertexColor"], auxMatrix);
        }
    }
    else {
        glm::mat3 auxMatrix = transform2D::Translate(cursorX, cursorY);
        auxMatrix *= transform2D::Scale(0.25f, 0.25f);
        RenderMesh2D(meshes["scope2"], shaders["VertexColor"], auxMatrix);

        for (int i{}; i < 4; ++i) {
            auxMatrix = transform2D::Translate(cursorX, cursorY);
            auxMatrix *= transform2D::Rotate(90 * i * PI / 180);
            auxMatrix *= transform2D::Scale(0.25f, 0.25f);
            RenderMesh2D(meshes["scope1"], shaders["VertexColor"], auxMatrix);
        }

        for (int i{}; i < 4; ++i) {
            auxMatrix = transform2D::Translate(cursorX, cursorY);
            auxMatrix *= transform2D::Rotate(90 * i * PI / 180);
            auxMatrix *= transform2D::Translate(-40, 0);
            RenderMesh2D(meshes["scope4"], shaders["VertexColor"], auxMatrix);

            auxMatrix = transform2D::Translate(cursorX, cursorY);
            auxMatrix *= transform2D::Rotate(90 * i * PI / 180);
            auxMatrix *= transform2D::Translate(-20, 0);
            RenderMesh2D(meshes["scope4"], shaders["VertexColor"], auxMatrix);
        }

        for (int i{}; i < 2; ++i) {
            auxMatrix = transform2D::Translate(cursorX, cursorY);
            auxMatrix *= transform2D::Rotate(90 * i * PI / 180);
            auxMatrix *= transform2D::Scale(0.25f, 0.25f);
            RenderMesh2D(meshes["scope3"], shaders["VertexColor"], auxMatrix);
        }
    }
}

/*This function is used to render on screen stuff needed when the game is running:
score, health, bullets, level, time and damage multiplier.*/
void Tema1::RenderRunningBackground()
{
    /*If the player is scope - ing only the grass will be rendered in the
    background.*/
    if (scopeOn)
        return;

    short minutes = (short)playTime / 60;
    short seconds = (short)playTime - 60 * minutes;
    if (seconds >= 10)
        textRenderer->RenderText(to_string(minutes) + ":" + to_string(seconds), window->GetResolution().x / 2 - 50, 25, 2, BLACK);
    else
        textRenderer->RenderText(to_string(minutes) + ":0" + to_string(seconds), window->GetResolution().x / 2 - 50, 25, 2, BLACK);
    textRenderer->RenderText("Level " + to_string(level), window->GetResolution().x / 2 - 75, 70, 2, BLACK);
    textRenderer->RenderText("Score : " + to_string(score), 1000, 25, 1.5f, BLACK);
    textRenderer->RenderText("Highscore : " + to_string(highscore), 1000, 50, 1.5f, BLACK);

    if (hearts.heartCount < 4) {
        for (int i{}; i < hearts.heartCount; ++i) {
            RenderMesh2D(meshes["circle1"], shaders["VertexColor"], transform2D::Translate(50 + 40 * i, 700));
            RenderMesh2D(meshes["circle1"], shaders["VertexColor"], transform2D::Translate(65 + 40 * i, 700));
            RenderMesh2D(meshes["base1"], shaders["VertexColor"], transform2D::Translate(40 + 40 * i, 697.5f));
        }
    } else {
        RenderMesh2D(meshes["circle1"], shaders["VertexColor"], transform2D::Translate(50, 700));
        RenderMesh2D(meshes["circle1"], shaders["VertexColor"], transform2D::Translate(65, 700));
        RenderMesh2D(meshes["base1"], shaders["VertexColor"], transform2D::Translate(40, 697.5f));
        textRenderer->RenderText("X" + to_string(hearts.heartCount), 85, 12.5f, 2, BLACK);
    }

    for (int i{}; i < bullets.bulletCount; ++i) {
        RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], transform2D::Translate(50 + 20 * i, 630));
    }

    if (gameMode == SNIPER && renderTime > 0) {
        if (eventManager.headshot)
            textRenderer->RenderText("X3", renderX, renderY, 2, BLACK);
        else
            textRenderer->RenderText("X2", renderX, renderY, 1.5f, BLACK);
    }
}

//This function is used to render the duck on screen.
void Tema1::RenderDuck()
{
    modelMatrix = GetTransformationMatrix(duck);
    for (Component elem : duck.components) {
        glm::mat3 auxMatrix = modelMatrix;
        if (elem.name == "wing1" || elem.name == "wing2") {
            auxMatrix *= transform2D::Translate(0, elem.offsetY);
            auxMatrix *= transform2D::MirrorOX(duck.wingScale);
            auxMatrix *= transform2D::Translate(0, -elem.offsetY);
        }

        auxMatrix *= elem.matrix;

        glm::vec3 color = meshes[elem.name]->vertices[0].color;
        glm::vec3 tint = glm::vec3(0);
        if (eventManager.changeColor) {
            if (elem.name == "eye" || elem.name == "pupil"
                || elem.name == "head" || elem.name == "feet") {
                tint = glm::vec3(0);
            }
            else {
                tint = glm::vec3(0.75f * (1 - color.x), 0.75f * (1 - color.y), 0.75f * (1 - color.z));
            }
        }
        
        RenderMesh2D(meshes[elem.name], auxMatrix, meshes[elem.name]->vertices[0].color + tint);
    }

    // For showing hitboxes live call Tema1::ShowHitboxes() method here.    
    // Tema1::ShowHitboxes(modelMatrix);
}

/*This function is used to know if our duck object will collide with the screen
borders.If this is the case we must modify the duck.direction vector based on
the border hit. For collisions we use the coordinate of the center of the 
duck head which is stored in duck.components[2].*/
void Tema1::CheckScreenCollision()
{
    float x = duck.components[2].offsetX + duck.offset.x + duck.translateX;
    float y = duck.components[2].offsetY + duck.offset.y + duck.translateY;

    if (x < 0) {
        duck.wallHit = 0;
        duck.direction.x = 1;
    }

    if (duck.state == FLYING && y < 0) {
        duck.direction.y = 1;
        duck.rotation = 1;
    }

    if (x > window->GetResolution().x) {
        duck.wallHit = 1;
        duck.direction.x = -1;
    }

    if (duck.state == FLYING && y > window->GetResolution().y) {
        duck.direction.y = -1;
        duck.rotation = -1;
    }

    if (duck.wallHit == 1)
        duck.direction.x = -1;

    if (duck.wallHit == 0)
        duck.direction.x = 1;
}

/*This function is used to check several conditions that happen while 
the game is running.*/
void Tema1::CheckConditions(float deltaTimeSeconds)
{
    /*Make sure that after the timer runs out the game will end.*/
    if (playTime <= 0) {
        skyColor = CYAN;
        scopeOn = false;
        duck.state = STATIC;
        gameState = RETRY;
    }

    /*Make sure to increase the total amount of lives left when killing
    10 ducks in a row.*/
    if (killStreak >= 10) {
        hearts.heartCount++;
        killStreak = 0;
    }

    /*Make sure to decrease the renderTime variable so that the multiplier
    text doesn't show for ever.*/
    if (renderTime > 0)
        renderTime -= deltaTimeSeconds;
    else
        renderTime = 0;

    /*Make sure to reset the camera position to default when the player is
    not scope-ing.*/
    if (!scopeOn) {
        camLeft = 0.0f;
        camRight = (float)window->GetResolution().x;
        camBottom = 0.0f;
        camTop = (float)window->GetResolution().y;
    }

    /*Make sure that after the player runs out of lives the game will end.*/
    if (hearts.heartCount == 0) {
        skyColor = CYAN;
        scopeOn = false;
        gameState = RETRY;
    }

    /*Make sure to increase falling acceleration of the duck when FALLING
    state is happening.*/
    if (duck.state == FALLING) {
        duck.deathAcceleration -= 800 * deltaTimeSeconds;
    }

    /*Make sure that after a duck lives for too long it will enter in 
    ESCAPING state and it's lifetime is reset based on the current level.*/
    if (duck.lifetime <= 0) {
        
        duck.lifetime = 5 - level / 4.0f;
        if (duck.lifetime < 1)
            duck.lifetime = 1;

        skyColor = !eventManager.changeColor ? PINK : CYAN;
        scopeOn = false;
        duck.state = ESCAPING;
    }

    /*These 2 conditions are for wing animations.*/
    if (duck.wingScale < -1) {
        duck.factor *= -1;
        duck.wingScale = -1;
    }

    if (duck.wingScale > 1) {
        duck.factor *= -1;
        duck.wingScale = 1;
    }

    /*Here we reset all the duck parameters set to default ones so that 
    we can give new values to our next duck that will spawn on screen.
    We set initial position, initial rotation, initial direction, bullets
    and are setting flags for the eventManager structure.*/
    if (duck.state == STATIC) {
        skyColor = CYAN;

        duck.offset.x = rand() % 800 - 100;
        duck.offset.y = 0;
        duck.angularStep = rand() % 50 + 30;

        duck.wallHit = rand() % 2;
        if (duck.wallHit == 0)
            duck.direction = glm::vec2(1, 1);
        else
            duck.direction = glm::vec2(-1, 1);

        if (gameMode == RIFLE)
            bullets.bulletCount = 3;
        else
            bullets.bulletCount = 1;

        duck.translateX = 0;
        duck.translateY = 0;
        duck.scaleX = 1;
        duck.scaleY = 1;
        duck.rotation = 1;
        duck.lifetime = 5 - level / 4.0f;
        if (duck.lifetime < 1)
            duck.lifetime = 1;

        eventManager.headshot = false;
        if (level > 1)
            TriggerEvents(eventManager);

        if (eventManager.changeSize) {
            float scale{ (float)(rand() % 3 + 5) / 10.0f };
            duck.scaleX = scale;
            duck.scaleY = scale;

            eventManager.changeSize = false;
        }

        if (eventManager.speedIncrease)
            duck.speed *= 2;

        duck.state = FLYING;
    }
}

/*This function sets the next parameters the duck will take in the next frame within
the running game. Also at the end of frame we need to set the flag for randomBounce
in order to change our duck direction.*/
void Tema1::SetNextFrameDuck(float deltaTimeSeconds)
{
    if (duck.state == FLYING) {
        duck.wingScale -= deltaTimeSeconds * duck.factor;
        duck.translateX += duck.speed.x * cos(abs(duck.angularStep) * PI / 180) * duck.direction.x * deltaTimeSeconds;
        duck.translateY += duck.speed.y * sin(abs(duck.angularStep) * PI / 180) * duck.direction.y * deltaTimeSeconds;
        duck.lifetime -= deltaTimeSeconds;

        if (level > 1) {
            int val = rand() % 1000 + 1;
            if (val % (1000 / eventManager.randomBounceChance) == 0) {
                eventManager.randomBounce = true;
                duck.wallHit ^= 1;
                eventManager.randomBounce = false;
            }
        }
    }

    float x = duck.components[5].offsetX + duck.offset.x + duck.translateX;
    float y = duck.components[5].offsetY + duck.offset.y + duck.translateY;

    if (duck.state == FALLING) {
        duck.translateX += 200 * duck.direction.x * deltaTimeSeconds;
        duck.translateY += duck.deathAcceleration * deltaTimeSeconds;
        duck.angularStep += -720 * duck.rotation * deltaTimeSeconds;
        if (duck.scaleX >= 0.3f) {
            duck.scaleX -= 0.4f * deltaTimeSeconds;
            duck.scaleY -= 0.4f * deltaTimeSeconds;
        }

        if (y < 0) {
            if (eventManager.speedIncrease) {
                duck.speed /= 2;
                eventManager.speedIncrease = false;
            }

            if (!eventManager.changeColor) {
                if (gameMode == SNIPER)
                    if (eventManager.headshot)
                        score += 30;
                    else
                        score += 20;
                else
                    score += 10;
                killCount++;
                killStreak++;
            }
            else {
                
                hearts.heartCount--;
                eventManager.changeColor = false;
            }

            
            if (level != killCount / 5 + 1) {
                duck.speed += 300 * 0.2f;
                increaseChances(eventManager);
            }

            level = killCount / 5 + 1;
            if (score > highscore)
                highscore = score;

            duck.state = STATIC;
        }
    }

    if (duck.state == ESCAPING) {
        duck.wingScale -= deltaTimeSeconds * duck.factor;
        duck.translateY += duck.escapingAcceleration * deltaTimeSeconds;
        duck.angularStep = 90 * duck.rotation;

        if (eventManager.speedIncrease) {
            duck.speed /= 2;
            eventManager.speedIncrease = false;
        }

        if (y > window->GetResolution().y + 200) {
            if (!eventManager.changeColor) {
                hearts.heartCount--;
            }
            else {
                score += 50;
                if (score > highscore)
                    highscore = score;

                eventManager.changeColor = false;
            }
            duck.state = STATIC;
            
        }
    }
}

void Tema1::ShowHitboxes(glm::mat3 modelMatrix)
{
    for (Component elem : duck.hitboxes) {
        glm::mat3 auxMatrix = modelMatrix;
        auxMatrix *= elem.matrix;
        RenderMesh2D(meshes[elem.name], shaders["VertexColor"], auxMatrix);
    }
}
