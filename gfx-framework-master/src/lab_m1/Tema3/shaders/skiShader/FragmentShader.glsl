#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 world_pos;
in vec3 world_normal;
in vec2 text_coord;

// Uniform properties
uniform sampler2D u_texture_0;

uniform vec3 presentLights[32];
uniform vec3 presentColors[32];
uniform int p_l_size;

uniform vec3 treeLights[32];
uniform vec3 treeColors[32];
uniform int t_l_size;

uniform vec3 spotLights[64];
uniform int s_l_size;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// Output
layout(location = 0) out vec4 out_color;

vec3 computeUniformLight(vec3 lightPos, vec3 lightColor, float ks, float kd, float kq, vec3 object_color)
{
    vec3 L = normalize( lightPos - world_pos );

    vec3 N = normalize(world_normal);

    vec3 V = normalize( eye_position - world_pos );

    vec3 R = reflect(-L, N);

    float diffuse_light =  kd * max(dot(N, L), 0);

    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = ks * pow(max(dot(V, R), 0), material_shininess);
    }

    vec3 color;
    float d = distance(lightPos, world_pos);
    float factor = 1 / (kq * pow(d, 2) + kq * d);

    color = object_color * (diffuse_light + specular_light) * lightColor * factor;

    return color;
}

vec3 computeSpotLight(vec3 lightPos, vec3 lightColor, float ks, float kd, vec3 object_color)
{
    vec3 L = normalize( lightPos - world_pos );

    vec3 N = normalize(world_normal);

    vec3 V = normalize( eye_position - world_pos );

    vec3 R = reflect(-L, N);

    float diffuse_light =  kd * max(dot(N, L), 0);

    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = ks * pow(max(dot(V, R), 0), material_shininess * 0.2f);
    }

    float cut_off = radians(30.0f);
    float spot_light = dot(-L, vec3(0, -1.0f, 0));
    float spot_light_limit = cos(cut_off);
    float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
    float light_att_factor = pow(linear_att, 2);

    vec3 color;
    if (spot_light > cos(cut_off)) {
        color = object_color * (diffuse_light + specular_light) * light_att_factor;
    } else {
        color = vec3(0);
    }

    return color;
}

vec3 computeDirectionalLight(vec3 object_color)
{
    vec3 L = vec3(0, -0.5f, 0.3f);

    vec3 N = normalize(world_normal);

    vec3 V = normalize( eye_position - world_pos );

    vec3 R = reflect(-L, N);

    float diffuse_light =  material_kd * 2 * max(dot(N, L), 0);

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

    vec3 color = object_color * (diffuse_light + specular_light);

    return color;
}

void main()
{
    vec4 obj_color = texture(u_texture_0, text_coord);
    vec3 uniformColor = vec3(0);
    vec3 spotColor = vec3(0);
    vec3 directionalColor = vec3(0);
    
    directionalColor = computeDirectionalLight(obj_color.xyz);

    for (int i = 1; i < t_l_size; i++)
        uniformColor = uniformColor + computeUniformLight(treeLights[i], treeColors[i], material_ks * 5, material_kd * 5, 5, obj_color.xyz);
       
    for (int i = 1; i < p_l_size; i++)
        uniformColor = uniformColor + computeUniformLight(presentLights[i], presentColors[i], material_ks, material_kd * 2, 10, obj_color.xyz);

    for (int i = 2; i < s_l_size; i++)
        spotColor = spotColor + computeSpotLight(spotLights[i], vec3(1), material_ks, material_kd, obj_color.xyz);

    out_color = vec4(directionalColor + uniformColor + spotColor, obj_color.a);
    // out_color = obj_color;
}
