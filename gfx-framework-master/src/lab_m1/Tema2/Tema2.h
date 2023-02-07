#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"
#include "components/text_renderer.h"

#include "lab_m1/Tema1/Macros.h"
#include "lab_m1/Tema2/Structs2.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateMeshes();

        void RenderCar(Car car);
        void RenderTrack();
        void RenderGround();
        void RenderTrees();
        void RenderLights();
        void RenderSpeedMeter();

        void CheckWallCollision();
        void CheckEnemyCollision(Car& car, EnemyCar& enemy);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        glm::vec3 skyColor;

        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;

        float fov;
        float aspect;
        short projection;

        ViewportArea miniViewportArea;
        ViewportArea miniViewportArea2;

        Car car;
        Track track;
        std::vector<Tree> treeLocations;
        std::vector<EnemyCar> enemies;
        std::vector<Light> lights;
        std::vector<glm::vec3> lightPositions;

        float scaleFactor;
        int nightMode;
        int frontCamera;
        int faze;
        int breakOn;
        int backOn;

        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        float speedometerAngle;

        int polygonMode;
    };
}   // namespace m1
