#include "lab_m1/Tema2/Structs2.h"
#include "obj3D.h"

void InitializeCarComponents(Car& car)
{
	InitializeWheels(car);
	InitializeBottomBody(car);
	InitializeTopBody(car);
	InitializeBack(car);
	InitializeFront(car);
}

void InitializeWheels(Car& car)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    for (int i{}; i < 2; ++i) {
        for (int j{}; j < 2; ++j) {
            glm::mat4 auxMatrix = glm::translate(modelMatrix, glm::vec3(i * 4, 0, -0.25f + j * 3.5f));
            auxMatrix = glm::scale(auxMatrix, glm::vec3(1, 1, 1.25f));
            if (i == 0)
                car.components.push_back({ "frontWheel", auxMatrix });
            else
                car.components.push_back({ "backWheel", auxMatrix });
        }
    }
}

void InitializeBottomBody(Car& car)
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.2f, 1.48f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(6.7f, 1.9f, 2));
    car.components.push_back({ "body1", modelMatrix });

    modelMatrix = glm::mat4(1);
    for (int i{}; i < 2; ++i) {
        glm::mat4 auxMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.3f, 2.5f - 2 * i));
        auxMatrix = glm::scale(auxMatrix, glm::vec3(1.3f, 1.7f, 2));
        car.components.push_back({ "body11", auxMatrix });
    }

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 1.2f, 1.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(6.7f, 0.1f, 4));
    car.components.push_back({ "body11", modelMatrix });

    modelMatrix = glm::mat4(1);
    for (int i{}; i < 2; ++i) {
        for (int j{}; j < 2; ++j) {
            glm::mat4 auxMatrix = glm::translate(modelMatrix, glm::vec3(1 + 4 * i, 0.65f, 3 - 3 * j));
            auxMatrix = glm::translate(auxMatrix, glm::vec3(0, 0.5f, 0));
            auxMatrix = glm::scale(auxMatrix, glm::vec3(0.7f, 1.7f, 1));
            auxMatrix = glm::translate(auxMatrix, glm::vec3(0, -0.5f, 0));
            car.components.push_back({ "body2", auxMatrix });

            glm::mat4 auxMatrix2 = glm::translate(modelMatrix, glm::vec3(-1 + 4 * i, 0.65f, 3 - 3 * j));
            auxMatrix2 = glm::translate(auxMatrix2, glm::vec3(0, 0.5f, 0));
            auxMatrix2 = glm::scale(auxMatrix2, glm::vec3(0.7f, 1.7f, 1));
            auxMatrix2 = glm::translate(auxMatrix2, glm::vec3(0, -0.5f, 0));
            auxMatrix2 = glm::scale(auxMatrix2, glm::vec3(-1, 1, 1));
            car.components.push_back({ "body2", auxMatrix2 });
        }
    }
}

void InitializeTopBody(Car& car) 
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 1.75f, 1.5f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 4));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, -1));
    car.components.push_back({ "body22", modelMatrix });

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2.5f, 1.75f, 1.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 4));
    car.components.push_back({ "body1", modelMatrix });

    modelMatrix = glm::mat4(1);
    for (int i{}; i < 2; ++i) {
        glm::mat4 auxMatrix = glm::translate(modelMatrix, glm::vec3(2.5f, 1.75f, 3.51f - 4.02f * i));
        auxMatrix = glm::scale(auxMatrix, glm::vec3(1.5f, 0.75f, 1));
        car.components.push_back({ "window", auxMatrix });
    }

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 1.76f, 1.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 3.5f));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-45), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
    car.components.push_back({ "window", modelMatrix });

    modelMatrix = glm::mat4(1);
    for (int i = 0; i < 2; ++i) {
        glm::mat4 auxMatrix = glm::translate(modelMatrix, glm::vec3(0.6f, 1.75f, 3.51f - 4.02f * i));
        auxMatrix = glm::scale(auxMatrix, glm::vec3(1.5f, 0.75f, 1));
        auxMatrix = glm::rotate(auxMatrix, RADIANS(180), glm::vec3(0, 1, 0));
        car.components.push_back({ "window2", auxMatrix });
    }
}

void InitializeBack(Car& car) 
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(5.5f, 0.35f, 1.5f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 1.8f, 4));
    car.components.push_back({ "body11", modelMatrix });

    for (int i{}; i < 2; ++i) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(5.7f, 0, 2.8f - 2.6f * i));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.4f, 0.8f));
        car.components.push_back({ "stop1", modelMatrix });
    }

    for (int i{}; i < 2; ++i) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(5.7f, 0.45f, 2.8f - 2.6f * i));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.4f, 0.8f));
        car.components.push_back({ "stop2", modelMatrix });
    }
}

void InitializeFront(Car& car) 
{
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.35f, -0.15f, 1.48f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.8f, 4));
    car.components.push_back({ "body11", modelMatrix });

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.35f, 0.75f, 1.5f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 4));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 0, -1));
    car.components.push_back({ "body2", modelMatrix });

    for (int i{}; i < 2; ++i) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.4f, -0.15f, 2.8f - 2.6f * i));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.4f, 0.8f));
        car.components.push_back({ "stop1", modelMatrix });
    }
    
}

glm::vec3 GetSplinePoint(std::vector<glm::vec3> points, float t, bool loop)
{
    unsigned int p0, p1, p2, p3;
    if (!loop) {
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
    }
    else {
        p1 = (int)t;
        p2 = (p1 + 1) % points.size();
        p3 = (p2 + 1) % points.size();
        p0 = p1 > 1 ? p1 - 1 : points.size() - 1;
        
    }
    
    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -ttt + 2.0f * tt - t;
    float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
    float q3 = -3.0f * ttt + 4.0f * tt + t;
    float q4 = ttt - tt;

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

    return glm::vec3(tx, 0, tz);
}

glm::vec3 GetSplineGradient(std::vector<glm::vec3> points, float t, bool loop)
{
    unsigned int p0, p1, p2, p3;
    if (!loop) {
        p1 = (int)t + 1;
        p2 = p1 + 1;
        p3 = p2 + 1;
        p0 = p1 - 1;
    }
    else {
        p1 = (int)t;
        p2 = (p1 + 1) % points.size();
        p3 = (p2 + 1) % points.size();
        p0 = p1 > 1 ? p1 - 1 : points.size() - 1;

    }

    t = t - (int)t;

    float tt = t * t;
    float ttt = tt * t;

    float q1 = -3.0f * tt + 4.0f * t - 1.0f;
    float q2 = 9.0f * tt - 10.0f * t;
    float q3 = -9.0f * tt + 8.0f * t + 1.0f;
    float q4 = 3.0f * tt - 2.0f * t;

    float tx = 0.5f * (points[p0].x * q1 + points[p1].x * q2 + points[p2].x * q3 + points[p3].x * q4);
    float tz = 0.5f * (points[p0].z * q1 + points[p1].z * q2 + points[p2].z * q3 + points[p3].z * q4);

    return glm::vec3(tx, 0, tz);
}

void GenerateTreeLocations(std::vector<Tree>& treeLocations, std::vector<VertexFormat> vertices)
{
    for (int i{-25}; i < 25; i += 2) {
        for (int j{-15}; j < 35; j += 2) {
            int ok = 1;
            for (unsigned int k {}; k < vertices.size(); k += 2) {
                if (vertices[k].position.x > i - 0.5f && vertices[k].position.x < i + 0.5f
                    && vertices[k].position.z > j - 0.5f && vertices[k].position.z < j + 0.5f) {
                    ok = 0;
                    break;
                }

                if (vertices[k + 1].position.x > i - 0.5f && vertices[k + 1].position.x < i + 0.5f
                    && vertices[k + 1].position.z > j - 0.5f && vertices[k + 1].position.z < j + 0.5f) {
                    ok = 0;
                    break;
                }

                glm::vec3 D = vertices[k + 1].position - vertices[k].position;
                glm::vec3 mid = vertices[k].position + D / 2.0f;
                if (mid.x > i - 0.5f && mid.x < i + 0.5f
                    && mid.z > j - 0.5f && mid.z < j + 0.5f) {
                    ok = 0;
                    break;
                }
            }

            if (ok) {
                int chance = rand() % 100 + 1;
                if (chance % 4 == 0) {
                    Tree tree = GenerateTree(glm::vec3(i, 0.0f, j));
                    treeLocations.push_back(tree);
                }                
            }
        }
    }
}

Tree GenerateTree(glm::vec3 location)
{
    Tree tree;
    tree.location = location;
    tree.height = (rand() % 4 + 4) / 2.0f;
    float h = tree.height;
    for (int i{-2}; i < 3; ++i) {
        for (int j{-2}; j < 3; ++j) {
            if (i > -2 && i < 2 && j > -2 && j < 2
                && (i != 0 || j != 0)) {
                if (rand() % 2 == 0) {
                    tree.leaves.push_back(location + glm::vec3(i, h + 0.5f, j));
                }

                if (rand() % 2 == 0) {
                    tree.leaves.push_back(location + glm::vec3(i, h -1.0f, j));
                }

                if (rand() % 4 == 0) {
                    tree.leaves.push_back(location + glm::vec3(i, h - 1.5f, j));
                }
            }

            tree.leaves.push_back(location + glm::vec3(i, h, j));
            if (i != 0 || j != 0) {
                tree.leaves.push_back(location + glm::vec3(i, h - 0.5f, j));
            }
        }
    }

    return tree;
}

std::vector<glm::vec3> GenerateTrackSkeleton()
{
    std::vector<glm::vec3> res = {
    glm::vec3(0.0f, 0.0f, 13.4306f),
    glm::vec3(4.43752f, 0.0f, 17.2437f),
    glm::vec3(2.16808f, 0.0f, 27.1461f),
    glm::vec3(7.83801f, 0.0f, 28.1358f),
    glm::vec3(7.25999f, 0.0f, 18.2846f),
    glm::vec3(6.38631f, 0.0f, 11.8196f),
    glm::vec3(8.37407f, 0.0f, 9.11955f),
    glm::vec3(11.1628f, 0.0f, 9.16232f),
    glm::vec3(14.4108f, 0.0f, 13.7994f),
    glm::vec3(9.93937f, 0.0f, 16.3065f),
    glm::vec3(11.9318f, 0.0f, 23.5963f),
    glm::vec3(16.0829f, 0.0f, 24.2934f),
    glm::vec3(18.3974f, 0.0f, 20.3638f),
    glm::vec3(15.4463f, 0.0f, 8.19933f),
    glm::vec3(12.2475f, 0.0f, 3.77631f),
    glm::vec3(15.5743f, 0.0f, -0.621192f),
    glm::vec3(13.4629f, 0.0f, -3.39586f),
    glm::vec3(7.26562f, 0.0f, -4.0203f),
    glm::vec3(-8.99294f, 0.0f, -3.67992f),
    glm::vec3(-16.3098f, 0.0f, -2.41384f),
    glm::vec3(-19.9765f, 0.0f, -0.227883f),
    glm::vec3(-19.1018f, 0.0f, 1.66084f),
    glm::vec3(-16.3233f, 0.0f, 1.71358f),
    glm::vec3(-11.3884f, 0.0f, -0.0555892f),
    glm::vec3(-6.51227f, 0.0f, -0.515522f),
    glm::vec3(1.54098f, 0.0f, -0.750862f),
    glm::vec3(3.31074f, 0.0f, 4.56199f),
    glm::vec3(-3.70592f, 0.0f, 6.9789f),
    glm::vec3(-15.2876f, 0.0f, 5.15013f),
    glm::vec3(-16.0945f, 0.0f, 9.77388f),
    glm::vec3(-10.8165f, 0.0f, 9.64104f),
    glm::vec3(-5.12969f, 0.0f, 11.7947f)
    };

    return res;
}

void SetTrackLength(Track& track)
{
    glm::vec3 oldPoint = GetSplinePoint(track.trackSk, 0.0f);
    glm::vec3 newPoint;
    for (float t{}; t < (float)track.trackSk.size(); t += TRACK_SPACING) {
        newPoint = GetSplinePoint(track.trackSk, t, true);
        float dist = glm::distance(newPoint, oldPoint);
        track.distances[(int)t] += dist;
        oldPoint = newPoint;
    }

    for (auto dist : track.distances)
        track.length += dist;
}

void SetEnemyPositions(std::vector<EnemyCar>& enemies, Track track)
{
    for (EnemyCar& car : enemies) {
        car.posLength = (float)(rand() % ((int)track.length + 1)); 
        car.offset = (float)(rand() % 151 - 75) / 100.0f;
    }
}

void ConvertLengthToPoint(std::vector<EnemyCar>& enemies, Track track)
{
    for (EnemyCar& car : enemies) {
        int i = 0;
        float pos = car.posLength;
        while (pos > 0 && i != track.distances.size()) {
            if (pos - track.distances[i] > 0) {
                pos -= track.distances[i];
                ++i;
            }
            else {
                break;
            }
        }

        float t = pos / track.distances[i];
        glm::vec3 pointS = GetSplinePoint(track.trackSk, t + i, true);
        glm::vec3 pointG = GetSplineGradient(track.trackSk, t + i, true);

        float r = atan2(-pointG.z, pointG.x);
        glm::vec3 carPos = glm::vec3(car.offset * sin(r) + pointS.x, 0, car.offset * cos(r) + pointS.z);
        car.position = carPos;
        car.rotation = r * 180 / PI;
    }
}

std::pair<glm::vec3, float> ConvertLengthToPoint(float length, Track track)
{
        int i = 0;
        float pos = length;
        while (pos > 0 && i != track.distances.size()) {
            if (pos - track.distances[i] > 0) {
                pos -= track.distances[i];
                ++i;
            }
            else {
                break;
            }
        }

        float t = pos / track.distances[i];
        glm::vec3 pointS = GetSplinePoint(track.trackSk, t + i, true);
        glm::vec3 pointG = GetSplineGradient(track.trackSk, t + i, true);

        float r = atan2(-pointG.z, pointG.x);

        return make_pair(pointS, r);
}

void SetLights(std::vector<Light> &lights, Track track)
{
    float distance = track.length / LIGHTS;
    for (int i{}; i < (int)LIGHTS; ++i) {
        std::pair<glm::vec3, float> coord = ConvertLengthToPoint(i * distance, track);
        glm::vec3 pos1 = glm::vec3(1.2f * sin(coord.second) + coord.first.x, 0, 1.2f * cos(coord.second) + coord.first.z);
        glm::vec3 pos2 = glm::vec3(-1.2f * sin(coord.second) + coord.first.x, 0, -1.2f * cos(coord.second) + coord.first.z);

        lights.push_back({ pos1, coord.second });
        lights.push_back({ pos2, coord.second });
    }
}
