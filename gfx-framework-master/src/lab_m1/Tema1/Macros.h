#pragma once

constexpr auto PI = 3.141592;

namespace colors
{
	#define CYAN glm::vec3(0.5f, 1, 1)
	#define BLACK glm::vec3(0, 0, 0)
	#define WHITE glm::vec3(1, 1, 1)
	#define ORANGE glm::vec3(1, 0.5f, 0)
	#define DARK_GREEN glm::vec3(0, 0.5f, 0)
	#define YELLOW glm::vec3(1, 1, 0)
	#define LIGHT_GREY glm::vec3(0.7f, 0.7f, 0.7f)
	#define DARK_GREY glm::vec3(0.4f, 0.4f, 0.4f)
	#define LIGHT_BROWN glm::vec3(1, 0.8f, 0.5f)
	#define RED glm::vec3(1, 0, 0) 
	#define VIOLET glm::vec3(0.5f, 0, 1)
	#define PINK glm::vec3(1, 0.4f, 0.77f)
	#define BLUE glm::vec3(0, 0, 1)
	#define GREEN glm::vec3(0, 1, 0)
}

#define INITIAL_DIRECTION vector<short>(2, 0)
#define INITIAL_SPEED glm::vec2(300, 300)
#define ORIGIN glm::vec3(0, 0, 0)
#define UP glm::vec3(0, 1, 0)
#define FORWARD glm::vec3(0, 0, 1)
#define RIGHT glm::vec3(1, 0, 0)
#define ANGLE_RANGE 50
#define FULL_CIRCLE 360
#define MIN_ANGLE 30
#define MAX_ANGLE 80

#define WIDTH 0.2f
#define TRACK_SPACING 0.01f
#define CAR_ENEMIES 6
#define SPEED 5.0f
#define LIGHTS 25.0f
#define OFFSET glm::vec3(0, 0.3f, 0)
#define MINANGLE 120
#define MAXANGLE -150