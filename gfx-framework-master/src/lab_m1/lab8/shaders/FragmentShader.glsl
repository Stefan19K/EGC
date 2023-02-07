#version 330

// Input
in vec3 world_pos;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction[2];
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int spotlightOn;
uniform float spotRadius;
uniform vec3 spotDirection;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

vec3 point_light_contribution(vec3 light_position, vec3 light_color, vec3 light_direction, int i)
{	
    vec3 N = normalize(world_normal);

    vec3 L = normalize( light_position - world_pos );

    vec3 V = normalize( eye_position - world_pos );

    vec3 R = reflect(-L, world_normal);

    float ambient_light = material_kd / 5;
    float diffuse_light = material_kd * max(dot(N, L), 0);
    float specular_light = 0;
    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
    specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    vec3 color;
    if (spotlightOn == 1) {
        float cut_off = radians(spotRadius);
        float spot_light = dot(-L, spotDirection);
        float spot_light_limit = cos(cut_off);
        float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        float light_att_factor = pow(linear_att, 2);

        if (spot_light > cos(cut_off)) {
            if (i == 0)
                color = object_color * ambient_light + object_color * (diffuse_light + specular_light) * light_att_factor;
            else
                color = object_color * (diffuse_light + specular_light) * light_att_factor;
        } else {
        if (i == 0)
            color = object_color * ambient_light;
        else
            color = vec3(0);
        }
    } else {
        float d = distance(light_position, world_pos);
        float factor = 1 / (pow(d, 2) + 1);
        if (i == 0)
            color = object_color * ambient_light + object_color * (diffuse_light + specular_light) * factor;
        else
            color = object_color * (diffuse_light + specular_light) * factor;
    }

	return color;
}

void main()
{
    vec3 light_color = vec3(1,1,1);
    vec3 color = point_light_contribution(light_position[0], light_color, light_direction[0], 0)
    + point_light_contribution(light_position[1], light_color, light_direction[1], 1);
    // TODO(student): Write pixel out color
    out_color = vec4(color, 1);

}
