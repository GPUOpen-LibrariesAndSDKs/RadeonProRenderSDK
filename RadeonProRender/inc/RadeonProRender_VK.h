/*****************************************************************************\
*
*  Module Name    FireRender_GL.h
*  Project        FireRender Engine OpenGL Interop API
*
*  Description    Fire Render Engine OpenGL Interop header
*
*  Copyright 2011 - 2013 Advanced Micro Devices, Inc.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
*  @author Dmitry Kozlov (dmitry.kozlov@amd.com)
*  @author Takahiro Harada (takahiro.harada@amd.com)
*  @bug No known bugs.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_VK_H
#define __RADEONPRORENDER_VK_H

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
#define RPR_MESH_VK_VERTEX_BUFFER 0x5501 // Provides access to vkBuffer with vertex data for mesh (on main instance)
                                         // Access is read only.
                                         // This info can be requested only in VK interop mode
#define RPR_MESH_VK_INDEX_BUFFER  0x5502 // Provides access to vkBuffer with index data for mesh (on main instance)
                                         // Access is read only.
                                         // This info can be requested only in VK interop mode

// rpr_mesh_info extensions
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFERS 0x5503   // Provides access to all vkBuffer-s with vertex data for mesh (on main instance), which created with RPR_MESH_IS_DYNAMIC_MESH flag
                                                    // Write to buffer with index RPR_MESH_DYNAMIC_MESH_INDEX_TO_MODIFY, signal to semaphore, then call rprVkDynamicMeshModified()
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFER_INDEX_TO_MODIFY 0x5504 // Index of dynamic vertex buffer that we can modify
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFER_SIGNAL_SEMAPHORES 0x5505 // Index of semaphore to be signaled when the dynamic buffer modification is complete
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFER_POSITION_OFFSET 0x5506 // Offset of position's float3 relative to the beginning of the buffer (in bytes)
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFER_NORMAL_OFFSET 0x5507 // Offset of normal's float3 relative to the beginning of the buffer (in bytes)

typedef rpr_int(*rprVkDynamicMeshModified_func)(rpr_shape mesh); // Notify rpr that dynamic vertex buffer was modifed. Semaphore with appropriate index must be signaled before this call
#define RPR_MESH_VK_DYNAMIC_VERTEX_BUFFER_MODIFED_FUNC_NAME "rprVkDynamicMeshModified"


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
    uint32_t instance_count; ///< Amount of vulkan instances
    uint32_t main_instance_index; ///< Index of main vulkan instance in instances array
    uint32_t frames_in_flight; ///< Number of frames in flight
    // VkSemaphore-s signaling that framebuffers are released (by the number of frames in flight)
    void* framebuffers_release_semaphores;

    struct VkInstance
    {
        void *physical_device;
        void *device;
    };

    VkInstance *instances; ///< Can be freed right after return from rprCreateContext
};

#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_GL_H  */
