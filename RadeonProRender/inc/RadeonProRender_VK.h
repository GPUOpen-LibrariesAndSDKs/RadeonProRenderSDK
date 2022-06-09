/*****************************************************************************\
*
*  Module Name    FireRender_GL.h
*  Project        FireRender Engine OpenGL Interop API
*
*  Description    Fire Render Engine OpenGL Interop header
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_VK_H
#define __RADEONPRORENDER_VK_H

#include "RadeonProRender.h"

#ifdef __cplusplus
extern "C" {
#endif

/* rpr_creation_flags */
#define RPR_CREATION_FLAGS_ENABLE_VK_INTEROP (1<<11)

/* rpr_framebuffer_properties */
#define RPR_VK_IMAGE_OBJECT 0x5001
#define RPR_VK_IMAGE_VIEW_OBJECT 0x5002
#define RPR_FRAMEBUFFER_READY_VK_SEMAPHORES 0x5003
#define RPR_FRAMEBUFFER_RELEASED_VK_SEMAPHORES 0x5004

/* rpr_image_propertis */
#define RPR_IMAGE_VK_OBJECT 0x5301
#define RPR_IMAGE_VK_VIEW_OBJECT 0x5302

// rprMeshGetInfo extension
/* rpr_mesh_info */
/**
 * Mesh interop functionality provides ability for application to get index and vertex buffer from
 * Hybrid renderer. This data accessible only in case of enabled VK interop.
 * rprMeshGetInfo function will return VkBuffer and transfer it's ownership.
 * Provided VkBuffer will point to internal GPU buffer.
 * Application responsible to destroying such buffer.
 * Following restrictions exists when application works with such buffer:
 * @li data must not be modified
 * @li data access must be synchronized by application
 * @li data provided only from main device
 * @li application should track moment of mesh deletion and destroy such buffer
 * @li each call to this function will return NEW buffer even if it is requested for same mesh
 * (e.g. no caching performed on plugin side)
 */
#define RPR_MESH_VK_MESH_BUFFER 0x5501      // Provides access to vkBuffer with mesh data (on main instance)
                                            // Access is read only.
                                            // This info can be requested only in VK interop mode

// rpr_mesh_info extensions
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFERS 0x5502         // Provides access to all vkBuffer-s with mesh data (on main instance),
                                                        // which created with RPR_MESH_IS_DYNAMIC_MESH flag
                                                        // Write to buffer with index RPR_MESH_DYNAMIC_MESH_INDEX_TO_MODIFY,
                                                        // signal to semaphore, then call rprVkDynamicMeshModified()
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_INDEX_TO_MODIFY 0x5503 // Index of dynamic vertex buffer that we can modify
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_SIGNAL_SEMAPHORES 0x5504 // Index of semaphore to be signaled when the dynamic buffer modification is complete
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_POSITION_OFFSET 0x5505 // Offset of position's float3 relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_POSITION_STRIDE 0x5506 // Stride of the position (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_NORMAL_OFFSET 0x5507 // Offset of normal's float3 relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_NORMAL_STRIDE 0x5508 // Stride of the normal (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_UV0_OFFSET 0x5509 // Offset of first UV set's float2 relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_UV0_STRIDE 0x550A // Stride of the first UV set (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_UV1_OFFSET 0x550B // Offset of second UV set's float2 relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_UV1_STRIDE 0x550C // Offset of the second UV set (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_ATTRIBUTE0_OFFSET 0x550D // Offset of first attribute relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_ATTRIBUTE1_OFFSET 0x550E // Offset of second attribute relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_ATTRIBUTE2_OFFSET 0x550F // Offset of third attribute relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_ATTRIBUTE3_OFFSET 0x5510 // Offset of fourth attribute relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_ATTRIBUTE_STRIDE 0x5511 // Stride of the attributes (in bytes)

typedef rpr_int(*rprVkDynamicMeshModified_func)(rpr_shape mesh, size_t num_vertices, size_t num_indices); // Notify rpr that dynamic vertex buffer was modifed.
                                                                                                          // Semaphore with appropriate index must be signaled before this call
#define RPR_MESH_VK_DYNAMIC_MESH_BUFFER_MODIFED_FUNC_NAME "rprVkDynamicMeshModified"


/* rpr_context_properties names */
#define RPR_CONTEXT_CREATEPROP_VK_INTEROP_INFO 0x5601

/**
 * Provides information for vulkan instance creation
 *
 * Each device should have following extensions activated:
 * @li VK_EXT_descriptor_indexing
 *
 * Following features must be enabled on device:
 * @li shaderSampledImageArrayNonUniformIndexing
 * @li geometryShader
 * @li anisotropicSampler
 */
struct VkInteropInfo
{
    rpr_uint instance_count; ///< Amount of vulkan instances
    rpr_uint main_instance_index; ///< Index of main vulkan instance in instances array
    rpr_uint frames_in_flight; ///< Number of frames in flight
    // VkSemaphore-s signaling that framebuffers are released (by the number of frames in flight)
    void* framebuffers_release_semaphores;

    struct VkInstance
    {
        void *physical_device;
        void *device;
    };

    VkInstance *instances; ///< Can be freed right after return from rprCreateContext
};

struct BufferInfo
{
    void* buffer_handle;
    size_t offset;
};

#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_GL_H  */
