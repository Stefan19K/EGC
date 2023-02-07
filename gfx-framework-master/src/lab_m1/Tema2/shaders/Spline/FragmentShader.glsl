#version 330

// Input
in vec3 world_pos;
in vec3 world_normal;

uniform int lightCount;
uniform vec3 light_position[56];
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 spotDirection;

uniform vec3 object_color;

uniform int nightMode;

// Output
layout(location = 0) out vec4 out_color;

vec3 computeLight(vec3 lightPos, vec3 lightColor, vec3 world_pos, vec3 world_normal, int i)
{
    vec3 L = normalize( lightPos - world_pos );

    vec3 N = normalize(world_normal);

    vec3 V = normalize( eye_position - world_pos );

    vec3 R = reflect(-L, N);

    float diffuse_light =  material_kd * max(dot(N, L), 0);

    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    vec3 color;
    float d = distance(lightPos, world_pos);
    float factor = 1 / (pow(d, 2) + 1);

    color = object_color * (diffuse_light + specular_light) * lightColor * factor;

    return color;
}

void main()
{
    vec3 color;
    if (nightMode == 1) {
        color = vec3(0); 
        float ambient_light = material_kd / 2.0f;
        
        int i;
        float d = distance(light_position[0], world_pos);
        for (i = 1; i < lightCount - 6; i++) {
            float dist = distance(light_position[i], world_pos);
            if (dist < d) {
                d = dist;
            }
        }
        
        float factor = 1 / (pow(d, 2) + 1);
        color = object_color * ambient_light * factor * 2;
        color += object_color * 0.1f;
        
        for (i = 0; i < lightCount - 6; i++) {
            if (i < 50)
                color = color + computeLight(light_position[i], vec3(1, 1, 1), world_pos, world_normal, i);
            else
                color = color + computeLight(light_position[i], vec3(0, 0, 1), world_pos, world_normal, i);
        }
    } else {
        color = object_color;
    }

    out_color = vec4(color, 1);
}
