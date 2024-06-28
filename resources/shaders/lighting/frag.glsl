/** !
 * Deferred fragment shader
 *
 * @file resources/shaders/lighting/frag.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes 
layout (location = 0) out vec3 geometry_xyz;
layout (location = 1) out vec3 geometry_nxyz;
layout (location = 2) out vec4 geometry_rgbe;

// Input
in vec2 uv;

// Output
out vec4 color;

// Constants
const int LIGHTS_QUANTITY = 32;

// Structure definitions
struct Light
{
    vec3 position, color;
    float linear, quadratic;
};

// Uniforms
uniform sampler2D geometry_diffuse;
uniform sampler2D geometry_normal;
uniform sampler2D geometry_albedo_specular;
uniform Light lights[LIGHTS_QUANTITY];
uniform vec3 view_position;

// Entry point
void main ( )
{

    // Initialized data
    vec3  xyz            = texture(geometry_diffuse, uv).rgb;
    vec3  nxyz           = texture(geometry_normal, uv).rgb;
    vec3  rgb            = texture(geometry_albedo_specular, uv).rgb;
    float s              = texture(geometry_albedo_specular, uv).a;
    vec3  lighting       = rgb * 0.1; 
    vec3  view_direction = normalize(view_position - xyz);

    // Iterate through each light
    for ( int i = 0; i < LIGHTS_QUANTITY; ++i )
    {

        // Initialized data
        float dist            = length(lights[i].position - xyz);
        vec3  light_direction = normalize(lights[i].position - xyz);
        vec3  diffuse         = max(dot(nxyz, light_direction), 0.0) * rgb * lights[i].color;
        vec3  half            = normalize(light_direction + view_direction);  
        float spec            = pow(max(dot(nxyz, half), 0.0), 16.0);
        vec3  specular        = lights[i].color * spec * s;
        float attenuation     = 1.0 / (1.0 + lights[i].linear * dist + lights[i].quadratic * dist * dist);

        // Scale diffuse by attenuation
        diffuse *= attenuation;

        // Scale specular intensity by attenuation
        specular *= attenuation;

        // Accumulate diffuse and specular components
        lighting += diffuse + specular;
    }

    // Store the color
    color = vec4(lighting, 1.0);
}
