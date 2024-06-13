/*
 * Geometry buffer fragment shader
 *
 * @file resources/shaders/gbuffer/frag.glsl
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
in vec3 xyz;
in vec2 uv;
in vec3 nxyz;

// Uniforms
uniform sampler2D diffuse;
uniform sampler2D specular;

// Entry point
void main ( )
{   

    // Store the position
    geometry_xyz = xyz;

    // Store the normal
    geometry_nxyz = normalize(nxyz);

    // Store the diffuse
    geometry_rgbe.rgb = texture(diffuse, uv).rgb;

    // Store the specular intensity
    geometry_rgbe.a = texture(specular, uv).r;

    // Done
}
