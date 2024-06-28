/** !
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

void main()
{

   // Pass through xyz
   xyz = xyz_in;

   // Store the position in clip space
   gl_Position = mat4(
      vec4(0.5 ,0   ,0   ,0),
      vec4(0   ,0.5 ,0   ,0),
      vec4(0   ,0   ,0.5 ,0),
      vec4(0   ,0   ,0   ,1)
   ) * vec4(xyz_in.x, xyz_in.y, xyz_in.z, 1.0);
}