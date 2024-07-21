/** !
 * @file resources/shaders/solid_color/frag.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Input
in vec3 xyz;

// Output
out vec4 FragColor;

// Entry point
void main()
{

   // Store the color in the framebuffer
   FragColor = vec4(vec3(0.0, 0.5, 0.5), 1.0f);
}