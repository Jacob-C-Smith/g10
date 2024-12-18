/** !
 * @file resources/shaders/holdout/frag.glsl
 *
 * @author Jacob Smith
 */

// 4.6
#version 460 core

// Output
out vec4 color;

// Entry point
void main()
{

   // Store the color in the framebuffer
   color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}