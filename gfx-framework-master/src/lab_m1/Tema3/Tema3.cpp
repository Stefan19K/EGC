#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <lab_m1/Tema1/Macros.h>
#include <lab_m1/Tema2/obj3D.h>

using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    camera = new implemented::Camera();

    left = -2.0f;
    right = 2.0f;
    bottom = -2.0f;
    top = 2.0f;
    zNear = 0.01f;
    zFar = 200.0f;
    fov = RADIANS(60);
    aspect = window->props.aspectRatio;
    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    CreateMeshes();
    CreateTextures();
    skier = Skier(ORIGIN, vec3(30, 0, 0), vec3(0.1f));
    ground = Ground(ORIGIN, vec3(30, 0, 0), vec3(15.9436, 1, 15.9436));

    treeLights = Bulbs();
    presentLights = Bulbs();
    spotLights.push_back(ORIGIN);
    spotLights.push_back(ORIGIN);

    counter = 29;
    materialShininess = 2;
    materialKd = 0.25f;
    materialKs = 0.5f;

    score = 0;
    highScore = 0;
    gameState = true;

    // Initialize shaders to be used.
    Shader* shader = new Shader("SkiShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "skiShader", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "skiShader", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    // Initialize object creation variables.
    createRoutine = false;
    creationState = 0;
    createMode = false;
    editMode = false;
    loadMode = false;
    colorState = 0;
    textureState = 0;
    currObject = 0;
    meshID = "";
    textureID = "";
    translation = ORIGIN;
    rotation = ORIGIN;
    scalation = vec3(1);
    color = WHITE;
    matrix = mat4(1);
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    // Here is the code for the object creation and modelation.
    if (createRoutine) {
        CreateObjectsRoutine();
        DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
        return;
    }

    // Here is the code for the game.
    camera->Set(skier.center + vec3(0, 1.5f, 4), skier.center + vec3(0, 0, 2), UP);
    //camera->Set(skier.center + vec3(0, 4.5f, 15), skier.center + vec3(0, 0, 2), UP);
    if (gameState == false) {
        RenderGround();
        RenderObstacles();
        return;
    }

    RenderSkier();
    RenderGround();
    RenderObstacles();

    vec3 t = vec3(sin(RADIANS(skier.rotation.y)) * deltaTimeSeconds * 2, -sin(RADIANS(30)) * deltaTimeSeconds * 2, cos(RADIANS(30)) * deltaTimeSeconds * 2);
    skier.Translate(t);
    ground.Translate(t);

    ClearObstacles();
    if (CheckCollisions() == true) {
        skier.Translate(-t);
        ground.Translate(-t);
        ClearObjects();
        gameState = false;
        cout << "Score : " << score << "\n";
        cout << "HighScore : " << highScore << "\n";
        cout << "Press ENTER to restart the game.\n";
        return;
    }

    ground.textureTranslation.y -= deltaTimeSeconds / 8.0f;
    ground.textureTranslation.x += sin(RADIANS(skier.rotation.y)) * deltaTimeSeconds / 8.0f;

    UpdateCounters();
}


void Tema3::FrameEnd()
{
    // DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture, bool animated)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Bind texture coordinates
    vec2 texTranslation = vec2(0);
    if (animated)
        texTranslation = ground.textureTranslation;

    int loc_text_coords = glGetUniformLocation(shader->program, "v_texture_translation");
    glUniform2fv(loc_text_coords, 1, glm::value_ptr(texTranslation));

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);

    }

    glm::vec3 eyePosition = camera->position;
    int loc_eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(loc_eye_position, 1, glm::value_ptr(eyePosition));

    int loc_light_position = glGetUniformLocation(shader->program, "presentLights");
    glUniform3fv(loc_light_position, presentLights.lights.size(), glm::value_ptr(presentLights.lights[0]));

    loc_light_position = glGetUniformLocation(shader->program, "presentColors");
    glUniform3fv(loc_light_position, presentLights.colors.size(), glm::value_ptr(presentLights.colors[0]));

    loc_light_position = glGetUniformLocation(shader->program, "treeLights");
    glUniform3fv(loc_light_position, treeLights.lights.size(), glm::value_ptr(treeLights.lights[0]));

    loc_light_position = glGetUniformLocation(shader->program, "treeColors");
    glUniform3fv(loc_light_position, treeLights.colors.size(), glm::value_ptr(treeLights.colors[0]));

    loc_light_position = glGetUniformLocation(shader->program, "spotLights");
    glUniform3fv(loc_light_position, spotLights.size(), glm::value_ptr(spotLights[0]));

    int size = glGetUniformLocation(shader->program, "p_l_size");
    glUniform1i(size, presentLights.lights.size());

    size = glGetUniformLocation(shader->program, "t_l_size");
    glUniform1i(size, treeLights.lights.size());

    size = glGetUniformLocation(shader->program, "s_l_size");
    glUniform1i(size, spotLights.size());

    int kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(kd, materialKd);

    int ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(ks, materialKs);

    int shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(shininess, materialShininess);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
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


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 2.0f;

        if (window->KeyHold(GLFW_KEY_W)) {
            camera->TranslateForward(cameraSpeed * deltaTime);

        }

        if (window->KeyHold(GLFW_KEY_A)) {
            camera->TranslateRight(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            camera->TranslateForward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            camera->TranslateRight(cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_Q)) {
            camera->TranslateUpward(-cameraSpeed * deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_E)) {
            camera->TranslateUpward(cameraSpeed * deltaTime);
        }
    }
    else {
        if (createMode) {
            switch (creationState)
            {
            case 2:
                // Translation operations.
                if (window->KeyHold(GLFW_KEY_W))
                    translation -= vec3(0, 0, deltaTime);

                if (window->KeyHold(GLFW_KEY_S))
                    translation += vec3(0, 0, deltaTime);

                if (window->KeyHold(GLFW_KEY_D))
                    translation += vec3(deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_A))
                    translation -= vec3(deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_Q))
                    translation -= vec3(0, deltaTime, 0);

                if (window->KeyHold(GLFW_KEY_E))
                    translation += vec3(0, deltaTime, 0);

                // Scalation operations.
                if (window->KeyHold(GLFW_KEY_T))
                    scalation += vec3(deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_B))
                    scalation -= vec3(deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_Y))
                    scalation += vec3(0, deltaTime, 0);

                if (window->KeyHold(GLFW_KEY_N))
                    scalation -= vec3(0, deltaTime, 0);

                if (window->KeyHold(GLFW_KEY_P))
                    scalation -= vec3(0, 0, deltaTime);

                if (window->KeyHold(GLFW_KEY_M))
                    scalation += vec3(0, 0, deltaTime);

                // Rotation operations.
                if (window->KeyHold(GLFW_KEY_I))
                    rotation -= vec3(120 * deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_K))
                    rotation += vec3(120 * deltaTime, 0, 0);

                if (window->KeyHold(GLFW_KEY_L))
                    rotation += vec3(0, 0, 120 * deltaTime);

                if (window->KeyHold(GLFW_KEY_J))
                    rotation -= vec3(0, 0, 120 * deltaTime);

                if (window->KeyHold(GLFW_KEY_U))
                    rotation -= vec3(0, 120 * deltaTime, 0);

                if (window->KeyHold(GLFW_KEY_O))
                    rotation += vec3(0, 120 * deltaTime, 0);

                break;

            default:
                break;
            }
        } else if (editMode) {
            // Translation operations.
            if (window->KeyHold(GLFW_KEY_W))
                components[currObject].translation -= vec3(0, 0, deltaTime / 2.0f);

            if (window->KeyHold(GLFW_KEY_S))
                components[currObject].translation += vec3(0, 0, deltaTime / 2.0f);

            if (window->KeyHold(GLFW_KEY_D))
                components[currObject].translation += vec3(deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_A))
                components[currObject].translation -= vec3(deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_Q))
                components[currObject].translation -= vec3(0, deltaTime / 2.0f, 0);

            if (window->KeyHold(GLFW_KEY_E))
                components[currObject].translation += vec3(0, deltaTime / 2.0f, 0);

            // Scalation operations.
            if (window->KeyHold(GLFW_KEY_T))
                components[currObject].scalation += vec3(deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_B))
                components[currObject].scalation -= vec3(deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_Y))
               components[currObject].scalation += vec3(0, deltaTime / 2.0f, 0);

            if (window->KeyHold(GLFW_KEY_N))
                components[currObject].scalation -= vec3(0, deltaTime / 2.0f, 0);

            if (window->KeyHold(GLFW_KEY_P))
                components[currObject].scalation -= vec3(0, 0, deltaTime / 2.0f);

            if (window->KeyHold(GLFW_KEY_M))
                components[currObject].scalation += vec3(0, 0, deltaTime / 2.0f);

            // Rotation operations.
            if (window->KeyHold(GLFW_KEY_I))
                components[currObject].rotation -= vec3(120 * deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_K))
                components[currObject].rotation += vec3(120 * deltaTime / 2.0f, 0, 0);

            if (window->KeyHold(GLFW_KEY_L))
               components[currObject].rotation += vec3(0, 0, 120 * deltaTime / 2.0f);

            if (window->KeyHold(GLFW_KEY_J))
                components[currObject].rotation -= vec3(0, 0, 120 * deltaTime / 2.0f);

            if (window->KeyHold(GLFW_KEY_U))
                components[currObject].rotation -= vec3(0, 120 * deltaTime / 2.0f, 0);

            if (window->KeyHold(GLFW_KEY_O))
                components[currObject].rotation += vec3(0, 120 * deltaTime / 2.0f, 0);
        }
    }
}

void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (gameState == false && key == GLFW_KEY_ENTER) {
        score = 0;
        gameState = true;
        skier = Skier(ORIGIN, vec3(30, 0, 0), vec3(0.1f));
        ground = Ground(ORIGIN, vec3(30, 0, 0), vec3(15.9436, 1, 15.9436));
        camera->Set(skier.center + vec3(0, 4.5f, 15), skier.center + vec3(0, 0, 2), UP);
    }

    if (key == GLFW_KEY_F9)
    {
        createRoutine ^= 1;
        if (createRoutine) {
            camera->Set(glm::vec3(0, 1, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
            cout << "-------------- Creation Routine Initialized ---------------------\n"
                << "Menu:\n1 : CREATE - create new 3d models.\n"
                << "2 : EDIT - edit current 3D model stored in components vector.\n"
                << "3 : LOAD - load a 3D model created from the game files.\n"
                << "4 : SAVE - save current model in the game files.\n\n";
        } else {
            cout << "-------------- Creation Routine Canceled ---------------------\n";
            creationState = 0;
            editMode = false;
            createMode = false;
            loadMode = false;
            meshID = "";
            textureID = "";
            translation = ORIGIN;
            scalation = vec3(1);
            rotation = ORIGIN;
            color = WHITE;
            matrix = mat4(1);
        }
    }

    if (createRoutine && (!createMode && !editMode && !loadMode)) {
        switch (key)
        {
        case GLFW_KEY_1:
            createMode = 1;
            cout << "--------------- CREATE MODE ENABLED ------------------------\n"
                << "Press ENTER to start creating objects\n\n";
            break;

        case GLFW_KEY_2:
            editMode = 1;
            cout << "--------------- EDIT MODE ENABLED ------------------------\n"
                << "Press ENTER to save current texture on selected object\n\n";
            break;

        case GLFW_KEY_3:
            loadMode = 1;
            components.clear();
            cout << "--------------- LOAD MODE ENABLED ------------------------\n\n";
            LoadFile();
            cout << "--------------- LOAD MODE DISABLED ------------------------\n\n";
            cout << "Menu:\n1 : CREATE - create new 3d models.\n"
                << "2 : EDIT - edit current 3D model stored in components vector.\n"
                << "3 : LOAD - load a 3D model created from the game files.\n"
                << "4 : SAVE - save current model in the game files.\n\n";
            loadMode = 0;
            break;

        case GLFW_KEY_4:
            cout << "--------------- SAVE MODE ENABLED ------------------------\n\n";
            SaveFile();
            cout << "--------------- SAVE MODE DISABLED ------------------------\n\n";
            cout << "Menu:\n1 : CREATE - create new 3d models.\n"
                << "2 : EDIT - edit current 3D model stored in components vector.\n"
                << "3 : LOAD - load a 3D model created from the game files.\n"
                << "4 : SAVE - save current model in the game files.\n\n";
            break;

        default:
            break;
        }
    }    

    if (createMode && key == GLFW_KEY_BACKSPACE) {
        createMode = 0;
        creationState = 0;
        meshID = "";
        textureID = "";
        cout << "--------------- CREATE MODE DISABLED ------------------------\n\n";
        cout << "Menu:\n1 : CREATE - create new 3d models.\n"
            << "2 : EDIT - edit current 3D model stored in components vector.\n"
            << "3 : LOAD - load a 3D model created from the game files.\n"
            << "4 : SAVE - save current model in the game files.\n\n";
    } else if (editMode && key == GLFW_KEY_BACKSPACE) {
        editMode = 0;
        currObject = 0;
        cout << "--------------- EDIT MODE DISABLED ------------------------\n\n";
        cout << "Menu:\n1 : CREATE - create new 3d models.\n"
            << "2 : EDIT - edit current 3D model stored in components vector.\n"
            << "3 : LOAD - load a 3D model created from the game files.\n"
            << "4 : SAVE - save current model in the game files.\n\n";
    }

    if (createMode && key == GLFW_KEY_ENTER)
    {
        creationState = creationState < 3 ? creationState + 1 : 0;
        switch (creationState)
        {
        case 0:
            components.push_back(Component3(meshID, color, translation, rotation, scalation, textureID));
            cout << "\nPress ENTER to modelate another object.\n\n";
            break;

        case 1:
            cout << "Select 2D/3D model to use:\n1 : Cube\n2 : Prism\n3 : Cyllinder\n4 : Sphere\n"
                << "5 : Triangle\n6 : Square\n7 : Circle\n8 : Cone\n9 : Quad\n\n";
            break;

        case 2:
            cout << "Select matrix transformation parameters:\nWS - Translate Oz, AD - Translate Ox, QE - Translate Oy"
                << "\nIK - Rotate Ox, JL - Rotate Oz, UO - Rotate Oy\n"
                << "TB - Scale Ox, YN - Scale Oy, PM - Scale Oz\n\n";
            break;

        case 3:
            cout << "Choose color to use for object and press ENTER to save your changes:\n";
            break;

        default:
            break;
        }
    }

    if (createMode && ( creationState == 1 || creationState == 2))
    {
        switch (key)
        {
        case GLFW_KEY_1:
            meshID = "box";
            break;

        case GLFW_KEY_2:
            meshID = "prism";
            break;

        case GLFW_KEY_3:
            meshID = "cyllinder";
            break;

        case GLFW_KEY_4:
            meshID = "sphere";
            break;

        case GLFW_KEY_5:
            meshID = "triangle";
            break;

        case GLFW_KEY_6:
            meshID = "square";
            break;

        case GLFW_KEY_7:
            meshID = "circle";
            break;

        case GLFW_KEY_8:
            meshID = "cone";
            break;

        case GLFW_KEY_9:
            meshID = "quad";
            break;

        default:
            break;
        }
    }

    if (editMode) {
        string textureID;
        switch (textureState)
        {
        case 0:
            textureID = components[currObject].texture;
            break;

        case 1:
            textureID = "denim";
            break;

        case 2:
            textureID = "glove";
            break;

        case 3:
            textureID = "shirt";
            break;

        case 4:
            textureID = "shoe";
            break;

        case 5:
            textureID = "skin";
            break;

        case 6:
            textureID = "board";
            break;

        case 7:
            textureID = "wood";
            break;

        case 8:
            textureID = "leaves";
            break;

        case 9:
            textureID = "stone";
            break;

        case 10:
            textureID = "iron";
            break;

        case 11:
            textureID = "present";
            break;

        case 12:
            textureID = "snow";
            break;

        default:
            break;
        }

        if (key == GLFW_KEY_X)
            textureState = textureState < 12 ? textureState + 1 : 0;

        if (key == GLFW_KEY_Z)
            textureState = textureState > 0 ? textureState - 1 : 12;

        if (key == GLFW_KEY_ENTER)
            components[currObject].texture = textureID;
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            // Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            float angleX = (-deltaX) * sensivityOY;
            float angleY = (-deltaY) * sensivityOX;
            camera->RotateFirstPerson_OY(angleX);
            camera->RotateFirstPerson_OX(angleY);
        }
    }
    else {
        float sensivityOX = 0.1f;

        // Rotate the camera in first-person mode around
        // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
        // variables for setting up the rotation speed.
        float angleY = deltaX * sensivityOX;
        skier.rotation += vec3(0, angleY, 0);
        if (skier.rotation.y > 80)
            skier.rotation.y = 80;

        if (skier.rotation.y < -80)
            skier.rotation.y = -80;
    }
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (createMode && creationState == 3 &&  button == GLFW_MOUSE_BUTTON_2) {
        colorState = colorState < 8 ? colorState + 1 : 0;

        switch (colorState)
        {
        case 0:
            color = WHITE;
            textureID = "white";
            cout << "WHITE\n";
            break;
        
        case 1:
            color = RED;
            textureID = "red";
            cout << "RED\n";
            break;

        case 2:
            color = GREEN;
            textureID = "green";
            cout << "GREEN\n";
            break;

        case 3:
            color = BLUE;
            textureID = "blue";
            cout << "BLUE\n";
            break;

        case 4:
            color = YELLOW;
            textureID = "yellow";
            cout << "YELLOW\n";
            break;

        case 5:
            color = DARK_GREY;
            textureID = "grey";
            cout << "GREY\n";
            break;

        case 6:
            color = CYAN;
            textureID = "cyan";
            cout << "CYAN\n";
            break;

        case 7:
            color = LIGHT_BROWN;
            textureID = "brown";
            cout << "BROWN\n";
            break;

        default:
            break;
        }
    } else if (editMode && button == GLFW_MOUSE_BUTTON_2) {
        currObject = currObject < components.size() - 1 ? currObject + 1 : 0;
    }
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}

void Tema3::CreateObjectsRoutine()
{
    if (createMode) {
        ModelateObjects();
        return;
    }

    if (editMode) {
        EditObjects();
        return;
    }

}

void Tema3::ModelateObjects()
{
    matrix = mat4(1);
    matrix = translate(matrix, translation);
    matrix = rotate(matrix, RADIANS(rotation.x), vec3(1, 0, 0));
    matrix = rotate(matrix, RADIANS(rotation.y), vec3(0, 1, 0));
    matrix = rotate(matrix, RADIANS(rotation.z), vec3(0, 0, 1));
    matrix = scale(matrix, scalation);
    RenderMesh(meshes[meshID], shaders["Simple"], matrix);

    for (auto& comp : components) {
        RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], comp.matrix, mapTextures[comp.texture]);
    }
}

void Tema3::EditObjects()
{
    int i = 0;
    for (auto& comp : components) {
        if (i == currObject) {
            comp.matrix = mat4(1);
            comp.matrix = translate(comp.matrix, comp.translation);
            comp.matrix = rotate(comp.matrix, RADIANS(comp.rotation.x), vec3(1, 0, 0));
            comp.matrix = rotate(comp.matrix, RADIANS(comp.rotation.y), vec3(0, 1, 0));
            comp.matrix = rotate(comp.matrix, RADIANS(comp.rotation.z), vec3(0, 0, 1));
            comp.matrix = scale(comp.matrix, comp.scalation);
            RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], comp.matrix, mapTextures[comp.texture]);
        }
        else
            RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], comp.matrix, mapTextures[comp.texture]);

        i++;
    }
}

void Tema3::LoadFile()
{
    string fileName;
    cout << "Write the file name from which u want to load model data.\n";
    cin >> fileName;
    ifstream fin(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "objects", fileName + ".in"));
    if (!fin) {
        cout << "Could not open file. Please try again.\n";
        return;
    }

    string line;
    while (getline(fin, line)) {
        char name[32];
        char texture[32];
        vec3 color;
        vec3 translation;
        vec3 rotation;
        vec3 scalation;
        int params = sscanf(line.c_str(), "%32s%f%f%f%f%f%f%f%f%f%f%f%f%32s", name,
            &color.x, &color.y, &color.z,
            &translation.x, &translation.y, &translation.z,
            &rotation.x, &rotation.y, &rotation.z,
            &scalation.x, &scalation.y, &scalation.z, texture);
        name[strlen(name)] = '\0';
        texture[strlen(texture)] = '\0';
        components.push_back(Component3(name, color, translation, rotation, scalation, texture));
    }

    cout << "Data loaded with success.\n";
}

void Tema3::SaveFile()
{
    cout << "Saving object data. Please enter the file name where u want to save:\n";
    string file;
    cin >> file;
    cout << "\n";
    ofstream fout(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "objects", file));
    ofstream fout2(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "objects", file + ".in"));
    for (auto& comp : components) {
        fout << "components.push_back(Component3(\"" << comp.name << "\", "
            << "vec3(" << comp.color.x << ", " << comp.color.y << ", " << comp.color.z << "), "
            << "vec3(" << comp.translation.x << ", " << comp.translation.y << ", " << comp.translation.z << "), "
            << "vec3(" << comp.rotation.x << ", " << comp.rotation.y << ", " << comp.rotation.z << "), "
            << "vec3(" << comp.scalation.x << ", " << comp.scalation.y << ", " << comp.scalation.z << "), \""
            << comp.texture << "\"));\n";

        fout2 << comp.name << " " << comp.color.x << " " << comp.color.y << " " << comp.color.z << " "
            << comp.translation.x << " " << comp.translation.y << " " << comp.translation.z << " "
            << comp.rotation.x << " " << comp.rotation.y << " " << comp.rotation.z << " "
            << comp.scalation.x << " " << comp.scalation.y << " " << comp.scalation.z << " "
            << comp.texture << "\n";
    }

    fout.close();
    fout2.close();
    cout << "File saved succesfully.\n";
    meshID = "";
    textureID = "";
    translation = ORIGIN;
    rotation = ORIGIN;
    scalation = vec3(1);
    creationState = 0;
    createMode = false;
    loadMode = false;
    editMode = false;
    components.clear();
}

void Tema3::CreateMeshes()
{
    Mesh* box = obj3D::CreateBox("box1", BLUE - glm::vec3(0, 0, 0.8f));
    AddMeshToList(box);

    Mesh* cyllinder = obj3D::CreateCyllinder("cyllinder", glm::vec3(0, 0, -0.5f), 1, 1, 1, 180, RED);
    AddMeshToList(cyllinder);

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    Mesh* sphere = new Mesh("sphere");
    sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[sphere->GetMeshID()] = sphere;

    Mesh* cone = new Mesh("cone");
    cone->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
    meshes[cone->GetMeshID()] = cone;

    Mesh* prism = obj3D::CreatePrism("prism", GREEN);
    AddMeshToList(prism);

    Mesh* square = obj3D::CreateSquare("square", DARK_GREY);
    AddMeshToList(square);

    Mesh* quad = obj3D::CreateQuad("quad");
    AddMeshToList(quad);

    Mesh* triangle = obj3D::CreateTriangle("triangle", YELLOW);
    AddMeshToList(triangle);
}

void Tema3::CreateTextures()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    Texture2D* texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "denim.jpg").c_str(), GL_REPEAT);
    mapTextures["denim"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "glove.jpg").c_str(), GL_REPEAT);
    mapTextures["glove"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "shirt.png").c_str(), GL_REPEAT);
    mapTextures["shirt"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "shoe.jpg").c_str(), GL_REPEAT);
    mapTextures["shoe"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "skin.jpg").c_str(), GL_REPEAT);
    mapTextures["skin"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "board.jpg").c_str(), GL_REPEAT);
    mapTextures["board"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.jpg").c_str(), GL_REPEAT);
    mapTextures["wood"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "leaves.png").c_str(), GL_REPEAT);
    mapTextures["leaves"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "stone.png").c_str(), GL_REPEAT);
    mapTextures["stone"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "iron.jpg").c_str(), GL_REPEAT);
    mapTextures["iron"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "present.jpg").c_str(), GL_REPEAT);
    mapTextures["present"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.png").c_str(), GL_REPEAT);
    mapTextures["snow"] = texture;

    mapTextures["white"] = GenerateTexture(0, 500, 500, WHITE);

    mapTextures["grey"] = GenerateTexture(1, 500, 500, DARK_GREY);

    mapTextures["cyan"] = GenerateTexture(2, 500, 500, CYAN);

    mapTextures["red"] = GenerateTexture(3, 500, 500, RED);

    mapTextures["green"] = GenerateTexture(4, 500, 500, GREEN);

    mapTextures["yellow"] = GenerateTexture(5, 500, 500, YELLOW);

    mapTextures["brown"] = GenerateTexture(6, 500, 500, LIGHT_BROWN);

    mapTextures["blue"] = GenerateTexture(7, 500, 500, BLUE);
}

Texture2D* Tema3::GenerateTexture(GLuint ID, unsigned int width, unsigned int height, vec3 color)
{
    GLuint textureID = ID;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // Generate random texture data
    for (int i{}; i < size; i += 3) {
        data[i] = (int)(color.x * 255);
        data[i + 1] = (int)(color.y * 255);
        data[i + 2] = (int)(color.z * 255);
    }

    // Generate and bind the new texture ID
    unsigned int gl_texture_object;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate texture mip-maps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::RenderSkier()
{
    skier.SetTransformationMatrix();
    for (auto& comp : skier.components) {
        mat4 auxMatrix = skier.matrix;
        auxMatrix *= comp.matrix;

        RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], auxMatrix, mapTextures[comp.texture]);
    }
}

void Tema3::RenderGround()
{
    ground.SetTransformationMatrix();
    for (auto& comp : ground.components) {
        mat4 auxMatrix = ground.matrix;
        auxMatrix *= comp.matrix;

        RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], auxMatrix, mapTextures[comp.texture], true);
    }
}

void Tema3::RenderObject(Obj& object)
{
    object.SetTransformationMatrix();
    for (auto& comp : object.components) {
        mat4 auxMatrix = object.matrix;
        auxMatrix *= comp.matrix;

        RenderSimpleMesh(meshes[comp.name], shaders["SkiShader"], auxMatrix, mapTextures[comp.texture]);
    }
}

void Tema3::RenderObstacles()
{
    for (auto& obj : trees)
        RenderObject(obj);

    for (auto& obj : rocks)
        RenderObject(obj);

    for (auto& obj : lamps)
        RenderObject(obj);

    for (auto& obj : presents)
        RenderObject(obj);
}

void Tema3::GenerateObstacles()
{
    for (int i{}; i < (int)ground.scalation.x; ++i) {
        int r = rand() % 100;
        int chance;
        if (i < (int)ground.scalation.x / 2 - 3 || i >(int)ground.scalation.x / 2 + 3)
            chance = 10;
        else
            chance = 4;

        if (r % chance == 0) {
            int id = rand() % 4;
            float x = ground.center.x - ground.scalation.x / 2.0f + ground.scalation.x / (int)ground.scalation.x * i;
            float y = ground.center.y - ground.scalation.z / 2.0f * sin(RADIANS(ground.rotation.x));
            float z = ground.center.z + ground.scalation.z / 2.0f * cos(RADIANS(ground.rotation.x));
            
            if (id == 0) {
                TextTree tree = TextTree(vec3(x, y, z), ORIGIN, vec3(0.3f));
                trees.push_back(tree);
                treeLights.PushBack((tree.components[0].center + vec3(0, tree.scalation.y / 2.0f, 0)) * 0.3f + tree.translation);
            }
            else if (id == 1) {
                rocks.push_back(Rock(vec3(x, y, z), ORIGIN, vec3(0.3f)));
            }
            else if (id == 2) {
                Lamp lamp = Lamp(vec3(x, y, z), ORIGIN, vec3(0.4f));
                lamps.push_back(lamp);
                spotLights.push_back(lamp.components[2].center * 0.4f + lamp.translation);
                spotLights.push_back(lamp.components[3].center * 0.4f + lamp.translation);
            } 
            else if (id == 3) {
                Present present = Present(vec3(x, y + 0.3f * sin(RADIANS(ground.rotation.x)), z), vec3(30, 0, 0), vec3(0.3f));
                presents.push_back(present);
                presentLights.PushBack(present.components[0].center * 0.3f + present.translation);
            }
        }
    }
}

void Tema3::ClearObstacles()
{
    for (int i{}; i < trees.size(); ++i) {
        if (!IsOnGround(trees[i], ground)) {
            trees.erase(trees.begin() + i);
            treeLights.Erase(i + 1);
            --i;
        }
    }

    for (int i{}; i < rocks.size(); ++i) {
        if (!IsOnGround(rocks[i], ground)) {
            rocks.erase(rocks.begin() + i);
            --i;
        }
    }

    for (int i{}; i < lamps.size(); ++i) {
        if (!IsOnGround(lamps[i], ground)) {
            lamps.erase(lamps.begin() + i);
            spotLights.erase(spotLights.begin() + 2 * i + 3);
            spotLights.erase(spotLights.begin() + 2 * i + 2);
            --i;
        }
    }

    for (int i{}; i < presents.size(); ++i) {
        if (!IsOnGround(presents[i], ground)) {
            presents.erase(presents.begin() + i);
            presentLights.Erase(i + 1);
            --i;
        }
    }
}

bool Tema3::CheckCollisions()
{
    for (auto& obj : trees)
        if (obj.CheckCollision(obj, skier) == true)
            return true;

    for (int i{}; i < rocks.size(); ++i)
        if (rocks[i].CheckCollision(rocks[i], skier) == true) {
            return true;
        }

    for (auto& obj : lamps)
        if (obj.CheckCollision(obj, skier) == true)
            return true;

    for (int i{}; i < presents.size(); ++i)
        if (presents[i].CheckCollision(presents[i], skier) == true) {
            presents.erase(presents.begin() + i);
            presentLights.Erase(i + 1);
            --i;
            score += 10;
            if (highScore < score)
                highScore = score;
        }

    return false;
}

void Tema3::UpdateCounters()
{
    for (int i = 1; i < treeLights.lights.size(); ++i) {
        treeLights.resetCounters[i] += 1;
        if (treeLights.resetCounters[i] == 60) {
            treeLights.resetCounters[i] = 0;
            treeLights.CycleColors(i);
        }
    }

    counter++;
    if (counter == 30) {
        counter = 0;
        GenerateObstacles();
    }
}

void Tema3::ClearObjects() {
    trees.clear();
    treeLights.Clear();
    presents.clear();
    presentLights.Clear();
    rocks.clear();
    lamps.clear();
    spotLights.erase(spotLights.begin() + 2, spotLights.end());
}