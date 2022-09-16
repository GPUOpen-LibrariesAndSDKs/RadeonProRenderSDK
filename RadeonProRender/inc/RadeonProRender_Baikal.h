/*****************************************************************************\
*
*  Module Name    RadeonProRender_Baikal.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

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
#define RPR_UBER_MATERIAL_LAYER_EMISSION             (1<<0)
#define RPR_UBER_MATERIAL_LAYER_TRANSPARENCY         (1<<1)
#define RPR_UBER_MATERIAL_LAYER_COATING              (1<<2)
#define RPR_UBER_MATERIAL_LAYER_REFLECTION           (1<<3)
#define RPR_UBER_MATERIAL_LAYER_DIFFUSE              (1<<4)
#define RPR_UBER_MATERIAL_LAYER_REFRACTION           (1<<5)
#define RPR_UBER_MATERIAL_LAYER_SHADING_NORMAL       (1<<6)
#define RPR_UBER_MATERIAL_LAYER_TRANSPARENCY_MASK    (1<<7)
#define RPR_UBER_MATERIAL_LAYER_BACKSCATTER          (1<<8)

/*rpr_material_node_arithmetic_operation*/
#define RPR_MATERIAL_NODE_OP_CEIL 0x102a
#define RPR_MATERIAL_NODE_OP_ROUND 0x102b
#define RPR_MATERIAL_NODE_OP_SIGN 0x102c
#define RPR_MATERIAL_NODE_OP_SQRT 0x102f
#define RPR_MATERIAL_NODE_OP_LOG2 0x1030
#define RPR_MATERIAL_NODE_OP_LOG10 0x1031
#define RPR_MATERIAL_NODE_OP_TRUNCATE 0x1032
#define RPR_MATERIAL_NODE_OP_SHUFFLE 0x1033
#define RPR_MATERIAL_NODE_OP_SHUFFLE2 0x1034
#define RPR_MATERIAL_NODE_OP_CLAMP 0x1035
#define RPR_MATERIAL_NODE_OP_SATURATE 0x1036
#define RPR_MATERIAL_NODE_OP_IF 0x1037

#define RPR_MATERIAL_NODE_OP_SELECT_0000 0x1038
#define RPR_MATERIAL_NODE_OP_SELECT_0001 0x1039
#define RPR_MATERIAL_NODE_OP_SELECT_0010 0x103a
#define RPR_MATERIAL_NODE_OP_SELECT_0011 0x103b
#define RPR_MATERIAL_NODE_OP_SELECT_0100 0x103c
#define RPR_MATERIAL_NODE_OP_SELECT_0101 0x103d
#define RPR_MATERIAL_NODE_OP_SELECT_0110 0x103e
#define RPR_MATERIAL_NODE_OP_SELECT_0111 0x103f
#define RPR_MATERIAL_NODE_OP_SELECT_1000 0x1040
#define RPR_MATERIAL_NODE_OP_SELECT_1001 0x1041
#define RPR_MATERIAL_NODE_OP_SELECT_1010 0x1042
#define RPR_MATERIAL_NODE_OP_SELECT_1011 0x1043
#define RPR_MATERIAL_NODE_OP_SELECT_1100 0x1044
#define RPR_MATERIAL_NODE_OP_SELECT_1101 0x1045
#define RPR_MATERIAL_NODE_OP_SELECT_1110 0x1046
#define RPR_MATERIAL_NODE_OP_SELECT_1111 0x1047

#define RPR_MATERIAL_NODE_OP_APPEND_0_0 0x1048
#define RPR_MATERIAL_NODE_OP_APPEND_0_1 0x1049
#define RPR_MATERIAL_NODE_OP_APPEND_0_2 0x104a
#define RPR_MATERIAL_NODE_OP_APPEND_0_3 0x104b
#define RPR_MATERIAL_NODE_OP_APPEND_0_4 0x104c
#define RPR_MATERIAL_NODE_OP_APPEND_1_0 0x104d
#define RPR_MATERIAL_NODE_OP_APPEND_1_1 0x104e
#define RPR_MATERIAL_NODE_OP_APPEND_1_2 0x104f
#define RPR_MATERIAL_NODE_OP_APPEND_1_3 0x1050
#define RPR_MATERIAL_NODE_OP_APPEND_2_0 0x1051
#define RPR_MATERIAL_NODE_OP_APPEND_2_1 0x1052
#define RPR_MATERIAL_NODE_OP_APPEND_2_2 0x1053
#define RPR_MATERIAL_NODE_OP_APPEND_3_0 0x1054
#define RPR_MATERIAL_NODE_OP_APPEND_3_1 0x1055
#define RPR_MATERIAL_NODE_OP_APPEND_4_0 0x1056

#define RPR_MATERIAL_NODE_OP_FRACT 0x1057
#define RPR_MATERIAL_NODE_OP_POSITIVE_CLAMP_POW 0x1058
#define RPR_MATERIAL_NODE_OP_STEP 0x1059
#define RPR_MATERIAL_NODE_OP_SMOOTH_STEP 0x105a
#define RPR_MATERIAL_NODE_OP_ATAN2 0x105b

#define RPR_MATERIAL_INPUT_UBER_TRANSPARENCY_MASK 0x1500

/*rpr_material_node_arithmetic_operation*/
#define RPR_MATERIAL_NODE_OP_SAMPLER 0x1000
#define RPR_MATERIAL_NODE_OP_SAMPLER_BUMPMAP 0x1001
#define RPR_MATERIAL_NODE_OP_CONSTANT 0x1002
#define RPR_MATERIAL_NODE_OP_LOOKUP 0x1003
#define RPR_MATERIAL_NODE_OP_NORMAL_MAP 0x1004
#define RPR_MATERIAL_NODE_OP_LEAF 0x1005
#define RPR_MATERIAL_NODE_OP_CHECKER 0x1006

/*rpr_material_node_lookup_value*/
#define RPR_MATERIAL_NODE_LOOKUP_VIEW_VECTOR 0x2000
#define RPR_MATERIAL_NODE_LOOKUP_TANGENT 0x2001
#define RPR_MATERIAL_NODE_LOOKUP_BITANGENT 0x2002

/* rpr_context_properties names */
#define RPR_CONTEXT_CREATEPROP_HYBRID_KERNELS_PATH_INFO 0x1600
#define RPR_CONTEXT_CREATEPROP_HYBRID_ACC_MEMORY_SIZE 0x1601
#define RPR_CONTEXT_CREATEPROP_HYBRID_MESH_MEMORY_SIZE 0x1602
#define RPR_CONTEXT_CREATEPROP_HYBRID_STAGING_MEMORY_SIZE 0x1603
#define RPR_CONTEXT_CREATEPROP_HYBRID_SCRATCH_MEMORY_SIZE 0x1604 // Size of acceleration scratch pool
#define RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_VCT 0x1605 // Tells Hybrid to enable support for VCT(Voxel Cone Tracing).
                                                        // Enabling this requires vulkan implementation to support VK_EXT_consevative_rasterisation
                                                        // and in case of VK inter-op mode it must be enabled on provided device
#define RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_PER_FACE_MATERIALS 0x1606 // Tells Hybrid to enable support for per-face materials.
                                                                       // This functionality requires additional memory on both -
                                                                       // CPU and GPU even when no per-face materials set in scene.
#define RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_RADEON_RAYS 0x1607 // Use RadeonRays instead of native Vulkan VK_KHR_ray_tracing extension for raytracing.
#define RPR_CONTEXT_CREATEPROP_HYBRID_VIDEO_API 0x1608 // Use RadeonRays instead of native Vulkan VK_KHR_ray_tracing extension for raytracing.

struct RPRHybridKernelsPathInfo
{
    char const* baikal_kernels_path;
    char const* rr_kernels_path;
};

//to avoid overlap
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
#define RPR_CONTEXT_ENABLE_LIGHTMAPS 0X100C // name: "light_baker.enabled"
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
#define RPR_CONTEXT_ENABLE_RELAXED_MATERIAL_CHECKS 0x1022 // fallback (RPR_FALSE - whole material, RPR_TRUE - invalid input) to default value
#define RPR_CONTEXT_IRRADIANCE_VOLUME_ENABLED 0x1023 // Use irradiance volume for dynamic indirect diffuse illumination (works only in medium quality)
#define RPR_CONTEXT_IRRADIANCE_VOLUME_RESOLUTION_X 0x1024 // Irradiance volume grid x resolution
#define RPR_CONTEXT_IRRADIANCE_VOLUME_RESOLUTION_Y 0x1025 // Irradiance volume grid y resolution
#define RPR_CONTEXT_IRRADIANCE_VOLUME_RESOLUTION_Z 0x1026 // Irradiance volume grid z resolution
#define RPR_CONTEXT_IRRADIANCE_VOLUME_NUM_LODS 0x1027 // Number of irradiance volume LODs
#define RPR_CONTEXT_IRRADIANCE_VOLUME_CELL_SIZE 0x1028 // Cell size of 0th LOD of irradiance volume grid
#define RPR_CONTEXT_ENABLE_RAYTRACE_SHADOWS 0x1029 // turn on ray-trace shadows in custom quality
#define RPR_CONTEXT_ENABLE_RAYTRACE_REFLECTION 0x102A // turn on ray-trace reflection in custom quality
#define RPR_CONTEXT_ENABLE_RAYTRACE_REFRACTION 0x102B // turn on ray-trace refraction in custom quality
#define RPR_CONTEXT_GLOBAL_ILLUMINATION_MODE 0x102C // change global illumination mode in custom quality
#define RPR_CONTEXT_PT_DENOISER 0x102D // Sets denoiser type.
#define RPR_CONTEXT_MAX_TEXTURES 0x102E // Defines maximim textures allowed
#define RPR_CONTEXT_RESERVOIR_SAMPLING 0x102F // Turn on reservoir sampling
#define RPR_CONTEXT_EXPOSURE 0x1030
#define RPR_CONTEXT_TONE_MAPPING 0x1031
#define RPR_CONTEXT_ENABLE_VOLUMES 0x1032 // Enable volume rendering. It is recommended to turn off this feature  (if scene hasn't volumes) because it creates a small overhead
#define RPR_CONTEXT_RESTIR_GI 0x1033 // Enable ReSTIR GI
#define RPR_CONTEXT_PSR_ATTENUATION 0x1034 // Path Space Regularization: Attenuation factor for roughness. [0.0, 1.0], 0.0 - Unbiased, high variance, 1.0 - Biased, low variance
#define RPR_CONTEXT_MATERIAL_CACHE 0x1035 // Reduce shaders compilation time, in some cases can increase performance
#define RPR_CONTEXT_ENABLE_RASTERIZATION 0x1036 // Enable first hit rasterization
#define RPR_CONTEXT_RESTIR_SPATIAL_RESAMPLE_ITERATIONS 0x1037 // World space ReSTIR spatial resample iteration count
#define RPR_CONTEXT_RESTIR_MAX_RESERVOIRS_PER_CELL 0x1038 // Max reservoirs per world space hash grid cell
#define RPR_CONTEXT_ENABLE_HALFRES_INDIRECT 0x1039 // Enable indirect downsample
#define RPR_CONTEXT_ENABLE_RADIANCE_CACHE 0x1003B // Enable Radiance Cache.
// Set ReSTIR GI bias correction method
// 0 - No bias correction (biased)
// 1 - Uniform weights    (unbiased, variance can be very high)
// 2 - Stochastic MIS     (unbiased, variance is very low)      [  optimal  ]
// 3 - Deterministic MIS  (unbiased, variance is even lower)    [ expensive ]
#define RPR_CONTEXT_RESTIR_GI_BIAS_CORRECTION 0x1003C
#define RPR_CONTEXT_RESTIR_GI_ENABLE_SAMPLE_VALIDATION 0x1003D // Enable sample validation which helps to remove temporal lag of indirect lighting at the cost of decreased performance
#define RPR_CONTEXT_ENABLE_MOTION_BLUR 0x1003E // Enable motion blur
#define RPR_CONTEXT_MOTION_BLUR_SAMPLE_COUNT 0x1003F // Set sample count for motion blur


/* Traversal modes */
#define RPR_HYBRID_TRAVERSAL_STATIC_TLAS_SEPARATE 0x1 ///< Use a separate acceleration structure for static objects
#define RPR_HYBRID_TRAVERSAL_STATIC_TLAS_INSTANCES 0x2 ///< Include instances into static tlas

/* possible render quality modes*/
#define RPR_RENDER_QUALITY_LOW 0u
#define RPR_RENDER_QUALITY_MEDIUM 1u
#define RPR_RENDER_QUALITY_HIGH 2u
#define RPR_RENDER_QUALITY_ULTRA 3u
#define RPR_RENDER_QUALITY_CUSTOM 4u

/*global illumination modes*/
#define RPR_GLOBAL_ILLUMINATION_SSAO 0u
#define RPR_GLOBAL_ILLUMINATION_IRRADIANCE_VOLUME 1u
#define RPR_GLOBAL_ILLUMINATION_PATH_TRACE_AO 2u

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
#define RPR_INSTANCE_USE_UNIQUE_ATTRIBUTES 0x1602

#define RPR_CAMERA_UV_LIGHTMAP_CHART_INDEX 0x1441
#define RPR_FRAMEBUFFER_TYPE 0x1442
#define RPR_CAMERA_MODE_UV 0x1443

// rprMeshGetInfo extension
#define RPR_MESH_IS_DYNAMIC_MESH 0x519
#define RPR_MESH_LOCAL_AABB 0x520
#define RPR_MESH_WORLD_AABB 0x521

/* Denoisers */
#define RPR_DENOISER_NONE 0u
#define RPR_DENOISER_SVGF 1u
#define RPR_DENOISER_ASVGF 2u

#define RPR_TONE_MAPPING_NONE 0u
#define RPR_TONE_MAPPING_FILMIC 1u
#define RPR_TONE_MAPPING_ACES 2u
#define RPR_TONE_MAPPING_REINHARD 3u
#define RPR_TONE_MAPPING_PHOTO_LINEAR 4u

/* RPR_CONTEXT_CREATEPROP_HYBRID_VIDEO_API acceptable values*/
#define RPR_HYBRID_VIDEO_API_VULKAN 0u
#define RPR_HYBRID_VIDEO_API_D3D12 1u

/** @brief Create an instance of an object with separate buffer for vertex attributes
*
*  Possible error codes are:
*
*      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
*      RPR_ERROR_OUT_OF_VIDEO_MEMORY
*      RPR_ERROR_INVALID_PARAMETER
*
*  @param  context  The context to create an instance for
*  @param  shape    Parent shape for an instance
*  @param  out_instance   Pointer to instance object
*  @return RPR_SUCCESS in case of success, error code otherwise
*/
extern RPR_API_ENTRY rpr_status rprContextCreateMeshInstanceWithUniqueAttributes(rpr_context in_context, rpr_shape in_shape, rpr_shape* shape_out);

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
 * Set an index of the lightmap chart for the shape.
 *
 * If progressive lightbaking mode is enabled, shape's lightmap chart index is being used
 * to determine which lightmap shape belongs to. UV1 set should be correctly laid out for
 * lightmapped geometry.
 *
 * @note By default index is -1 (which means disable shape from baking).
 *
 * @param shape Shape to set index on.
 * @param Lightmap chart index.
 */
typedef rpr_int(*rprShapeSetLightmapChartIndex_func)(rpr_shape shape, rpr_int chart_index);
#define RPR_SHAPE_SET_LIGHTMAP_CHART_INDEX_FUNC_NAME "rprShapeSetLightmapChartIndex"

/**
 */
typedef rpr_int(*rprUvCameraSetChartIndex_func)(rpr_camera camera, rpr_int chart_index);
#define RPR_UV_CAMERA_SET_CHART_INDEX_FUNC_NAME "rprUvCameraSetChartIndex"

/**
 * Framebuffer format types for light baking.
 */
typedef enum // rpr_framebuffer_type
{
    RPR_FRAMEBUFFER_TYPE_DIFFUSE_RGB = 0x1,
    RPR_FRAMEBUFFER_TYPE_DOMINANT_DIRECTION = 0x2,
    RPR_FRAMEBUFFER_TYPE_DOMINANT_DIRECTION_RGB = 0x3,
    RPR_FRAMEBUFFER_TYPE_HL2 = 0x4,
    RPR_FRAMEBUFFER_TYPE_AMBIENT_CUBE = 0x5,
    RPR_FRAMEBUFFER_TYPE_SH1 = 0x6,
    RPR_FRAMEBUFFER_TYPE_SH2 = 0x7,
    RPR_FRAMEBUFFER_TYPE_SG = 0x8
} rpr_framebuffer_type;

/**
 * Creates special typed framebuffer used for lightmapping.
 */
typedef rpr_int(*rprContextCreateFramebufferTyped_func)(rpr_context context, rpr_framebuffer_type fb_type,
    const rpr_framebuffer_desc* fb_desc, rpr_framebuffer* out_framebuffer);
#define RPR_CONTEXT_CREATE_FRAMEBUFFER_TYPED_FUNC_NAME "rprContextCreateFramebufferTyped"

/**
 * Create a editable triangle mesh.
 *
 * Mesh becomes editable if the set of indices for all vertex components are similar. With editable
 * meshes rprMeshUpdate can be used to update the mesh content. You may still need to use rprContextCreateMesh
 * and modifications, since this function does not support non-triangle meshes.
 *
 * @param  num_vertices         Number of vertices provided.
 * @param  max_vertices         Maximum number of vertices mesh may contain.
 * @param  positions            Pointer to position data (each position is described with 3 rpr_float numbers).
 * @param  positions_stride     Number of bytes between the beginnings of two successive position entries.
 * @param  normals              Pointer to normal data (each normal is described with 3 rpr_float numbers), can be NULL.
 * @param  normals_stride       Number of bytes between the beginnings of two successive normal entries.
 * @param  uvs0                 Pointer to first UV set data (each texcoord is described with 2 rpr_float numbers), can be NULL.
 * @param  uv0s_stride          Number of bytes between the beginnings of two successive texcoord entries in the first UV set array.
 * @param  uvs1                 Pointer to second UV set data (each texcoord is described with 2 rpr_float numbers), can be NULL.
 * @param  uv1s_stride          Number of bytes between the beginnings of two successive texcoord entries in the second UV set array.
 * @param  attribute_count      Number of attributes pre vertex.
 * @param  attribute_components Number of components of each vertex attribute (type of every component is rpr_float).
 * @param  vertex_attributes    Pointer to array of vertex attributes data (each attribute described with coresponding number of rpr_float values).
 * @param  attribute_strides    Pointer to array of numbers of bytes between the beginnings of two successive vertex attributes entries.
 * @param  num_indices          Number of indices provided.
 * @param  max_indices          Maximum number of indices mesh may contain.
 * @param  indices              Pointer to index data (each index is described with rpr_int number).
 * @param  mesh_properties      Properties of the mesh.
 * @param  out_mesh             Pointer to mesh object.
 * @return                      RPR_SUCCESS in case of success, error code otherwise
 */
typedef rpr_status (*rprContextCreateMeshEditable_func)(rpr_context context,
    size_t num_vertices, size_t max_vertices,
    rpr_float const* positions, rpr_int positions_stride, rpr_float const* normals, rpr_int normals_stride,
    rpr_float const* uvs0, rpr_int uv0s_stride, rpr_float const* uvs1, rpr_int uv1s_stride,
    rpr_int attribute_count, rpr_int const* attribute_components, rpr_float const** vertex_attributes, rpr_int const* attribute_strides,
    size_t num_indices, size_t max_indices, rpr_int const* indices,
    rpr_mesh_info const* mesh_properties,
    rpr_shape* out_mesh);
#define RPR_CONTEXT_CREATE_MESH_EDITABLE_FUNC_NAME "rprContextCreateMeshEditable"

typedef rpr_status(*rprMaterialNodeSetInputSByKey_func)(rpr_material_node in_node, rpr_material_node_input in_input, rpr_char const* name);
#define RPR_MATERIAL_SET_INPUT_BY_S_KEY_FUNC_NAME "rprMaterialNodeSetInputSByKey"

typedef rpr_status(*rprMaterialXSetFile_func)(rpr_material_node in_node, rpr_char const* name);
#define RPR_MATERIALX_SET_FILE_FUNC_NAME "rprMaterialXSetFile"

typedef rpr_status(*rprMaterialXSetFileAsBuffer_func)(rpr_material_node in_node, rpr_char const* buffer, size_t bufferSize);
#define RPR_MATERIALX_SET_FILE_AS_BUFFER_FUNC_NAME "rprMaterialXSetFileAsBuffer"

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

// Extends rpr_component_type
typedef enum
{
    RPR_FORMAT_BC1_UNORM =           0x1000,
    RPR_FORMAT_BC1_UNORM_SRGB =      0x1001,
    RPR_FORMAT_BC2_UNORM  =          0x1002,
    RPR_FORMAT_BC2_UNORM_SRGB =      0x1003,
    RPR_FORMAT_BC3_UNORM =           0x1004,
    RPR_FORMAT_BC3_UNORM_SRGB =      0x1005,
    RPR_FORMAT_BC4_UNORM =           0x1006,
    RPR_FORMAT_BC4_SNORM =           0x1007,
    RPR_FORMAT_BC5_UNORM =           0x1008,
    RPR_FORMAT_BC5_SNORM =           0x1009,
    RPR_FORMAT_BC6H_SF16 =           0x100A,
    RPR_FORMAT_BC6H_UF16 =           0x100B,
    RPR_FORMAT_BC7_UNORM =           0x100C,
    RPR_FORMAT_BC7_UNORM_SRGB =      0x100D,
    RPR_FORMAT_B8G8R8A8_UNORM =      0x100E,
    RPR_FORMAT_B8G8R8A8_UNORM_SRGB = 0x100F,
} rpr_format_ext ;

typedef enum
{
    RPR_VECTOR_COMPONENT_TYPE_UNDEFINED = 0x0,
    RPR_VECTOR_COMPONENT_TYPE_FLOAT32 = 0x1,
    RPR_VECTOR_COMPONENT_TYPE_UINT32 =  0x2,
    RPR_VECTOR_COMPONENT_TYPE_INT32 =   0x3,
} rpr_vector_component_type ;

typedef struct //rpr_image_desc_ext
{
    rpr_uint image_width;
    rpr_uint image_height;
    rpr_uint mip_count;
} rpr_image_desc_ext;

/**
 * Create compressed image from memory.
 *
 * This function exist, because rprContextCreateImage() doesn't know about compression, blocks, etc.
 * It is unclear how to calculate block size inside rprContextCreateImage() without knowledge about concrete compressed format.
 * 
 * @param  format       Compressed format.
 * @param  image_desc   Some image info.
 * @param  mip_data     Array of pointers to each mip data (length is image_desc->mip_count).
 * @param  data_size    Array of data size for each mip (length is image_desc->mip_count).
 */
typedef rpr_int(*rprContextCreateCompressedImage_func)(rpr_context context, rpr_format_ext format, const rpr_image_desc_ext* image_desc, const void** mip_data, const size_t* data_size, rpr_image* out_image);
#define RPR_CONTEXT_CREATE_COMPRESSED_IMAGE "rprContextCreateCompressedImage"

/**
 * Create image from external image handle.
 *
 * @param  context        The context to create image
 * @param  image_format   Format description.
 * @param  format_ext     Extended format description if rpr_image_format could't describe actual format, in this case image_format.type should be RPR_COMPONENT_TYPE_UNKNOWN.
 * @param  image_desc     Image description.
 * @param  external_image External image handle. Supported handles: VkImage*, ID3D12Resource*.
 * @param  out_image      Pointer to image object
 */
typedef rpr_int(*rprContextCreateImageFromExternalHandle_func)(rpr_context context, rpr_image_format image_format, rpr_format_ext format_ext,
    const rpr_image_desc_ext* image_desc, const void* external_image, rpr_image* out_image);
#define RPR_CONTEXT_CREATE_IMAGE_FROM_EXTERNAL_HANDLE "rprContextCreateImageFromExternalHandle"

struct rpr_stats
{
    struct rpr_stat
    {
        rpr_uint count;
        size_t size;
    };

    rpr_uint primitives;
    rpr_uint draw_calls;

    struct rpr_stat images;
    struct rpr_stat images_cpu;
    struct rpr_stat images_cpu_gpu;
    struct rpr_stat images_gpu_cpu;
    struct rpr_stat images_gpu;
    struct rpr_stat buffers;
    struct rpr_stat buffers_cpu;
    struct rpr_stat buffers_cpu_gpu;
    struct rpr_stat buffers_gpu_cpu;
    struct rpr_stat buffers_gpu;
    struct rpr_stat staging_buffers;
    struct rpr_stat vertex_buffers;
    struct rpr_stat index_buffers;
    struct rpr_stat as_scratch_buffers;
    struct rpr_stat face_material_buffers;
    struct rpr_stat mesh_buffers;
    struct rpr_stat color_attachments;
    struct rpr_stat depth_attachments;
    struct rpr_stat storage_images;
    struct rpr_stat blas;
    struct rpr_stat tlas;

    size_t mesh_data_pool_size;
    size_t acceleration_structure_pool_size;
    size_t staging_pool_size;
    size_t acc_scratch_pool_size;

    size_t total_allocated;
    size_t total_used;
    size_t total_free;
};

#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_BAIKAL_H  */
