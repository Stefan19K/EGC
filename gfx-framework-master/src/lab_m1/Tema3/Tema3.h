#pragma once

#include<fstream>

#include "lab_m1/Tema3/Structs3.h"
#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"

using namespace std;
using namespace glm;

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
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

        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = NULL, bool animated = false);
        void RenderSkier();
        void RenderGround();
        void RenderObject(Obj& object);    
        void RenderObstacles();

        void GenerateObstacles();
        void ClearObstacles();

        bool CheckCollisions();

        void UpdateCounters();

        void ClearObjects();

        // Functions used for object creation.
        void CreateObjectsRoutine();
        void ModelateObjects();
        void EditObjects();
        void LoadFile();
        void SaveFile();
        // Functions used for object creation.

        void CreateMeshes();
        void CreateTextures();
        Texture2D* GenerateTexture(GLuint ID, unsigned int width, unsigned int height, vec3 color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;

        std::unordered_map<std::string, Texture2D*> mapTextures;

        float left;
        float right;
        float bottom;
        float top;
        float zNear;
        float zFar;

        float fov;
        float aspect;
        short projection;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        Skier skier;
        Ground ground;
        vector<TextTree> trees;
        vector<Rock> rocks;
        vector<Lamp> lamps;
        vector<Present> presents;
        Bulbs presentLights;
        Bulbs treeLights;
        vector<vec3> spotLights;

        short counter;
        int score;
        int highScore;
        bool gameState;

        // Variables used for manual object creation.
        bool createRoutine;
        bool createMode;
        bool editMode;
        bool loadMode;
        unsigned short creationState;
        unsigned short colorState;
        unsigned short textureState;
        unsigned short currObject;
        string meshID;
        string textureID;
        vec3 translation;
        vec3 rotation;
        vec3 scalation;
        vec3 color;
        mat4 matrix;
        vector<Component3> components;
    };
}   // namespace m1
