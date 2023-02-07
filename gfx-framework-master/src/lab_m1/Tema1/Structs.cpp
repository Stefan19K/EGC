#include "lab_m1/Tema1/Structs.h"

void InitializeStructs(
    GameState& gameState,
    Gamemode& gameMode,
    EventManager &eventManager,
    Duck &duck, 
    Heart &hearts, 
    Bullet &bullets)
{
	gameState = MENU;

    gameMode = RIFLE;

    eventManager = EventManager();

	duck.wingScale = 1;
	duck.factor = 2;
    duck.translateX = 0;
    duck.translateY = 0;
    duck.scaleX = 1;
    duck.scaleY = 1;
    duck.offset = glm::vec2(0, 0);
    duck.speed = glm::vec2(300, 300);
    duck.direction = glm::vec2(0, 0);
    duck.angularStep = 0;
    duck.rotation = 1;
    duck.lifetime = 0;
    duck.deathAcceleration = 0;
    duck.escapingAcceleration = 200;
    duck.lifetime = 5;
    duck.wallHit = 0;
    duck.state = STATIC;

    hearts.heartCount = 3;

    bullets.bulletCount = 3;
}

void InitializeDuckComponents(Duck &duck)
{
    glm::mat3 modelMatrix;
    duck.components.push_back({ "pupil", 463, 127, 0, transform2D::Translate(463, 127) });
    duck.components.push_back({ "eye", 460, 125, 0, transform2D::Translate(460, 125) });
    duck.components.push_back({ "head", 450, 120, 0, transform2D::Translate(450, 120) });
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(315, 105);
    modelMatrix *= transform2D::Rotate(5 * PI / 180);
    duck.components.push_back({ "tail", 315, 105, 5, modelMatrix });
    duck.components.push_back({ "wing1", 350, 130, 0, transform2D::Translate(350, 130) });
    duck.components.push_back({ "body", 380, 120, 0, transform2D::Translate(380, 120) });
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(325, 98);
    modelMatrix *= transform2D::Rotate(5 * PI / 180);
    duck.components.push_back({ "feet", 325, 98, 5, modelMatrix });
    duck.components.push_back({ "nose", 475, 110, 0, transform2D::Translate(475, 110) });
    duck.components.push_back({ "wing2", 350, 130, 0, transform2D::Translate(350, 130) });
    duck.hitboxes.push_back({ "hitbox1", 380, 120, 0, transform2D::Translate(380, 120) });
    duck.hitboxes.push_back({ "hitbox2", 450, 120, 0, transform2D::Translate(450, 120) });    
}

glm::mat3 GetTransformationMatrix(Duck duck)
{
    glm::mat3 modelMatrix;
    modelMatrix = glm::mat3(1);

    float x = duck.components[5].offsetX + duck.offset.x + duck.translateX;
    float y = duck.components[5].offsetY + duck.offset.y + duck.translateY;

    if (duck.wallHit == 1) {
        modelMatrix *= transform2D::Translate(x, 0);
        modelMatrix *= transform2D::MirrorOY(-1);
        modelMatrix *= transform2D::Translate(-x, 0);
    }

    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Rotate((double)duck.angularStep * duck.rotation * PI / 180);
    modelMatrix *= transform2D::Translate(-x, -y);
    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(duck.scaleX, duck.scaleY);
    modelMatrix *= transform2D::Translate(-x, -y);
    modelMatrix *= transform2D::Translate(duck.translateX, duck.translateY);
    modelMatrix *= transform2D::Translate(duck.offset.x, duck.offset.y);

    return modelMatrix;
}

glm::mat3 GetReverseTransformationMatrix(Duck duck)
{
    glm::mat3 auxMatrix;
    auxMatrix = glm::mat3(1);

    float x = duck.components[5].offsetX + duck.offset.x + duck.translateX;
    float y = duck.components[5].offsetY + duck.offset.y + duck.translateY;

    auxMatrix *= transform2D::Translate(-duck.offset.x, -duck.offset.y);
    auxMatrix *= transform2D::Translate(-duck.translateX, -duck.translateY);
    auxMatrix *= transform2D::Translate(x, y);
    auxMatrix *= transform2D::Scale(1 / duck.scaleX, 1 / duck.scaleY);
    auxMatrix *= transform2D::Translate(-x, -y);
    auxMatrix *= transform2D::Translate(x, y);
    auxMatrix *= transform2D::Rotate((double)-duck.angularStep * duck.rotation * PI / 180);
    auxMatrix *= transform2D::Translate(-x, -y);
    if (duck.wallHit == 1) {
        auxMatrix *= transform2D::Translate(x, 0);
        auxMatrix *= transform2D::MirrorOY(-1);
        auxMatrix *= transform2D::Translate(-x, 0);
    }

    return auxMatrix;
}

void TriggerEvents(EventManager &eventManager)
{
    int val = rand() % 100 + 1;
    if (val % (100 / eventManager.changeSizeChance) == 0)
        eventManager.changeSize = true;

    val = rand() % 100 + 1;
    if (val % (100 / eventManager.speedIncreaseChance) == 0)
        eventManager.speedIncrease = true;

    val = rand() % 100 + 1;
    if (val % (100 / eventManager.changeColorChance) == 0)
        eventManager.changeColor = true;
}

void increaseChances(EventManager &eventManager)
{
    eventManager.changeSizeChance += 4;
    eventManager.speedIncreaseChance += 2;
    eventManager.randomBounceChance += 1;
    eventManager.changeColorChance += 1;
}
