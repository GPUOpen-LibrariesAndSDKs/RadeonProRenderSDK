#ifndef __RADEONPRORENDER_BAIKAL_H
#define __RADEONPRORENDER_BAIKAL_H

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif

/* rpr_material_node_input */
#define RPR_UBER_MATERIAL_LAYERS                        0x11000U
#define RPR_MATERIAL_INPUT_CAST_SHADOW                  0x11001U

/* rpr uber material layers */
#define RPR_UBER_MATERIAL_LAYER_EMISSION        (1<<0)
#define RPR_UBER_MATERIAL_LAYER_TRANSPARENCY    (1<<1)
#define RPR_UBER_MATERIAL_LAYER_COATING         (1<<2)
#define RPR_UBER_MATERIAL_LAYER_REFLECTION      (1<<3)
#define RPR_UBER_MATERIAL_LAYER_DIFFUSE         (1<<4)
#define RPR_UBER_MATERIAL_LAYER_REFRACTION      (1<<5)
#define RPR_UBER_MATERIAL_LAYER_SHADING_NORMAL  (1<<6)

/*rpr_material_node_arithmetic_operation*/
#define RPR_MATERIAL_NODE_OP_SAMPLER 0x1000
#define RPR_MATERIAL_NODE_OP_SAMPLER_BUMPMAP 0x1001
#define RPR_MATERIAL_NODE_OP_CONSTANT 0x1002
#define RPR_MATERIAL_NODE_OP_LOOKUP 0x1003

/* rpr_context_properties names */
#define RPR_CONTEXT_CREATEPROP_HYBRID_KERNELS_PATH_INFO 0x1600
#define RPR_CONTEXT_CREATEPROP_HYBRID_ACC_MEMORY_SIZE 0x1601
#define RPR_CONTEXT_CREATEPROP_HYBRID_VERTEX_MEMORY_SIZE 0x1602
#define RPR_CONTEXT_CREATEPROP_HYBRID_INDEX_MEMORY_SIZE 0x1603
#define RPR_CONTEXT_CREATEPROP_HYBRID_STAGING_MEMORY_SIZE 0x1604
#define RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_VCT 0x1605 // Tells Hybrid to enable support for VCT(Voxel Cone Tracing).
                                                        // Enabling this requires vulkan implementation to support VK_EXT_consevative_rasterisation
                                                        // and in case of VK inter-op mode it must be enabled on provided device
#define RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_PER_FACE_MATERIALS 0x1606 // Tells Hybrid to enable support for per-face materials.
                                                                       // This functionality requires additional memory on both -
                                                                       // CPU and GPU even when no per-face materials set in scene.
#define RPR_CONTEXT_CREATEPROP_HYBRID_FACE_MEMORY_SIZE 0x1607 // Size of per-face memory buffer in bytes. Used only if per-face materials enabled

struct RPRHybridKernelsPathInfo
{
    char const* baikal_kernels_path;
    char const* rr_kernels_path;
};

//to avoid overlap
#define RPR_CONTEXT_RANDOM_SEED     0x1000 // name: "randseed"
#define RPR_CONTEXT_RENDER_QUALITY  0x1001 // name: "render_quality"
#define RPR_CONTEXT_NUMBER_PRERENDERED_FRAMES 0x1002 // name: "num_prerendered_frames"
#define RPR_CONTEXT_SSAO_RADIUS     0x1003 // name: "ssao.radius"
#define RPR_CONTEXT_SSAO_POWER      0x1004 // name: "ssao.power"
#define RPR_CONTEXT_TAA_ACCUMULATE_SAMPLES 0X1006 // Obsolete. name: "taa.accumulate_samples"
#define RPR_CONTEXT_PT_UPSCALE_ENABLED 0X1007 // name: "pt.upscale_enabled"
#define RPR_CONTEXT_PT_UPSCALE_FACTOR 0X1008 // name: "pt.upscale_factor"
#define RPR_CONTEXT_RTAO_UPSCALE_ENABLED 0X1009 // name: "rtao.upscale_enabled"
#define RPR_CONTEXT_RTAO_UPSCALE_FACTOR 0X100A // name: "rtao.upscale_factor"
#define RPR_CONTEXT_VCT_RESOLUTION 0X100B // name: "vct.resolution". Only valid if RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_VCT was set during context creation
#define RPR_CONTEXT_PROGRESSIVE_LIGHT_BAKE_ENABLED 0X100C // name: "light_baker.enabled"
#define RPR_CONTEXT_AREA_LIGHT_SHADOWS_ENABLED 0X100D // name: "area_light_shadows.enabled"
#define RPR_CONTEXT_SPOT_LIGHT_DEPTH_BIAS 0x100E // name: "spot_light.depth_bias". type float
#define RPR_CONTEXT_POINT_LIGHT_DEPTH_BIAS 0x100F // name: "point_light.depth_bias. type float
#define RPR_CONTEXT_DIRECTIONAL_LIGHT_DEPTH_BIAS 0x1010 // name: "directional_light.depth_bias. type float
#define RPR_CONTEXT_STEREO_RENDER  0X1011 // name: "stereo_render"
#define RPR_CONTEXT_HIERARCHICAL_LIGHT_SAMPLING_ENABLED 0X1012 // name: "hierarchical_light_sampling_enabled"
#define RPR_CONTEXT_ENABLE_ARITHMETICS 0x1013 // Enables/Disables arithmetics in context. Changing this parameter must be followed by call to rprFramebufferClear
#define RPR_CONTEXT_NUMBER_CAMERA_JITTER_POINTS 0x1014 // Number of camera jitter points
#define RPR_CONTEXT_ENABLE_MSAA 0x1015 // Use MSAA instead of TAA
#define RPR_CONTEXT_FOG_DENSITY 0x1016 // Fog density
#define RPR_CONTEXT_FOG_MIE_G 0x1017 // Fog Mie g
#define RPR_CONTEXT_FOG_ENABLE_HALF_RESOLUTION 0x1018 // Compute fog in half resolution
#define RPR_CONTEXT_FOG_ENABLE_VISIBILITY 0x1019 // Enable visibility checks during fog computation
#define RPR_CONTEXT_FOG_USE_DXR_VISIBILITY 0x101A // Use DXR fog visibility checks during fog computation
#define RPR_CONTEXT_FOG_MAX_DISTANCE 0x101B // Max fog distance
#define RPR_CONTEXT_FOG_MAX_SCATTER_DISTANCE 0x101C // Max fog scatter distance
#define RPR_CONTEXT_FOG_MAX_VIEW_SAMPLES 0x101D // Max fog view samples count
#define RPR_CONTEXT_HYBRID_TRAVERSAL_MODE 0x101E // Bitflag of traversal modes
#define RPR_CONTEXT_USE_HALF_RES_SHADOWS 0x101F // Trace shadows at half resolution
#define RPR_CONTEXT_USE_HALF_RES_AO 0x1020 // Trace ambient occlusion at half resolution
#define RPR_CONTEXT_USE_HALF_RES_REFLECTIONS 0x1021 // Trace reflections at half resolution
#define RPR_CONTEXT_ENABLE_RELAXED_MATERIAL_CHECKS 0x101D // fallback (RPR_FALSE - whole material, RPR_TRUE - invalid input) to default value

/* Traversal modes */
#define RPR_HYBRID_TRAVERSAL_STATIC_TLAS_SEPARATE 0x1 ///< Use a separate acceleration structure for static objects
#define RPR_HYBRID_TRAVERSAL_STATIC_TLAS_INSTANCES 0x2 ///< Include instances into static tlas

/* possible render quality modes*/
#define RPR_RENDER_QUALITY_LOW 0u
#define RPR_RENDER_QUALITY_MEDIUM 1u
#define RPR_RENDER_QUALITY_HIGH 2u
#define RPR_RENDER_QUALITY_ULTRA 3u

/* Extended rpr_image_filter_type */
#define RPR_IMAGE_FILTER_TYPE_TRILINEAR 0x4
#define RPR_IMAGE_FILTER_TYPE_ANISOTROPIC 0x8

/* Shadow split count */
#define RPR_DIRECTIONAL_LIGHT_RASTER_NO_SHADOWS 0
#define RPR_DIRECTIONAL_LIGHT_RASTER_SPLITS_AUTO -1
#define RPR_DIRECTIONAL_LIGHT_RASTER_SPLITS_MAX 4

// rprContextGetInfo extension
#define RPR_CONTEXT_FRAMEBUFFERS_READY_SEMAPHORES 0x6000
#define RPR_CONTEXT_INTEROP_SEMAPHORE_INDEX 0x6001

// rprShapeGetInfo extension
/* rpr_shape_info */
#define RPR_SHAPE_LIGHTMAP_CHART_INDEX 0x1440

// rprMeshGetInfo extension
#define RPR_MESH_AABB 0x519

/**
 * Sets directional light shadow splits count for for rasterization renderer.
 *
 * This function sets cascade shadow map split count for directional light.
 * Additional splits improve quality of shadows but each split has the significant performance cost.
 * Special values for split count:
 * @li RPR_DIRECTIONAL_LIGHT_RASTER_NO_SHADOWS - turns off shadows completely.
 * @li RPR_DIRECTIONAL_LIGHT_RASTER_SPLITS_AUTO - calculates split count automatically.
 * Split count can't be greater than RPR_DIRECTIONAL_LIGHT_RASTER_SPLITS_MAX.
 *
 * @param light Directional light
 * @param splits Number of shadow splits
 */
extern RPR_API_ENTRY rpr_int rprDirectionalLightSetRasterShadowSplits(rpr_light light, rpr_int splits);

/**
 * Copies render data to all framebuffers connected as AOVs in interop mode.
 *
 * This functions makes renderer to wait on a user's semaphore, copy framebuffers
 * images and signal to a ready semaphore with the same index.
 *
 * @note User can retrieve current semaphore index by calling rprContextGetInfo function
 * with RPR_CONTEXT_INTEROP_SEMAPHORE_INDEX as the query type after calling
 * rprContextFlushFrameBuffers function.
 *
 * @param context RPR context
 */
typedef rpr_int (*rprContextFlushFrameBuffers_func)(rpr_context in_context);
#define RPR_CONTEXT_FLUSH_FRAMEBUFFERS_FUNC_NAME "rprContextFlushFrameBuffers"

/**
 * Set an index of the lighmap char for the shape.
 *
 * If progressive lightbaking mode is enabled, shape's lightmap chart index is being used
 * to determine which lighmap shape belongs to. UV1 set should be correctly laid out for
 * lightmapped geomtry.
 *
 * @note By default index is -1 (which means disable shape from baking).
 *
 * @param shape Shape to set index on.
 * @param Lightmap chart index.
 */
typedef rpr_int(*rprShapeSetLightmapChartIndex_func)(rpr_shape shape, rpr_int chart_index);
#define RPR_SHAPE_SET_LIGHTMAP_CHART_INDEX_FUNC_NAME "rprShapeSetLightmapChartIndex"

struct DataChange
{
    size_t stride; // Size in bytes between the start of two consecutive entries
    size_t src_offset; // Offset in source buffer
    size_t first_vertex; // Index of first vertex in update
    size_t vertex_count; // Elements to update
};

typedef rpr_int (*rprMeshUpdate_func)(rpr_shape mesh,
    rpr_float *vertex_array, DataChange *vertex_changes, size_t vertex_change_count, size_t vertex_array_size,
    rpr_float *normals_array, DataChange *normals_changes, size_t normal_change_count, size_t normals_array_size,
    rpr_float *uv0_array, DataChange *uv0_changes, size_t uv0_change_count, size_t uv0_array_size,
    rpr_float *uv1_array, DataChange *uv1_changes, size_t uv1_change_count, size_t uv1_array_size);
#define RPR_MESH_UPDATE_FUNC_NAME "rprMeshUpdate"

struct ShapeTransform
{
    rpr_shape shape;
    float transform[16];
};

typedef rpr_int (*rprShapeSetTransformBatch_func)(const ShapeTransform *transforms, size_t transform_count);
#define RPR_SHAPE_SET_TRANSFORM_BATCH_FUNC_NAME "rprShapeSetTransformBatch"

#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_BAIKAL_H  */
