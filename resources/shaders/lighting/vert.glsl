/*
 * Deferred vertex shader
 *
 * @file resources/shaders/lighting/vert.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes
layout (location = 0) in vec3 xyz_in;
layout (location = 1) in vec2 uv_in;

// Output
out vec2 uv;

// Entry point
void main ( )
{

    // Pass through the texture coordinates
    uv = uv_in;
    
    // Store the position in clip space
    gl_Position = vec4(xyz_in, 1.0);
    
    // Done
}
