#pragma once
#include <SDL2/SDL.h>

// Here are a few useful typedefs used throughout the engine
typedef unsigned char      u8;
typedef signed   char      s8;
typedef          char      i8;
typedef unsigned short     u16;
typedef signed   short     s16;
typedef          short     i16;
typedef unsigned long      u32;
typedef signed   long      s32;
typedef          long      i32;
typedef unsigned long long u64;
typedef signed   long long s64;
typedef          long long i64;
typedef float              f32;
typedef double             f64;

// 2D vector
struct GXvec2_s {
    float x,
          y;
};
typedef struct GXvec2_s vec2;

// 3D / 4D vector
struct GXvec4_s {
    float x,
          y,
          z,
          w;
};
typedef struct GXvec4_s vec4;
typedef struct GXvec4_s vec3;

// 2x2 matrix
struct GXmat2_s {
    float a, b,
          c, d;
};
typedef struct GXmat2_s mat2;

// 4x4 matrix
struct GXmat4_s {
    float a, b, c, d,
          e, f, g, h,
          i, j, k, l,
          m, n, o, p;
};
typedef struct GXmat4_s mat4;

// Quaternion
struct quaternion_s
{
    float u,
          i,
          j,
          k;
};
typedef struct quaternion_s quaternion;

// Insatnce 
struct GXInstance_s;

/* !
    An instance stores information about G10 in an application
 */
typedef struct GXInstance_s GXInstance_t;

// Threads
struct GXThread_s;
typedef struct GXThread_s GXThread_t;

// Tasks
struct GXTask_s;
typedef struct GXTask_s GXTask_s;


// Bind
struct GXBind_s;
/* !
    A bind is a set of callback function triggered by an input
*/
typedef struct GXBind_s GXBind_t;

// Input
struct GXInput_s;
typedef struct GXInput_s GXInput_t;

typedef struct { SDL_KeyCode code; const char* name; } kn_t;

// Scene type
struct GXScene_s;

/* !
   A scene stores lights, cameras, and entities
*/
typedef struct GXScene_s GXScene_t;

// Entity type
struct GXEntity_s;
typedef struct GXEntity_s GXEntity_t;

// Camera type
struct GXCamera_s;
typedef struct GXCamera_s GXCamera_t;

enum uniform_type_e;
enum uniform_type_e uniform_type_t;

// Light type
struct GXLight_s;
typedef struct GXLight_s GXLight_t;

// Bounding volume 
struct GXBV_s;
typedef struct GXBV_s GXBV_t;

// Skybox type
struct GXSkybox_s;
typedef struct GXSkybox_s GXSkybox_t;

// Part type
struct GXPart_s;
typedef struct GXPart_s GXPart_t;

// Shader type
struct GXShader_s;

/* !
   A shader contains a vulkan shader, its sets, descriptors, and push constant.
*/
typedef struct GXShader_s GXShader_t;

// Material 
struct GXMaterial_s;
typedef struct GXMaterial_s GXMaterial_t;

// Texture
struct GXTexture_s;
typedef struct GXTexture_s GXTexture_t;

// Transform
struct GXTransform_s;
typedef struct GXTransform_s GXTransform_t;

// Rigid body
struct GXRigidbody_s;
typedef struct GXRigidbody_s GXRigidbody_t;

// Collider 
struct GXCollider_s;
typedef struct GXCollider_s GXCollider_t;

// Collision
struct GXCollision_s;
typedef struct GXCollision_s GXCollision_t;

// Armature
struct GXRig_s;
typedef struct GXRig_s GXRig_t;

// Pose
struct GXPose_s;
typedef struct GXPose_s GXPose_t;

// Bone
struct GXBone_s;
typedef struct GXBone_s GXBone_t;

// Particle system
struct GXParticleSystem_s;
typedef struct GXParticleSystem_s GXParticleSystem_t;

// Particle
struct GXParticle_s;
typedef struct GXParticle_s GXParticle_t;

// AI
struct GXAI_s;
typedef struct GXAI_s GXAI_t;

// Sound
struct GXSound_s;
typedef struct GXSound_s GXSound_t;

// Server
struct GXServer_s;
typedef struct GXServer_s GXServer_t;

// Client
struct GXClient_s;
typedef struct GXClient_s GXClient_t;

// Command
struct GXCommand_s;
typedef struct GXCommand_s GXCommand_t;

// Connect command
struct connect_s;
typedef struct connect_s connect_t;

// Chat command
struct chat_s;
typedef struct chat_s chat_t;

// Renderer
struct GXRenderer_s;
typedef struct GXRenderer_s GXRenderer_t;

struct GXRenderPass_s;
typedef struct GXRenderPass_s GXRenderPass_t;

struct GXSubpass_s;
typedef struct GXSubpass_s    GXSubpass_t;

struct GXAttachment_s;
typedef struct GXAttachment_s GXAttachment_t;

// Scheduler
struct GXScheduler_s;

// A schedule is how G10 gets work done. A definable number of threads concurrently process lists of tasks. If that doesn't make sense to you, please just load the provided schedule.
typedef struct GXScheduler_s GXSchedule_t;

// Displace rotate command
struct displace_rotate_s;
typedef struct displace_rotate_s displace_rotate_t;

// Disconnect command
struct disconnect_s;
typedef struct disconnect_s disconnect_t;

// Camera controller
struct GXCameraController_s;
typedef struct GXCameraController_s GXCameraController_t;

// Passed on callback execution
struct callback_parameter_s;
typedef struct callback_parameter_s callback_parameter_t;
