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
layout (location = 0) out vec3 xyz;

// Input
layout (set = 1, binding = 0) uniform UBO
{
   mat4x4 M;
   mat4x4 V;
   mat4x4 P;
};

void main()
{

   // Pass through xyz
   xyz = xyz_in;

   // Store the position in clip space
   gl_Position = P * V * M * vec4(xyz_in, 1.0f);
   
}