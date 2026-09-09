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

/*rpr_material_node_arithmetic_operation*/
#define RPR_MATERIAL_NODE_OP_LOG2 0x1030
#define RPR_MATERIAL_NODE_OP_LOG10 0x1031
#define RPR_MATERIAL_NODE_OP_TRUNCATE 0x1032
#define RPR_MATERIAL_NODE_OP_SHUFFLE 0x1033
#define RPR_MATERIAL_NODE_OP_SHUFFLE2 0x1034
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

#define RPR_MATERIAL_NODE_OP_TANGENT_TO_WORLD 0x105c
#define RPR_MATERIAL_NODE_OP_WORLD_TO_TANGENT 0x105d
#define RPR_MATERIAL_NODE_OP_LOCAL_TO_WORLD 0x105e
#define RPR_MATERIAL_NODE_OP_WORLD_TO_LOCAL 0x105f

#define RPR_MATERIAL_NODE_OP_LERP 0x1060
#define RPR_MATERIAL_NODE_OP_BLACK_BODY 0x1061
#define RPR_MATERIAL_NODE_OP_ROTATE_ABOUT_AXIS 0x1062

#define RPR_MATERIAL_INPUT_UBER_TRANSPARENCY_MASK 0x1500
#define RPR_MATERIAL_INPUT_TRANSPARENCY           0x1501
#define RPR_MATERIAL_INPUT_CAST_SHADOW            0x11001U

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
#define RPR_MATERIAL_NODE_LOOKUP_OBJECT_LOCAL_BOUNDS_HALF_EXTENTS 0x2003
#define RPR_MATERIAL_NODE_LOOKUP_OBJECT_LOCAL_BOUNDS_EXTENTS 0x2004
#define RPR_MATERIAL_NODE_LOOKUP_OBJECT_LOCAL_BOUNDS_MIN 0x2005
#define RPR_MATERIAL_NODE_LOOKUP_OBJECT_LOCAL_BOUNDS_MAX 0x2006
#define RPR_MATERIAL_NODE_LOOKUP_PER_INSTANCE_RANDOM 0x2007
#define RPR_MATERIAL_NODE_LOOKUP_LINEAR_DEPTH 0x2008

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
#define RPR_CONTEXT_PT_UPSCALE_ENABLED 0x1007 // name: "pt.upscale_enabled"
#define RPR_CONTEXT_PT_UPSCALE_FACTOR 0x1008 // name: "pt.upscale_factor"
#define RPR_CONTEXT_RTAO_UPSCALE_ENABLED 0x1009 // name: "rtao.upscale_enabled"
#define RPR_CONTEXT_RTAO_UPSCALE_FACTOR 0x100A // name: "rtao.upscale_factor"
#define RPR_CONTEXT_VCT_RESOLUTION 0x100B // name: "vct.resolution". Only valid if RPR_CONTEXT_CREATEPROP_HYBRID_ENABLE_VCT was set during context creation
#define RPR_CONTEXT_AREA_LIGHT_SHADOWS_ENABLED 0x100D // name: "area_light_shadows.enabled"
#define RPR_CONTEXT_SPOT_LIGHT_DEPTH_BIAS 0x100E // name: "spot_light.depth_bias". type float
#define RPR_CONTEXT_POINT_LIGHT_DEPTH_BIAS 0x100F // name: "point_light.depth_bias. type float
#define RPR_CONTEXT_LIGHT_SAMPLER 0x1010
#define RPR_CONTEXT_STEREO_RENDER  0x1011 // name: "stereo_render"
#define RPR_CONTEXT_LIGHTBVH_SPLIT_METHOD 0x1012
#define RPR_CONTEXT_ENABLE_ARITHMETICS 0x1013 // Enables/Disables arithmetics in context. Changing this parameter must be followed by call to rprFramebufferClear
#define RPR_CONTEXT_NUMBER_CAMERA_JITTER_POINTS 0x1014 // Number of camera jitter points
#define RPR_CONTEXT_VOLUMETRIC_FOG 0x1015 // Enables Volumetric Fog
#define RPR_CONTEXT_VOLUMETRIC_FOG_CAST_SHADOWS 0x1016 // Volumetric Fog Visibility Test Override. Individual lights might choose via RPR_LIGHT_CAST_VOLUMETRIC_SHADOWS.
#define RPR_CONTEXT_VOLUMETRIC_FOG_SPP 0x1017 // Volumetric Fog Visibility Test
#define RPR_CONTEXT_VOLUMETRIC_FOG_ALBEDO 0x1018 // Volumetric Fog Albedo
#define RPR_CONTEXT_VOLUMETRIC_FOG_EMISSIVE 0x1019 // Volumetric Fog Scattering Coefficient [0,1]
#define RPR_CONTEXT_VOLUMETRIC_FOG_ANISOTROPY 0x101A // Volumetric Fog Anisotropy [-1,1]
#define RPR_CONTEXT_VOLUMETRIC_FOG_DENSITY 0x101B // Volumetric Fog Density [0,1]
#define RPR_CONTEXT_VOLUMETRIC_FOG_EXTINCTION_MULTIPLIER 0x101C // Volumetric Fog Extinction Multiplier [0,10]
#define RPR_CONTEXT_VOLUMETRIC_FOG_NEAR_PLANE 0x101D // Volumetric Fog Near Plane
#define RPR_CONTEXT_VOLUMETRIC_FOG_DISTANCE 0x101E // Volumetric Fog View Distance
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
#define RPR_CONTEXT_RESTIR_DI 0x102F // Turn on reservoir sampling (1 = screen-space, 2 = path-space)
#define RPR_CONTEXT_EXPOSURE 0x1030
#define RPR_CONTEXT_TONE_MAPPING 0x1031
#define RPR_CONTEXT_ENABLE_VOLUMES 0x1032 // Enable volume rendering. It is recommended to turn off this feature  (if scene hasn't volumes) because it creates a small overhead
#define RPR_CONTEXT_RESTIR_GI 0x1033 // Enable ReSTIR GI
#define RPR_CONTEXT_PSR_ATTENUATION 0x1034 // Path Space Regularization: Attenuation factor for roughness. [0.0, 1.0], 0.0 - Unbiased, high variance, 1.0 - Biased, low variance
#define RPR_CONTEXT_MATERIAL_CACHE 0x1035 // Reduce shaders compilation time, in some cases can increase performance
#define RPR_CONTEXT_ENABLE_RASTERIZATION 0x1036 // Enable first hit rasterization
#define RPR_CONTEXT_RESTIR_DI_SPATIAL_RESAMPLE_ITERATIONS 0x1037 // World space ReSTIR spatial resample iteration count
#define RPR_CONTEXT_RESTIR_DI_MAX_RESERVOIRS_PER_CELL 0x1038 // Max reservoirs per world space hash grid cell
#define RPR_CONTEXT_ENABLE_HALFRES_INDIRECT 0x1039 // Enable indirect downsample
#define RPR_CONTEXT_ENABLE_DISPLACEMENT_LODS 0x103A // Global switch for enabling level of detail for displaced meshes
// Global parameters for displacement LOD [lod_min_distance, lod_max_distance, lod_distance_scale, bounce_scale]
// -lod_min_distance: default=0. The distance from the camera where LODs start to decrease.
// -lod_max_distance: default=FLT_MAX. The distance from the camera where LODs stop decreasing.
// -lod_distance_scale: default=0.5. How fast the LOD transitions given the [lod_min_distance, lod_max_distance] range.
// -bounce_scale: default=0. Bias for the LOD on consecutive ray bounces. This LOD decrease may cause visual artifacts. Does not affect primary rays.
#define RPR_CONTEXT_DISPLACEMENT_LOD_GLOBAL_PARAMS 0x103B
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
#define RPR_CONTEXT_USE_GMON 0x10040 // Use Median Of meaNs in accumulate and copy to remove fireflies
#define RPR_CONTEXT_GINI_COEFFICIENT_FOR_GMON 0x10041
#define RPR_CONTEXT_UPSCALER 0x10042
#define RPR_CONTEXT_FSR_QUALITY 0x10043
#define RPR_CONTEXT_CONSERVATIVE_GPU_MEM_ALLOCATION 0x10044
#define RPR_CONTEXT_PSR 0x10045 // Primary surface replacement
#define RPR_CONTEXT_CSFR 0x10046 // Checkerboarded split-frame rendering
#define RPR_CONTEXT_RESTIR_DI_FIREFLY_FILTER_STRENGTH 0x10049 // Sets the strength of the firefly filter used in the screen-space ReSTIR DI [0-1].
#define RPR_CONTEXT_ENABLE_NEURAL_RADIANCE_CACHE 0x1004A // Enable Neural Radiance Cache.
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_TRAINING_RATIO 0x1004B // NRC Training ratio. [0; 1]
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_HIDDEN_LAYERS_COUNT 0x1004C // NRC model hidden layers count. 1, 2 or 3
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_BATCH_COUNT 0x1004D // NRC batch count for training and optimization passes
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_USE_WMMA 0x1004E // Enables WMMA for NRC (if GPU supports it)
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_GRID_LEVELS_COUNT 0x1004F // NRC number of grid levels in hash-grid used in position encoding
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_RADIANCE_SCALE 0x10050 // NRC factor used to scale radiance values before training and after inference
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_MAX_BOUNCES 0x10051 // Number of bounces before NRC cache.
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_ONLY 0x10052 // Display only NRC cache or mix.
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_LEAKY_RELU 0x10053 // Use Leaky ReLU activation function in NRC model
#define RPR_CONTEXT_JITTER_MODE 0x10054 // Sets the jittering mode
#define RPR_CONTEXT_ENABLE_UNLIT_TRANSPARENT_OVERLAYS_SORTING 0x10055 // Enables sorting of overlays
#define RPR_CONTEXT_INVERT_TEXTURE_UV_Y 0x10056 // When enabled, UV.y is inverted (1 - uv.y) when reading from a texture
#define RPR_CONTEXT_TEXTURE_MIP_BIAS_MODE 0x10057 // 0-None, 1-Auto (default), 2-Manual (externally controlled)
#define RPR_CONTEXT_TEXTURE_MIP_BIAS 0x10058 // Bias to be applied (when using RPR_CONTEXT_TEXTURE_MIP_BIAS_MODE_MANUAL). e.g., log2(render_res/display_res)-1
#define RPR_CONTEXT_MATERIAL_DELTA_ROUGHNESS_THRESHOLD 0x10059 // Roughness threshold for mirror-like surface detection
#define RPR_CONTEXT_RESTIR_DI_INITIAL_CANDIDATE_COUNT 0x1005A
#define RPR_CONTEXT_RESTIR_DI_SS_SPATIAL_REUSE_RADIUS 0x1005B // Reuse Radius for Screen-Space ReSTIR
#define RPR_CONTEXT_RESTIR_DI_SS_SPATIAL_REUSE_SAMPLES 0x1005C // Reuse Samples for Screen-Space ReSTIR
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_GRID_BASE_RESOLUTION 0x1005D // NRC grid resolution
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_DIRECTLY 0x1005E // See NRC output directly.
#define RPR_CONTEXT_RESTIR_DI_ENABLE_TEMPORAL_REUSE 0x1005F
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_GRID_USE_CUBE_SIDES 0x10060 // NRC grid uses cube sides
#define RPR_CONTEXT_ENABLE_UNLIT_TRANSPARENT_OVERLAYS_RENDERING 0x10061 // Enables rendering of overlays
#define RPR_CONTEXT_ENABLE_ASYNC_PIPELINE_CREATION 0x10062 // Use fallback pipelines for asynchronous compilation
#define RPR_CONTEXT_ENABLE_PREV_FRAME_TLAS 0x10063 // Enables previous frame TLAS to be stored
#define RPR_CONTEXT_NEURAL_RADIANCE_CACHE_GRID_CASCADE 0x10064 // NRC grid uses cube sides
#define RPR_CONTEXT_RESTIR_DI_CHECK_VISIBILITY_INIT 0x10065
#define RPR_CONTEXT_RESTIR_DI_CHECK_VISIBILITY_SPATIAL 0x10066
#define RPR_CONTEXT_RESERVOIR_GRID_Z_UP 0x10067 // Z up coordinate system
#define RPR_CONTEXT_RESERVOIR_GRID_LAYOUT 0x10068
#define RPR_CONTEXT_RESERVOIR_GRID_CASCADE_COUNT 0x10069
#define RPR_CONTEXT_RESERVOIR_GRID_SPLIT_LAMBDA 0x1006A
#define RPR_CONTEXT_RESERVOIR_GRID_MAX_DISTANCE 0x1006B
#define RPR_CONTEXT_ENABLE_CAMERA_DUMP 0x1006C
#define RPR_CONTEXT_CAMERA_DUMP_FILENAME 0x1006D
#define RPR_CONTEXT_CAMERA_DUMP_DESTINATION_FOLDER 0x1006E
#define RPR_CONTEXT_PAUSE_CAMERA_DUMP 0x1006F // Can be used to set on pause: data won't be dumped, and frame number won't be reset
#define RPR_CONTEXT_CAMERA_DUMP_FRAME_NUMBER_OVERRIDE 0x10070

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

/*texture mip bias mode*/
#define RPR_CONTEXT_TEXTURE_MIP_BIAS_MODE_NONE   0u
#define RPR_CONTEXT_TEXTURE_MIP_BIAS_MODE_AUTO   1u
#define RPR_CONTEXT_TEXTURE_MIP_BIAS_MODE_MANUAL 2u

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

/* Mesh buffer access on GPU */
#define RPR_CONTEXT_MESH_BUFFER 0x6002

/* Semaphores to synchronise any work on GPU */
#define RPR_CONTEXT_EXTERNAL_SEMAPHORES 0x6003
#define RPR_CONTEXT_EXTERNAL_SEMAPHORE_TO_WAIT_INDEX 0x6004 // The index of semaphore we must wait for the last Hybrid frame to finish.
#define RPR_CONTEXT_EXTERNAL_SEMAPHORE_TO_SIGNAL_INDEX 0x6005 // The index of semapore we must signal after our work completed.

/* rpr_light_info */
#define RPR_LIGHT_CAST_VOLUMETRIC_SHADOWS 0x829
#define RPR_LIGHT_ATTENUATION_RADIUS 0x82A  

/*
 * rprExternalWorkEnqueued
 *
 * Notify Hybrid that we have just queued arbutary work on GPU and we are waiting for the last Hybrid frame to finish.
 * When our work is done, we must signal an appropriate semaphore.
 * For example, a work can be performed on either mesh buffer or transform buffer.
 * You can pass an additional rpr_scene argument, specifying which scene your work affects. This will trigger TLAS update.
 */
typedef rpr_int(*rprExternalWorkEnqueued_func)(rpr_context, rpr_scene);
#define RPR_EXTERNAL_WORK_ENQUEUED_FUNC_NAME "rprExternalWorkEnqueued"

/* Notify Hybrid that we have just changed the dynamic mesh */
typedef rpr_int(*rprDynamicMeshChangedExternally_func)(rpr_shape mesh, size_t num_vertices, size_t num_indices);
#define RPR_MESH_DYNAMIC_MESH_CHANGED_EXTERNALLY_FUNC_NAME "rprDynamicMeshChangedExternally"

/* Transform buffer access on GPU */
#define RPR_CONTEXT_TRANSFORM_BUFFER 0x6006
// "Current" and "prev" transforms are in the same buffer RPR_CONTEXT_TRANSFORM_BUFFER.
// So we need to know offsets of these regions relative the buffer start:
#define RPR_CONTEXT_CURRENT_TRANSFORM_BUFFER_OFFSET 0x6007 // The number of matrices (12x float) we should skip to access to "current buffer".
#define RPR_CONTEXT_PREVIOUS_TRANSFORM_BUFFER_OFFSET 0x6008 // The number of matrices (12x float) we should skip to access to "previous buffer".

// Returns index of the transform matrix and hands over control over it to API user.
// From this point onwards, the API user is responsible for filling the transform. Nothing will be done on RPR side.
typedef rpr_uint(*rprShapeGetGPUTransformIndex_func)(rpr_shape shape);
#define RPR_SHAPE_GET_GPU_TRANSFORM_INDEX_FUNC_NAME "rprShapeGetGPUTransformIndex"

typedef rpr_int(*rprLightSetParameterByKey1u_func)(rpr_light light, rpr_light_info info, rpr_uint x);
#define RPR_LIGHT_SET_PARAM_1U_FUNC_NAME "rprLightSetParameterByKey1u"

typedef rpr_int(*rprLightSetParameterByKey1f_func)(rpr_light light, rpr_light_info info, rpr_float x);
#define RPR_LIGHT_SET_PARAM_1F_FUNC_NAME "rprLightSetParameterByKey1f"
typedef rpr_int(*rprLightSetParameterByKey2f_func)(rpr_light light, rpr_light_info info, rpr_float x, rpr_float y);
#define RPR_LIGHT_SET_PARAM_2F_FUNC_NAME "rprLightSetParameterByKey2f"
typedef rpr_int(*rprLightSetParameterByKey3f_func)(rpr_light light, rpr_light_info info, rpr_float x, rpr_float y, rpr_float z);
#define RPR_LIGHT_SET_PARAM_3F_FUNC_NAME "rprLightSetParameterByKey3f"
typedef rpr_int(*rprLightSetParameterByKey4f_func)(rpr_light light, rpr_light_info info, rpr_float x, rpr_float y, rpr_float z, rpr_float w);
#define RPR_LIGHT_SET_PARAM_4F_FUNC_NAME "rprLightSetParameterByKey4f"

// rprShapeGetInfo extension
/* rpr_shape_info */
#define RPR_INSTANCE_USE_UNIQUE_ATTRIBUTES 0x1602

#define RPR_FRAMEBUFFER_TYPE 0x1442
#define RPR_CAMERA_CUSTOM_JITTER 0x1444

// values for RPR_CONTEXT_JITTER_MODE
#define RPR_CAMERA_JITTER_MODE_NONE             0u // No camera jittering
#define RPR_CAMERA_JITTER_MODE_AUTO             1u // [default] Auto (depending on configuration might be per frame or per pixel)
#define RPR_CAMERA_JITTER_MODE_AUTO_PER_FRAME   2u // Auto but do per-frame jitter only
#define RPR_CAMERA_JITTER_MODE_CUSTOM_PER_FRAME 3u // Custom per-frame jitter offset

// rpr_mesh_info extension
#define RPR_MESH_IS_DYNAMIC_MESH 0x51B
#define RPR_MESH_LOCAL_AABB 0x51C
#define RPR_MESH_WORLD_AABB 0x51D
#define RPR_MESH_IS_UNLIT_TRANSPARENT_OVERLAY 0x51E

/* Denoisers */
#define RPR_DENOISER_NONE 0u
#define RPR_DENOISER_SVGF 1u
#define RPR_DENOISER_ASVGF 2u
#define RPR_DENOISER_ML    3u

#define RPR_TONE_MAPPING_NONE 0u
#define RPR_TONE_MAPPING_FILMIC 1u
#define RPR_TONE_MAPPING_ACES 2u
#define RPR_TONE_MAPPING_REINHARD 3u
#define RPR_TONE_MAPPING_PHOTO_LINEAR 4u

/* Upscalers */
#define RPR_UPSCALER_NONE 0u
#define RPR_UPSCALER_CAS 1u // AMD FidelityFX Contrast Adaptive Sharpening
#define RPR_UPSCALER_FSR 2u // AMD FidelityFX Super Resolution

/* FSR specific settings */
#define RPR_FSR_QUALITY_NONE 0u
#define RPR_FSR_QUALITY_ULTRA_QUALITY 1u
#define RPR_FSR_QUALITY_MODE_QUALITY 2u
#define RPR_FSR_QUALITY_MODE_BALANCE 3u
#define RPR_FSR_QUALITY_MODE_PERFORMANCE 4u
#define RPR_FSR_QUALITY_MODE_ULTRA_PERFORMANCE 5u

/* LightSamplerType */
#define RPR_LIGHT_SAMPLER_UNIFORM 0u
#define RPR_LIGHT_SAMPLER_LIGHTBVH 1u
#define RPR_LIGHT_SAMPLER_RESERVOIR_GRID 2u

/* LightBVH */
#define RPR_LIGHTBVH_MORTON_CODE 0u
#define RPR_LIGHTBVH_SPATIAL_SPLIT 1u

/* ReservoirGrid */
#define RPR_RESERVOIR_GRID_LAYOUT_GRID  0u
#define RPR_RESERVOIR_GRID_LAYOUT_ONION 1u

/* ReSTIR DI MIS `Check Visibility Init` mode */
#define RPR_ReSTIR_DI_CHECK_VISIBILITY_INIT_NONE 0u
#define RPR_ReSTIR_DI_CHECK_VISIBILITY_INIT_ONLY_FINAL 1u
#define RPR_ReSTIR_DI_CHECK_VISIBILITY_INIT_EACH_CANDIDATE 2u

/* RPR_CONTEXT_CREATEPROP_HYBRID_VIDEO_API acceptable values*/
#define RPR_HYBRID_VIDEO_API_VULKAN 0u
#define RPR_HYBRID_VIDEO_API_D3D12 1u

/* Additional AOVs */
#define RPR_HYBRID_AOV_DIFFUSE_RADIANCE                  0x10000
#define RPR_HYBRID_AOV_SPECULAR_RADIANCE                 0x10001
#define RPR_HYBRID_AOV_TRANSMISSIVE_RADIANCE             0x10002
#define RPR_HYBRID_AOV_SPECULAR_REFLECT                  0x10003
#define RPR_HYBRID_AOV_DIFFUSE_REFLECT                   0x10004
#define RPR_HYBRID_AOV_DELTA_REFLECTIVE_ALBEDO           0x10005
#define RPR_HYBRID_AOV_DELTA_REFLECTIVE_NORMAL           0x10006
#define RPR_HYBRID_AOV_REFLECTION_ROUGHNESS              0x10007
#define RPR_HYBRID_AOV_TRANSMISSIVE_MASK                 0x10008
#define RPR_HYBRID_AOV_DIFFUSE_NORMAL_INCOMING_ANGLE     0x10009
#define RPR_HYBRID_AOV_REFLECTION_NORMAL_INCOMING_ANGLE  0x1000A
#define RPR_HYBRID_AOV_SPECULAR_ALBEDO                   0x1000B
#define RPR_HYBRID_AOV_ROUGHNESS                         0x10013
#define RPR_HYBRID_AOV_REPROJECTION_VELOCITY             0x10014
#define RPR_HYBRID_AOV_DEPTH_NDC                         0x10015 /// Depth in NDC appropriate for UE5. "Near" is 1 and "far" is 0
#define RPR_HYBRID_AOV_REFLECTION_RAY_LENGTH             0x10016
#define RPR_HYBRID_AOV_SPECULAR_ALBEDO_2ND_HIT           0x10017
#define RPR_HYBRID_AOV_NSSD_INPUT_COLOR                  0x10018
#define RPR_HYBRID_AOV_NSSD_INPUT_ALBEDO                 0x10019
#define RPR_HYBRID_AOV_NSSD_INPUT_ROUGHNESS              0x1001A
#define RPR_HYBRID_AOV_NSSD_INPUT_NORMAL                 0x1001B
#define RPR_HYBRID_AOV_NSSD_INPUT_DEPTH                  0x1001C
#define RPR_HYBRID_AOV_NSSD_INPUT_MOTION                 0x1001D
#define RPR_HYBRID_AOV_NSSD_INPUT_JITTER                 0x1001E

// rprSceneGetInfo extension
/* rpr_scene_info */
#define RPR_SCENE_TLAS      0x713 // Current frame top level acceleration structure
#define RPR_SCENE_PREV_TLAS 0x714 // Previous frame top level acceleration structure

/* Logging*/
/** !This should be the same as the one defined in vid_log.h
* It'll help to map our internal log levels to external one's 
*/
#define LOG_LEVEL_CRT 0
#define LOG_LEVEL_ERR 1
#define LOG_LEVEL_WRN 2
#define LOG_LEVEL_INF 3
#define LOG_LEVEL_DBG 4
/** !This should be the same as the one defined in the vid_log.h
* I decided to not include this file in log.h, cause log.h itself will be included in many places in Hybrid, an this will add some dependencies between Hybrid and RPR
*/
typedef void (*log_handler)(int, const char*);    // Log function prototype

/*
* Set a log function that can be used to log something
*/
extern RPR_API_ENTRY rpr_status rprSetLogFunction(log_handler log_function_ptr);

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
 * Returns hardware devices that support required Vulkan extensions.
 * 
 * Checks required extensions on each available device, stores its number if supported.
 *
 * @param supported_devices array to store supported device numbers
 * @param[in,out] device_count size of array, updated with actual count of supported devices
 * @return RPR_SUCCESS in case of success, error code otherwise
 */
typedef rpr_status (*rprGetSupportedDevices_func)(int* supported_devices, size_t* device_count);
#define RPR_GET_SUPPORTED_DEVICES_FUNC_NAME "rprGetSupportedDevices"

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

typedef rpr_int (*rprShapeSetTransforms_func)(rpr_shape const* shapes, rpr_float const* transform_data, rpr_uint transform_count);
#define RPR_SHAPE_SET_TRANSFORMS_FUNC_NAME "rprShapeSetTransforms"

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

typedef rpr_int(*rprContextLoadVDB_func)(rpr_context context, const char* path, rpr_grid* out_grid, rpr_uint *out_resolution);
#define RPR_CONTEXT_LOAD_VDB "rprContextLoadVDB"

// rpr_grid_indices_topology extension.
#define RPR_GRID_NANOVDB_TOPOLOGY 0x963

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


/** @brief Create an image from file as sRGB (if supported)
*
*  @param  context     The context to create image
*  @param  path        NULL terminated path to an image file (can be relative) (for UNICODE, supports UTF-8 encoding)
*  @param  out_image   Pointer to image object
*  @return             RPR_SUCCESS in case of success, error code otherwise
*/
typedef rpr_int (*rprContextCreateImageFromFileSRGB_func)(rpr_context context, rpr_char const* path, rpr_image* out_image);
#define RPR_CONTEXT_CREATE_IMAGE_FROM_FILE_SRGB "rprContextCreateImageFromFileSRGB"

/**
 * Update external image handle.
 *
 * @param  context        The context to create image
 * @param  image          Pointer to image object
 * @param  external_image External image handle. Supported handles: VkImage*, ID3D12Resource*.
 */
typedef rpr_int(*rprContextUpdateImageExternalHandle_func)(rpr_context context, rpr_image image, const void* external_image);
#define RPR_CONTEXT_UPDATE_IMAGE_EXTERNAL_HANDLE "rprContextUpdateImageExternalHandle"


typedef enum
{
    /*
     * @brief All registered timings without any filtering.
     */ 
    ALL_TIMINGS = 0x0,

    /*
     * @brief Timings with no overlaping.
     * Smarter mode than ALL_TIMINGS, useful for displaying timings as column diagram.
     * The problem with ALL_TIMINGS is that some of the timings are nested to each other.
     * This mode has the useful property: the sum of the returned timings is ~ frame time.
     * You can set appropriate level of detalization through the functions.
     */
    NO_OVERLAPPING_TIMINGS = 0x1,
} rpr_debug_timings_mode;

/**
 * @brief Get GPU timings.
 * Rerturns names and its times (start/end) relative begining of the frame.
 * Just substract out_t1[i] - out_t0[i] to calculate delta time.
 * 
 * @param  context        The context.
 * @param  mode           Set of timings.
 * @param  gpu_index      Device index.
 * @param  detalization   Detalization level for the NO_OVERLAPPING_TIMINGS mode. 0 means full GPU frame time, >0 more detalized timings.
 * @param  out_names      Name of the timings.
 * @param  out_t0         Timings's start point in milliseconds relative to frame start.
 * @param  out_t1         Timings's end point in milliseconds relative to frame start.
 * @param  out_count      The number of timings.
 */
typedef rpr_int(*rprDebugGetGPUTimings_func)(rpr_context context, rpr_debug_timings_mode mode, rpr_uint gpu_index, rpr_uint detalization,
    const char** out_names, float* out_t0, float* out_t1, rpr_uint* out_count);
#define RPR_CONTEXT_DEBUG_GET_GPU_TIMINGS "rprDebugGetGPUTimings"

/**
 * @brief Get CPU timings.
 * Rerturns names and its times (start/end) relative begining of the frame.
 * Just substract out_t1[i] - out_t0[i] to calculate delta time.
 * 
 * @param  context        The context.
 * @param  mode           Set of timings.
 * @param  detalization   Detalization level for the NO_OVERLAPPING_TIMINGS mode. 0 means full CPU frame time, >0 more detalized timings.
 * @param  out_names      Name of the timings.
 * @param  out_t0         Timings's start point in milliseconds relative to frame start.
 * @param  out_t1         Timings's end point in milliseconds relative to frame start.
 * @param  out_count      The number of timings.
 */
typedef rpr_int(*rprDebugGetCPUTimings_func)(rpr_context context, rpr_debug_timings_mode mode, rpr_uint detalization,
    const char** out_names, float* out_t0, float* out_t1, rpr_uint* out_count);
#define RPR_CONTEXT_DEBUG_GET_CPU_TIMINGS "rprDebugGetCPUTimings"

typedef rpr_int(*rprCameraSetJitterOffset_func)(rpr_camera camera, rpr_float x, rpr_float y);
#define RPR_CAMERA_SET_JITTER_OFFSET_FUNC_NAME "rprCameraSetJitterOffset"

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
