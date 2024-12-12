/** !
 * @file resources/shaders/holdout/vert.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Attributes
layout (location = 0) in vec3 xyz_in;

// Output
out vec3 xyz;

// Input
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{

   // Pass through xyz
   xyz = xyz_in;

   // Store the position in clip space
   gl_Position = P * V * M * vec4(xyz_in, 1.0f);
   
}