/** !
 * @file resources/shaders/holdout/vert.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes
layout (location = 0) in vec3 xyz_in;

// Input
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{

   // Store the position in clip space
   gl_Position = P * V * M * vec4(xyz_in, 1.0f);
}