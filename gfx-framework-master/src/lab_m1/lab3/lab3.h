#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Lab3 : public gfxc::SimpleScene
    {
     public:
        Lab3();
        ~Lab3();

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

     protected:
        float cx, cy;
        float cx2, cy2;
        glm::mat3 modelMatrix;
        //glm::mat3 modelMatrix1;
        //glm::mat3 modelMatrix2;
        //glm::mat3 modelMatrix3;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;
        int count1;
        int count2;

        // TODO(student): If you need any other class variables, define them here.
        short jumpstate;
        short rotateState;
        float acceleration;
        float translateX2, translateY2;
        float angularStep2;
    };
}   // namespace m1
