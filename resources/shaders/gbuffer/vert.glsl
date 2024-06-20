/*
 * Solid color vertex shader
 *
 * @file resources/shaders/solid_color/vert.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes
layout (location = 0) in vec3 xyz_in;

// Output
out vec3 xyz;

// Uniforms
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

// Entry point
void main ( )
{

    // Initialized data
    vec4 xyz_world = M * vec4(xyz_in, 1.0);
    mat3 normal_matrix = transpose(inverse(mat3(M)));

    // Store world coordinates
    xyz = xyz_world.xyz; 

    // Store the position in clip space
    gl_Position = P * V * xyz_world;
    
    // Done
}
