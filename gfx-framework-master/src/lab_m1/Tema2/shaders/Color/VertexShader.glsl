#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 carPos;
uniform float scaleFactor;

// Output values to fragment shader
out vec3 world_pos;
out vec3 world_normal;

void main()
{
    world_pos = (Model * vec4(v_position,1)).xyz;
    world_normal = normalize( mat3(Model) * v_normal );
    
    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * vec4(world_pos - pow(distance(carPos, world_pos), 2) * scaleFactor, 1.0);
}