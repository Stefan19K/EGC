#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Spline : public gfxc::SimpleScene
    {
    public:
        Spline();
        ~Spline();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 GetSplinePoint(std::vector<glm::vec3> points, float t, bool loop);
        glm::vec3 GetSplineGradient(std::vector<glm::vec3> points, float t, bool loop);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        // TODO(student): If you need any other class variables, define them here.
        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;

        float fov;
        float aspect;
        short projection;

        std::vector<glm::vec3> controlPoints;
        unsigned int selected;
    };
}   // namespace m1
