#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

#include "lab_m1/Tema1/Macros.h"
#include "Structs.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateMeshes();

        int RenderScene(float deltaTimeSeconds);
        void RenderIdleBackground();
        void RenderRunningBackground();
        void RenderDuck();

        void CheckConditions(float deltaTimeSeconds);
        void CheckScreenCollision();
        void SetNextFrameDuck(float deltaTimeSeconds);

        void ShowHitboxes(glm::mat3 modelMatrix);

    protected:
        glm::mat3 modelMatrix;
        glm::vec3 skyColor;

        GameState gameState;
        Gamemode gameMode;
        EventManager eventManager;

        Duck duck;
        Heart hearts;
        Bullet bullets;

        // The actual renderer object
        gfxc::TextRenderer* textRenderer;

        // Draw primitives mode
        GLenum polygonMode;

        // Colors
        const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
        const glm::vec3 kBackgroundColor = NormalizedRGB(41, 45, 62);

        float playTime;
        float cursorX;
        float cursorY;

        unsigned int score;
        unsigned int highscore;

        unsigned int level;
        unsigned int killCount;

        float camLeft, camRight, camBottom, camTop;
        bool scopeOn;
        float scopeSpeed;

        float renderX;
        float renderY;
        float renderTime;

        short killStreak;
    };
}   // namespace m1
