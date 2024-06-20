/*
 * Solid color fragment shader
 *
 * @file resources/shaders/solid_color/frag.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes 
layout (location = 0) out vec3 geometry_xyz;

// Output
out vec4 color;

// Uniforms
uniform vec3 solid_color;

// Entry point
void main ( )
{

    // Store the color
    color = vec4(solid_color, 1.0);
}
