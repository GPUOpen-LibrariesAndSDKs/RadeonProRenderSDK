/*****************************************************************************\
*
*  Module Name    RadeonProRender.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

//using FireRender.AMD.RenderEngine.Common;
//using FireRender.Types;
//using FireRender.Types.Common;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace FireRender.AMD.RenderEngine.Core
{
public static class Rpr
{
private const string dllName = "RadeonProRender64";
//public const string TahoeDll = "Tahoe64.dll";

static void Check(Status result)
{
	 Rpr.Status res = (Rpr.Status)result;
    if (Rpr.Status.SUCCESS != res)
    {
        var name = new StackFrame(1).GetMethod().Name;
        var message = string.Format("Error in Rpr.cs, method ({0}) result is {1}", name, res);
        //throw new RprException(res, message);
    }
}
[StructLayout(LayoutKind.Sequential)]
public struct ImageDesc // rpr_image_desc
{
    public uint Width;
    public uint Height;
    public uint Depth;
    public uint RowPitch;
    public uint SlicePitch;
};
[StructLayout(LayoutKind.Sequential)]
public struct BufferDesc // rpr_buffer_desc
{
    public uint NbElement;
    public uint ElementType;
    public uint ElementChannelSize;
};
[StructLayout(LayoutKind.Sequential)]
public struct FrameBufferDesc // rpr_framebuffer_desc
{
    public uint FbWidth;
    public uint FbHeight;
};
[StructLayout(LayoutKind.Sequential)]
public struct RenderStatistics // rpr_render_statistics
{
    public long GpumemUsage;
    public long GpumemTotal;
    public long GpumemMaxAllocation;
    public long SysmemUsage;
};
[StructLayout(LayoutKind.Sequential)]
public struct ImageFormat // rpr_image_format
{
    public uint NumComponents;
    public uint Type;
};
[StructLayout(LayoutKind.Sequential)]
public struct FramebufferFormat // rpr_framebuffer_format
{
    public uint NumComponents;
    public uint Type;
};
[StructLayout(LayoutKind.Sequential)]
public struct IesImageDesc // rpr_ies_image_desc
{
    public int W;
    public int H;
    public string Data;
    public string Filename;
};
/*rpr_status*/
public enum Status : int
{
SUCCESS = 0 ,
ERROR_COMPUTE_API_NOT_SUPPORTED = -1 ,
ERROR_OUT_OF_SYSTEM_MEMORY = -2 ,
ERROR_OUT_OF_VIDEO_MEMORY = -3 ,
ERROR_INVALID_LIGHTPATH_EXPR = -5 ,
ERROR_INVALID_IMAGE = -6 ,
ERROR_INVALID_AA_METHOD = -7 ,
ERROR_UNSUPPORTED_IMAGE_FORMAT = -8 ,
ERROR_INVALID_GL_TEXTURE = -9 ,
ERROR_INVALID_CL_IMAGE = -10 ,
ERROR_INVALID_OBJECT = -11 ,
ERROR_INVALID_PARAMETER = -12 ,
ERROR_INVALID_TAG = -13 ,
ERROR_INVALID_LIGHT = -14 ,
ERROR_INVALID_CONTEXT = -15 ,
ERROR_UNIMPLEMENTED = -16 ,
ERROR_INVALID_API_VERSION = -17 ,
ERROR_INTERNAL_ERROR = -18 ,
ERROR_IO_ERROR = -19 ,
ERROR_UNSUPPORTED_SHADER_PARAMETER_TYPE = -20 ,
ERROR_MATERIAL_STACK_OVERFLOW = -21 ,
ERROR_INVALID_PARAMETER_TYPE = -22 ,
ERROR_UNSUPPORTED = -23 ,
ERROR_OPENCL_OUT_OF_HOST_MEMORY = -24 ,
ERROR_OPENGL = -25 ,
ERROR_OPENCL = -26 ,
ERROR_NULLPTR = -27 ,
ERROR_NODETYPE = -28 ,
ERROR_ABORTED = -29 ,
}
/*rpr_parameter_type*/
public enum ParameterType : int
{
UNDEF = 0x0 ,
FLOAT = 0x1 ,
FLOAT2 = 0x2 ,
FLOAT3 = 0x3 ,
FLOAT4 = 0x4 ,
IMAGE = 0x5 ,
STRING = 0x6 ,
SHADER = 0x7 ,
UINT = 0x8 ,
ULONG = 0x9 ,
LONGLONG = 0xa ,
}
/*rpr_creation_flags*/
public enum CreationFlags : int
{
ENABLE_GPU0 = (1 << 0) ,
ENABLE_GPU1 = (1 << 1) ,
ENABLE_GPU2 = (1 << 2) ,
ENABLE_GPU3 = (1 << 3) ,
ENABLE_CPU = (1 << 4) ,
ENABLE_GL_INTEROP = (1 << 5) ,
ENABLE_GPU4 = (1 << 6) ,
ENABLE_GPU5 = (1 << 7) ,
ENABLE_GPU6 = (1 << 8) ,
ENABLE_GPU7 = (1 << 9) ,
ENABLE_METAL = (1 << 10) ,
ENABLE_GPU8 = (1 << 11) ,
ENABLE_GPU9 = (1 << 12) ,
ENABLE_GPU10 = (1 << 13) ,
ENABLE_GPU11 = (1 << 14) ,
ENABLE_GPU12 = (1 << 15) ,
ENABLE_GPU13 = (1 << 16) ,
ENABLE_GPU14 = (1 << 17) ,
ENABLE_GPU15 = (1 << 18) ,
ENABLE_HIP = (1 << 19) ,
ENABLE_DEBUG = (1 << 31) ,
}
/*rpr_aa_filter*/
public enum FilterType : int
{
NONE = 0x0 ,
BOX = 0x1 ,
TRIANGLE = 0x2 ,
GAUSSIAN = 0x3 ,
MITCHELL = 0x4 ,
LANCZOS = 0x5 ,
BLACKMANHARRIS = 0x6 ,
}
/*rpr_context_sampler_type*/
public enum ContextSamplerType : int
{
SOBOL = 0x1 ,
RANDOM = 0x2 ,
CMJ = 0x3 ,
}
/*rpr_primvar_interpolation_type*/
public enum PrimvarInterpolationType : int
{
CONSTANT = 0x1 ,
UNIFORM = 0x2 ,
VERTEX = 0x3 ,
FACEVARYING_NORMAL = 0x4 ,
FACEVARYING_UV = 0x5 ,
}
/*rpr_shape_type*/
public enum ShapeType : int
{
MESH = 0x1 ,
INSTANCE = 0x2 ,
}
/*rpr_light_type*/
public enum LightType : int
{
POINT = 0x1 ,
DIRECTIONAL = 0x2 ,
SPOT = 0x3 ,
ENVIRONMENT = 0x4 ,
SKY = 0x5 ,
IES = 0x6 ,
SPHERE = 0x7 ,
DISK = 0x8 ,
}
/*rpr_context_info*/
public enum ContextInfo : int
{
CREATION_FLAGS = 0x102 ,
CACHE_PATH = 0x103 ,
RENDER_STATUS = 0x104 ,
RENDER_STATISTICS = 0x105 ,
DEVICE_COUNT = 0x106 ,
PARAMETER_COUNT = 0x107 ,
ACTIVE_PLUGIN = 0x108 ,
SCENE = 0x109 ,
ITERATIONS = 0x10B ,
IMAGE_FILTER_TYPE = 0x10C ,
TONE_MAPPING_TYPE = 0x113 ,
TONE_MAPPING_LINEAR_SCALE = 0x114 ,
TONE_MAPPING_PHOTO_LINEAR_SENSITIVITY = 0x115 ,
TONE_MAPPING_PHOTO_LINEAR_EXPOSURE = 0x116 ,
TONE_MAPPING_PHOTO_LINEAR_FSTOP = 0x117 ,
TONE_MAPPING_REINHARD02_PRE_SCALE = 0x118 ,
TONE_MAPPING_REINHARD02_POST_SCALE = 0x119 ,
TONE_MAPPING_REINHARD02_BURN = 0x11A ,
MAX_RECURSION = 0x11B ,
RAY_CAST_EPSILON = 0x11C ,
RADIANCE_CLAMP = 0x11D ,
X_FLIP = 0x11E ,
Y_FLIP = 0x11F ,
TEXTURE_GAMMA = 0x120 ,
PDF_THRESHOLD = 0x121 ,
RENDER_MODE = 0x122 ,
ROUGHNESS_CAP = 0x123 ,
DISPLAY_GAMMA = 0x124 ,
MATERIAL_STACK_SIZE = 0x125 ,
CUTTING_PLANES = 0x126 ,
GPU0_NAME = 0x127 ,
GPU1_NAME = 0x128 ,
GPU2_NAME = 0x129 ,
GPU3_NAME = 0x12A ,
CPU_NAME = 0x12B ,
GPU4_NAME = 0x12C ,
GPU5_NAME = 0x12D ,
GPU6_NAME = 0x12E ,
GPU7_NAME = 0x12F ,
TONE_MAPPING_EXPONENTIAL_INTENSITY = 0x130 ,
FRAMECOUNT = 0x131 ,
TEXTURE_COMPRESSION = 0x132 ,
AO_RAY_LENGTH = 0x133 ,
OOC_TEXTURE_CACHE = 0x134 ,
PREVIEW = 0x135 ,
CPU_THREAD_LIMIT = 0x136 ,
LAST_ERROR_MESSAGE = 0x137 ,
MAX_DEPTH_DIFFUSE = 0x138 ,
MAX_DEPTH_GLOSSY = 0x139 ,
OOC_CACHE_PATH = 0x13a ,
MAX_DEPTH_REFRACTION = 0x13B ,
MAX_DEPTH_GLOSSY_REFRACTION = 0x13C ,
RENDER_LAYER_MASK = 0x13D ,
SINGLE_LEVEL_BVH_ENABLED = 0x13E ,
TRANSPARENT_BACKGROUND = 0x13F ,
MAX_DEPTH_SHADOW = 0x140 ,
API_VERSION = 0x141 ,
GPU8_NAME = 0x142 ,
GPU9_NAME = 0x143 ,
GPU10_NAME = 0x144 ,
GPU11_NAME = 0x145 ,
GPU12_NAME = 0x146 ,
GPU13_NAME = 0x147 ,
GPU14_NAME = 0x148 ,
GPU15_NAME = 0x149 ,
API_VERSION_MINOR = 0x14A ,
METAL_PERFORMANCE_SHADER = 0x14B ,
USER_TEXTURE_0 = 0x14C ,
USER_TEXTURE_1 = 0x14D ,
USER_TEXTURE_2 = 0x14E ,
USER_TEXTURE_3 = 0x14F ,
MIPMAP_LOD_OFFSET = 0x150 ,
AO_RAY_COUNT = 0x151 ,
SAMPLER_TYPE = 0x152 ,
ADAPTIVE_SAMPLING_TILE_SIZE = 0x153 ,
ADAPTIVE_SAMPLING_MIN_SPP = 0x154 ,
ADAPTIVE_SAMPLING_THRESHOLD = 0x155 ,
TILE_SIZE = 0x156 ,
LIST_CREATED_CAMERAS = 0x157 ,
LIST_CREATED_MATERIALNODES = 0x158 ,
LIST_CREATED_LIGHTS = 0x159 ,
LIST_CREATED_SHAPES = 0x15A ,
LIST_CREATED_POSTEFFECTS = 0x15B ,
LIST_CREATED_HETEROVOLUMES = 0x15C ,
LIST_CREATED_GRIDS = 0x15D ,
LIST_CREATED_BUFFERS = 0x15E ,
LIST_CREATED_IMAGES = 0x15F ,
LIST_CREATED_FRAMEBUFFERS = 0x160 ,
LIST_CREATED_SCENES = 0x161 ,
LIST_CREATED_CURVES = 0x162 ,
LIST_CREATED_MATERIALSYSTEM = 0x163 ,
LIST_CREATED_COMPOSITE = 0x164 ,
LIST_CREATED_LUT = 0x165 ,
AA_ENABLED = 0x166 ,
ACTIVE_PIXEL_COUNT = 0x167 ,
TRACING_ENABLED = 0x168 ,
TRACING_PATH = 0x169 ,
TILE_RECT = 0x16A ,
PLUGIN_VERSION = 0x16B ,
RUSSIAN_ROULETTE_DEPTH = 0x16C ,
SHADOW_CATCHER_BAKING = 0x16D ,
RENDER_UPDATE_CALLBACK_FUNC = 0x16E ,
RENDER_UPDATE_CALLBACK_DATA = 0x16F ,
COMPILE_CALLBACK_FUNC = 0x601 ,
COMPILE_CALLBACK_DATA = 0x602 ,
TEXTURE_CACHE_PATH = 0x170 ,
OCIO_CONFIG_PATH = 0x171 ,
OCIO_RENDERING_COLOR_SPACE = 0x172 ,
CONTOUR_USE_OBJECTID = 0x173 ,
CONTOUR_USE_MATERIALID = 0x174 ,
CONTOUR_USE_NORMAL = 0x175 ,
CONTOUR_USE_UV = 0x186 ,
CONTOUR_NORMAL_THRESHOLD = 0x176 ,
CONTOUR_UV_THRESHOLD = 0x187 ,
CONTOUR_UV_SECONDARY = 0x194 ,
CONTOUR_LINEWIDTH_OBJECTID = 0x177 ,
CONTOUR_LINEWIDTH_MATERIALID = 0x178 ,
CONTOUR_LINEWIDTH_NORMAL = 0x179 ,
CONTOUR_LINEWIDTH_UV = 0x188 ,
CONTOUR_ANTIALIASING = 0x17A ,
CONTOUR_DEBUG_ENABLED = 0x17F ,
GPUINTEGRATOR = 0x17B ,
CPUINTEGRATOR = 0x17C ,
BEAUTY_MOTION_BLUR = 0x17D ,
CAUSTICS_REDUCTION = 0x17E ,
GPU_MEMORY_LIMIT = 0x180 ,
RENDER_LAYER_LIST = 0x181 ,
WINDING_ORDER_CORRECTION = 0x182 ,
DEEP_SUBPIXEL_MERGE_Z_THRESHOLD = 0x183 ,
DEEP_GPU_ALLOCATION_LEVEL = 0x184 ,
DEEP_COLOR_ENABLED = 0x185 ,
FOG_COLOR = 0x189 ,
FOG_DISTANCE = 0x18A ,
FOG_HEIGHT = 0x18B ,
ATMOSPHERE_VOLUME_COLOR = 0x18C ,
ATMOSPHERE_VOLUME_DENSITY = 0x18D ,
ATMOSPHERE_VOLUME_RADIANCE_CLAMP = 0x18F ,
FOG_HEIGHT_OFFSET = 0x18E ,
INDIRECT_DOWNSAMPLE = 0x190 ,
CRYPTOMATTE_EXTENDED = 0x191 ,
CRYPTOMATTE_SPLIT_INDIRECT = 0x192 ,
FOG_DIRECTION = 0x193 ,
RANDOM_SEED = 0x1000 ,
IBL_DISPLAY = 0x195 ,
FRAMEBUFFER_SAVE_FLOAT32 = 0x196 ,
UPDATE_TIME_CALLBACK_FUNC = 0x197 ,
UPDATE_TIME_CALLBACK_DATA = 0x198 ,
RENDER_TIME_CALLBACK_FUNC = 0x199 ,
RENDER_TIME_CALLBACK_DATA = 0x19A ,
FIRST_ITERATION_TIME_CALLBACK_FUNC = 0x19B ,
FIRST_ITERATION_TIME_CALLBACK_DATA = 0x19C ,
IMAGE_FILTER_RADIUS = 0x19D ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_camera_info*/
public enum CameraInfo : int
{
TRANSFORM = 0x201 ,
FSTOP = 0x202 ,
APERTURE_BLADES = 0x203 ,
RESPONSE = 0x204 ,
EXPOSURE = 0x205 ,
FOCAL_LENGTH = 0x206 ,
SENSOR_SIZE = 0x207 ,
MODE = 0x208 ,
ORTHO_WIDTH = 0x209 ,
ORTHO_HEIGHT = 0x20A ,
FOCUS_DISTANCE = 0x20B ,
POSITION = 0x20C ,
LOOKAT = 0x20D ,
UP = 0x20E ,
FOCAL_TILT = 0x20F ,
LENS_SHIFT = 0x210 ,
IPD = 0x211 ,
TILT_CORRECTION = 0x212 ,
NEAR_PLANE = 0x213 ,
FAR_PLANE = 0x214 ,
LINEAR_MOTION = 0x215 ,
ANGULAR_MOTION = 0x216 ,
MOTION_TRANSFORMS_COUNT = 0x217 ,
MOTION_TRANSFORMS = 0x218 ,
POST_SCALE = 0x219 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_image_info*/
public enum ImageInfo : int
{
FORMAT = 0x301 ,
DESC = 0x302 ,
DATA = 0x303 ,
DATA_SIZEBYTE = 0x304 ,
WRAP = 0x305 ,
FILTER = 0x306 ,
GAMMA = 0x307 ,
MIPMAP_ENABLED = 0x308 ,
MIP_COUNT = 0x309 ,
GAMMA_FROM_FILE = 0x30A ,
UDIM = 0x30B ,
OCIO_COLORSPACE = 0x30C ,
INTERNAL_COMPRESSION = 0x30D ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_buffer_info*/
public enum Buffer : int
{
DESC = 0x350 ,
DATA = 0x351 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_shape_info*/
public enum ShapeInfo : int
{
TYPE = 0x401 ,
VIDMEM_USAGE = 0x402 ,
TRANSFORM = 0x403 ,
MATERIAL = 0x404 ,
LINEAR_MOTION = 0x405 ,
ANGULAR_MOTION = 0x406 ,
SHADOW_FLAG = 0x408,
SUBDIVISION_FACTOR = 0x409 ,
DISPLACEMENT_SCALE = 0x40A ,
SHADOW_CATCHER_FLAG = 0x40E ,
VOLUME_MATERIAL = 0x40F ,
OBJECT_GROUP_ID = 0x410 ,
SUBDIVISION_CREASEWEIGHT = 0x411 ,
SUBDIVISION_BOUNDARYINTEROP = 0x412 ,
DISPLACEMENT_MATERIAL = 0x413 ,
MATERIALS_PER_FACE = 0x415 ,
SCALE_MOTION = 0x416 ,
HETERO_VOLUME = 0x417 ,
LAYER_MASK = 0x418 ,
VISIBILITY_PRIMARY_ONLY_FLAG = 0x40C ,
VISIBILITY_SHADOW = 0x41A ,
VISIBILITY_REFLECTION = 0x41B ,
VISIBILITY_REFRACTION = 0x41C ,
VISIBILITY_TRANSPARENT = 0x41D ,
VISIBILITY_DIFFUSE = 0x41E ,
VISIBILITY_GLOSSY_REFLECTION = 0x41F ,
VISIBILITY_GLOSSY_REFRACTION = 0x420 ,
VISIBILITY_LIGHT = 0x421 ,
LIGHT_GROUP_ID = 0x422 ,
STATIC = 0x423,
PER_VERTEX_VALUE0 = 0x424,
PER_VERTEX_VALUE1 = 0x425,
PER_VERTEX_VALUE2 = 0x426,
PER_VERTEX_VALUE3 = 0x427,
REFLECTION_CATCHER_FLAG = 0x428 ,
OBJECT_ID = 0x429 ,
SUBDIVISION_AUTO_RATIO_CAP = 0x42A ,
MOTION_TRANSFORMS_COUNT = 0x42B ,
MOTION_TRANSFORMS = 0x42C ,
CONTOUR_IGNORE = 0x42D ,
RENDER_LAYER_LIST = 0x42E ,
SHADOW_COLOR = 0x42F ,
VISIBILITY_RECEIVE_SHADOW = 0x430 ,
PRIMVARS = 0x431,
ENVIRONMENT_LIGHT = 0x432,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_mesh_info*/
public enum Mesh : int
{
POLYGON_COUNT = 0x501 ,
VERTEX_COUNT = 0x502 ,
NORMAL_COUNT = 0x503 ,
UV_COUNT = 0x504 ,
VERTEX_ARRAY = 0x505 ,
NORMAL_ARRAY = 0x506 ,
UV_ARRAY = 0x507 ,
VERTEX_INDEX_ARRAY = 0x508 ,
NORMAL_INDEX_ARRAY = 0x509 ,
UV_INDEX_ARRAY = 0x50A ,
VERTEX_STRIDE = 0x50C ,
NORMAL_STRIDE = 0x50D ,
UV_STRIDE = 0x50E ,
VERTEX_INDEX_STRIDE = 0x50F ,
NORMAL_INDEX_STRIDE = 0x510 ,
UV_INDEX_STRIDE = 0x511 ,
NUM_FACE_VERTICES_ARRAY = 0x512 ,
UV2_COUNT = 0x513 ,
UV2_ARRAY = 0x514 ,
UV2_INDEX_ARRAY = 0x515 ,
UV2_STRIDE = 0x516 ,
UV2_INDEX_STRIDE = 0x517 ,
UV_DIM = 0x518 ,
MOTION_DIMENSION = 0x519 ,
VOLUME_FLAG = 0x51A ,
}
/*rpr_scene_info*/
public enum Scene : int
{
SHAPE_COUNT = 0x701 ,
LIGHT_COUNT = 0x702 ,
SHAPE_LIST = 0x704 ,
LIGHT_LIST = 0x705 ,
CAMERA = 0x706 ,
CAMERA_RIGHT = 0x707 ,
BACKGROUND_IMAGE = 0x708 ,
AABB = 0x70D ,
HETEROVOLUME_LIST = 0x70E ,
HETEROVOLUME_COUNT = 0x70F ,
CURVE_LIST = 0x710 ,
CURVE_COUNT = 0x711 ,
ENVIRONMENT_LIGHT = 0x712 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_lut_info*/
public enum Lut : int
{
LUT_FILENAME = 0x850 ,
LUT_DATA = 0x851 ,
}
/*rpr_light_info*/
public enum Light : int
{
LIGHT_TYPE = 0x801 ,
LIGHT_TRANSFORM = 0x803 ,
LIGHT_GROUP_ID = 0x805 ,
LIGHT_RENDER_LAYER_LIST = 0x806 ,
LIGHT_VISIBILITY_LIGHT = 0x807 ,
LIGHT_NAME = 0x777777 ,
LIGHT_UNIQUE_ID = 0x777778 ,
LIGHT_CUSTOM_PTR = 0x777779 ,
/* rpr_light_info - point light */
POINT_LIGHT_RADIANT_POWER = 0x804 ,
/* rpr_light_info - directional light */
DIRECTIONAL_LIGHT_RADIANT_POWER = 0x808 ,
DIRECTIONAL_LIGHT_SHADOW_SOFTNESS_ANGLE = 0x80A ,
/* rpr_light_info - spot light */
SPOT_LIGHT_RADIANT_POWER = 0x80B ,
SPOT_LIGHT_CONE_SHAPE = 0x80C ,
SPOT_LIGHT_IMAGE = 0x80D ,
/* rpr_light_info - environment light */
ENVIRONMENT_LIGHT_IMAGE = 0x80F ,
ENVIRONMENT_LIGHT_INTENSITY_SCALE = 0x810 ,
ENVIRONMENT_LIGHT_PORTAL_LIST = 0x818 ,
ENVIRONMENT_LIGHT_PORTAL_COUNT = 0x819 ,
ENVIRONMENT_LIGHT_OVERRIDE_REFLECTION = 0x81A ,
ENVIRONMENT_LIGHT_OVERRIDE_REFRACTION = 0x81B ,
ENVIRONMENT_LIGHT_OVERRIDE_TRANSPARENCY = 0x81C ,
ENVIRONMENT_LIGHT_OVERRIDE_BACKGROUND = 0x81D ,
ENVIRONMENT_LIGHT_OVERRIDE_IRRADIANCE = 0x81E ,
/* rpr_light_info - sky light */
SKY_LIGHT_TURBIDITY = 0x812 ,
SKY_LIGHT_ALBEDO = 0x813 ,
SKY_LIGHT_SCALE = 0x814 ,
SKY_LIGHT_DIRECTION = 0x815 ,
SKY_LIGHT_PORTAL_LIST = 0x820 ,
SKY_LIGHT_PORTAL_COUNT = 0x821 ,
/* rpr_light_info - IES light */
IES_LIGHT_RADIANT_POWER = 0x816 ,
IES_LIGHT_IMAGE_DESC = 0x817 ,
/* rpr_light_info - sphere light */
SPHERE_LIGHT_RADIANT_POWER = 0x822 ,
SPHERE_LIGHT_RADIUS = 0x824 ,
/* rpr_light_info - disk light */
DISK_LIGHT_RADIANT_POWER = 0x823 ,
DISK_LIGHT_RADIUS = 0x825 ,
DISK_LIGHT_ANGLE = 0x826 ,
DISK_LIGHT_INNER_ANGLE = 0x827 ,
}
/*rpr_parameter_info*/
public enum Parameter : int
{
NAME = 0x1201 ,
TYPE = 0x1203 ,
DESCRIPTION = 0x1204 ,
VALUE = 0x1205 ,
}
/*rpr_framebuffer_info*/
public enum FrameBuffer : int
{
FORMAT = 0x1301 ,
DESC = 0x1302 ,
DATA = 0x1303 ,
GL_TARGET = 0x1304 ,
GL_MIPLEVEL = 0x1305 ,
GL_TEXTURE = 0x1306 ,
LPE = 0x1307 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_component_type*/
public enum ComponentType : int
{
UINT8 = 0x1 ,
FLOAT16 = 0x2,
FLOAT32 = 0x3,
UNKNOWN = 0x4,
DEEP = 0x5,
UINT32 = 0x6,
}
/*rpr_buffer_element_type*/
public enum BufferElementType : int
{
INT32 = 0x1 ,
FLOAT32 = 0x2 ,
}
/*rpr_render_mode*/
public enum RenderMode : int
{
GLOBAL_ILLUMINATION = 0x1 ,
DIRECT_ILLUMINATION = 0x2 ,
DIRECT_ILLUMINATION_NO_SHADOW = 0x3 ,
WIREFRAME = 0x4 ,
MATERIAL_INDEX = 0x5 ,
POSITION = 0x6 ,
NORMAL = 0x7 ,
TEXCOORD = 0x8 ,
AMBIENT_OCCLUSION = 0x9 ,
DIFFUSE = 0x0a ,
}
/*rpr_camera_mode*/
public enum CameraMode : int
{
PERSPECTIVE = 0x1 ,
ORTHOGRAPHIC = 0x2 ,
LATITUDE_LONGITUDE_360 = 0x3 ,
LATITUDE_LONGITUDE_STEREO = 0x4 ,
CUBEMAP = 0x5 ,
CUBEMAP_STEREO = 0x6 ,
FISHEYE = 0x7 ,
}
/*rpr_tonemapping_operator*/
public enum ToneMappingOperator : int
{
NONE = 0x0 ,
LINEAR = 0x1 ,
PHOTOLINEAR = 0x2 ,
AUTOLINEAR = 0x3 ,
MAXWHITE = 0x4 ,
REINHARD02 = 0x5 ,
EXPONENTIAL = 0x6 ,
}
/*rpr_volume_type*/
public enum VolumeType : int
{
NONE = 0xFFFF ,
HOMOGENEOUS = 0x0 ,
HETEROGENEOUS = 0x1 ,
}
/*rpr_material_system_info*/
public enum MaterialSystemInfo : int
{
NODE_LIST = 0x1100,
}
/*rpr_material_node_info*/
public enum MaterialNodeInfo : int
{
TYPE = 0x1101 ,
SYSTEM = 0x1102 ,
INPUT_COUNT = 0x1103 ,
ID = 0x1104 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
RPR_MATERIAL_CUSTOM_PTR = 0x777779 ,
}
/*rpr_material_node_input_info*/
public enum MaterialNodeInput : int
{
NAME = 0x1103 ,
DESCRIPTION = 0x1105 ,
VALUE = 0x1106 ,
TYPE = 0x1107 ,
}
/*rpr_material_node_type*/
public enum MaterialNodeType : int
{
DIFFUSE = 0x1 ,
MICROFACET = 0x2 ,
REFLECTION = 0x3 ,
REFRACTION = 0x4 ,
MICROFACET_REFRACTION = 0x5 ,
TRANSPARENT = 0x6 ,
EMISSIVE = 0x7 ,
WARD = 0x8 ,
ADD = 0x9 ,
BLEND = 0xA ,
ARITHMETIC = 0xB ,
FRESNEL = 0xC ,
NORMAL_MAP = 0xD ,
IMAGE_TEXTURE = 0xE ,
NOISE2D_TEXTURE = 0xF ,
DOT_TEXTURE = 0x10 ,
GRADIENT_TEXTURE = 0x11 ,
CHECKER_TEXTURE = 0x12 ,
CONSTANT_TEXTURE = 0x13 ,
INPUT_LOOKUP = 0x14 ,
BLEND_VALUE = 0x16 ,
PASSTHROUGH = 0x17 ,
ORENNAYAR = 0x18 ,
FRESNEL_SCHLICK = 0x19 ,
DIFFUSE_REFRACTION = 0x1B ,
BUMP_MAP = 0x1C ,
VOLUME = 0x1D ,
MICROFACET_ANISOTROPIC_REFLECTION = 0x1E ,
MICROFACET_ANISOTROPIC_REFRACTION = 0x1F ,
TWOSIDED = 0x20 ,
UV_PROCEDURAL = 0x21 ,
MICROFACET_BECKMANN = 0x22 ,
PHONG = 0x23 ,
BUFFER_SAMPLER = 0x24 ,
UV_TRIPLANAR = 0x25 ,
AO_MAP = 0x26 ,
USER_TEXTURE_0 = 0x27 ,
USER_TEXTURE_1 = 0x28 ,
USER_TEXTURE_2 = 0x29 ,
USER_TEXTURE_3 = 0x2a ,
UBERV2 = 0x2b,
TRANSFORM = 0x2c,
RGB_TO_HSV = 0x2d,
HSV_TO_RGB = 0x2e,
USER_TEXTURE = 0x2f,
TOON_CLOSURE = 0x30,
TOON_RAMP = 0x31,
VORONOI_TEXTURE = 0x32 ,
GRID_SAMPLER = 0x33 ,
BLACKBODY = 0x34 ,
RAMP = 0x35 ,
PRIMVAR_LOOKUP = 0x36 ,
ROUNDED_CORNER = 0x37 ,

	// MaterialX materials
MATX_DIFFUSE_BRDF = 0x1000,
MATX_DIELECTRIC_BRDF = 0x1001,
MATX_GENERALIZED_SCHLICK_BRDF = 0x1002,
MATX_NOISE3D = 0x1003,
MATX_TANGENT = 0x1004,
MATX_NORMAL = 0x1005,
MATX_POSITION = 0x1006,
MATX_ROUGHNESS_ANISOTROPY = 0x1007,
MATX_ROTATE3D = 0x1008,
MATX_NORMALIZE = 0x1009,
MATX_IFGREATER = 0x100A,
MATX_SHEEN_BRDF = 0x100B,
MATX_DIFFUSE_BTDF = 0x100C,
MATX_CONVERT = 0x100D,
MATX_SUBSURFACE_BRDF = 0x100E,
MATX_DIELECTRIC_BTDF = 0x100F,
MATX_CONDUCTOR_BRDF = 0x1010,
MATX_FRESNEL = 0x1011,
MATX_LUMINANCE = 0x1012,
MATX_FRACTAL3D = 0x1013,
MATX_MIX = 0x1014,
MATX = 0x1015,
MATX_ARTISTIC_IOR = 0x1016,
MATX_GENERALIZED_SCHLICK_BTDF = 0x1017,
MATX_LAYER = 0x1018,
MATX_THIN_FILM = 0x1019,
MATX_BITANGENT = 0x101A,
MATX_TEXCOORD = 0x101B,
MATX_MODULO = 0x101C,
MATX_ABSVAL = 0x101D,
MATX_SIGN = 0x101E,
MATX_FLOOR = 0x101F,
MATX_CEIL = 0x1020,
MATX_ATAN2 = 0x1021,
MATX_SQRT = 0x1022,
MATX_LN = 0x1023,
MATX_EXP = 0x1024,
MATX_CLAMP = 0x1025,
MATX_MIN = 0x1026,
MATX_MAX = 0x1027,
MATX_MAGNITUDE = 0x1028,
MATX_CROSSPRODUCT = 0x1029,
MATX_REMAP = 0x102A,
MATX_SMOOTHSTEP = 0x102B,
MATX_RGBTOHSV = 0x102C,
MATX_HSVTORGB = 0x102D,
MATX_IFGREATEREQ = 0x102E,
MATX_IFEQUAL = 0x102F,
MATX_SWIZZLE = 0x1030,
MATX_NOISE2D = 0x1031,
MATX_PLUS = 0x1032,
MATX_MINUS = 0x1033,
MATX_DIFFERENCE = 0x1034,
MATX_BURN = 0x1035,
MATX_DODGE = 0x1036,
MATX_SCREEN = 0x1037,
MATX_OVERLAY = 0x1038,
MATX_INSIDE = 0x1039,
MATX_OUTSIDE = 0x103A,
MATX_RAMPLR = 0x103B,
MATX_RAMPTB = 0x103C,
MATX_SPLITLR = 0x103D,
MATX_SPLITTB = 0x103E,
MATX_CELLNOISE2D = 0x103F,
MATX_CELLNOISE3D = 0x1040,
MATX_ROTATE2D = 0x1041,
MATX_DOT = 0x1042,
MATX_RANGE = 0x1043,
MATX_SWITCH = 0x1044,
MATX_EXTRACT = 0x1045,
MATX_COMBINE2 = 0x1046,
MATX_COMBINE3 = 0x1047,
MATX_COMBINE4 = 0x1048,
MATX_TRIPLANARPROJECTION = 0x1049,
MATX_MULTIPLY = 0x104A,
}
/*rpr_material_node_input*/
public enum MaterialInput : int
{
COLOR = 0x0 ,
COLOR0 = 0x1 ,
COLOR1 = 0x2 ,
NORMAL = 0x3 ,
UV = 0x4 ,
DATA = 0x5 ,
ROUGHNESS = 0x6 ,
IOR = 0x7 ,
ROUGHNESS_X = 0x8 ,
ROUGHNESS_Y = 0x9 ,
ROTATION = 0xA ,
WEIGHT = 0xB ,
OP = 0xC ,
INVEC = 0xD ,
UV_SCALE = 0xE ,
VALUE = 0xF ,
REFLECTANCE = 0x10 ,
SCALE = 0x11 ,
SCATTERING = 0x12 ,
ABSORBTION = 0x13 ,
EMISSION = 0x14 ,
G = 0x15 ,
MULTISCATTER = 0x16 ,
COLOR2 = 0x17 ,
COLOR3 = 0x18 ,
ANISOTROPIC = 0x19 ,
FRONTFACE = 0x1a ,
BACKFACE = 0x1b ,
ORIGIN = 0x1c ,
ZAXIS = 0x1d ,
XAXIS = 0x1e ,
THRESHOLD = 0x1f ,
OFFSET = 0x20 ,
UV_TYPE = 0x21 ,
RADIUS = 0x22 ,
SIDE = 0x23 ,
CAUSTICS = 0x24 ,
TRANSMISSION_COLOR = 0x25 ,
THICKNESS = 0x26 ,
INDEX0 = 0x27 ,
INDEX1 = 0x28 ,
INDEX2 = 0x29 ,
INDEX3 = 0x2a ,
INDEX4 = 0x2b ,
SCHLICK_APPROXIMATION = 0x2c ,
APPLYSURFACE = 0x2d ,
TANGENT = 0x2e ,
DISTRIBUTION = 0x2f ,
BASE = 0x30 ,
TINT = 0x31 ,
EXPONENT = 0x32 ,
AMPLITUDE = 0x33 ,
PIVOT = 0x34 ,
POSITION = 0x35 ,
AMOUNT = 0x36 ,
AXIS = 0x37 ,
LUMACOEFF = 0x38 ,
REFLECTIVITY = 0x39 ,
EDGE_COLOR = 0x3a ,
VIEW_DIRECTION = 0x3b ,
INTERIOR = 0x3c ,
OCTAVES = 0x3d ,
LACUNARITY = 0x3e ,
DIMINISH = 0x3f ,
WRAP_U = 0x40 ,
WRAP_V = 0x41 ,
WRAP_W = 0x42 ,
INDEX5 = 0x43 ,
INDEX6 = 0x44 ,
INDEX7 = 0x45 ,
INDEX8 = 0x46 ,
INDEX9 = 0x47 ,
INDEX10 = 0x48 ,
INDEX11 = 0x49 ,
INDEX12 = 0x4a ,
INDEX13 = 0x4b ,
INDEX14 = 0x4c ,
INDEX15 = 0x4d ,
DIFFUSE_RAMP = 0x4e ,
SHADOW = 0x4f ,
MID = 0x50 ,
HIGHLIGHT = 0x51 ,
POSITION1 = 0x52 ,
POSITION2 = 0x53 ,
RANGE1 = 0x54 ,
RANGE2 = 0x55 ,
INTERPOLATION = 0x56 ,
RANDOMNESS = 0x57 ,
DIMENSION = 0x58 ,
OUTTYPE = 0x59 ,
DENSITY = 0x5a ,
DENSITYGRID = 0x5b ,
DISPLACEMENT = 0x5c ,
TEMPERATURE = 0x5d ,
KELVIN = 0x5e ,
EXTINCTION = 0x5f ,
THIN_FILM = 0x60 ,
TOP = 0x61 ,
HIGHLIGHT2 = 0x62 ,
SHADOW2 = 0x63 ,
POSITION_SHADOW = 0x64 ,
POSITION_HIGHLIGHT = 0x65 ,
RANGE_SHADOW = 0x66 ,
RANGE_HIGHLIGHT = 0x67 ,
TOON_5_COLORS = 0x68 ,
X = 0x69 ,
Y = 0x6a ,
Z = 0x6b ,
W = 0x6c ,
LIGHT = 0x6d ,
MID_IS_ALBEDO = 0x6e ,
SAMPLES = 0x6f ,
BASE_NORMAL = 0x70 ,
UBER_DIFFUSE_COLOR = 0x910,
UBER_DIFFUSE_WEIGHT = 0x927,
UBER_DIFFUSE_ROUGHNESS = 0x911,
UBER_DIFFUSE_NORMAL = 0x912,
UBER_REFLECTION_COLOR = 0x913,
UBER_REFLECTION_WEIGHT = 0x928,
UBER_REFLECTION_ROUGHNESS = 0x914,
UBER_REFLECTION_ANISOTROPY = 0x915,
UBER_REFLECTION_ANISOTROPY_ROTATION = 0x916,
UBER_REFLECTION_MODE = 0x917,
UBER_REFLECTION_IOR = 0x918,
UBER_REFLECTION_METALNESS = 0x919,
UBER_REFLECTION_NORMAL = 0x929,
UBER_REFLECTION_DIELECTRIC_REFLECTANCE = 0x93e,
UBER_REFRACTION_COLOR = 0x91A,
UBER_REFRACTION_WEIGHT = 0x92a,
UBER_REFRACTION_ROUGHNESS = 0x91B,
UBER_REFRACTION_IOR = 0x91C,
UBER_REFRACTION_NORMAL = 0x92b,
UBER_REFRACTION_THIN_SURFACE = 0x91D,
UBER_REFRACTION_ABSORPTION_COLOR = 0x92c,
UBER_REFRACTION_ABSORPTION_DISTANCE = 0x92d,
UBER_REFRACTION_CAUSTICS = 0x92e,
UBER_COATING_COLOR = 0x91E,
UBER_COATING_WEIGHT = 0x92f,
UBER_COATING_ROUGHNESS = 0x91F,
UBER_COATING_MODE = 0x920,
UBER_COATING_IOR = 0x921,
UBER_COATING_METALNESS = 0x922,
UBER_COATING_NORMAL = 0x923,
UBER_COATING_TRANSMISSION_COLOR = 0x930,
UBER_COATING_THICKNESS = 0x931,
UBER_SHEEN = 0x932,
UBER_SHEEN_TINT = 0x933,
UBER_SHEEN_WEIGHT = 0x934,
UBER_EMISSION_COLOR = 0x924,
UBER_EMISSION_WEIGHT = 0x925,
UBER_EMISSION_MODE = 0x935,
UBER_TRANSPARENCY = 0x926,
UBER_SSS_SCATTER_COLOR = 0x937,
UBER_SSS_SCATTER_DISTANCE = 0x938,
UBER_SSS_SCATTER_DIRECTION = 0x939,
UBER_SSS_WEIGHT = 0x93a,
UBER_SSS_MULTISCATTER = 0x93b,
UBER_BACKSCATTER_WEIGHT = 0x93c,
UBER_BACKSCATTER_COLOR = 0x93d,
ADDRESS = 0x93e,
TYPE = 0x93f,
UBER_FRESNEL_SCHLICK_APPROXIMATION = 0x2c ,
}
/*rpr_material_input_raster*/
public enum MaterialStandardInput : int
{
RPR_MATERIAL_INPUT_RASTER_METALLIC = 0x901 ,
RPR_MATERIAL_INPUT_RASTER_ROUGHNESS = 0x902 ,
RPR_MATERIAL_INPUT_RASTER_SUBSURFACE = 0x903 ,
RPR_MATERIAL_INPUT_RASTER_ANISOTROPIC = 0x904 ,
RPR_MATERIAL_INPUT_RASTER_SPECULAR = 0x905 ,
RPR_MATERIAL_INPUT_RASTER_SPECULARTINT = 0x906 ,
RPR_MATERIAL_INPUT_RASTER_SHEEN = 0x907 ,
RPR_MATERIAL_INPUT_RASTER_SHEENTINT = 0x908 ,
RPR_MATERIAL_INPUT_RASTER_CLEARCOAT = 0x90A ,
RPR_MATERIAL_INPUT_RASTER_CLEARCOATGLOSS = 0x90B ,
RPR_MATERIAL_INPUT_RASTER_COLOR = 0x90C ,
RPR_MATERIAL_INPUT_RASTER_NORMAL = 0x90D ,
}
/*rpr_interpolation_mode*/
public enum InterpolationMode : int
{
NONE = 0x0,
LINEAR = 0x1,
}
/*rpr_ubermaterial_ior_mode*/
public enum UberMaterialMode : int
{
PBR = 0x1,
METALNESS = 0x2,
}
/*rpr_ubermaterial_emission_mode*/
public enum UberMaterialEmissionMode : int
{
SINGLESIDED = 0x1,
DOUBLESIDED = 0x2,
}
/*rpr_material_node_arithmetic_operation*/
public enum MaterialNodeOp : int
{
ADD = 0x00 ,
SUB = 0x01 ,
MUL = 0x02 ,
DIV = 0x03 ,
SIN = 0x04 ,
COS = 0x05 ,
TAN = 0x06 ,
SELECT_X = 0x07 ,
SELECT_Y = 0x08 ,
SELECT_Z = 0x09 ,
COMBINE = 0x0A ,
DOT3 = 0x0B ,
CROSS3 = 0x0C ,
LENGTH3 = 0x0D ,
NORMALIZE3 = 0x0E ,
POW = 0x0F ,
ACOS = 0x10 ,
ASIN = 0x11 ,
ATAN = 0x12 ,
AVERAGE_XYZ = 0x13 ,
AVERAGE = 0x14 ,
MIN = 0x15 ,
MAX = 0x16 ,
FLOOR = 0x17 ,
MOD = 0x18 ,
ABS = 0x19 ,
SHUFFLE_YZWX = 0x1a ,
SHUFFLE_ZWXY = 0x1b ,
SHUFFLE_WXYZ = 0x1c ,
MAT_MUL = 0x1d ,
SELECT_W = 0x1e ,
DOT4 = 0x1f ,
LOG = 0x20 ,
LOWER_OR_EQUAL = 0x21 ,
LOWER = 0x22 ,
GREATER_OR_EQUAL = 0x23 ,
GREATER = 0x24 ,
EQUAL = 0x25 ,
NOT_EQUAL = 0x26 ,
AND = 0x27 ,
OR = 0x28 ,
TERNARY = 0x29 ,
}
/*rpr_material_node_lookup_value*/
public enum MaterialNodeLookup : int
{
UV = 0x0 ,
N = 0x1 ,
P = 0x2 ,
INVEC = 0x3 ,
OUTVEC = 0x4 ,
UV1 = 0x5 ,
P_LOCAL = 0x6 ,
VERTEX_VALUE0 = 0x7 ,
VERTEX_VALUE1 = 0x8 ,
VERTEX_VALUE2 = 0x9 ,
VERTEX_VALUE3 = 0xa ,
SHAPE_RANDOM_COLOR = 0xb ,
OBJECT_ID = 0xc ,
PRIMITIVE_RANDOM_COLOR = 0xd ,
}
/*rpr_material_gradient_procedural_type*/
public enum MaterialGradientProceduralType : int
{
LINEAR = 0x1 ,
QUADRATIC = 0x2 ,
EASING = 0x3 ,
DIAGONAL = 0x4 ,
SPHERICAL = 0x5 ,
QUAD_SPHERE = 0x6 ,
RADIAL = 0x7 ,
}
/*rpr_material_node_uvtype_value*/
public enum MaterialNodeUvtype : int
{
PLANAR = 0x0 ,
CYLINDICAL = 0x1 ,
SPHERICAL = 0x2 ,
PROJECT = 0x3 ,
}
/*rpr_material_node_transform_op*/
public enum MaterialNodeTransformType : int
{
ROTATE_LOCAL_TO_WORLD = 0x1 ,
}
/*rpr_post_effect_info*/
public enum PostEffectInfo : int
{
TYPE = 0x0 ,
WHITE_BALANCE_COLOR_SPACE = 0x4 ,
WHITE_BALANCE_COLOR_TEMPERATURE = 0x5 ,
SIMPLE_TONEMAP_EXPOSURE = 0x6 ,
SIMPLE_TONEMAP_CONTRAST = 0x7 ,
SIMPLE_TONEMAP_ENABLE_TONEMAP = 0x8 ,
BLOOM_RADIUS = 0x9 ,
BLOOM_THRESHOLD = 0x0a ,
BLOOM_WEIGHT = 0x0b ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_aov*/
public enum Aov : int
{
COLOR = 0x0 ,
OPACITY = 0x1 ,
WORLD_COORDINATE = 0x2 ,
UV = 0x3 ,
MATERIAL_ID = 0x4 ,
GEOMETRIC_NORMAL = 0x5 ,
SHADING_NORMAL = 0x6 ,
DEPTH = 0x7 ,
OBJECT_ID = 0x8 ,
OBJECT_GROUP_ID = 0x9 ,
SHADOW_CATCHER = 0x0a ,
BACKGROUND = 0x0b ,
EMISSION = 0x0c ,
VELOCITY = 0x0d ,
DIRECT_ILLUMINATION = 0x0e ,
INDIRECT_ILLUMINATION = 0x0f ,
AO = 0x10 ,
DIRECT_DIFFUSE = 0x11 ,
DIRECT_REFLECT = 0x12 ,
INDIRECT_DIFFUSE = 0x13 ,
INDIRECT_REFLECT = 0x14 ,
REFRACT = 0x15 ,
VOLUME = 0x16 ,
LIGHT_GROUP0 = 0x17 ,
LIGHT_GROUP1 = 0x18 ,
LIGHT_GROUP2 = 0x19 ,
LIGHT_GROUP3 = 0x1a ,
DIFFUSE_ALBEDO = 0x1b ,
VARIANCE = 0x1c ,
VIEW_SHADING_NORMAL = 0x1d,
REFLECTION_CATCHER = 0x1e,
COLOR_RIGHT = 0x1f ,
LPE_0 = 0x20 ,
LPE_1 = 0x21 ,
LPE_2 = 0x22 ,
LPE_3 = 0x23 ,
LPE_4 = 0x24 ,
LPE_5 = 0x25 ,
LPE_6 = 0x26 ,
LPE_7 = 0x27 ,
LPE_8 = 0x28 ,
CAMERA_NORMAL = 0x29 ,
MATTE_PASS = 0x2a ,
SSS = 0x2b ,
CRYPTOMATTE_MAT0 = 0x30,
CRYPTOMATTE_MAT1 = 0x31,
CRYPTOMATTE_MAT2 = 0x32,
CRYPTOMATTE_MAT3 = 0x33,
CRYPTOMATTE_MAT4 = 0x34,
CRYPTOMATTE_MAT5 = 0x35,
CRYPTOMATTE_OBJ0 = 0x38,
CRYPTOMATTE_OBJ1 = 0x39,
CRYPTOMATTE_OBJ2 = 0x3a,
CRYPTOMATTE_OBJ3 = 0x3b,
CRYPTOMATTE_OBJ4 = 0x3c,
CRYPTOMATTE_OBJ5 = 0x3d,
DEEP_COLOR = 0x40,
LIGHT_GROUP4 = 0x50 ,
LIGHT_GROUP5 = 0x51 ,
LIGHT_GROUP6 = 0x52 ,
LIGHT_GROUP7 = 0x53 ,
LIGHT_GROUP8 = 0x54 ,
LIGHT_GROUP9 = 0x55 ,
LIGHT_GROUP10 = 0x56 ,
LIGHT_GROUP11 = 0x57 ,
LIGHT_GROUP12 = 0x58 ,
LIGHT_GROUP13 = 0x59 ,
LIGHT_GROUP14 = 0x5a ,
LIGHT_GROUP15 = 0x5b ,
MESH_ID = 0x60 ,
}
/*rpr_post_effect_type*/
public enum PostEffectType : int
{
TONE_MAP = 0x0 ,
WHITE_BALANCE = 0x1 ,
SIMPLE_TONEMAP = 0x2 ,
NORMALIZATION = 0x3 ,
GAMMA_CORRECTION = 0x4 ,
BLOOM = 0x5 ,
}
/*rpr_color_space*/
public enum ColorSpace : int
{
SRGB = 0x1 ,
ADOBE_RGB = 0x2 ,
REC2020 = 0x3 ,
DCIP3 = 0x4 ,
}
/*rpr_material_node_input_type*/
public enum MaterialNodeInputType : int
{
/* rpr_material_node_input_type */
FLOAT4 = 0x1 ,
UINT = 0x2 ,
NODE = 0x3 ,
IMAGE = 0x4 ,
BUFFER = 0x5 ,
GRID = 0x6 ,
DATA = 0x7 ,
LIGHT = 0x8 ,
}
/*rpr_subdiv_boundary_interfop_type*/
public enum SubdivBoundaryInteropType : int
{
EDGE_AND_CORNER = 0x1 ,
EDGE_ONLY = 0x2 ,
}
/*rpr_image_wrap_type*/
public enum ImageWrapType : int
{
REPEAT = 0x1 ,
MIRRORED_REPEAT = 0x2 ,
CLAMP_TO_EDGE = 0x3 ,
CLAMP_ZERO = 0x5 ,
CLAMP_ONE = 0x6 ,
}
/*rpr_voronoi_out_type*/
public enum VoronoiOutType : int
{
DISTANCE = 0x1 ,
COLOR = 0x2 ,
POSITION = 0x3 ,
}
/*rpr_image_filter_type*/
public enum ImageFilterType : int
{
NEAREST = 0x1 ,
LINEAR = 0x2 ,
}
/*rpr_composite_info*/
public enum CompositeInfo : int
{
TYPE = 0x1  ,
FRAMEBUFFER_INPUT_FB = 0x2  ,
NORMALIZE_INPUT_COLOR = 0x3  ,
NORMALIZE_INPUT_AOVTYPE = 0x4  ,
CONSTANT_INPUT_VALUE = 0x5  ,
LERP_VALUE_INPUT_COLOR0 = 0x6  ,
LERP_VALUE_INPUT_COLOR1 = 0x7  ,
LERP_VALUE_INPUT_WEIGHT = 0x8  ,
ARITHMETIC_INPUT_COLOR0 = 0x9  ,
ARITHMETIC_INPUT_COLOR1 = 0x0a  ,
ARITHMETIC_INPUT_OP = 0x0b  ,
GAMMA_CORRECTION_INPUT_COLOR = 0x0c  ,
LUT_INPUT_LUT = 0x0d  ,
LUT_INPUT_COLOR = 0x0e  ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_composite_type*/
public enum CompositeType : int
{
ARITHMETIC = 0x1  ,
LERP_VALUE = 0x2  ,
INVERSE = 0x3  ,
NORMALIZE = 0x4  ,
GAMMA_CORRECTION = 0x5  ,
EXPOSURE = 0x6  ,
CONTRAST = 0x7  ,
SIDE_BY_SIDE = 0x8  ,
TONEMAP_ACES = 0x9  ,
TONEMAP_REINHARD = 0xa  ,
TONEMAP_LINEAR = 0xb  ,
FRAMEBUFFER = 0xc  ,
CONSTANT = 0xd  ,
LUT = 0xe  ,
}
/*rpr_grid_parameter*/
public enum Grid : int
{
SIZE_X = 0x930 ,
SIZE_Y = 0x931 ,
SIZE_Z = 0x932 ,
DATA = 0x933 ,
DATA_SIZEBYTE = 0x934 ,
INDICES = 0x936 ,
INDICES_NUMBER = 0x937 ,
INDICES_TOPOLOGY = 0x938 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_hetero_volume_parameter*/
public enum HeteroVolume : int
{
TRANSFORM = 0x735 ,
ALBEDO_V2 = 0x73c ,
DENSITY_V2 = 0x73d ,
EMISSION_V2 = 0x73e ,
ALBEDO_LOOKUP_VALUES = 0x73f ,
ALBEDO_LOOKUP_VALUES_COUNT = 0x740 ,
DENSITY_LOOKUP_VALUES = 0x741 ,
DENSITY_LOOKUP_VALUES_COUNT = 0x742 ,
EMISSION_LOOKUP_VALUES = 0x743 ,
EMISSION_LOOKUP_VALUES_COUNT = 0x744 ,
ALBEDO_SCALE = 0x745 ,
DENSITY_SCALE = 0x746 ,
EMISSION_SCALE = 0x747 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
}
/*rpr_grid_indices_topology*/
public enum GridIndicesTopology : int
{
I_U64 = 0x950 ,
XYZ_U32 = 0x951 ,
I_S64 = 0x952 ,
XYZ_S32 = 0x953 ,
}
/*rpr_curve_parameter*/
public enum Curve : int
{
CONTROLPOINTS_COUNT = 0x830 ,
CONTROLPOINTS_DATA = 0x831 ,
CONTROLPOINTS_STRIDE = 0x832 ,
INDICES_COUNT = 0x833 ,
INDICES_DATA = 0x834 ,
RADIUS = 0x835 ,
UV = 0x836 ,
COUNT_CURVE = 0x837 ,
SEGMENTS_PER_CURVE = 0x838 ,
CREATION_FLAG = 0x839 ,
NAME = 0x777777 ,
UNIQUE_ID = 0x777778 ,
CUSTOM_PTR = 0x777779 ,
TRANSFORM = 0x403 ,
MATERIAL = 0x404 ,
VISIBILITY_PRIMARY_ONLY_FLAG = 0x40C ,
VISIBILITY_SHADOW = 0x41A ,
VISIBILITY_REFLECTION = 0x41B ,
VISIBILITY_REFRACTION = 0x41C ,
VISIBILITY_TRANSPARENT = 0x41D ,
VISIBILITY_DIFFUSE = 0x41E ,
VISIBILITY_GLOSSY_REFLECTION = 0x41F ,
VISIBILITY_GLOSSY_REFRACTION = 0x420 ,
VISIBILITY_LIGHT = 0x421 ,
VISIBILITY_RECEIVE_SHADOW = 0x430 ,
}
public const uint RPR_VERSION_MAJOR = 2 ;
public const uint RPR_VERSION_MINOR = 2 ;
public const uint RPR_VERSION_REVISION = 16 ;
public const uint RPR_VERSION_BUILD = 0xb050a508 ;
public const uint RPR_VERSION_MAJOR_MINOR_REVISION = 0x00200216 ;
// Deprecated version naming - will be removed in the future :

public const uint RPR_API_VERSION = RPR_VERSION_MAJOR_MINOR_REVISION ;
public const uint RPR_API_VERSION_MINOR = RPR_VERSION_BUILD ;
public const uint RPR_OBJECT_NAME = 0x777777 ;
public const uint RPR_OBJECT_UNIQUE_ID = 0x777778 ;
public const uint RPR_OBJECT_CUSTOM_PTR = 0x777779 ;
/* last of the RPR_CONTEXT_* */
  
// RPR_PARAMETER_NAME_STRING 0x1202   not used anymore  you can only set/get parameters using  RPR_CONTEXT_*


/* rpr_instance_info */

public const uint RPR_INSTANCE_PARENT_SHAPE = 0x1601 ;
// RPR_MATERIAL_NODE_INPUT_NAME_STRING 0x1104  not used anymore  you can only set/get parameters using  RPR_MATERIAL_INPUT_*


/* rpr_bool */

public const uint RPR_FALSE = 0u;
public const uint RPR_TRUE = 1u;
/* Library types */
/* This is going to be moved to rpr_platform.h or similar */

/* API functions */

    /** @brief Register rendering plugin
    *
    *  <Description>
    *
    *  @param path     Path of plugin to load (for UNICODE, supports UTF-8 encoding)
    *  @return         unique identifier of plugin, -1 otherwise
    */
  
[DllImport(dllName)] static extern int rprRegisterPlugin(string path);
public static int RegisterPlugin(string path)
{
return rprRegisterPlugin(path);
}

    /** @brief Create rendering context
    *
    *  Rendering context is a root concept encapsulating the render states and responsible
    *  for execution control. All the entities in Radeon ProRender are created for a particular rendering context.
    *  Entities created for some context can't be used with other contexts. Possible error codes for this call are:
    *
    *     RPR_ERROR_COMPUTE_API_NOT_SUPPORTED
    *     RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *     RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *     RPR_ERROR_INVALID_API_VERSION
    *     RPR_ERROR_INVALID_PARAMETER
    *
    *  @param api_version     Api version constant
    *	 @param context_type    Determines compute API to use, OPENCL only is supported for now
    *  @param creation_flags  Determines multi-gpu or cpu-gpu configuration
    *  @param props           Context creation properties. Specifies a list of context property names and their corresponding values.
    *                         Each property name is immediately followed by the corresponding desired value.
    *                         The list is terminated with 0.
    *  @param cache_path      Full path to kernel cache created by Radeon ProRender, NULL means to use current folder (for UNICODE, supports UTF-8 encoding)
    *  @param cpu_thread_limit	Limit for the number of threads used for CPU rendering
    *  @param out_context		Pointer to context object
    *  @return                RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCreateContext(uint api_version, int[] pluginIDs, long pluginCount, CreationFlags creation_flags, IntPtr props, string cache_path, out IntPtr out_context);
public static Status CreateContext(uint api_version, int[] pluginIDs, long pluginCount, CreationFlags creation_flags, IntPtr props, string cache_path, out IntPtr out_context)
{
return rprCreateContext(api_version, pluginIDs, pluginCount, creation_flags, props, cache_path, out out_context);
}

    /** @breif Set active context plugin
    *
    */
  
[DllImport(dllName)] static extern Status rprContextSetActivePlugin(IntPtr context, int pluginID);
public static Status ContextSetActivePlugin(IntPtr context, int pluginID)
{
return rprContextSetActivePlugin(context, pluginID);
}

    /** @brief Query information about a context
    *
    *  The workflow is usually two-step: query with the data == NULL and size = 0 to get the required buffer size in size_ret,
    *  then query with size_ret == NULL to fill the buffer with the data.
    *   Possible error codes:
    *     RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  context         The context to query
    *  @param  context_info    The type of info to query
    *  @param  size            The size of the buffer pointed by data
    *  @param  data            The buffer to store queried info
    *  @param  size_ret        Returns the size in bytes of the data being queried
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextGetInfo(IntPtr context, ContextInfo context_info, long size, IntPtr data, out long size_ret);
public static Status ContextGetInfo(IntPtr context, ContextInfo context_info, long size, IntPtr data, out long size_ret)
{
return rprContextGetInfo(context, context_info, size, data, out size_ret);
}

    /** @brief Query information about a context parameter
    *
    *  The workflow is usually two-step: query with the data == NULL and size = 0 to get the required buffer size in size_ret,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *     RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  context         The context to query
    *  @param  param_idx	   The index of the parameter - must be between 0 and  (value stored by RPR_CONTEXT_PARAMETER_COUNT)-1
    *  @param  parameter_info  The type of info to query
    *  @param  size            The size of the buffer pointed by data
    *  @param  data            The buffer to store queried info
    *  @param  size_ret        Returns the size in bytes of the data being queried
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextGetParameterInfo(IntPtr context, int param_idx, Parameter parameter_info, long size, IntPtr data, out long size_ret);
public static Status ContextGetParameterInfo(IntPtr context, int param_idx, Parameter parameter_info, long size, IntPtr data, out long size_ret)
{
return rprContextGetParameterInfo(context, param_idx, parameter_info, size, data, out size_ret);
}

    /** @brief Query the AOV
    *
    *  @param  context     The context to get a frame buffer from
    *  @param  out_fb		Pointer to framebuffer object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextGetAOV(IntPtr context, Aov aov, out IntPtr out_fb);
public static Status ContextGetAOV(IntPtr context, Aov aov, out IntPtr out_fb)
{
return rprContextGetAOV(context, aov, out out_fb);
}

    /** @brief Set AOV
    *
    *  @param  context         The context to set AOV
    *  @param  aov				AOV
    *  @param  frame_buffer    Frame buffer object to set
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextSetAOV(IntPtr context, Aov aov, IntPtr frame_buffer);
public static Status ContextSetAOV(IntPtr context, Aov aov, IntPtr frame_buffer)
{
return rprContextSetAOV(context, aov, frame_buffer);
}

    /** @brief
    *
    *  @param  context               The context to set 
    *  @param  renderLayerString   Render layer name to attach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextAttachRenderLayer(IntPtr context, string renderLayerString);
public static Status ContextAttachRenderLayer(IntPtr context, string renderLayerString)
{
return rprContextAttachRenderLayer(context, renderLayerString);
}

    /** @brief
    *
    *  @param  context               The context to set 
    *  @param  renderLayerString   Render layer name to detach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextDetachRenderLayer(IntPtr context, string renderLayerString);
public static Status ContextDetachRenderLayer(IntPtr context, string renderLayerString)
{
return rprContextDetachRenderLayer(context, renderLayerString);
}

    /** @brief Set a LPE ( Light Path Expression ) to a framebuffer.
    *          Note that this framebuffer should also be assigned to a LPE AOV:  RPR_AOV_LPE_0 , RPR_AOV_LPE_1 ....
    *
    *
    *  @param  frame_buffer    Frame buffer object to set
    *  @param  lpe             Light Path Expression
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferSetLPE(IntPtr frame_buffer, string lpe);
public static Status FrameBufferSetLPE(IntPtr frame_buffer, string lpe)
{
return rprFrameBufferSetLPE(frame_buffer, lpe);
}

    /** @brief Set AOV Index Lookup Color
    *          change the color of  AOV rendering IDs,  like : RPR_AOV_MATERIAL_ID , RPR_AOV_OBJECT_ID, RPR_AOV_OBJECT_GROUP_ID.
    *          for example, you can render all the  shapes with ObjectGroupID=4  in the Red color inside the RPR_AOV_OBJECT_GROUP_ID AOV
    *
    *  @param  context         The context to set AOV index lookup
    *  @param  key				id
    *  @param  colorR			red channel
    *  @param  colorG			green channel
    *  @param  colorB			blue channel
    *  @param  colorA			alpha channel
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextSetAOVindexLookup(IntPtr context, int key, float colorR, float colorG, float colorB, float colorA);
public static Status ContextSetAOVindexLookup(IntPtr context, int key, float colorR, float colorG, float colorB, float colorA)
{
return rprContextSetAOVindexLookup(context, key, colorR, colorG, colorB, colorA);
}

    /** @brief Set a Cutting Plane (also called Clipping plane).
    *
    * Notes:
    *  - In order to disable the 'index' cutting plane, set (A,B,C,D) = (0,0,0,0)
    *    By default, on context creation all cutting planes are disabled.
    * 
    *  - Index can be any number. It doesn't need to define the list of plane as a contiguous list of indices.
    *
    *  - If the number of enabled planes is greater than the limit supported by the renderer,
    *    then RPR_ERROR_UNSUPPORTED is return by the function.
    *
    *  - The normal of the equation plane points toward the area that is kept.
    *
    *  - If several clipping planes are used the rendered area will be the one commonly facing all the planes.
    *
    *  - Plane equation is Ax + By + Cz + D = 0
    *
    *  @param  context			The context to set the Cutting Plane
    *  @param  index			cutting plane index ( starts from 0 )
    *  @param  a				equation plane A
    *  @param  b				equation plane B
    *  @param  c				equation plane C
    *  @param  d				equation plane D
    *  @return					RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextSetCuttingPlane(IntPtr context, int index, float a, float b, float c, float d);
public static Status ContextSetCuttingPlane(IntPtr context, int index, float a, float b, float c, float d)
{
return rprContextSetCuttingPlane(context, index, a, b, c, d);
}

    /** @brief call a batch of rprContextSetAOVindexLookup
    *
    * example:
    * rprContextSetAOVindicesLookup(ctx, 2, 3, table);
    * is equivalent to call :
    * rprContextSetAOVindexLookup(ctx, 2, table[0], table[1], table[2],  table[3]);
    * rprContextSetAOVindexLookup(ctx, 3, table[4], table[5], table[6],  table[7]);
    * rprContextSetAOVindexLookup(ctx, 4, table[8], table[9], table[10], table[11]);
    * 
    * Depending on the plugin, rprContextSetAOVindicesLookup could be faster than calling several rprContextSetAOVindexLookup.
    */
  
[DllImport(dllName)] static extern Status rprContextSetAOVindicesLookup(IntPtr context, int keyOffset, int keyCount, IntPtr colorRGBA);
public static Status ContextSetAOVindicesLookup(IntPtr context, int keyOffset, int keyCount, IntPtr colorRGBA)
{
return rprContextSetAOVindicesLookup(context, keyOffset, keyCount, colorRGBA);
}

    /**
    * API user can create its own procedural texture.
    * The API needs both a GPU code ( OpenCL string code ) and a CPU code ( callback )
    * (API function supported by Northstar only)
    *
    * example:  
    *      #define DEFINE_FUNC(FUNCNAME, FUNC) FUNC; const std::string g_str_##FUNCNAME = #FUNC;
    *      DEFINE_FUNC( RprCustomMatA ,   void RprCustomMatA(float* a, float* b, float* c, float* d, float* e, float* f, float* valueOut){   valueOut[0]=0.0;  valueOut[1]=sin(d[0]*3.14159);  valueOut[2]=0.0;  }   );
    *      rprContextSetUserTexture(context, 3, g_str_RprCustomMatA.c_str(), RprCustomMatA); // use slot 3
    *      // create material based on slot 3 :
    *      rpr_material_node matUserTex3; rprMaterialSystemCreateNode(matsys, RPR_MATERIAL_NODE_USER_TEXTURE, & matUserTex3);
    *      rprMaterialNodeSetInputUByKey(matUserTex3, RPR_MATERIAL_INPUT_OP, 3 ); // bind matUserTex3 to slot 3
    *      rprMaterialNodeSetInputNByKey(matUserTex3, RPR_MATERIAL_INPUT_4, paramInput ); // bind 'paramInput' to the 'e' argument of 'RprCustomMatA'
    *
    * Notes:
    * - If only the GPU is used, a nullptr can be given to 'cpuCode'.
    * - RPR_MATERIAL_NODE_USER_TEXTURE_0...RPR_MATERIAL_NODE_USER_TEXTURE_3   ,    RPR_CONTEXT_USER_TEXTURE_0...RPR_CONTEXT_USER_TEXTURE_3  are deprecated and should only be used with the old Tahoe plugin.
    * - When exporting the RPR scene to RPRS or GLTF, the CPU callback pointer will be lost in the imported scene.
    *
    */
  
[DllImport(dllName)] static extern Status rprContextSetUserTexture(IntPtr context, int index, string gpuCode, IntPtr cpuCode);
public static Status ContextSetUserTexture(IntPtr context, int index, string gpuCode, IntPtr cpuCode)
{
return rprContextSetUserTexture(context, index, gpuCode, cpuCode);
}

    /**
    * get the gpuCode string set by rprContextSetUserTexture.
    * (API function supported by Northstar only)
    */
  
[DllImport(dllName)] static extern Status rprContextGetUserTexture(IntPtr context, int index, long bufferSizeByte, IntPtr buffer, IntPtr size_ret);
public static Status ContextGetUserTexture(IntPtr context, int index, long bufferSizeByte, IntPtr buffer, IntPtr size_ret)
{
return rprContextGetUserTexture(context, index, bufferSizeByte, buffer, size_ret);
}

    /** @brief Set the scene
    *
    *  The scene is a collection of objects and lights
    *  along with all the data required to shade those. The scene is
    *  used by the context to render the image.
    *
    *  @param  context     The context to set the scene
    *  @param  scene       The scene to set
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextSetScene(IntPtr context, IntPtr scene);
public static Status ContextSetScene(IntPtr context, IntPtr scene)
{
return rprContextSetScene(context, scene);
}

    /** @brief Get the current scene
    *
    *  The scene is a collection of objects and lights
    *  along with all the data required to shade those. The scene is
    *  used by the context ro render the image.
    *
    *  @param  context     The context to get the scene from
    *  @param  out_scene   Pointer to scene object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextGetScene(IntPtr arg0, out IntPtr out_scene);
public static Status ContextGetScene(IntPtr arg0, out IntPtr out_scene)
{
return rprContextGetScene(arg0, out out_scene);
}

    /** @brief Set context parameter
    *
    * Parameters are used to control rendering modes, global sampling and AA settings, etc
    *
    * @param  context                        The context to set the value to
    * @param  in_input						  Param name, can be any  RPR_CONTEXT_*  value
    * @param x,y,z,w						   Parameter value
    * @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextSetParameterByKey1u(IntPtr context, ContextInfo in_input, uint x);
public static Status ContextSetParameterByKey1u(IntPtr context, ContextInfo in_input, uint x)
{
return rprContextSetParameterByKey1u(context, in_input, x);
}
[DllImport(dllName)] static extern Status rprContextSetParameterByKeyPtr(IntPtr context, ContextInfo in_input, IntPtr x);
public static Status ContextSetParameterByKeyPtr(IntPtr context, ContextInfo in_input, IntPtr x)
{
return rprContextSetParameterByKeyPtr(context, in_input, x);
}
[DllImport(dllName)] static extern Status rprContextSetParameterByKey1f(IntPtr context, ContextInfo in_input, float x);
public static Status ContextSetParameterByKey1f(IntPtr context, ContextInfo in_input, float x)
{
return rprContextSetParameterByKey1f(context, in_input, x);
}
[DllImport(dllName)] static extern Status rprContextSetParameterByKey3f(IntPtr context, ContextInfo in_input, float x, float y, float z);
public static Status ContextSetParameterByKey3f(IntPtr context, ContextInfo in_input, float x, float y, float z)
{
return rprContextSetParameterByKey3f(context, in_input, x, y, z);
}
[DllImport(dllName)] static extern Status rprContextSetParameterByKey4f(IntPtr context, ContextInfo in_input, float x, float y, float z, float w);
public static Status ContextSetParameterByKey4f(IntPtr context, ContextInfo in_input, float x, float y, float z, float w)
{
return rprContextSetParameterByKey4f(context, in_input, x, y, z, w);
}
[DllImport(dllName)] static extern Status rprContextSetParameterByKeyString(IntPtr context, ContextInfo in_input, string value);
public static Status ContextSetParameterByKeyString(IntPtr context, ContextInfo in_input, string value)
{
return rprContextSetParameterByKeyString(context, in_input, value);
}

    /** @brief This is an internal/experimental backdoor for RPR developers team.
    *
    * A classic usage of RPR doesn't require usage of this call.
    * Use this only if you understand what you are doing.
    * It's sending the name/value directly to the plugin without any process of RPR API.
    * the 'paramName' is not related with the parameters of rprContextSetParameterByKey4f.
    * 'pluginIndex' can be used if the context has more than one plugin - Not Implemented for now, set it to 0.
    */
  
[DllImport(dllName)] static extern Status rprContextSetInternalParameter4f(IntPtr context, uint pluginIndex, string paramName, float x, float y, float z, float w);
public static Status ContextSetInternalParameter4f(IntPtr context, uint pluginIndex, string paramName, float x, float y, float z, float w)
{
return rprContextSetInternalParameter4f(context, pluginIndex, paramName, x, y, z, w);
}
[DllImport(dllName)] static extern Status rprContextSetInternalParameter1u(IntPtr context, uint pluginIndex, string paramName, uint x);
public static Status ContextSetInternalParameter1u(IntPtr context, uint pluginIndex, string paramName, uint x)
{
return rprContextSetInternalParameter1u(context, pluginIndex, paramName, x);
}
[DllImport(dllName)] static extern Status rprContextSetInternalParameterBuffer(IntPtr context, uint pluginIndex, string paramName, IntPtr buffer, long bufferSizeByte);
public static Status ContextSetInternalParameterBuffer(IntPtr context, uint pluginIndex, string paramName, IntPtr buffer, long bufferSizeByte)
{
return rprContextSetInternalParameterBuffer(context, pluginIndex, paramName, buffer, bufferSizeByte);
}
[DllImport(dllName)] static extern Status rprContextGetInternalParameter4f(IntPtr context, uint pluginIndex, string paramName, IntPtr x, IntPtr y, IntPtr z, IntPtr w);
public static Status ContextGetInternalParameter4f(IntPtr context, uint pluginIndex, string paramName, IntPtr x, IntPtr y, IntPtr z, IntPtr w)
{
return rprContextGetInternalParameter4f(context, pluginIndex, paramName, x, y, z, w);
}
[DllImport(dllName)] static extern Status rprContextGetInternalParameter1u(IntPtr context, uint pluginIndex, string paramName, IntPtr x);
public static Status ContextGetInternalParameter1u(IntPtr context, uint pluginIndex, string paramName, IntPtr x)
{
return rprContextGetInternalParameter1u(context, pluginIndex, paramName, x);
}
[DllImport(dllName)] static extern Status rprContextGetInternalParameterBuffer(IntPtr context, uint pluginIndex, string paramName, long bufferSizeByte, IntPtr buffer, IntPtr size_ret);
public static Status ContextGetInternalParameterBuffer(IntPtr context, uint pluginIndex, string paramName, long bufferSizeByte, IntPtr buffer, IntPtr size_ret)
{
return rprContextGetInternalParameterBuffer(context, pluginIndex, paramName, bufferSizeByte, buffer, size_ret);
}

    /** @brief Perform evaluation and accumulation of a single sample (or number of AA samples if AA is enabled)
    *
    *  The call is blocking and the image is ready when returned. The context accumulates the samples in order
    *  to progressively refine the image and enable interactive response. So each new call to Render refines the
    *  resultin image with 1 (or num aa samples) color samples. Call rprFrameBufferClear if you want to start rendering new image
    *  instead of refining the previous one.
    *
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_INTERNAL_ERROR
    *      RPR_ERROR_MATERIAL_STACK_OVERFLOW
    *
    *  if you have the RPR_ERROR_MATERIAL_STACK_OVERFLOW error, you have created a shader graph with too many nodes.
    *  you can check the nodes limit with rprContextGetInfo(,RPR_CONTEXT_MATERIAL_STACK_SIZE,)
    *
    *  @param  context     The context object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextRender(IntPtr context);
public static Status ContextRender(IntPtr context)
{
return rprContextRender(context);
}

    /** @brief  can be called in a different thread to interrupt the rendering
	*  then, rprContextRender will return  RPR_ERROR_ABORTED instead of RPR_SUCCESS
    */
  
[DllImport(dllName)] static extern Status rprContextAbortRender(IntPtr context);
public static Status ContextAbortRender(IntPtr context)
{
return rprContextAbortRender(context);
}

    /** @brief Perform evaluation and accumulation of a single sample (or number of AA samples if AA is enabled) on the part of the image
    *
    *  The call is blocking and the image is ready when returned. The context accumulates the samples in order
    *  to progressively refine the image and enable interactive response. So each new call to Render refines the
    *  resultin image with 1 (or num aa samples) color samples. Call rprFrameBufferClear if you want to start rendering new image
    *  instead of refining the previous one. Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_INTERNAL_ERROR
    *
    *  @param  context     The context to use for the rendering
    *  @param  xmin        X coordinate of the top left corner of a tile
    *  @param  xmax        X coordinate of the bottom right corner of a tile
    *  @param  ymin        Y coordinate of the top left corner of a tile
    *  @param  ymax        Y coordinate of the bottom right corner of a tile
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextRenderTile(IntPtr context, uint xmin, uint xmax, uint ymin, uint ymax);
public static Status ContextRenderTile(IntPtr context, uint xmin, uint xmax, uint ymin, uint ymax)
{
return rprContextRenderTile(context, xmin, xmax, ymin, ymax);
}

    /** @brief Clear all video memory used by the context
    *
    *  This function should be called after all context objects have been destroyed.
    *  It guarantees that all context memory is freed and returns the context into its initial state.
    *  Will be removed later. Possible error codes are:
    *
    *      RPR_ERROR_INTERNAL_ERROR
    *
    *  @param  context     The context to wipe out
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextClearMemory(IntPtr context);
public static Status ContextClearMemory(IntPtr context)
{
return rprContextClearMemory(context);
}

    /** @brief Create an image from memory data
    *
    *  Images are used as HDRI maps or inputs for various shading system nodes.
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  context     The context to create image
    *  @param  format      Image format
    *  @param  image_desc  Image layout description
    *  @param  data        Image data in system memory, can be NULL in which case the memory is allocated
    *  @param  out_image   Pointer to image object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateImage(IntPtr context, ImageFormat format, IntPtr image_desc, IntPtr data, out IntPtr out_image);
public static Status ContextCreateImage(IntPtr context, ImageFormat format, IntPtr image_desc, IntPtr data, out IntPtr out_image)
{
return rprContextCreateImage(context, format, image_desc, data, out out_image);
}

    /** @brief Create a buffer from memory data
    *
    *  Buffers are used as arbitrary input for other nodes
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  context     The context to create image
    *  @param  buffer_desc Buffer layout description
    *  @param  data        Image data in system memory, can be NULL in which case the memory is allocated
    *  @param  out_buffer  Pointer to buffer object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateBuffer(IntPtr context, IntPtr buffer_desc, IntPtr data, out IntPtr out_buffer);
public static Status ContextCreateBuffer(IntPtr context, IntPtr buffer_desc, IntPtr data, out IntPtr out_buffer)
{
return rprContextCreateBuffer(context, buffer_desc, data, out out_buffer);
}

    /** @brief Create an image from file
    *
    *   Images are used as HDRI maps or inputs for various shading system nodes.
    *
    *  The following image formats are supported:
    *      PNG, JPG, TGA, BMP, TIFF, TX(0-mip), HDR, EXR
    *
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT
    *      RPR_ERROR_INVALID_PARAMETER
    *      RPR_ERROR_IO_ERROR
    *
    *  @param  context     The context to create image
    *  @param  path        NULL terminated path to an image file (can be relative) (for UNICODE, supports UTF-8 encoding)
    *  @param  out_image   Pointer to image object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateImageFromFile(IntPtr context, string path, out IntPtr out_image);
public static Status ContextCreateImageFromFile(IntPtr context, string path, out IntPtr out_image)
{
return rprContextCreateImageFromFile(context, path, out out_image);
}

    /** @brief  similar to rprContextCreateImageFromFile, except that the file input as a memory buffer
    *   extension must look like : ".png" , ".bmp" , ".hdr" , ".exr" , ".jpg" ....
    */
  
[DllImport(dllName)] static extern Status rprContextCreateImageFromFileMemory(IntPtr context, string extension, IntPtr data, long dataSizeByte, out IntPtr out_image);
public static Status ContextCreateImageFromFileMemory(IntPtr context, string extension, IntPtr data, long dataSizeByte, out IntPtr out_image)
{
return rprContextCreateImageFromFileMemory(context, extension, data, dataSizeByte, out out_image);
}

    /** @brief Create a scene
    *
    *  Scene serves as a container for lights and objects.
    *
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  out_scene   Pointer to scene object
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateScene(IntPtr context, out IntPtr out_scene);
public static Status ContextCreateScene(IntPtr context, out IntPtr out_scene)
{
return rprContextCreateScene(context, out out_scene);
}

    /** @brief Create an instance of an object
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
  
[DllImport(dllName)] static extern Status rprContextCreateInstance(IntPtr context, IntPtr shape, out IntPtr out_instance);
public static Status ContextCreateInstance(IntPtr context, IntPtr shape, out IntPtr out_instance)
{
return rprContextCreateInstance(context, shape, out out_instance);
}

    /** @brief Create a mesh
    *
    *  Radeon ProRender supports mixed meshes consisting of triangles and quads.
    *
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  vertices            Pointer to position data (each position is described with 3 rpr_float numbers)
    *  @param  num_vertices        Number of entries in position array
    *  @param  vertex_stride       Number of bytes between the beginnings of two successive position entries
    *  @param  normals             Pointer to normal data (each normal is described with 3 rpr_float numbers), can be NULL
    *  @param  num_normals         Number of entries in normal array
    *  @param  normal_stride       Number of bytes between the beginnings of two successive normal entries
    *  @param  texcoord            Pointer to texcoord data (each texcoord is described with 2 rpr_float numbers), can be NULL
    *  @param  num_texcoords       Number of entries in texcoord array
    *  @param  texcoord_stride     Number of bytes between the beginnings of two successive texcoord entries
    *  @param  vertex_indices      Pointer to an array of vertex indices
    *  @param  vidx_stride         Number of bytes between the beginnings of two successive vertex index entries
    *  @param  normal_indices      Pointer to an array of normal indices
    *  @param  nidx_stride         Number of bytes between the beginnings of two successive normal index entries
    *  @param  texcoord_indices    Pointer to an array of texcoord indices
    *  @param  tidx_stride         Number of bytes between the beginnings of two successive texcoord index entries
    *  @param  num_face_vertices   Pointer to an array of num_faces numbers describing number of vertices for each face (can be 3(triangle) or 4(quad))
    *  @param  num_faces           Number of faces in the mesh
    *  @param  out_mesh            Pointer to mesh object
    *  @return                     RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateMesh(IntPtr context, float[] vertices, long num_vertices, int vertex_stride, float[] normals, long num_normals, int normal_stride, float[] texcoords, long num_texcoords, int texcoord_stride, int[] vertex_indices, int vidx_stride, int[] normal_indices, int nidx_stride, int[] texcoord_indices, int tidx_stride, int[] num_face_vertices, long num_faces, out IntPtr out_mesh);
public static Status ContextCreateMesh(IntPtr context, float[] vertices, long num_vertices, int vertex_stride, float[] normals, long num_normals, int normal_stride, float[] texcoords, long num_texcoords, int texcoord_stride, int[] vertex_indices, int vidx_stride, int[] normal_indices, int nidx_stride, int[] texcoord_indices, int tidx_stride, int[] num_face_vertices, long num_faces, out IntPtr out_mesh)
{
return rprContextCreateMesh(context, vertices, num_vertices, vertex_stride, normals, num_normals, normal_stride, texcoords, num_texcoords, texcoord_stride, vertex_indices, vidx_stride, normal_indices, nidx_stride, texcoord_indices, tidx_stride, num_face_vertices, num_faces, out out_mesh);
}

    /*  @brief Create a mesh
    *
    *  @return                     RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateMeshEx(IntPtr context, float[] vertices, long num_vertices, int vertex_stride, float[] normals, long num_normals, int normal_stride, int[] perVertexFlag, long num_perVertexFlags, int perVertexFlag_stride, int numberOfTexCoordLayers, IntPtr[] texcoords, IntPtr num_texcoords, IntPtr texcoord_stride, int[] vertex_indices, int vidx_stride, int[] normal_indices, int nidx_stride, IntPtr[] texcoord_indices, IntPtr tidx_stride, int[] num_face_vertices, long num_faces, out IntPtr out_mesh);
public static Status ContextCreateMeshEx(IntPtr context, float[] vertices, long num_vertices, int vertex_stride, float[] normals, long num_normals, int normal_stride, int[] perVertexFlag, long num_perVertexFlags, int perVertexFlag_stride, int numberOfTexCoordLayers, IntPtr[] texcoords, IntPtr num_texcoords, IntPtr texcoord_stride, int[] vertex_indices, int vidx_stride, int[] normal_indices, int nidx_stride, IntPtr[] texcoord_indices, IntPtr tidx_stride, int[] num_face_vertices, long num_faces, out IntPtr out_mesh)
{
return rprContextCreateMeshEx(context, vertices, num_vertices, vertex_stride, normals, num_normals, normal_stride, perVertexFlag, num_perVertexFlags, perVertexFlag_stride, numberOfTexCoordLayers, texcoords, num_texcoords, texcoord_stride, vertex_indices, vidx_stride, normal_indices, nidx_stride, texcoord_indices, tidx_stride, num_face_vertices, num_faces, out out_mesh);
}

    /*  @brief Create a mesh
    *
    *  @return                     RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateMeshEx2(IntPtr context, IntPtr vertices, long num_vertices, int vertex_stride, IntPtr normals, long num_normals, int normal_stride, IntPtr perVertexFlag, long num_perVertexFlags, int perVertexFlag_stride, int numberOfTexCoordLayers, IntPtr texcoords, IntPtr num_texcoords, IntPtr texcoord_stride, IntPtr vertex_indices, int vidx_stride, IntPtr normal_indices, int nidx_stride, IntPtr texcoord_indices, IntPtr tidx_stride, IntPtr num_face_vertices, long num_faces, IntPtr mesh_properties, out IntPtr out_mesh);
public static Status ContextCreateMeshEx2(IntPtr context, IntPtr vertices, long num_vertices, int vertex_stride, IntPtr normals, long num_normals, int normal_stride, IntPtr perVertexFlag, long num_perVertexFlags, int perVertexFlag_stride, int numberOfTexCoordLayers, IntPtr texcoords, IntPtr num_texcoords, IntPtr texcoord_stride, IntPtr vertex_indices, int vidx_stride, IntPtr normal_indices, int nidx_stride, IntPtr texcoord_indices, IntPtr tidx_stride, IntPtr num_face_vertices, long num_faces, IntPtr mesh_properties, out IntPtr out_mesh)
{
return rprContextCreateMeshEx2(context, vertices, num_vertices, vertex_stride, normals, num_normals, normal_stride, perVertexFlag, num_perVertexFlags, perVertexFlag_stride, numberOfTexCoordLayers, texcoords, num_texcoords, texcoord_stride, vertex_indices, vidx_stride, normal_indices, nidx_stride, texcoord_indices, tidx_stride, num_face_vertices, num_faces, mesh_properties, out out_mesh);
}

    /** @brief Create a camera
    *
    *  There are several camera types supported by a single rpr_camera type.
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    *  @param  context The context to create a camera for
    *  @param  out_camera Pointer to camera object
    *  @return RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateCamera(IntPtr context, out IntPtr out_camera);
public static Status ContextCreateCamera(IntPtr context, out IntPtr out_camera)
{
return rprContextCreateCamera(context, out out_camera);
}

    /** @brief Create framebuffer object
    *
    *  Framebuffer is used to store final rendering result.
    *
    *  Possible error codes are:
    *
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    *  @param  context  The context to create framebuffer
    *  @param  format   Framebuffer format
    *  @param  fb_desc  Framebuffer layout description
    *  @param  status   Pointer to framebuffer object
    *  @return          RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprContextCreateFrameBuffer(IntPtr context, FramebufferFormat format, IntPtr fb_desc, out IntPtr out_fb);
public static Status ContextCreateFrameBuffer(IntPtr context, FramebufferFormat format, IntPtr fb_desc, out IntPtr out_fb)
{
return rprContextCreateFrameBuffer(context, format, fb_desc, out out_fb);
}

      /** @brief Loads extension function from context
      */
  
[DllImport(dllName)] static extern Status rprContextGetFunctionPtr(IntPtr context, string function_name, out IntPtr out_function_ptr);
public static Status ContextGetFunctionPtr(IntPtr context, string function_name, out IntPtr out_function_ptr)
{
return rprContextGetFunctionPtr(context, function_name, out out_function_ptr);
}
/* rpr_camera */

    /** @brief Query information about a camera
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data.
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  camera      The camera to query
    *  @param  camera_info The type of info to query
    *  @param  size        The size of the buffer pointed by data
    *  @param  data        The buffer to store queried info
    *  @param  size_ret    Returns the size in bytes of the data being queried
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraGetInfo(IntPtr camera, CameraInfo camera_info, long size, IntPtr data, out long size_ret);
public static Status CameraGetInfo(IntPtr camera, CameraInfo camera_info, long size, IntPtr data, out long size_ret)
{
return rprCameraGetInfo(camera, camera_info, size, data, out size_ret);
}

    /** @brief Set camera focal length.
    *
    *  @param  camera  The camera to set focal length
    *  @param  flength Focal length in millimeters, default is 35mm
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetFocalLength(IntPtr camera, float flength);
public static Status CameraSetFocalLength(IntPtr camera, float flength)
{
return rprCameraSetFocalLength(camera, flength);
}

    /* Number of motion matrices (set with rprCameraSetMotionTransform) to use.
    *  Set  transformCount=0  if you don't use Motion.
    *  For the moment, if you use motion in Northstar, only transformCount=1 is supported.
    *  example: to create a motion from matA to matB:
    *      rprCameraSetTransform(camera, false, matA) // matrix at time=0
    *      rprCameraSetMotionTransform(camera, false, matB, 1) // matrix at time=1
    *      rprCameraSetMotionTransformCount(camera,1) // use 1 motion matrix
    */
  
[DllImport(dllName)] static extern Status rprCameraSetMotionTransformCount(IntPtr camera, uint transformCount);
public static Status CameraSetMotionTransformCount(IntPtr camera, uint transformCount)
{
return rprCameraSetMotionTransformCount(camera, transformCount);
}

    /* For Motion effect, set the transform of camera at different time index.
    * 'transform' is an array of 16 rpr_float values (row-major form).
    *  timeIndex=1 is camera position at camera exposure = 1.0
    *  For the moment, in Nortstar plugin only timeIndex=1 is implemented
    *  You also have to call  rprCameraSetMotionTransformCount, to define the number of indices to use.
    */
  
[DllImport(dllName)] static extern Status rprCameraSetMotionTransform(IntPtr camera, bool transpose, IntPtr transform, uint timeIndex);
public static Status CameraSetMotionTransform(IntPtr camera, bool transpose, IntPtr transform, uint timeIndex)
{
return rprCameraSetMotionTransform(camera, transpose, transform, timeIndex);
}

    /** @brief Set camera focus distance
    *
    *  @param  camera  The camera to set focus distance
    *  @param  fdist   Focus distance in meters, default is 1m
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetFocusDistance(IntPtr camera, float fdist);
public static Status CameraSetFocusDistance(IntPtr camera, float fdist)
{
return rprCameraSetFocusDistance(camera, fdist);
}

    /** @brief Set world transform for the camera
    *
    *  @param  camera      The camera to set transform for
    *  @param  transpose   Determines whether the basis vectors are in columns(false) or in rows(true) of the matrix
    *  @param  transform   Array of 16 rpr_float values (row-major form)
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetTransform(IntPtr camera, bool transpose, float[] transform);
public static Status CameraSetTransform(IntPtr camera, bool transpose, float[] transform)
{
return rprCameraSetTransform(camera, transpose, transform);
}

    /** @brief Set sensor size for the camera
    *
    *  Default sensor size is the one corresponding to full frame 36x24mm sensor
    *
    *  @param  camera  The camera to set transform for
    *  @param  width   Sensor width in millimeters
    *  @param  height  Sensor height in millimeters
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetSensorSize(IntPtr camera, float width, float height);
public static Status CameraSetSensorSize(IntPtr camera, float width, float height)
{
return rprCameraSetSensorSize(camera, width, height);
}

    /** @brief Set camera transform in lookat form
    *
    *  @param  camera  The camera to set transform for
    *  @param  posx    X component of the position
    *  @param  posy    Y component of the position
    *  @param  posz    Z component of the position
    *  @param  atx     X component of the center point
    *  @param  aty     Y component of the center point
    *  @param  atz     Z component of the center point
    *  @param  upx     X component of the up vector
    *  @param  upy     Y component of the up vector
    *  @param  upz     Z component of the up vector
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraLookAt(IntPtr camera, float posx, float posy, float posz, float atx, float aty, float atz, float upx, float upy, float upz);
public static Status CameraLookAt(IntPtr camera, float posx, float posy, float posz, float atx, float aty, float atz, float upx, float upy, float upz)
{
return rprCameraLookAt(camera, posx, posy, posz, atx, aty, atz, upx, upy, upz);
}

    /** @brief Set f-stop for the camera
    *
    *  @param  camera  The camera to set f-stop for
    *  @param  fstop   f-stop value in mm^-1, default is FLT_MAX
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetFStop(IntPtr camera, float fstop);
public static Status CameraSetFStop(IntPtr camera, float fstop)
{
return rprCameraSetFStop(camera, fstop);
}

    /** @brief Set the number of aperture blades
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  camera      The camera to set aperture blades for
    *  @param  num_blades  Number of aperture blades 4 to 32
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetApertureBlades(IntPtr camera, uint num_blades);
public static Status CameraSetApertureBlades(IntPtr camera, uint num_blades)
{
return rprCameraSetApertureBlades(camera, num_blades);
}

    /** @brief Set the exposure of a camera
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  camera    The camera to set aperture blades for
    *  @param  exposure  Represents a time length in the same time scale than rprShapeSetMotionTransform,rprCameraSetMotionTransform...
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetExposure(IntPtr camera, float exposure);
public static Status CameraSetExposure(IntPtr camera, float exposure)
{
return rprCameraSetExposure(camera, exposure);
}

    /** @brief Set camera mode
    *
    *  Camera modes include:
    *      RPR_CAMERA_MODE_PERSPECTIVE
    *      RPR_CAMERA_MODE_ORTHOGRAPHIC
    *      RPR_CAMERA_MODE_LATITUDE_LONGITUDE_360
    *      RPR_CAMERA_MODE_LATITUDE_LONGITUDE_STEREO
    *      RPR_CAMERA_MODE_CUBEMAP
    *      RPR_CAMERA_MODE_CUBEMAP_STEREO
    *      RPR_CAMERA_MODE_FISHEYE
    *
    *  @param  camera  The camera to set mode for
    *  @param  mode    Camera mode, default is RPR_CAMERA_MODE_PERSPECTIVE
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetMode(IntPtr camera, CameraMode mode);
public static Status CameraSetMode(IntPtr camera, CameraMode mode)
{
return rprCameraSetMode(camera, mode);
}

    /** @brief Set orthographic view volume width
    *
    *  @param  camera  The camera to set volume width for
    *  @param  width   View volume width in meters, default is 1 meter
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetOrthoWidth(IntPtr camera, float width);
public static Status CameraSetOrthoWidth(IntPtr camera, float width)
{
return rprCameraSetOrthoWidth(camera, width);
}
[DllImport(dllName)] static extern Status rprCameraSetFocalTilt(IntPtr camera, float tilt);
public static Status CameraSetFocalTilt(IntPtr camera, float tilt)
{
return rprCameraSetFocalTilt(camera, tilt);
}
[DllImport(dllName)] static extern Status rprCameraSetIPD(IntPtr camera, float ipd);
public static Status CameraSetIPD(IntPtr camera, float ipd)
{
return rprCameraSetIPD(camera, ipd);
}
[DllImport(dllName)] static extern Status rprCameraSetLensShift(IntPtr camera, float shiftx, float shifty);
public static Status CameraSetLensShift(IntPtr camera, float shiftx, float shifty)
{
return rprCameraSetLensShift(camera, shiftx, shifty);
}
[DllImport(dllName)] static extern Status rprCameraSetTiltCorrection(IntPtr camera, float tiltX, float tiltY);
public static Status CameraSetTiltCorrection(IntPtr camera, float tiltX, float tiltY)
{
return rprCameraSetTiltCorrection(camera, tiltX, tiltY);
}

    /** @brief Set orthographic view volume height
    *
    *  @param  camera  The camera to set volume height for
    *  @param  width   View volume height in meters, default is 1 meter
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetOrthoHeight(IntPtr camera, float height);
public static Status CameraSetOrthoHeight(IntPtr camera, float height)
{
return rprCameraSetOrthoHeight(camera, height);
}

    /** @brief Set near plane of a camear
    *
    *  @param  camera  The camera to set near plane for
    *  @param  near   Near plane distance in meters, default is 0.01f
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetNearPlane(IntPtr camera, float near);
public static Status CameraSetNearPlane(IntPtr camera, float near)
{
return rprCameraSetNearPlane(camera, near);
}

    /** @brief Set the post scale of camera ( 2D camera zoom )
    *
    *  @param  camera  The camera to set
    *  @param  scale   post scale value.
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetPostScale(IntPtr camera, float scale);
public static Status CameraSetPostScale(IntPtr camera, float scale)
{
return rprCameraSetPostScale(camera, scale);
}

    /** @brief Set far plane of a camear
    *
    *  @param  camera  The camera to set far plane for
    *  @param  far   Far plane distance in meters, default is 100000000.f
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCameraSetFarPlane(IntPtr camera, float far);
public static Status CameraSetFarPlane(IntPtr camera, float far)
{
return rprCameraSetFarPlane(camera, far);
}
/* rpr_image*/

    /** @brief Query information about an image
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  image       An image object to query
    *  @param  image_info  The type of info to query
    *  @param  size        The size of the buffer pointed by data
    *  @param  data        The buffer to store queried info
    *  @param  size_ret    Returns the size in bytes of the data being queried
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprImageGetInfo(IntPtr image, ImageInfo image_info, long size, IntPtr data, out long size_ret);
public static Status ImageGetInfo(IntPtr image, ImageInfo image_info, long size, IntPtr data, out long size_ret)
{
return rprImageGetInfo(image, image_info, size, data, out size_ret);
}

    /** @brief
    * 
    * this is DEPRECATED in the Northstar plugin.
    * In this plugin, the wrapping is done inside the RPR_MATERIAL_NODE_IMAGE_TEXTURE owning the image, 
    * example: rprMaterialNodeSetInputUByKey(materialNodeTexture, RPR_MATERIAL_INPUT_WRAP_U, RPR_IMAGE_WRAP_TYPE_REPEAT);
    *
    *  @param  image       The image to set wrap for
    *  @param  type
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprImageSetWrap(IntPtr image, ImageWrapType type);
public static Status ImageSetWrap(IntPtr image, ImageWrapType type)
{
return rprImageSetWrap(image, type);
}

    /** @brief
    * 
    * ( Northstar-only feature )
    * By default, images are compressed by the Northstar renderer.
    * Setting 'compressionEnabled'=0 will disable the compression for the images.
    * For better performance, it's advised to only disable it for textures that need it.
    *
    */
  
[DllImport(dllName)] static extern Status rprImageSetInternalCompression(IntPtr image, uint compressionEnabled);
public static Status ImageSetInternalCompression(IntPtr image, uint compressionEnabled)
{
return rprImageSetInternalCompression(image, compressionEnabled);
}

    /** @brief Set the OCIO Color Space
    *
    */
  
[DllImport(dllName)] static extern Status rprImageSetOcioColorspace(IntPtr image, string ocioColorspace);
public static Status ImageSetOcioColorspace(IntPtr image, string ocioColorspace)
{
return rprImageSetOcioColorspace(image, ocioColorspace);
}

    /** @brief  Set a tile to an UDIM image.
    *
    *  @param  imageUdimRoot   must be an UDIM image  ( created with no data:  rprContextCreateImage(context, {0,RPR_COMPONENT_TYPE_UINT8}, nullptr, nullptr, ); )
    *  @param  tileIndex       a valid UDIM index: 1001 , 1002, 1003 ... 1011, 1012, 1013 ... etc ...
    *  @param  imageTile       a valid classic rpr_image
    */
  
[DllImport(dllName)] static extern Status rprImageSetUDIM(IntPtr imageUdimRoot, uint tileIndex, IntPtr imageTile);
public static Status ImageSetUDIM(IntPtr imageUdimRoot, uint tileIndex, IntPtr imageTile)
{
return rprImageSetUDIM(imageUdimRoot, tileIndex, imageTile);
}

    /** @brief
    *
    *
    *  @param  image       The image to set filter for
    *  @param  type
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprImageSetFilter(IntPtr image, ImageFilterType type);
public static Status ImageSetFilter(IntPtr image, ImageFilterType type)
{
return rprImageSetFilter(image, type);
}
/** @brief
*
*  @param  image       The image to set gamma for
*  @param  type
*  @return             RPR_SUCCESS in case of success, error code otherwise
*/
[DllImport(dllName)] static extern Status rprImageSetGamma(IntPtr image, float type);
public static Status ImageSetGamma(IntPtr image, float type)
{
return rprImageSetGamma(image, type);
}
/** @brief
*
*  @param  image       The image to set mipmap for
*  @param  enabled     true (enable) or false (disable)
*  @return             RPR_SUCCESS in case of success, error code otherwise
*/
[DllImport(dllName)] static extern Status rprImageSetMipmapEnabled(IntPtr image, bool enabled);
public static Status ImageSetMipmapEnabled(IntPtr image, bool enabled)
{
return rprImageSetMipmapEnabled(image, enabled);
}
/* rpr_shape */

    /** @brief Set shape world transform
    *
    *
    *  @param  shape       The shape to set transform for
    *  @param  transpose   Determines whether the basis vectors are in columns(false) or in rows(true) of the matrix
    *  @param  transform   Array of 16 rpr_float values (row-major form)
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetTransform(IntPtr shape, bool transpose, float[] transform);
public static Status ShapeSetTransform(IntPtr shape, bool transpose, float[] transform)
{
return rprShapeSetTransform(shape, transpose, transform);
}

    /** @brief assign custom float value to some vertices of the mesh
    *
    * example : 
    *   // indicesSet and values must have the same size
    *   rpr_int indicesSet[] = {4,0,1,2,3};
    *   rpr_float values[] = { 0.8 , 0.1 , 0.0 , 1.0 , 1.0 };
    *   rprShapeSetVertexValue(meshC, 0 , indicesSet , values , sizeof(indicesSet)/sizeof(indicesSet[0]) );
    *   
    *  setIndex can be between  0 and 3  :  we can assign up to 4 floats for each vertex.
    *
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetVertexValue(IntPtr in_shape, int setIndex, IntPtr indices, IntPtr values, int indicesCount);
public static Status ShapeSetVertexValue(IntPtr in_shape, int setIndex, IntPtr indices, IntPtr values, int indicesCount)
{
return rprShapeSetVertexValue(in_shape, setIndex, indices, values, indicesCount);
}

    /* @brief set primvars data for a specific 'key'
    * 
    * A shape can have several primvars data. Each primvar of the shape is identified with a 'key'
    * 'data' is a list of float
    * 'floatCount' is a number of float in the 'data' buffer
    * 'componentCount' specifies the number of float(s) per component.  For example if you want to attach an RGB color to vertices, you need 'componentCount'=3  and  'floatCount' = 3 * 'number of vertices'
    * 'interop' defines how the data is interpolated.
    * 
    * @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetPrimvar(IntPtr in_shape, uint key, IntPtr data, uint floatCount, uint componentCount, PrimvarInterpolationType interop);
public static Status ShapeSetPrimvar(IntPtr in_shape, uint key, IntPtr data, uint floatCount, uint componentCount, PrimvarInterpolationType interop)
{
return rprShapeSetPrimvar(in_shape, key, data, floatCount, componentCount, interop);
}

    /** @brief Set shape subdivision
    *
    *
    *  @param  shape       The shape to set subdivision for
    *  @param  factor	   Number of subdivision steps to do
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetSubdivisionFactor(IntPtr shape, uint factor);
public static Status ShapeSetSubdivisionFactor(IntPtr shape, uint factor)
{
return rprShapeSetSubdivisionFactor(shape, factor);
}

    /** @brief Enable or Disable the auto ratio cap for subdivision
    *
    * autoRatioCap is a value from 0.0 to 1.0.
    * autoRatioCap=1.0 means very large polygons, less tessellation. as it goes to 0.0, it does more tessellation.
    * This value is ratio of the largest edge in the screen.
    * Example: If you want to make an edge 10 pixels on 1080p, you need to set 10/1080.
    *
    *  @param  shape           The shape to set
    *  @param  autoRatioCap    0.0 to 1.0
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetSubdivisionAutoRatioCap(IntPtr shape, float autoRatioCap);
public static Status ShapeSetSubdivisionAutoRatioCap(IntPtr shape, float autoRatioCap)
{
return rprShapeSetSubdivisionAutoRatioCap(shape, autoRatioCap);
}

    /** @brief
    *
    *
    *  @param  shape       The shape to set subdivision for
    *  @param  factor
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetSubdivisionCreaseWeight(IntPtr shape, float factor);
public static Status ShapeSetSubdivisionCreaseWeight(IntPtr shape, float factor)
{
return rprShapeSetSubdivisionCreaseWeight(shape, factor);
}

    /** @brief
    *
    *  @param  shape               The shape to set 
    *  @param  renderLayerString   Render layer name to attach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeAttachRenderLayer(IntPtr shape, string renderLayerString);
public static Status ShapeAttachRenderLayer(IntPtr shape, string renderLayerString)
{
return rprShapeAttachRenderLayer(shape, renderLayerString);
}

    /** @brief
    *
    *  @param  shape               The shape to set 
    *  @param  renderLayerString   Render layer name to detach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeDetachRenderLayer(IntPtr shape, string renderLayerString);
public static Status ShapeDetachRenderLayer(IntPtr shape, string renderLayerString)
{
return rprShapeDetachRenderLayer(shape, renderLayerString);
}

    /** @brief
    *
    *  @param  light               The light to set 
    *  @param  renderLayerString   Render layer name to attach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightAttachRenderLayer(IntPtr light, string renderLayerString);
public static Status LightAttachRenderLayer(IntPtr light, string renderLayerString)
{
return rprLightAttachRenderLayer(light, renderLayerString);
}

    /** @brief
    *
    *  @param  light               The light to set 
    *  @param  renderLayerString   Render layer name to detach
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightDetachRenderLayer(IntPtr light, string renderLayerString);
public static Status LightDetachRenderLayer(IntPtr light, string renderLayerString)
{
return rprLightDetachRenderLayer(light, renderLayerString);
}

    /** @brief
    *
    *
    *  @param  shape       The shape to set subdivision for
    *  @param  type
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetSubdivisionBoundaryInterop(IntPtr shape, SubdivBoundaryInteropType type);
public static Status ShapeSetSubdivisionBoundaryInterop(IntPtr shape, SubdivBoundaryInteropType type)
{
return rprShapeSetSubdivisionBoundaryInterop(shape, type);
}

    /** @brief Call this function to automatically set the Subdivision Factor depending on the camera position, frame buffer size.
    *          You can retrieve the internally computed factor with rprShapeGetInfo(...,RPR_SHAPE_SUBDIVISION_FACTOR,...)
    *          You have to call this function each time you want to re-adapt the Subdivision Factor :
    *          internally the factor will NOT be automatically re-computed when camera/shape/framebuffer changes.
    *
    *  @param  shape       The shape to set subdivision for
    *  @param  framebuffer frame buffer used for factor adaptation
    *  @param  camera      camera used for factor adaptation
    *  @param  factor      factor to regulate the intensity of adaptation
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeAutoAdaptSubdivisionFactor(IntPtr shape, IntPtr framebuffer, IntPtr camera, int factor);
public static Status ShapeAutoAdaptSubdivisionFactor(IntPtr shape, IntPtr framebuffer, IntPtr camera, int factor)
{
return rprShapeAutoAdaptSubdivisionFactor(shape, framebuffer, camera, factor);
}

    /** @brief Set displacement scale
    *
    *
    *  @param  shape       The shape to set subdivision for
    *  @param  scale	   The amount of displacement applied
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetDisplacementScale(IntPtr shape, float minscale, float maxscale);
public static Status ShapeSetDisplacementScale(IntPtr shape, float minscale, float maxscale)
{
return rprShapeSetDisplacementScale(shape, minscale, maxscale);
}

    /** @brief Set object group ID (mainly for debugging).
    *
    *
    *  @param  shape          The shape to set
    *  @param  objectGroupID  The ID
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetObjectGroupID(IntPtr shape, uint objectGroupID);
public static Status ShapeSetObjectGroupID(IntPtr shape, uint objectGroupID)
{
return rprShapeSetObjectGroupID(shape, objectGroupID);
}

    /** @brief Set object ID (mainly for debugging).
    *
    *
    *  @param  shape          The shape to set
    *  @param  objectID       The ID
    *  @return                RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetObjectID(IntPtr shape, uint objectID);
public static Status ShapeSetObjectID(IntPtr shape, uint objectID)
{
return rprShapeSetObjectID(shape, objectID);
}

    /** @brief Set light group ID when shape has an emissive material (mainly for debugging).
    *
    *  @param  shape          The shape to set
    *  @param  lightGroupID  The ID
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetLightGroupID(IntPtr shape, uint lightGroupID);
public static Status ShapeSetLightGroupID(IntPtr shape, uint lightGroupID)
{
return rprShapeSetLightGroupID(shape, lightGroupID);
}

    /** @brief Set object rendering layer mask
    *          then, use   rprContextSetParameter1u(context,"renderLayerMask",mask)
    *          in order to render only a group of shape
    *
    *  @param  shape       The shape to set
    *  @param  layerMask   The render mask
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    *
    * WARNING: this function is deprecated and will be removed in the future, 
    *          use   rprShapeAttachRenderLayer/rprShapeDetachRenderLayer   and   rprContextAttachRenderLayer/rprContextDetachRenderLayer   instead
    *
    */
  
[DllImport(dllName)] static extern Status rprShapeSetLayerMask(IntPtr shape, uint layerMask);
public static Status ShapeSetLayerMask(IntPtr shape, uint layerMask)
{
return rprShapeSetLayerMask(shape, layerMask);
}

    /** @brief Set displacement texture
    *
    *
    *  @param  shape         The shape to set subdivision for
    *  @param  materialNode  Displacement texture , as material.
    *  @return               RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetDisplacementMaterial(IntPtr shape, IntPtr materialNode);
public static Status ShapeSetDisplacementMaterial(IntPtr shape, IntPtr materialNode)
{
return rprShapeSetDisplacementMaterial(shape, materialNode);
}

    /** @brief Set shape material
    *
    */
  
[DllImport(dllName)] static extern Status rprShapeSetMaterial(IntPtr shape, IntPtr node);
public static Status ShapeSetMaterial(IntPtr shape, IntPtr node)
{
return rprShapeSetMaterial(shape, node);
}

    /** @brief Set shape materials for specific faces
    *
    *  @param  shape	The shape to set the material for
    *  @param  node 	The material to set
    *  @param  face_indices
    *  @return		RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetMaterialFaces(IntPtr shape, IntPtr node, IntPtr face_indices, long num_faces);
public static Status ShapeSetMaterialFaces(IntPtr shape, IntPtr node, IntPtr face_indices, long num_faces)
{
return rprShapeSetMaterialFaces(shape, node, face_indices, num_faces);
}




    /** @brief Set shape volume material
    *
    */
  
[DllImport(dllName)] static extern Status rprShapeSetVolumeMaterial(IntPtr shape, IntPtr node);
public static Status ShapeSetVolumeMaterial(IntPtr shape, IntPtr node)
{
return rprShapeSetVolumeMaterial(shape, node);
}

    /* Number of motion matrices (set with rprShapeSetMotionTransform) to use.
    *  Set  transformCount=0  if you don't use Motion.
    *  For the moment, if you use motion in Northstar, only transformCount=1 is supported.
    *  example: to create a motion from matA to matB:
    *      rprShapeSetTransform(shape, false, matA) // matrix at time=0
    *      rprShapeSetMotionTransform(shape, false, matB, 1) // matrix at time=1
    *      rprShapeSetMotionTransformCount(shape,1) // use 1 motion matrix
    */
  
[DllImport(dllName)] static extern Status rprShapeSetMotionTransformCount(IntPtr shape, uint transformCount);
public static Status ShapeSetMotionTransformCount(IntPtr shape, uint transformCount)
{
return rprShapeSetMotionTransformCount(shape, transformCount);
}

    /* For Motion effect, set the transform of shape at different time index.
    * 'transform' is an array of 16 rpr_float values (row-major form).
    *  timeIndex=1 is shape position at camera exposure = 1.0
    *  For the moment, in Nortstar plugin only timeIndex=1 is implemented
    *  You also have to call  rprShapeSetMotionTransformCount, to define the number of indices to use.
    */
  
[DllImport(dllName)] static extern Status rprShapeSetMotionTransform(IntPtr shape, bool transpose, IntPtr transform, uint timeIndex);
public static Status ShapeSetMotionTransform(IntPtr shape, bool transpose, IntPtr transform, uint timeIndex)
{
return rprShapeSetMotionTransform(shape, transpose, transform, timeIndex);
}

    /** @brief Set visibility flag
    *
    *  @param  shape           The shape to set visibility for
    *  @param  visibilityFlag   . one of the visibility flags :
    *                             RPR_SHAPE_VISIBILITY_PRIMARY_ONLY_FLAG
    *                             RPR_SHAPE_VISIBILITY_SHADOW
    *                             RPR_SHAPE_VISIBILITY_REFLECTION
    *                             RPR_SHAPE_VISIBILITY_REFRACTION
    *                             RPR_SHAPE_VISIBILITY_TRANSPARENT
    *                             RPR_SHAPE_VISIBILITY_DIFFUSE
    *                             RPR_SHAPE_VISIBILITY_GLOSSY_REFLECTION
    *                             RPR_SHAPE_VISIBILITY_GLOSSY_REFRACTION
    *                             RPR_SHAPE_VISIBILITY_LIGHT
    *                             RPR_SHAPE_VISIBILITY_RECEIVE_SHADOW
    *  @param  visible          set the flag to TRUE or FALSE
    *  @return                  RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetVisibilityFlag(IntPtr shape, ShapeInfo visibilityFlag, bool visible);
public static Status ShapeSetVisibilityFlag(IntPtr shape, ShapeInfo visibilityFlag, bool visible)
{
return rprShapeSetVisibilityFlag(shape, visibilityFlag, visible);
}

    /** @brief Set visibility flag
    *
    *  @param  curve           The curve to set visibility for
    *  @param  visibilityFlag   . one of the visibility flags :
    *                             RPR_CURVE_VISIBILITY_PRIMARY_ONLY_FLAG
    *                             RPR_CURVE_VISIBILITY_SHADOW
    *                             RPR_CURVE_VISIBILITY_REFLECTION
    *                             RPR_CURVE_VISIBILITY_REFRACTION
    *                             RPR_CURVE_VISIBILITY_TRANSPARENT
    *                             RPR_CURVE_VISIBILITY_DIFFUSE
    *                             RPR_CURVE_VISIBILITY_GLOSSY_REFLECTION
    *                             RPR_CURVE_VISIBILITY_GLOSSY_REFRACTION
    *                             RPR_CURVE_VISIBILITY_LIGHT
    *                             RPR_CURVE_VISIBILITY_RECEIVE_SHADOW
    *  @param  visible          set the flag to TRUE or FALSE
    *  @return                  RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCurveSetVisibilityFlag(IntPtr curve, Curve visibilityFlag, bool visible);
public static Status CurveSetVisibilityFlag(IntPtr curve, Curve visibilityFlag, bool visible)
{
return rprCurveSetVisibilityFlag(curve, visibilityFlag, visible);
}

    /** @brief Set visibility flag
    *
    * This function sets all RPR_SHAPE_VISIBILITY_* flags to the 'visible' argument value
    * Calling rprShapeSetVisibilityFlag(xxx,visible); on each flags would lead to the same result.
    *
    *  @param  shape       The shape to set visibility for
    *  @param  visible     Determines if the shape is visible or not
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetVisibility(IntPtr shape, bool visible);
public static Status ShapeSetVisibility(IntPtr shape, bool visible)
{
return rprShapeSetVisibility(shape, visible);
}

    /** @brief Set visibility flag for Light
    *
    *  @param  light           The light to set visibility for
    *  @param  visibilityFlag     one of the visibility flags :
    *                            - RPR_LIGHT_VISIBILITY_LIGHT
    *  @param  visible          set the flag to TRUE or FALSE
    *  @return                  RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightSetVisibilityFlag(IntPtr light, Light visibilityFlag, bool visible);
public static Status LightSetVisibilityFlag(IntPtr light, Light visibilityFlag, bool visible)
{
return rprLightSetVisibilityFlag(light, visibilityFlag, visible);
}

    /** @brief Set visibility flag
    *
    * This function sets all RPR_CURVE_VISIBILITY_* flags to the 'visible' argument value
    * Calling rprCurveSetVisibilityFlag(xxx,visible); on each flags would lead to the same result.
    *
    *  @param  curve       The curve to set visibility for
    *  @param  visible     Determines if the curve is visible or not
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCurveSetVisibility(IntPtr curve, bool visible);
public static Status CurveSetVisibility(IntPtr curve, bool visible)
{
return rprCurveSetVisibility(curve, visible);
}

    /** @brief Set visibility flag for specular refleacted\refracted rays
    *
    * This function sets both RPR_SHAPE_VISIBILITY_REFLECTION and RPR_SHAPE_VISIBILITY_REFRACTION flags to the 'visible' argument value
    * Calling rprShapeSetVisibilityFlag(xxx,visible); on those 2 flags would lead to the same result.
    *
    *  @param  shape       The shape to set visibility for
    *  @param  visible     Determines if the shape is visible or not
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetVisibilityInSpecular(IntPtr shape, bool visible);
public static Status ShapeSetVisibilityInSpecular(IntPtr shape, bool visible)
{
return rprShapeSetVisibilityInSpecular(shape, visible);
}

    /** @brief Set shadow catcher flag
    *
    *  @param  shape         The shape to set shadow catcher flag for
    *  @param  shadowCatcher Determines if the shape behaves as shadow catcher
    *  @return               RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetShadowCatcher(IntPtr shape, bool shadowCatcher);
public static Status ShapeSetShadowCatcher(IntPtr shape, bool shadowCatcher)
{
return rprShapeSetShadowCatcher(shape, shadowCatcher);
}

    /** @brief Set shadow color
    *
    *  @param  shape         The shape to set shadow color for
    *  @param  r             Red component of the color
    *  @param  g             Green component of the color
    *  @param  b             Blue component of the color
    *  @return               RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetShadowColor(IntPtr shape, float r, float g, float b);
public static Status ShapeSetShadowColor(IntPtr shape, float r, float g, float b)
{
return rprShapeSetShadowColor(shape, r, g, b);
}

    /** @brief Set Reflection catcher flag
    *
    *  @param  shape             The shape to set Reflection catcher flag for
    *  @param  reflectionCatcher Determines if the shape behaves as Reflection catcher
    *  @return                   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetReflectionCatcher(IntPtr shape, bool reflectionCatcher);
public static Status ShapeSetReflectionCatcher(IntPtr shape, bool reflectionCatcher)
{
return rprShapeSetReflectionCatcher(shape, reflectionCatcher);
}

    /** @brief Set 1 if ignore shape in the Contour rendering flag.
    *   ( This flag is used only if Contour is enabled )
    *
    *  @param  shape             The shape to set
    *  @param  ignoreInContour   0 or 1.
    *  @return                   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetContourIgnore(IntPtr shape, bool ignoreInContour);
public static Status ShapeSetContourIgnore(IntPtr shape, bool ignoreInContour)
{
return rprShapeSetContourIgnore(shape, ignoreInContour);
}

    /** @brief Set 1 if the shape should be treated as an environment light (finite sphere environment light).
    *
    *  @param  shape             The shape to set
    *  @param  envLight   0 or 1.
    *  @return                   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeSetEnvironmentLight(IntPtr shape, bool envLight);
public static Status ShapeSetEnvironmentLight(IntPtr shape, bool envLight)
{
return rprShapeSetEnvironmentLight(shape, envLight);
}

      /**
      * Sets static flag on shape.
      *
      * Setting such flag will result in marking object as static.
      * Such objects can be processed more efficiently but with some restrictions:
      * @li Static object can't change its properties.
      * @li Static object can't change its transformation.
      *
      * @note Static flag can be set only before first call to rprContextRender.
      * By default all objects created as dynamic.
      *
      * @param in_shape shape to set flag on
      * @param in_is_static is object static or not
      */
  
[DllImport(dllName)] static extern Status rprShapeMarkStatic(IntPtr in_shape, bool in_is_static);
public static Status ShapeMarkStatic(IntPtr in_shape, bool in_is_static)
{
return rprShapeMarkStatic(in_shape, in_is_static);
}

    /** @brief Set light world transform
    *
    *
    *  @param  light       The light to set transform for
    *  @param  transpose   Determines whether the basis vectors are in columns(false) or in rows(true) of the matrix
    *  @param  transform   Array of 16 rpr_float values (row-major form)
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightSetTransform(IntPtr light, bool transpose, float[] transform);
public static Status LightSetTransform(IntPtr light, bool transpose, float[] transform)
{
return rprLightSetTransform(light, transpose, transform);
}

    /** @brief Set light group ID. This parameter can be used with RPR_AOV_LIGHT_GROUP0, RPR_AOV_LIGHT_GROUP1, ...
    *
    *  @param  light       The light to set transform for
    *  @param  groupId     -1 to remove the group.  or a value between 0 and 3.
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightSetGroupId(IntPtr light, uint groupId);
public static Status LightSetGroupId(IntPtr light, uint groupId)
{
return rprLightSetGroupId(light, groupId);
}

    /** @brief Query information about a shape
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  shape           The shape object to query
    *  @param  material_info   The type of info to query
    *  @param  size            The size of the buffer pointed by data
    *  @param  data            The buffer to store queried info
    *  @param  size_ret        Returns the size in bytes of the data being queried
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprShapeGetInfo(IntPtr arg0, ShapeInfo arg1, long arg2, IntPtr arg3, out long arg4);
public static Status ShapeGetInfo(IntPtr arg0, ShapeInfo arg1, long arg2, IntPtr arg3, out long arg4)
{
return rprShapeGetInfo(arg0, arg1, arg2, arg3, out arg4);
}
/* rpr_shape - mesh */

    /** @brief Query information about a mesh
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  shape       The mesh to query
    *  @param  mesh_info   The type of info to query
    *  @param  size        The size of the buffer pointed by data
    *  @param  data        The buffer to store queried info
    *  @param  size_ret    Returns the size in bytes of the data being queried
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprMeshGetInfo(IntPtr mesh, Mesh mesh_info, long size, IntPtr data, out long size_ret);
public static Status MeshGetInfo(IntPtr mesh, Mesh mesh_info, long size, IntPtr data, out long size_ret)
{
return rprMeshGetInfo(mesh, mesh_info, size, data, out size_ret);
}

    /** @brief Query information about a Curve
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  shape       The Curve to query
    *  @param  rpr_curve_parameter   The type of info to query
    *  @param  size        The size of the buffer pointed by data
    *  @param  data        The buffer to store queried info
    *  @param  size_ret    Returns the size in bytes of the data being queried
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprCurveGetInfo(IntPtr curve, Curve curve_info, long size, IntPtr data, out long size_ret);
public static Status CurveGetInfo(IntPtr curve, Curve curve_info, long size, IntPtr data, out long size_ret)
{
return rprCurveGetInfo(curve, curve_info, size, data, out size_ret);
}

    /** @brief Query information about a hetero volume
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  heteroVol       The heteroVolume to query
    *  @param  heteroVol_info  The type of info to query
    *  @param  size            The size of the buffer pointed by data
    *  @param  data            The buffer to store queried info
    *  @param  size_ret        Returns the size in bytes of the data being queried
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprHeteroVolumeGetInfo(IntPtr heteroVol, HeteroVolume heteroVol_info, long size, IntPtr data, out long size_ret);
public static Status HeteroVolumeGetInfo(IntPtr heteroVol, HeteroVolume heteroVol_info, long size, IntPtr data, out long size_ret)
{
return rprHeteroVolumeGetInfo(heteroVol, heteroVol_info, size, data, out size_ret);
}
[DllImport(dllName)] static extern Status rprGridGetInfo(IntPtr grid, Grid grid_info, long size, IntPtr data, out long size_ret);
public static Status GridGetInfo(IntPtr grid, Grid grid_info, long size, IntPtr data, out long size_ret)
{
return rprGridGetInfo(grid, grid_info, size, data, out size_ret);
}

    /** @brief Query information about a Buffer
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  buffer       The heteroVolume to query
    *  @param  buffer_info  The type of info to query
    *  @param  size            The size of the buffer pointed by data
    *  @param  data            The buffer to store queried info
    *  @param  size_ret        Returns the size in bytes of the data being queried
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprBufferGetInfo(IntPtr buffer, Buffer buffer_info, long size, IntPtr data, out long size_ret);
public static Status BufferGetInfo(IntPtr buffer, Buffer buffer_info, long size, IntPtr data, out long size_ret)
{
return rprBufferGetInfo(buffer, buffer_info, size, data, out size_ret);
}

    /** @brief Get the parent shape for an instance
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  shape    The shape to get a parent shape from
    *  @param  status   RPR_SUCCESS in case of success, error code otherwise
    *  @return          Shape object
    */
  
[DllImport(dllName)] static extern Status rprInstanceGetBaseShape(IntPtr shape, out IntPtr out_shape);
public static Status InstanceGetBaseShape(IntPtr shape, out IntPtr out_shape)
{
return rprInstanceGetBaseShape(shape, out out_shape);
}
/* rpr_light - point */

    /** @brief Create point light
    *
    *  Create analytic point light represented by a point in space.
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreatePointLight(IntPtr context, out IntPtr out_light);
public static Status ContextCreatePointLight(IntPtr context, out IntPtr out_light)
{
return rprContextCreatePointLight(context, out out_light);
}

    /** @brief Set radiant power of a point light source
    *
    *  @param  r       R component of a radiant power vector
    *  @param  g       G component of a radiant power vector
    *  @param  b       B component of a radiant power vector
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprPointLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status PointLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprPointLightSetRadiantPower3f(light, r, g, b);
}
/* rpr_light - spot */

    /** @brief Create spot light
    *
    *  Create analytic spot light
    *
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreateSpotLight(IntPtr context, out IntPtr light);
public static Status ContextCreateSpotLight(IntPtr context, out IntPtr light)
{
return rprContextCreateSpotLight(context, out light);
}
[DllImport(dllName)] static extern Status rprContextCreateSphereLight(IntPtr context, out IntPtr light);
public static Status ContextCreateSphereLight(IntPtr context, out IntPtr light)
{
return rprContextCreateSphereLight(context, out light);
}
[DllImport(dllName)] static extern Status rprContextCreateDiskLight(IntPtr context, out IntPtr light);
public static Status ContextCreateDiskLight(IntPtr context, out IntPtr light)
{
return rprContextCreateDiskLight(context, out light);
}

    /** @brief Set radiant power of a spot light source
    *
    *  @param  r R component of a radiant power vector
    *  @param  g G component of a radiant power vector
    *  @param  b B component of a radiant power vector
    *  @return   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSpotLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status SpotLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprSpotLightSetRadiantPower3f(light, r, g, b);
}

    /** @brief turn this spot-light into a textured light.
    *
    * 'img' can be NULL to disable textured.
    */
  
[DllImport(dllName)] static extern Status rprSpotLightSetImage(IntPtr light, IntPtr img);
public static Status SpotLightSetImage(IntPtr light, IntPtr img)
{
return rprSpotLightSetImage(light, img);
}

    /** @brief Set Power for Sphere Light
    *
    *
    *  @param  r R component of a radiant power vector
    *  @param  g G component of a radiant power vector
    *  @param  b B component of a radiant power vector
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSphereLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status SphereLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprSphereLightSetRadiantPower3f(light, r, g, b);
}

    /** @brief Set Radius for Sphere Light
    *
    *
    *  @param radius  Radius to set
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSphereLightSetRadius(IntPtr light, float radius);
public static Status SphereLightSetRadius(IntPtr light, float radius)
{
return rprSphereLightSetRadius(light, radius);
}

    /** @brief Set Power for Disk Light
    *
    *
    *  @param  r R component of a radiant power vector
    *  @param  g G component of a radiant power vector
    *  @param  b B component of a radiant power vector
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDiskLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status DiskLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprDiskLightSetRadiantPower3f(light, r, g, b);
}

    /** @brief Set Radius for Disk Light
    *
    *
    *  @param radius  Radius to set
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDiskLightSetRadius(IntPtr light, float radius);
public static Status DiskLightSetRadius(IntPtr light, float radius)
{
return rprDiskLightSetRadius(light, radius);
}

    /** @brief Set Outer Angle for Disk Light
    *
    *
    *  @param  angle Outer angle in radians
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDiskLightSetAngle(IntPtr light, float angle);
public static Status DiskLightSetAngle(IntPtr light, float angle)
{
return rprDiskLightSetAngle(light, angle);
}

    /** @brief Set Inner Angle for Disk Light
    *
    *  @param  innerAngle Inner angle in radians
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDiskLightSetInnerAngle(IntPtr light, float innerAngle);
public static Status DiskLightSetInnerAngle(IntPtr light, float innerAngle)
{
return rprDiskLightSetInnerAngle(light, innerAngle);
}

    /** @brief Set cone shape for a spot light
    *
    * Spot light produces smooth penumbra in a region between inner and outer circles,
    * the area inside the inner cicrle receives full power while the area outside the
    * outer one is fully in shadow.
    *
    *  @param  iangle Inner angle of a cone in radians
    *  @param  oangle Outer angle of a coner in radians, should be greater that or equal to inner angle
    *  @return status RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSpotLightSetConeShape(IntPtr light, float iangle, float oangle);
public static Status SpotLightSetConeShape(IntPtr light, float iangle, float oangle)
{
return rprSpotLightSetConeShape(light, iangle, oangle);
}
/* rpr_light - directional */

    /** @brief Create directional light
    *
    *  Create analytic directional light.
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return light id of a newly created light
    */
  
[DllImport(dllName)] static extern Status rprContextCreateDirectionalLight(IntPtr context, out IntPtr out_light);
public static Status ContextCreateDirectionalLight(IntPtr context, out IntPtr out_light)
{
return rprContextCreateDirectionalLight(context, out out_light);
}

    /** @brief Set radiant power of a directional light source
    *
    *  @param  r R component of a radiant power vector
    *  @param  g G component of a radiant power vector
    *  @param  b B component of a radiant power vector
    *  @return   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDirectionalLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status DirectionalLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprDirectionalLightSetRadiantPower3f(light, r, g, b);
}

    /** @brief Set softness of shadow produced by the light
    *
    *  @param  softnessAngle  (in Radian)  value should be between [ 0 ; pi/4 ]. 0.0 means sharp shadow
    *  @return   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprDirectionalLightSetShadowSoftnessAngle(IntPtr light, float softnessAngle);
public static Status DirectionalLightSetShadowSoftnessAngle(IntPtr light, float softnessAngle)
{
return rprDirectionalLightSetShadowSoftnessAngle(light, softnessAngle);
}
/* rpr_light - environment */

    /** @brief Create an environment light
    *
    *  Environment light is a light based on lightprobe.
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreateEnvironmentLight(IntPtr context, out IntPtr out_light);
public static Status ContextCreateEnvironmentLight(IntPtr context, out IntPtr out_light)
{
return rprContextCreateEnvironmentLight(context, out out_light);
}

    /** @brief Set image for an environment light
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT
    *
    *  @param  env_light Environment light
    *  @param  image     Image object to set
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprEnvironmentLightSetImage(IntPtr env_light, IntPtr image);
public static Status EnvironmentLightSetImage(IntPtr env_light, IntPtr image)
{
return rprEnvironmentLightSetImage(env_light, image);
}

    /** @brief Set intensity scale or an env light
    *
    *  @param  env_light       Environment light
    *  @param  intensity_scale Intensity scale
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprEnvironmentLightSetIntensityScale(IntPtr env_light, float intensity_scale);
public static Status EnvironmentLightSetIntensityScale(IntPtr env_light, float intensity_scale)
{
return rprEnvironmentLightSetIntensityScale(env_light, intensity_scale);
}

    /** @brief Set portal for environment light to accelerate convergence of indoor scenes
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  env_light Environment light
    *  @param  portal    Portal mesh, might have multiple components
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprEnvironmentLightAttachPortal(IntPtr scene, IntPtr env_light, IntPtr portal);
public static Status EnvironmentLightAttachPortal(IntPtr scene, IntPtr env_light, IntPtr portal)
{
return rprEnvironmentLightAttachPortal(scene, env_light, portal);
}

    /** @brief Remove portal for environment light.
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  env_light Environment light
    *  @param  portal    Portal mesh, that have been added to light.
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprEnvironmentLightDetachPortal(IntPtr scene, IntPtr env_light, IntPtr portal);
public static Status EnvironmentLightDetachPortal(IntPtr scene, IntPtr env_light, IntPtr portal)
{
return rprEnvironmentLightDetachPortal(scene, env_light, portal);
}

      /**
      * Sets/Gets environment override on IBL
      *
      * This function sets overrides for different parts of IBL.
      * overrideType argument can take following values:
      * @li RPR_ENVIRONMENT_LIGHT_OVERRIDE_REFLECTION
      * @li RPR_ENVIRONMENT_LIGHT_OVERRIDE_REFRACTION
      * @li RPR_ENVIRONMENT_LIGHT_OVERRIDE_TRANSPARENCY
      * @li RPR_ENVIRONMENT_LIGHT_OVERRIDE_BACKGROUND
      * @li RPR_ENVIRONMENT_LIGHT_OVERRIDE_IRRADIANCE
      *
      * @param in_ibl image based light created with rprContextCreateEnvironmentLight
      * @param overrideType override parameter
      * @param in_iblOverride image based light created with rprContextCreateEnvironmentLight
      */
  
[DllImport(dllName)] static extern Status rprEnvironmentLightSetEnvironmentLightOverride(IntPtr in_ibl, uint overrideType, IntPtr in_iblOverride);
public static Status EnvironmentLightSetEnvironmentLightOverride(IntPtr in_ibl, uint overrideType, IntPtr in_iblOverride)
{
return rprEnvironmentLightSetEnvironmentLightOverride(in_ibl, overrideType, in_iblOverride);
}
[DllImport(dllName)] static extern Status rprEnvironmentLightGetEnvironmentLightOverride(IntPtr in_ibl, uint overrideType, out IntPtr out_iblOverride);
public static Status EnvironmentLightGetEnvironmentLightOverride(IntPtr in_ibl, uint overrideType, out IntPtr out_iblOverride)
{
return rprEnvironmentLightGetEnvironmentLightOverride(in_ibl, overrideType, out out_iblOverride);
}
/* rpr_light - sky */

    /** @brief Create sky light
    *
    *  Analytical sky model
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreateSkyLight(IntPtr context, out IntPtr out_light);
public static Status ContextCreateSkyLight(IntPtr context, out IntPtr out_light)
{
return rprContextCreateSkyLight(context, out out_light);
}

    /** @brief Set turbidity of a sky light
    *
    *  @param  skylight        Sky light
    *  @param  turbidity       Turbidity value
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightSetTurbidity(IntPtr skylight, float turbidity);
public static Status SkyLightSetTurbidity(IntPtr skylight, float turbidity)
{
return rprSkyLightSetTurbidity(skylight, turbidity);
}

    /** @brief Set albedo of a sky light
    *
    *  @param  skylight        Sky light
    *  @param  albedo          Albedo value
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightSetAlbedo(IntPtr skylight, float albedo);
public static Status SkyLightSetAlbedo(IntPtr skylight, float albedo)
{
return rprSkyLightSetAlbedo(skylight, albedo);
}

    /** @brief Set scale of a sky light
    *
    *  @param  skylight        Sky light
    *  @param  scale           Scale value
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightSetScale(IntPtr skylight, float scale);
public static Status SkyLightSetScale(IntPtr skylight, float scale)
{
return rprSkyLightSetScale(skylight, scale);
}

    /** @brief Set the direction of the sky light
    *
    *  @param  skylight        Sky light
    *  @param  x               direction x
    *  @param  y               direction y
    *  @param  z               direction z
    *  @return                 RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightSetDirection(IntPtr skylight, float x, float y, float z);
public static Status SkyLightSetDirection(IntPtr skylight, float x, float y, float z)
{
return rprSkyLightSetDirection(skylight, x, y, z);
}

    /** @brief Set portal for sky light to accelerate convergence of indoor scenes
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  skylight  Sky light
    *  @param  portal    Portal mesh, might have multiple components
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightAttachPortal(IntPtr scene, IntPtr skylight, IntPtr portal);
public static Status SkyLightAttachPortal(IntPtr scene, IntPtr skylight, IntPtr portal)
{
return rprSkyLightAttachPortal(scene, skylight, portal);
}

    /** @brief Remove portal for Sky light.
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  env_light Sky light
    *  @param  portal    Portal mesh, that have been added to light.
    *  @return           RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSkyLightDetachPortal(IntPtr scene, IntPtr skylight, IntPtr portal);
public static Status SkyLightDetachPortal(IntPtr scene, IntPtr skylight, IntPtr portal)
{
return rprSkyLightDetachPortal(scene, skylight, portal);
}

    /** @brief Create IES light
    *
    *  Create IES light
    *
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreateIESLight(IntPtr context, out IntPtr light);
public static Status ContextCreateIESLight(IntPtr context, out IntPtr light)
{
return rprContextCreateIESLight(context, out light);
}

    /** @brief Set radiant power of a IES light source
    *
    *  @param  r R component of a radiant power vector
    *  @param  g G component of a radiant power vector
    *  @param  b B component of a radiant power vector
    *  @return   RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprIESLightSetRadiantPower3f(IntPtr light, float r, float g, float b);
public static Status IESLightSetRadiantPower3f(IntPtr light, float r, float g, float b)
{
return rprIESLightSetRadiantPower3f(light, r, g, b);
}

    /** @brief Set image for an IES light
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT : If the format of the IES file is not supported by Radeon ProRender.
    *      RPR_ERROR_IO_ERROR : If the IES image path file doesn't exist.
    *
    *  @param  env_light     Environment light
    *  @param  imagePath     Image path to set (for UNICODE, supports UTF-8 encoding)
    *  @param  nx			  resolution X of the IES image
    *  @param  ny            resolution Y of the IES image
    *  @return               RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprIESLightSetImageFromFile(IntPtr env_light, string imagePath, int nx, int ny);
public static Status IESLightSetImageFromFile(IntPtr env_light, string imagePath, int nx, int ny)
{
return rprIESLightSetImageFromFile(env_light, imagePath, nx, ny);
}

    /** @brief Set image for an IES light
    *
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *      RPR_ERROR_UNSUPPORTED_IMAGE_FORMAT : If the format of the IES data is not supported by Radeon ProRender.
    *
    *  @param  env_light     Environment light
    *  @param  iesData       Image data string defining the IES. null terminated string. IES format.
    *  @param  nx			  resolution X of the IES image
    *  @param  ny            resolution Y of the IES image
    *  @return               RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprIESLightSetImageFromIESdata(IntPtr env_light, string iesData, int nx, int ny);
public static Status IESLightSetImageFromIESdata(IntPtr env_light, string iesData, int nx, int ny)
{
return rprIESLightSetImageFromIESdata(env_light, iesData, nx, ny);
}
/* rpr_light */

    /** @brief Query information about a light
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  light    The light to query
    *  @param  light_info The type of info to query
    *  @param  size     The size of the buffer pointed by data
    *  @param  data     The buffer to store queried info
    *  @param  size_ret Returns the size in bytes of the data being queried
    *  @return          RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprLightGetInfo(IntPtr light, Light info, long size, IntPtr data, out long size_ret);
public static Status LightGetInfo(IntPtr light, Light info, long size, IntPtr data, out long size_ret)
{
return rprLightGetInfo(light, info, size, data, out size_ret);
}
/* rpr_scene */

    /** @brief Remove all objects from a scene
    *          Also detaches the camera
    *
    *  A scene is essentially a collection of shapes, lights and volume regions.
    *
    *  @param  scene   The scene to clear
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneClear(IntPtr scene);
public static Status SceneClear(IntPtr scene)
{
return rprSceneClear(scene);
}

    /** @brief Attach a shape to the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions.
    *
    *  @param  scene  The scene to attach
    *  @param  shape  The shape to attach
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneAttachShape(IntPtr scene, IntPtr shape);
public static Status SceneAttachShape(IntPtr scene, IntPtr shape)
{
return rprSceneAttachShape(scene, shape);
}

    /** @brief Detach a shape from the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions.
    *
    *  @param  scene   The scene to dettach from
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneDetachShape(IntPtr scene, IntPtr shape);
public static Status SceneDetachShape(IntPtr scene, IntPtr shape)
{
return rprSceneDetachShape(scene, shape);
}

    /** @brief Attach a heteroVolume to the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions.
    *
    *  @param  scene  The scene to attach
    *  @param  heteroVolume  The heteroVolume to attach
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneAttachHeteroVolume(IntPtr scene, IntPtr heteroVolume);
public static Status SceneAttachHeteroVolume(IntPtr scene, IntPtr heteroVolume)
{
return rprSceneAttachHeteroVolume(scene, heteroVolume);
}

    /** @brief Detach a heteroVolume from the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions.
    *
    *  @param  scene   The scene to dettach from
    *  @return         RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneDetachHeteroVolume(IntPtr scene, IntPtr heteroVolume);
public static Status SceneDetachHeteroVolume(IntPtr scene, IntPtr heteroVolume)
{
return rprSceneDetachHeteroVolume(scene, heteroVolume);
}
[DllImport(dllName)] static extern Status rprSceneAttachCurve(IntPtr scene, IntPtr curve);
public static Status SceneAttachCurve(IntPtr scene, IntPtr curve)
{
return rprSceneAttachCurve(scene, curve);
}
[DllImport(dllName)] static extern Status rprSceneDetachCurve(IntPtr scene, IntPtr curve);
public static Status SceneDetachCurve(IntPtr scene, IntPtr curve)
{
return rprSceneDetachCurve(scene, curve);
}
[DllImport(dllName)] static extern Status rprCurveSetMaterial(IntPtr curve, IntPtr material);
public static Status CurveSetMaterial(IntPtr curve, IntPtr material)
{
return rprCurveSetMaterial(curve, material);
}
[DllImport(dllName)] static extern Status rprCurveSetTransform(IntPtr curve, bool transpose, IntPtr transform);
public static Status CurveSetTransform(IntPtr curve, bool transpose, IntPtr transform)
{
return rprCurveSetTransform(curve, transpose, transform);
}

/** @brief Create a set of curves
*
*  A rpr_curve is a set of curves
*  A curve is a set of segments
*  A segment is always composed of 4 3D points
*
*  @param controlPointsData  array of rpr_float[num_controlPoints*3]
*
*  @param controlPointsStride  in most of cases, for contiguous controlPointsData, should be set to 3*sizeof(float)
*
*  @param num_indices  should be set at : 4*(number of segments)
*
*  @param indicesData  array of rpr_uint[num_indices] .
*                      those are indices to the controlPointsData array.
*
*  @param radius  array of N float. 
*                 if curve is not tapered, N = curveCount.
*                 if curve is tapered, N = 2*(number of segments)
*
*  @param textureUV   array of float2[curveCount]. 
*
*  @param segmentPerCurve   array of rpr_int[curveCount]. 
*                           (number of segments) = sum of each element of this array.
*
*  @param  creationFlag_tapered  Set it to 0 by default.
*                                Set to 1 if using tapered radius.
*                                May be used for other bit field options in the future (so, don't set it to a value > 1 for now.)
*
*/

[DllImport(dllName)] static extern Status rprContextCreateCurve(IntPtr context, out IntPtr out_curve, long num_controlPoints, IntPtr controlPointsData, int controlPointsStride, long num_indices, uint curveCount, IntPtr indicesData, IntPtr radius, IntPtr textureUV, IntPtr segmentPerCurve, uint creationFlag_tapered);
public static Status ContextCreateCurve(IntPtr context, out IntPtr out_curve, long num_controlPoints, IntPtr controlPointsData, int controlPointsStride, long num_indices, uint curveCount, IntPtr indicesData, IntPtr radius, IntPtr textureUV, IntPtr segmentPerCurve, uint creationFlag_tapered)
{
return rprContextCreateCurve(context, out out_curve, num_controlPoints, controlPointsData, controlPointsStride, num_indices, curveCount, indicesData, radius, textureUV, segmentPerCurve, creationFlag_tapered);
}

    /** @brief Attach a light to the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions
    *
    *  @param  scene  The scene to attach
    *  @param  light  The light to attach
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneAttachLight(IntPtr scene, IntPtr light);
public static Status SceneAttachLight(IntPtr scene, IntPtr light)
{
return rprSceneAttachLight(scene, light);
}

    /** @brief Detach a light from the scene
    *
    *  A scene is essentially a collection of shapes, lights and volume regions
    *
    *  @param  scene  The scene to dettach from
    *  @param  light  The light to detach
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneDetachLight(IntPtr scene, IntPtr light);
public static Status SceneDetachLight(IntPtr scene, IntPtr light)
{
return rprSceneDetachLight(scene, light);
}

      /**
      * Sets/gets environment override as active in scene
      *
      * @param in_scene scene
      * @param in_light ibl
      */
  
[DllImport(dllName)] static extern Status rprSceneSetEnvironmentLight(IntPtr in_scene, IntPtr in_light);
public static Status SceneSetEnvironmentLight(IntPtr in_scene, IntPtr in_light)
{
return rprSceneSetEnvironmentLight(in_scene, in_light);
}
[DllImport(dllName)] static extern Status rprSceneGetEnvironmentLight(IntPtr in_scene, out IntPtr out_light);
public static Status SceneGetEnvironmentLight(IntPtr in_scene, out IntPtr out_light)
{
return rprSceneGetEnvironmentLight(in_scene, out out_light);
}

    /** @brief Query information about a scene
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  scene    The scene to query
    *  @param  info     The type of info to query
    *  @param  size     The size of the buffer pointed by data
    *  @param  data     The buffer to store queried info
    *  @param  size_ret Returns the size in bytes of the data being queried
    *  @return          RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneGetInfo(IntPtr scene, Scene info, long size, IntPtr data, out long size_ret);
public static Status SceneGetInfo(IntPtr scene, Scene info, long size, IntPtr data, out long size_ret)
{
return rprSceneGetInfo(scene, info, size, data, out size_ret);
}

    /** @brief Set background image for the scene which does not affect the scene lighting,
    *    it is shown as view-independent rectangular background
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  scene  The scene to set background for
    *  @param  image  Background image
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneSetBackgroundImage(IntPtr scene, IntPtr image);
public static Status SceneSetBackgroundImage(IntPtr scene, IntPtr image)
{
return rprSceneSetBackgroundImage(scene, image);
}

    /** @brief Get background image
    *
    *  @param  scene  The scene to get background image from
    *  @param  status RPR_SUCCESS in case of success, error code otherwise
    *  @return        Image object
    */
  
[DllImport(dllName)] static extern Status rprSceneGetBackgroundImage(IntPtr scene, out IntPtr out_image);
public static Status SceneGetBackgroundImage(IntPtr scene, out IntPtr out_image)
{
return rprSceneGetBackgroundImage(scene, out out_image);
}

    /** @brief Set right camera for the scene
    *
    *  This is the main camera which for rays generation for the scene.
    *
    *  @param  scene  The scene to set camera for
    *  @param  camera Camera
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneSetCameraRight(IntPtr scene, IntPtr camera);
public static Status SceneSetCameraRight(IntPtr scene, IntPtr camera)
{
return rprSceneSetCameraRight(scene, camera);
}

    /** @brief Get right camera for the scene
    *
    *  @param  scene  The scene to get camera for
    *  @param  status RPR_SUCCESS in case of success, error code otherwise
    *  @return camera id for the camera if any, NULL otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneGetCameraRight(IntPtr scene, out IntPtr out_camera);
public static Status SceneGetCameraRight(IntPtr scene, out IntPtr out_camera)
{
return rprSceneGetCameraRight(scene, out out_camera);
}

    /** @brief Set camera for the scene
    *
    *  This is the main camera which for rays generation for the scene.
    *
    *  @param  scene  The scene to set camera for
    *  @param  camera Camera
    *  @return        RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneSetCamera(IntPtr scene, IntPtr camera);
public static Status SceneSetCamera(IntPtr scene, IntPtr camera)
{
return rprSceneSetCamera(scene, camera);
}

    /** @brief Get camera for the scene
    *
    *  @param  scene  The scene to get camera for
    *  @param  status RPR_SUCCESS in case of success, error code otherwise
    *  @return camera id for the camera if any, NULL otherwise
    */
  
[DllImport(dllName)] static extern Status rprSceneGetCamera(IntPtr scene, out IntPtr out_camera);
public static Status SceneGetCamera(IntPtr scene, out IntPtr out_camera)
{
return rprSceneGetCamera(scene, out out_camera);
}

    /** @brief Query information about a framebuffer
    *
    *  The workflow is usually two-step: query with the data == NULL to get the required buffer size,
    *  then query with size_ret == NULL to fill the buffer with the data
    *   Possible error codes:
    *      RPR_ERROR_INVALID_PARAMETER
    *
    *  @param  framebuffer  Framebuffer object to query
    *  @param  info         The type of info to query
    *  @param  size         The size of the buffer pointed by data
    *  @param  data         The buffer to store queried info
    *  @param  size_ret     Returns the size in bytes of the data being queried
    *  @return              RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferGetInfo(IntPtr framebuffer, FrameBuffer info, long size, IntPtr data, out long size_ret);
public static Status FrameBufferGetInfo(IntPtr framebuffer, FrameBuffer info, long size, IntPtr data, out long size_ret)
{
return rprFrameBufferGetInfo(framebuffer, info, size, data, out size_ret);
}

    /** @brief Clear contents of a framebuffer to zero
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    *  The call is blocking and the image is ready when returned
    *
    *  @param  frame_buffer  Framebuffer to clear
    *  @return              RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferClear(IntPtr frame_buffer);
public static Status FrameBufferClear(IntPtr frame_buffer)
{
return rprFrameBufferClear(frame_buffer);
}

    /** @brief Fill contents of a framebuffer with a single color
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    *  The call is blocking and the image is ready when returned.
    *  If you want to fill with zeros, it's advised to use rprFrameBufferClear.
    *
    *  @param  frame_buffer  Framebuffer to clear
    *  @param r,g,b,a : the color to fill
    *  @return              RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferFillWithColor(IntPtr frame_buffer, float r, float g, float b, float a);
public static Status FrameBufferFillWithColor(IntPtr frame_buffer, float r, float g, float b, float a)
{
return rprFrameBufferFillWithColor(frame_buffer, r, g, b, a);
}

    /** @brief Save frame buffer to file. In case the file format is .bin, the header of the saved file contains
    * rpr_framebuffer_desc and rpr_framebuffer_format at very begining. The remaining data is raw data of saved framebuffer.
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    *  @param  frame_buffer Frame buffer to save
    *  @param  file_path    Path to file (for UNICODE, supports UTF-8 encoding)
    *  @return              RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferSaveToFile(IntPtr frame_buffer, string file_path);
public static Status FrameBufferSaveToFile(IntPtr frame_buffer, string file_path)
{
return rprFrameBufferSaveToFile(frame_buffer, file_path);
}

    /** @brief Save frame buffer to file
    *
    *  Same that rprFrameBufferSaveToFile, but more options.
    *  A list of frambuffers can be given, they will be saved to a multilayer EXR.
    *
    *  'extraOptions' is not used for now, but may be use in the future to define some export options, like channel configurations, compression...
    *                 It must be set to NULL for now.
    *
    *  For layer names, the framebuffer names ( from rprObjectSetName ) will be used if it exists.
    *
    *  As this function is new ( 2.01.6 SDK ) and still experimental, its arguments may change in the future.
    */
  
[DllImport(dllName)] static extern Status rprFrameBufferSaveToFileEx(IntPtr framebufferList, uint framebufferCount, string filePath, IntPtr extraOptions);
public static Status FrameBufferSaveToFileEx(IntPtr framebufferList, uint framebufferCount, string filePath, IntPtr extraOptions)
{
return rprFrameBufferSaveToFileEx(framebufferList, framebufferCount, filePath, extraOptions);
}

    /** @brief Resolve framebuffer
    *
    * Convert the input Renderer's native raw format 'src_frame_buffer' into an output 'dst_frame_buffer' that can be used for final rendering.
    *
    * src_frame_buffer and dst_frame_buffer should usually have the same dimension/format.
    * src_frame_buffer is the result of a rprContextRender and should be attached to an AOV with rprContextSetAOV before the rprContextRender call.
    * dst_frame_buffer should not be attached to any AOV.
    *
    * The post process that is applied to src_frame_buffer depends on the AOV it's attached to. So it's important to not modify its AOV ( with rprContextSetAOV )
    * between the rprContextRender and rprContextResolveFrameBuffer calls.
    *
    * If noDisplayGamma=FALSE, then RPR_CONTEXT_DISPLAY_GAMMA is applied to the dst_frame_buffer otherwise, display gamma is not used.
    * It's recommended to set it to FALSE for AOVs representing colors ( like RPR_AOV_COLOR ) and use TRUE for other AOVs.
    *
    */
  
[DllImport(dllName)] static extern Status rprContextResolveFrameBuffer(IntPtr context, IntPtr src_frame_buffer, IntPtr dst_frame_buffer, bool noDisplayGamma);
public static Status ContextResolveFrameBuffer(IntPtr context, IntPtr src_frame_buffer, IntPtr dst_frame_buffer, bool noDisplayGamma)
{
return rprContextResolveFrameBuffer(context, src_frame_buffer, dst_frame_buffer, noDisplayGamma);
}

    /** @brief Create material system
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialSystemGetInfo(IntPtr in_material_system, MaterialSystemInfo type, long in_size, IntPtr in_data, out long out_size);
public static Status MaterialSystemGetInfo(IntPtr in_material_system, MaterialSystemInfo type, long in_size, IntPtr in_data, out long out_size)
{
return rprMaterialSystemGetInfo(in_material_system, type, in_size, in_data, out out_size);
}

      /** @brief Get material system information
      *
      *   Possible error codes:
      *      RPR_ERROR_INTERNAL_ERROR
      *
      */
  
[DllImport(dllName)] static extern Status rprContextCreateMaterialSystem(IntPtr in_context, uint type, out IntPtr out_matsys);
public static Status ContextCreateMaterialSystem(IntPtr in_context, uint type, out IntPtr out_matsys)
{
return rprContextCreateMaterialSystem(in_context, type, out out_matsys);
}

    /** @brief Create material node
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialSystemGetSize(IntPtr in_context, out uint out_size);
public static Status MaterialSystemGetSize(IntPtr in_context, out uint out_size)
{
return rprMaterialSystemGetSize(in_context, out out_size);
}

    /** @brief Returns the number of material nodes for a given material system
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialSystemCreateNode(IntPtr in_matsys, MaterialNodeType in_type, out IntPtr out_node);
public static Status MaterialSystemCreateNode(IntPtr in_matsys, MaterialNodeType in_type, out IntPtr out_node)
{
return rprMaterialSystemCreateNode(in_matsys, in_type, out out_node);
}

    /** @brief set the RPR_MATERIAL_NODE_ID of a material.
    *   this ID doesn't need to be unique.
    *   this ID can be rendered with the RPR_AOV_MATERIAL_ID AOV - color of this AOV can be customized with rprContextSetAOVindexLookup.
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetID(IntPtr in_node, uint id);
public static Status MaterialNodeSetID(IntPtr in_node, uint id)
{
return rprMaterialNodeSetID(in_node, id);
}

    /** @brief Connect nodes
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputNByKey(IntPtr in_node, MaterialInput in_input, IntPtr in_input_node);
public static Status MaterialNodeSetInputNByKey(IntPtr in_node, MaterialInput in_input, IntPtr in_input_node)
{
return rprMaterialNodeSetInputNByKey(in_node, in_input, in_input_node);
}

    /** @brief Set float input value
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputFByKey(IntPtr in_node, MaterialInput in_input, float in_value_x, float in_value_y, float in_value_z, float in_value_w);
public static Status MaterialNodeSetInputFByKey(IntPtr in_node, MaterialInput in_input, float in_value_x, float in_value_y, float in_value_z, float in_value_w)
{
return rprMaterialNodeSetInputFByKey(in_node, in_input, in_value_x, in_value_y, in_value_z, in_value_w);
}

    /** @brief Set generic data input value: Some complex materials inputs may need more than 4-floats or int.
    *  This API can be used to set any generic input data.
    *  Use it only when documentation requests to do it for specific material inputs.
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr data, long dataSizeByte);
public static Status MaterialNodeSetInputDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr data, long dataSizeByte)
{
return rprMaterialNodeSetInputDataByKey(in_node, in_input, data, dataSizeByte);
}

    /** @brief Set uint input value
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputUByKey(IntPtr in_node, MaterialInput in_input, uint in_value);
public static Status MaterialNodeSetInputUByKey(IntPtr in_node, MaterialInput in_input, uint in_value)
{
return rprMaterialNodeSetInputUByKey(in_node, in_input, in_value);
}

    /** @brief Set image input value
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputImageDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr image);
public static Status MaterialNodeSetInputImageDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr image)
{
return rprMaterialNodeSetInputImageDataByKey(in_node, in_input, image);
}

    /** @brief Set light input value
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputLightDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr light);
public static Status MaterialNodeSetInputLightDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr light)
{
return rprMaterialNodeSetInputLightDataByKey(in_node, in_input, light);
}

    /** @brief Set Buffer input value
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputBufferDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr buffer);
public static Status MaterialNodeSetInputBufferDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr buffer)
{
return rprMaterialNodeSetInputBufferDataByKey(in_node, in_input, buffer);
}

    /** @brief Set Grid input value
    *
    */
  
[DllImport(dllName)] static extern Status rprMaterialNodeSetInputGridDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr grid);
public static Status MaterialNodeSetInputGridDataByKey(IntPtr in_node, MaterialInput in_input, IntPtr grid)
{
return rprMaterialNodeSetInputGridDataByKey(in_node, in_input, grid);
}
[DllImport(dllName)] static extern Status rprMaterialNodeGetInfo(IntPtr in_node, MaterialNodeInfo in_info, long in_size, IntPtr in_data, out long out_size);
public static Status MaterialNodeGetInfo(IntPtr in_node, MaterialNodeInfo in_info, long in_size, IntPtr in_data, out long out_size)
{
return rprMaterialNodeGetInfo(in_node, in_info, in_size, in_data, out out_size);
}
[DllImport(dllName)] static extern Status rprMaterialNodeGetInputInfo(IntPtr in_node, int in_input_idx, MaterialNodeInput in_info, long in_size, IntPtr in_data, out long out_size);
public static Status MaterialNodeGetInputInfo(IntPtr in_node, int in_input_idx, MaterialNodeInput in_info, long in_size, IntPtr in_data, out long out_size)
{
return rprMaterialNodeGetInputInfo(in_node, in_input_idx, in_info, in_size, in_data, out out_size);
}
[DllImport(dllName)] static extern Status rprContextCreateComposite(IntPtr context, CompositeType in_type, out IntPtr out_composite);
public static Status ContextCreateComposite(IntPtr context, CompositeType in_type, out IntPtr out_composite)
{
return rprContextCreateComposite(context, in_type, out out_composite);
}
[DllImport(dllName)] static extern Status rprContextCreateLUTFromFile(IntPtr context, IntPtr fileLutPath, out IntPtr out_lut);
public static Status ContextCreateLUTFromFile(IntPtr context, IntPtr fileLutPath, out IntPtr out_lut)
{
return rprContextCreateLUTFromFile(context, fileLutPath, out out_lut);
}
[DllImport(dllName)] static extern Status rprContextCreateLUTFromData(IntPtr context, IntPtr lutData, out IntPtr out_lut);
public static Status ContextCreateLUTFromData(IntPtr context, IntPtr lutData, out IntPtr out_lut)
{
return rprContextCreateLUTFromData(context, lutData, out out_lut);
}
[DllImport(dllName)] static extern Status rprCompositeSetInputFb(IntPtr composite, IntPtr inputName, IntPtr input);
public static Status CompositeSetInputFb(IntPtr composite, IntPtr inputName, IntPtr input)
{
return rprCompositeSetInputFb(composite, inputName, input);
}
[DllImport(dllName)] static extern Status rprCompositeSetInputC(IntPtr composite, IntPtr inputName, IntPtr input);
public static Status CompositeSetInputC(IntPtr composite, IntPtr inputName, IntPtr input)
{
return rprCompositeSetInputC(composite, inputName, input);
}
[DllImport(dllName)] static extern Status rprCompositeSetInputLUT(IntPtr composite, IntPtr inputName, IntPtr input);
public static Status CompositeSetInputLUT(IntPtr composite, IntPtr inputName, IntPtr input)
{
return rprCompositeSetInputLUT(composite, inputName, input);
}
[DllImport(dllName)] static extern Status rprCompositeSetInput4f(IntPtr composite, IntPtr inputName, float x, float y, float z, float w);
public static Status CompositeSetInput4f(IntPtr composite, IntPtr inputName, float x, float y, float z, float w)
{
return rprCompositeSetInput4f(composite, inputName, x, y, z, w);
}
[DllImport(dllName)] static extern Status rprCompositeSetInput1u(IntPtr composite, IntPtr inputName, uint value);
public static Status CompositeSetInput1u(IntPtr composite, IntPtr inputName, uint value)
{
return rprCompositeSetInput1u(composite, inputName, value);
}
[DllImport(dllName)] static extern Status rprCompositeSetInputOp(IntPtr composite, IntPtr inputName, MaterialNodeOp op);
public static Status CompositeSetInputOp(IntPtr composite, IntPtr inputName, MaterialNodeOp op)
{
return rprCompositeSetInputOp(composite, inputName, op);
}
[DllImport(dllName)] static extern Status rprCompositeCompute(IntPtr composite, IntPtr fb);
public static Status CompositeCompute(IntPtr composite, IntPtr fb)
{
return rprCompositeCompute(composite, fb);
}
[DllImport(dllName)] static extern Status rprCompositeGetInfo(IntPtr composite, CompositeInfo composite_info, long size, IntPtr data, out long size_ret);
public static Status CompositeGetInfo(IntPtr composite, CompositeInfo composite_info, long size, IntPtr data, out long size_ret)
{
return rprCompositeGetInfo(composite, composite_info, size, data, out size_ret);
}

    /** @brief Delete object
    *
    *  rprObjectDelete(obj) deletes 'obj' from memory.
    *  User has to make sure that 'obj' will not be used anymore after this call.
    *
    *   Possible error codes:
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *
    */
  
[DllImport(dllName)] static extern Status rprObjectDelete(IntPtr obj);
public static void ObjectDelete(ref IntPtr obj)
{
	try
	{
		if (obj != IntPtr.Zero)
		{
			Check(rprObjectDelete(obj));
			obj = IntPtr.Zero;
		}
	}
	catch (Exception ex)
	{
		//Log.Write(LoggerLevel.Error, ProjectType.RadeonProRender, ex, "Error on deleting IntPtr in FR", true);
	}
}

    /** @brief Set material node name
    *
    *
    *  @param  node        Node to set the name for
    *  @param  name       NULL terminated string name
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprObjectSetName(IntPtr node, string name);
public static Status ObjectSetName(IntPtr node, string name)
{
return rprObjectSetName(node, name);
}

    /** @brief Set a custom pointer to an RPR object ( rpr_shape, rpr_image ... )
    *          The custom pointer is not used internally by RPR. The API user only is responsible of it.
    *          An example of usage of this pointer is the C++ wrapper ( RadeonProRender.hpp )
    *
    *  @param  node        Node to set the 'custom pointer' for
    *  @param  customPtr   Any 8 bytes value decided by the API user.
    *  @return             RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprObjectSetCustomPointer(IntPtr node, IntPtr customPtr);
public static Status ObjectSetCustomPointer(IntPtr node, IntPtr customPtr)
{
return rprObjectSetCustomPointer(node, customPtr);
}

    /** @brief   outputs the 'custom pointer' set by rprObjectSetCustomPointer.
    *            Equivalent of the calls :
    *             rprImageGetInfo(image,RPR_IMAGE_CUSTOM_PTR,...) for rpr_image ,
    *             rprCameraGetInfo(camera,RPR_CAMERA_CUSTOM_PTR,...) for rpr_camera ,
    *             ...etc...
    *
    *  @return      RPR_SUCCESS in case of success, error code otherwise
    */
  
[DllImport(dllName)] static extern Status rprObjectGetCustomPointer(IntPtr node, out IntPtr customPtr_out);
public static Status ObjectGetCustomPointer(IntPtr node, out IntPtr customPtr_out)
{
return rprObjectGetCustomPointer(node, out customPtr_out);
}
/* rpr_post_effect */

    /** @brief Create post effect
    *
    *  Create analytic point light represented by a point in space.
    *  Possible error codes:
    *      RPR_ERROR_OUT_OF_VIDEO_MEMORY
    *      RPR_ERROR_OUT_OF_SYSTEM_MEMORY
    *
    *  @param  context The context to create a light for
    *  @param  status  RPR_SUCCESS in case of success, error code otherwise
    *  @return         Light object
    */
  
[DllImport(dllName)] static extern Status rprContextCreatePostEffect(IntPtr context, PostEffectType type, out IntPtr out_effect);
public static Status ContextCreatePostEffect(IntPtr context, PostEffectType type, out IntPtr out_effect)
{
return rprContextCreatePostEffect(context, type, out out_effect);
}
[DllImport(dllName)] static extern Status rprContextAttachPostEffect(IntPtr context, IntPtr effect);
public static Status ContextAttachPostEffect(IntPtr context, IntPtr effect)
{
return rprContextAttachPostEffect(context, effect);
}
[DllImport(dllName)] static extern Status rprContextDetachPostEffect(IntPtr context, IntPtr effect);
public static Status ContextDetachPostEffect(IntPtr context, IntPtr effect)
{
return rprContextDetachPostEffect(context, effect);
}
[DllImport(dllName)] static extern Status rprPostEffectSetParameter1u(IntPtr effect, string name, uint x);
public static Status PostEffectSetParameter1u(IntPtr effect, string name, uint x)
{
return rprPostEffectSetParameter1u(effect, name, x);
}
[DllImport(dllName)] static extern Status rprPostEffectSetParameter1f(IntPtr effect, string name, float x);
public static Status PostEffectSetParameter1f(IntPtr effect, string name, float x)
{
return rprPostEffectSetParameter1f(effect, name, x);
}
[DllImport(dllName)] static extern Status rprPostEffectSetParameter3f(IntPtr effect, string name, float x, float y, float z);
public static Status PostEffectSetParameter3f(IntPtr effect, string name, float x, float y, float z)
{
return rprPostEffectSetParameter3f(effect, name, x, y, z);
}
[DllImport(dllName)] static extern Status rprPostEffectSetParameter4f(IntPtr effect, string name, float x, float y, float z, float w);
public static Status PostEffectSetParameter4f(IntPtr effect, string name, float x, float y, float z, float w)
{
return rprPostEffectSetParameter4f(effect, name, x, y, z, w);
}
[DllImport(dllName)] static extern Status rprContextGetAttachedPostEffectCount(IntPtr context, IntPtr nb);
public static Status ContextGetAttachedPostEffectCount(IntPtr context, IntPtr nb)
{
return rprContextGetAttachedPostEffectCount(context, nb);
}
[DllImport(dllName)] static extern Status rprContextGetAttachedPostEffect(IntPtr context, uint i, out IntPtr out_effect);
public static Status ContextGetAttachedPostEffect(IntPtr context, uint i, out IntPtr out_effect)
{
return rprContextGetAttachedPostEffect(context, i, out out_effect);
}
[DllImport(dllName)] static extern Status rprPostEffectGetInfo(IntPtr effect, PostEffectInfo info, long size, IntPtr data, out long size_ret);
public static Status PostEffectGetInfo(IntPtr effect, PostEffectInfo info, long size, IntPtr data, out long size_ret)
{
return rprPostEffectGetInfo(effect, info, size, data, out size_ret);
}
[DllImport(dllName)] static extern Status rprContextCreateGrid(IntPtr context, out IntPtr out_grid, long gridSizeX, long gridSizeY, long gridSizeZ, IntPtr indicesList, long numberOfIndices, GridIndicesTopology indicesListTopology, IntPtr gridData, long gridDataSizeByte, uint gridDataTopology___unused);
public static Status ContextCreateGrid(IntPtr context, out IntPtr out_grid, long gridSizeX, long gridSizeY, long gridSizeZ, IntPtr indicesList, long numberOfIndices, GridIndicesTopology indicesListTopology, IntPtr gridData, long gridDataSizeByte, uint gridDataTopology___unused)
{
return rprContextCreateGrid(context, out out_grid, gridSizeX, gridSizeY, gridSizeZ, indicesList, numberOfIndices, indicesListTopology, gridData, gridDataSizeByte, gridDataTopology___unused);
}
[DllImport(dllName)] static extern Status rprContextCreateHeteroVolume(IntPtr context, out IntPtr out_heteroVolume);
public static Status ContextCreateHeteroVolume(IntPtr context, out IntPtr out_heteroVolume)
{
return rprContextCreateHeteroVolume(context, out out_heteroVolume);
}
[DllImport(dllName)] static extern Status rprShapeSetHeteroVolume(IntPtr shape, IntPtr heteroVolume);
public static Status ShapeSetHeteroVolume(IntPtr shape, IntPtr heteroVolume)
{
return rprShapeSetHeteroVolume(shape, heteroVolume);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetTransform(IntPtr heteroVolume, bool transpose, IntPtr transform);
public static Status HeteroVolumeSetTransform(IntPtr heteroVolume, bool transpose, IntPtr transform)
{
return rprHeteroVolumeSetTransform(heteroVolume, transpose, transform);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetEmissionGrid(IntPtr heteroVolume, IntPtr grid);
public static Status HeteroVolumeSetEmissionGrid(IntPtr heteroVolume, IntPtr grid)
{
return rprHeteroVolumeSetEmissionGrid(heteroVolume, grid);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetDensityGrid(IntPtr heteroVolume, IntPtr grid);
public static Status HeteroVolumeSetDensityGrid(IntPtr heteroVolume, IntPtr grid)
{
return rprHeteroVolumeSetDensityGrid(heteroVolume, grid);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetAlbedoGrid(IntPtr heteroVolume, IntPtr grid);
public static Status HeteroVolumeSetAlbedoGrid(IntPtr heteroVolume, IntPtr grid)
{
return rprHeteroVolumeSetAlbedoGrid(heteroVolume, grid);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetEmissionLookup(IntPtr heteroVolume, IntPtr ptr, uint n);
public static Status HeteroVolumeSetEmissionLookup(IntPtr heteroVolume, IntPtr ptr, uint n)
{
return rprHeteroVolumeSetEmissionLookup(heteroVolume, ptr, n);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetDensityLookup(IntPtr heteroVolume, IntPtr ptr, uint n);
public static Status HeteroVolumeSetDensityLookup(IntPtr heteroVolume, IntPtr ptr, uint n)
{
return rprHeteroVolumeSetDensityLookup(heteroVolume, ptr, n);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetAlbedoLookup(IntPtr heteroVolume, IntPtr ptr, uint n);
public static Status HeteroVolumeSetAlbedoLookup(IntPtr heteroVolume, IntPtr ptr, uint n)
{
return rprHeteroVolumeSetAlbedoLookup(heteroVolume, ptr, n);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetAlbedoScale(IntPtr heteroVolume, float scale);
public static Status HeteroVolumeSetAlbedoScale(IntPtr heteroVolume, float scale)
{
return rprHeteroVolumeSetAlbedoScale(heteroVolume, scale);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetEmissionScale(IntPtr heteroVolume, float scale);
public static Status HeteroVolumeSetEmissionScale(IntPtr heteroVolume, float scale)
{
return rprHeteroVolumeSetEmissionScale(heteroVolume, scale);
}
[DllImport(dllName)] static extern Status rprHeteroVolumeSetDensityScale(IntPtr heteroVolume, float scale);
public static Status HeteroVolumeSetDensityScale(IntPtr heteroVolume, float scale)
{
return rprHeteroVolumeSetDensityScale(heteroVolume, scale);
}



  
}
}
