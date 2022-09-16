/*****************************************************************************\
*
*  Module Name    RadeonProRenderCpp.cpp
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#include "RadeonProRender.hpp"
#include "RadeonProRender_MaterialX.h"
#include <cassert>
#include <vector>
#include "rprDeprecatedApi.h"

#ifdef RPR_CPPWRAPER_DISABLE_MUTEXLOCK
#define RPR_CPPWRAPER_MUTEXLOCK
#else
#define RPR_CPPWRAPER_MUTEXLOCK std::lock_guard<std::mutex> lock(GetContext().GetMutex());
#endif

#define RPR_CPPWRAPER_CALL_PREFIX  RPR_CPPWRAPER_MUTEXLOCK; Status status; if ( (status =
#define RPR_CPPWRAPER_CALL_SUFFIX  ) != RPR_SUCCESS ) { return status; } return RPR_SUCCESS;
#define RPR_CPPWRAPER_CALL_SUFFIX_CREATOR  ) != RPR_SUCCESS ) { if ( out_status ) { *out_status = status; } return nullptr; }
#define RPR_CPPWRAPER_GET_CALL_SUFFIX  ) != RPR_SUCCESS ) { return status; }
#define RPR_CPPWRAPER_SET_CUSTOM_PTR  if ( (status = rprObjectSetCustomPointer(newRprObj,newCppObj) ) != RPR_SUCCESS ) { if ( out_status ) { *out_status = status; } return nullptr; }

namespace rpr {

namespace {

// input is a rpr node : rpr_image, rpr_context, rpr_shape ....
// output is the 'BaseContextObject' holding this RPR node.
template<typename Wrapper>
Wrapper* GetContextObjectFromRprApiObject(typename RprApiTypeOf<Wrapper>::value rprApiObject) {
    const void* customPtr = nullptr;
    if (rprObjectGetCustomPointer(rprApiObject, &customPtr) != RPR_SUCCESS) {
        return nullptr;
    }
    assert(dynamic_cast<Wrapper const*>(static_cast<ContextObject const*>(customPtr)));
    return const_cast<Wrapper*>(static_cast<Wrapper const*>(customPtr));
}

} // namespace anonymous

Context::~Context() {
    RPR_CPPWRAPER_MUTEXLOCK;
    rprObjectDelete(m_materialSystem);
    rprObjectDelete(m_context);
}

Context* Context::Create(rpr_int api_version, rpr_int* pluginIDs, size_t pluginCount, CreationFlags creationFlags, ContextProperties const* properties, rpr_char const* cachePath, Status* out_status) {
    Status status;

    rpr_context contextObject = nullptr;
    if ((status = rprCreateContext(api_version, pluginIDs, pluginCount, creationFlags, properties, cachePath, &contextObject)) != RPR_SUCCESS) {
        if (out_status) {
            *out_status = status;
        }
        return nullptr;
    }

    rpr_material_system matSysObject = nullptr;
    if ((status = rprContextCreateMaterialSystem(contextObject, 0, &matSysObject)) != RPR_SUCCESS) {
        rprObjectDelete(contextObject);
        if (out_status) {
            *out_status = status;
        }
        return nullptr;
    }

    auto context = new Context();
    context->m_context = contextObject;
    context->m_materialSystem = matSysObject;
    return context;
}

Scene* Context::CreateScene(Status* out_status) {
    typename RprApiTypeOf<Scene>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateScene(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Scene* newCppObj = new Scene(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Shape* Context::CreateShape(rpr_float const* vertices, size_t num_vertices, rpr_int vertex_stride, rpr_float const* normals, size_t num_normals, rpr_int normal_stride, rpr_float const* texcoords, size_t num_texcoords, rpr_int texcoord_stride, rpr_int const* vertex_indices, rpr_int vidx_stride, rpr_int const* normal_indices, rpr_int nidx_stride, rpr_int const* texcoord_indices, rpr_int tidx_stride, rpr_int const* num_face_vertices, size_t num_faces, Status* out_status) {
    typename RprApiTypeOf<Shape>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateMeshEx2(
        m_context,
        vertices, num_vertices, vertex_stride,
        normals, num_normals, normal_stride,
        nullptr, 0, 0, //No per vertex flags
        1, &texcoords, &num_texcoords, &texcoord_stride, //Single texcoord layer
        vertex_indices, vidx_stride,
        normal_indices, nidx_stride,
        &texcoord_indices, &tidx_stride,
        num_face_vertices, num_faces,
        nullptr, //No mesh props
        &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Shape* newCppObj = new Shape(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Shape* Context::CreateShape(rpr_float const* vertices, size_t num_vertices, rpr_int vertex_stride , rpr_float const * normals , size_t num_normals , rpr_int normal_stride , rpr_int const * perVertexFlag , size_t num_perVertexFlags , rpr_int perVertexFlag_stride , rpr_int numberOfTexCoordLayers , rpr_float const ** texcoords , size_t const * num_texcoords , rpr_int const * texcoord_stride , rpr_int const * vertex_indices , rpr_int vidx_stride , rpr_int const * normal_indices , rpr_int nidx_stride , rpr_int const ** texcoord_indices , rpr_int const * tidx_stride , rpr_int const * num_face_vertices , size_t num_faces , rpr_mesh_info const * mesh_properties, Status* out_status) {
    typename RprApiTypeOf<Shape>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateMeshEx2(
        m_context,
        vertices, num_vertices, vertex_stride, 
        normals , num_normals , normal_stride, 
        perVertexFlag, num_perVertexFlags, perVertexFlag_stride, 
        numberOfTexCoordLayers, texcoords, num_texcoords, texcoord_stride, 
        vertex_indices, vidx_stride, 
        normal_indices, nidx_stride, 
        texcoord_indices, tidx_stride, 
        num_face_vertices, num_faces, 
        mesh_properties,
        &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Shape* newCppObj = new Shape(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Shape* Context::CreateShapeInstance(Shape* prototypeShape, Status* out_status) {
    typename RprApiTypeOf<Shape>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateInstance(m_context, GetRprObject(prototypeShape), &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Shape* newCppObj = new Shape(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Camera* Context::CreateCamera(Status* out_status) {
    typename RprApiTypeOf<Camera>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateCamera(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Camera* newCppObj = new Camera(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

FrameBuffer* Context::CreateFrameBuffer(FramebufferFormat const& format, FramebufferDesc const& fb_desc, Status* out_status) {
    typename RprApiTypeOf<FrameBuffer>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateFrameBuffer(m_context, format, &fb_desc, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    FrameBuffer* newCppObj = new FrameBuffer(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Image* Context::CreateImage(ImageFormat const& format, ImageDesc const& imageDesc, void const* data, Status* out_status) {
    typename RprApiTypeOf<Image>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateImage(m_context, format, &imageDesc, data, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Image* newCppObj = new Image(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Image* Context::CreateImageFromFileMemory(rpr_char const* extension, void const* data, size_t dataSizeByte, Status* out_status) {
    typename RprApiTypeOf<Image>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateImageFromFileMemory(m_context, extension, data, dataSizeByte, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Image* newCppObj = new Image(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Image* Context::CreateImageFromFile(rpr_char const* path, Status* out_status) {
    typename RprApiTypeOf<Image>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateImageFromFile(m_context, path, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Image* newCppObj = new Image(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

MaterialNode* Context::CreateMaterialNode(MaterialNodeType in_type, Status* out_status) {
    typename RprApiTypeOf<MaterialNode>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialSystemCreateNode(m_materialSystem, in_type, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    MaterialNode* newCppObj = new MaterialNode(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Buffer* Context::CreateBuffer(BufferDesc const& buffer_desc, void const* data, Status* out_status) {
    typename RprApiTypeOf<Buffer>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateBuffer(m_context, &buffer_desc, data, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Buffer* newCppObj = new Buffer(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

HeteroVolume* Context::CreateHeteroVolume(Status* out_status) {
    typename RprApiTypeOf<HeteroVolume>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateHeteroVolume(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    HeteroVolume* newCppObj = new HeteroVolume(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Grid* Context::CreateGrid(size_t gridSizeX, size_t gridSizeY, size_t gridSizeZ, void const* indicesList, size_t numberOfIndices, rpr_grid_indices_topology indicesListTopology, void const* gridData, size_t gridDataSizeByte, rpr_uint gridDataTopology___unused, Status* out_status) {
    typename RprApiTypeOf<Grid>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateGrid(m_context, &newRprObj, gridSizeX, gridSizeY, gridSizeZ, indicesList, numberOfIndices, indicesListTopology, gridData, gridDataSizeByte, gridDataTopology___unused)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Grid* newCppObj = new Grid(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Curve* Context::CreateCurve(size_t num_controlPoints, rpr_float const* controlPointsData, rpr_int controlPointsStride, size_t num_indices, rpr_uint curveCount, rpr_uint const* indicesData, rpr_float const* radius, rpr_float const* textureUV, rpr_int const* segmentPerCurve, CurveCreationFlags creationFlags, Status* out_status) {
    typename RprApiTypeOf<Curve>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateCurve(m_context, &newRprObj, num_controlPoints, controlPointsData, controlPointsStride, num_indices, curveCount, indicesData, radius, textureUV, segmentPerCurve, rpr_uint(creationFlags))
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Curve* newCppObj = new Curve(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

PostEffect* Context::CreatePostEffect(PostEffectType type, Status* out_status) {
    typename RprApiTypeOf<PostEffect>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreatePostEffect(m_context, type, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    PostEffect* newCppObj = new PostEffect(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Composite* Context::CreateComposite(CompositeType in_type, Status* out_status) {
    typename RprApiTypeOf<Composite>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateComposite(m_context, in_type, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Composite* newCppObj = new Composite(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

Lut* Context::CreateLutFromFile(const rpr_char* fileLutPath, Status* out_status) {
    typename RprApiTypeOf<Lut>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateLUTFromFile(m_context, fileLutPath, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    Lut* newCppObj = new Lut(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

SphereLight* Context::CreateSphereLight(Status* out_status) {
    typename RprApiTypeOf<SphereLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateSphereLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    SphereLight* newCppObj = new SphereLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

DiskLight* Context::CreateDiskLight(Status* out_status) {
    typename RprApiTypeOf<DiskLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateDiskLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    DiskLight* newCppObj = new DiskLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

PointLight* Context::CreatePointLight(Status* out_status) {
    typename RprApiTypeOf<PointLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreatePointLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    PointLight* newCppObj = new PointLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

SpotLight* Context::CreateSpotLight(Status* out_status) {
    typename RprApiTypeOf<SpotLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateSpotLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    SpotLight* newCppObj = new SpotLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

DirectionalLight* Context::CreateDirectionalLight(Status* out_status) {
    typename RprApiTypeOf<DirectionalLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateDirectionalLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    DirectionalLight* newCppObj = new DirectionalLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

EnvironmentLight* Context::CreateEnvironmentLight(Status* out_status) {
    typename RprApiTypeOf<EnvironmentLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateEnvironmentLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    EnvironmentLight* newCppObj = new EnvironmentLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

SkyLight* Context::CreateSkyLight(Status* out_status) {
    typename RprApiTypeOf<SkyLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateSkyLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    SkyLight* newCppObj = new SkyLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

IESLight* Context::CreateIESLight(Status* out_status) {
    typename RprApiTypeOf<IESLight>::value newRprObj = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextCreateIESLight(m_context, &newRprObj)
    RPR_CPPWRAPER_CALL_SUFFIX_CREATOR
    IESLight* newCppObj = new IESLight(*this, newRprObj);
    RPR_CPPWRAPER_SET_CUSTOM_PTR
    return newCppObj;
}

MaterialXNode* Context::CreateMaterialXNode(char const* xmlData, char const* basePath, int imageAlreadyCreated_count, char const** imageAlreadyCreated_paths, rpr::Image** imageAlreadyCreated_list, Status* out_status) {
 
    
/*
// ... WAITING STABLE  rprLoadMaterialX  API  BEFORE IMPLEMENTING THE C++ WRAPPER ...
 
    std::vector<rpr_image> imageAlreadyCreated_handles;
    if (imageAlreadyCreated_count > 0) {
        imageAlreadyCreated_handles.reserve(imageAlreadyCreated_count);
        for (int i = 0; i < imageAlreadyCreated_count; ++i) {
            imageAlreadyCreated_handles[i] = GetRprObject(imageAlreadyCreated_list[i]);
        }
    }

    rpr_material_node* nodes;
    rpr_uint numNodes;
    rpr_image* images;
    rpr_uint numImages;
    rpr_uint rootNodeIdx;

    RPR_CPPWRAPER_MUTEXLOCK;

    Status status = rprLoadMaterialX(
        m_context, m_materialSystem, xmlData, basePath,
        int(imageAlreadyCreated_handles.size()), imageAlreadyCreated_paths, imageAlreadyCreated_handles.data(),
        &nodes, &numNodes, &images, &numImages, &rootNodeIdx);
    if (status != RPR_SUCCESS) {
        if (out_status) {
            *out_status = status;
        }
        return nullptr;
    }
    return new MaterialXNode(*this, nodes, numNodes, images, numImages, rootNodeIdx);
*/


    return nullptr;
}

ContextObject::~ContextObject() {
    RPR_CPPWRAPER_MUTEXLOCK
    rprObjectDelete(m_rprObject);
}

Status ContextObject::SetName(rpr_char const* name) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprObjectSetName(m_rprObject, name)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Buffer::Buffer(Context& ctx, rpr_buffer obj) : ContextObject(ctx, obj) {}
Camera::Camera(Context& ctx, rpr_camera obj) : SceneObject(ctx, obj) {}
Composite::Composite(Context& ctx, rpr_composite obj) : ContextObject(ctx, obj) {}
Curve::Curve(Context& ctx, rpr_curve obj) : SceneObject(ctx, obj) {}
DirectionalLight::DirectionalLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}
EnvironmentLight::EnvironmentLight(Context& ctx, rpr_light obj) : Light(ctx, obj) {}
FrameBuffer::FrameBuffer(Context& ctx, rpr_framebuffer obj) : ContextObject(ctx, obj) {}
Grid::Grid(Context& ctx, rpr_grid obj) : ContextObject(ctx, obj) {}
HeteroVolume::HeteroVolume(Context& ctx, rpr_hetero_volume obj) : SceneObject(ctx, obj) {}
IESLight::IESLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}
Image::Image(Context& ctx, rpr_image obj) : ContextObject(ctx, obj) {}
Light::Light(Context& ctx, rpr_light obj) : SceneObject(ctx, obj) {}
RadiantLight::RadiantLight(Context& ctx, rpr_light obj) : Light(ctx, obj) {}
Lut::Lut(Context& ctx, rpr_lut obj) : ContextObject(ctx, obj) {}
MaterialNode::MaterialNode(Context& ctx, rpr_material_node obj) : ContextObject(ctx, obj) {}
PointLight::PointLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}
PostEffect::PostEffect(Context& ctx, rpr_post_effect obj) : ContextObject(ctx, obj) {}
Scene::Scene(Context& ctx, rpr_scene obj) : ContextObject(ctx, obj) {}
Shape::Shape(Context& ctx, rpr_shape obj) : SceneObject(ctx, obj) {}
SkyLight::SkyLight(Context& ctx, rpr_light obj) : Light(ctx, obj) {}
SpotLight::SpotLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}
DiskLight::DiskLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}
SphereLight::SphereLight(Context& ctx, rpr_light obj) : RadiantLight(ctx, obj) {}

Light::~Light() {}

Status Context::SetActivePlugin(rpr_int pluginID) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetActivePlugin(m_context, pluginID)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetInfo(ContextInfo context_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetInfo(m_context, context_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetParameterInfo(int param_idx, ParameterInfo parameter_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetParameterInfo(m_context, param_idx, parameter_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetAOV(Aov aov, FrameBuffer** out_fb) {
    rpr_framebuffer temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetAOV(m_context, aov, &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_fb = GetContextObjectFromRprApiObject<FrameBuffer>(temp);
    return RPR_SUCCESS;
}

Status Context::SetAOV(Aov aov, FrameBuffer* frame_buffer) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetAOV(m_context, aov, GetRprObject(frame_buffer))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetAOVindexLookup(rpr_int key, float colorR, float colorG, float colorB, float colorA) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetAOVindexLookup(m_context, key, colorR, colorG, colorB, colorA)
    RPR_CPPWRAPER_CALL_SUFFIX
}


Status Context::SetCuttingPlane(rpr_int index, float x, float y, float z, float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetCuttingPlane(m_context, index, x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}


Status Context::SetScene(Scene* scene) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetScene(m_context, GetRprObject(scene))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetScene(Scene** out_scene) {
    rpr_scene temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetScene(m_context, &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_scene = GetContextObjectFromRprApiObject<Scene>(temp);
    return RPR_SUCCESS;
}

Status Context::SetParameter(ContextInfo in_input, rpr_uint x) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKey1u(m_context, in_input, x)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetParameter(ContextInfo in_input, rpr_float x) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKey1f(m_context, in_input, x)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetParameter(ContextInfo in_input, rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKey3f(m_context, in_input, x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetParameter(ContextInfo in_input, rpr_float x, rpr_float y, rpr_float z, rpr_float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKey4f(m_context, in_input, x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetParameter(ContextInfo in_input, rpr_char const* value) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKeyString(m_context, in_input, value)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::SetParameter(ContextInfo in_input, void* value) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextSetParameterByKeyPtr(m_context, in_input, value)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::Render() {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextRender(m_context)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::AbortRender() {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextAbortRender(m_context)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::RenderTile(rpr_uint xmin, rpr_uint xmax, rpr_uint ymin, rpr_uint ymax) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextRenderTile(m_context, xmin, xmax, ymin, ymax)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::ClearMemory() {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextClearMemory(m_context)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetFunctionPtr(rpr_char const* function_name, void** out_function_ptr) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetFunctionPtr(m_context, function_name, out_function_ptr)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::ResolveFrameBuffer(FrameBuffer* src_frame_buffer, FrameBuffer* dst_frame_buffer, rpr_bool noDisplayGamma) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextResolveFrameBuffer(m_context, GetRprObject(src_frame_buffer), GetRprObject(dst_frame_buffer), noDisplayGamma)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::AttachPostEffect(PostEffect* effect) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextAttachPostEffect(m_context, GetRprObject(effect))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::DetachPostEffect(PostEffect* effect) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextDetachPostEffect(m_context, GetRprObject(effect))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetAttachedPostEffectCount(rpr_uint* nb) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetAttachedPostEffectCount(m_context, nb)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Context::GetAttachedPostEffect(rpr_uint i, PostEffect** out_effect) {
    rpr_post_effect temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprContextGetAttachedPostEffect(m_context, i, &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_effect = GetContextObjectFromRprApiObject<PostEffect>(temp);
    return RPR_SUCCESS;
}

Status Scene::Clear() {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneClear(GetRprObject(this))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Attach(Shape* shape) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneAttachShape(GetRprObject(this), GetRprObject(shape))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Detach(Shape* shape) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneDetachShape(GetRprObject(this), GetRprObject(shape))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Attach(HeteroVolume* heteroVolume) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneAttachHeteroVolume(GetRprObject(this), GetRprObject(heteroVolume))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Detach(HeteroVolume* heteroVolume) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneDetachHeteroVolume(GetRprObject(this), GetRprObject(heteroVolume))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Attach(Curve* curve) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneAttachCurve(GetRprObject(this), GetRprObject(curve))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Detach(Curve* curve) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneDetachCurve(GetRprObject(this), GetRprObject(curve))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Attach(Light* light) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneAttachLight(GetRprObject(this), GetRprObject(light))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::Detach(Light* light) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneDetachLight(GetRprObject(this), GetRprObject(light))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::SetEnvironmentLight(Light* in_light) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneSetEnvironmentLight(GetRprObject(this), GetRprObject(in_light))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::GetEnvironmentLight(Light** out_light) {
    rpr_light temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneGetEnvironmentLight(GetRprObject(this), &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_light = GetContextObjectFromRprApiObject<Light>(temp);
    return RPR_SUCCESS;
}

Status Scene::GetInfo(SceneInfo info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneGetInfo(GetRprObject(this), info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::SetBackgroundImage(Image* image) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneSetBackgroundImage(GetRprObject(this), GetRprObject(image))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::GetBackgroundImage(Image** out_image) {
    rpr_image temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneGetBackgroundImage(GetRprObject(this), &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_image = GetContextObjectFromRprApiObject<Image>(temp);
    return RPR_SUCCESS;
}

Status Scene::SetCameraRight(Camera* camera) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneSetCameraRight(GetRprObject(this), GetRprObject(camera))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::GetCameraRight(Camera** out_camera) {
    rpr_camera temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneGetCameraRight(GetRprObject(this), &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_camera = GetContextObjectFromRprApiObject<Camera>(temp);
    return RPR_SUCCESS;
}

Status Scene::SetCamera(Camera* camera) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneSetCamera(GetRprObject(this), GetRprObject(camera))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Scene::GetCamera(Camera** out_camera) {
    rpr_camera temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprSceneGetCamera(GetRprObject(this), &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_camera = GetContextObjectFromRprApiObject<Camera>(temp);
    return RPR_SUCCESS;
}

Status Camera::GetInfo(CameraInfo camera_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraGetInfo(GetRprObject(this), camera_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetFocalLength(rpr_float flength) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetFocalLength(GetRprObject(this), flength)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetLinearMotion(rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetLinearMotion(GetRprObject(this), x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetAngularMotion(rpr_float x, rpr_float y, rpr_float z, rpr_float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetAngularMotion(GetRprObject(this), x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetFocusDistance(rpr_float fdist) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetFocusDistance(GetRprObject(this), fdist)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetTransform(float const* transform, rpr_bool transpose) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetTransform(GetRprObject(this), transpose, (rpr_float*) transform)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetSensorSize(rpr_float width, rpr_float height) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetSensorSize(GetRprObject(this), width, height)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::LookAt(rpr_float posx, rpr_float posy, rpr_float posz, rpr_float atx, rpr_float aty, rpr_float atz, rpr_float upx, rpr_float upy, rpr_float upz) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraLookAt(GetRprObject(this), posx, posy, posz, atx, aty, atz, upx, upy, upz)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetFStop(rpr_float fstop) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetFStop(GetRprObject(this), fstop)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetApertureBlades(rpr_uint num_blades) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetApertureBlades(GetRprObject(this), num_blades)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetExposure(rpr_float exposure) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetExposure(GetRprObject(this), exposure)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetMode(CameraMode mode) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetMode(GetRprObject(this), mode)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetOrthoWidth(rpr_float width) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetOrthoWidth(GetRprObject(this), width)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetFocalTilt(rpr_float tilt) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetFocalTilt(GetRprObject(this), tilt)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetIPD(rpr_float ipd) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetIPD(GetRprObject(this), ipd)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetLensShift(rpr_float shiftx, rpr_float shifty) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetLensShift(GetRprObject(this), shiftx, shifty)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetTiltCorrection(rpr_float tiltX, rpr_float tiltY) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetTiltCorrection(GetRprObject(this), tiltX, tiltY)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetOrthoHeight(rpr_float height) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetOrthoHeight(GetRprObject(this), height)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetNearPlane(rpr_float near) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetNearPlane(GetRprObject(this), near)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetPostScale(rpr_float scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetPostScale(GetRprObject(this), scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Camera::SetFarPlane(rpr_float far) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCameraSetFarPlane(GetRprObject(this), far)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetMaterial(MaterialNode* material) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetMaterial(GetRprObject(this), GetRprObject(material))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetTransform(float const* transform, rpr_bool transpose) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetTransform(GetRprObject(this), transpose, transform)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetVertexValue(rpr_int setIndex, rpr_int const* indices, rpr_float const* values, rpr_int indicesCount) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetVertexValue(GetRprObject(this), setIndex, indices, values, indicesCount)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetPrimvar(rpr_uint key, rpr_float const * data, rpr_uint floatCount, rpr_uint componentCount, PrimvarInterpolationType interop) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetPrimvar(GetRprObject(this), key, data, floatCount, componentCount, interop )
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetSubdivisionFactor(rpr_uint factor) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetSubdivisionFactor(GetRprObject(this), factor)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetSubdivisionCreaseWeight(rpr_float factor) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetSubdivisionCreaseWeight(GetRprObject(this), factor)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetSubdivisionBoundaryInterop(SubdivBoundaryInterfopType type) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetSubdivisionBoundaryInterop(GetRprObject(this), type)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::AutoAdaptSubdivisionFactor(FrameBuffer* framebuffer, Camera* camera, rpr_int factor) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeAutoAdaptSubdivisionFactor(GetRprObject(this), GetRprObject(framebuffer), GetRprObject(camera), factor)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetDisplacementScale(rpr_float minscale, rpr_float maxscale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetDisplacementScale(GetRprObject(this), minscale, maxscale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetObjectGroupID(rpr_uint objectGroupID) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetObjectGroupID(GetRprObject(this), objectGroupID)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetObjectID(rpr_uint objectID) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetObjectID(GetRprObject(this), objectID)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetLightGroupID(rpr_uint lightGroupID) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetLightGroupID(GetRprObject(this), lightGroupID)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetLayerMask(rpr_uint layerMask) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetLayerMask(GetRprObject(this), layerMask)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetDisplacementMaterial(MaterialNode* materialNode) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetDisplacementMaterial(GetRprObject(this), GetRprObject(materialNode))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetMaterialFaces(MaterialNode* node, rpr_int const* face_indices, size_t num_faces) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetMaterialFaces(GetRprObject(this), GetRprObject(node), face_indices, num_faces)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetVolumeMaterial(MaterialNode* node) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetVolumeMaterial(GetRprObject(this), GetRprObject(node))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetLinearMotion(rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetLinearMotion(GetRprObject(this), x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetAngularMotion(rpr_float x, rpr_float y, rpr_float z, rpr_float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetAngularMotion(GetRprObject(this), x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetScaleMotion(rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetScaleMotion(GetRprObject(this), x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetVisibilityFlag(ShapeInfo visibilityFlag, rpr_bool visible) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetVisibilityFlag(GetRprObject(this), visibilityFlag, visible)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetVisibility(rpr_bool visible) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetVisibility(GetRprObject(this), visible)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetVisibilityInSpecular(rpr_bool visible) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetVisibilityInSpecular(GetRprObject(this), visible)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetShadowCatcher(rpr_bool shadowCatcher) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetShadowCatcher(GetRprObject(this), shadowCatcher)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetShadowColor(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetShadowColor(GetRprObject(this), r,g,b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetReflectionCatcher(rpr_bool reflectionCatcher) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetReflectionCatcher(GetRprObject(this), reflectionCatcher)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::MarkStatic(rpr_bool in_is_static) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeMarkStatic(GetRprObject(this), in_is_static)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::GetInfo(ShapeInfo arg1, size_t arg2, void* arg3, size_t* arg4) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeGetInfo(GetRprObject(this), arg1, arg2, arg3, arg4)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Shape::SetHeteroVolume(HeteroVolume* heteroVolume) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprShapeSetHeteroVolume(GetRprObject(this), GetRprObject(heteroVolume))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Light::SetTransform(float const* transform, rpr_bool transpose) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprLightSetTransform(GetRprObject(this), transpose, transform)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Light::SetGroupId(rpr_uint groupId) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprLightSetGroupId(GetRprObject(this), groupId)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Light::SetVisibilityFlag(LightInfo visibilityFlag, rpr_bool visible) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprLightSetVisibilityFlag(GetRprObject(this), visibilityFlag, visible)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Light::GetInfo(LightInfo info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprLightGetInfo(GetRprObject(this), info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status PointLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPointLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DiskLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDiskLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DiskLight::SetRadius(float radius) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDiskLightSetRadius(GetRprObject(this), radius)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DiskLight::SetAngle(float angle) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDiskLightSetAngle(GetRprObject(this), angle)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DiskLight::SetInnerAngle(float angle) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDiskLightSetInnerAngle(GetRprObject(this), angle)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SphereLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSphereLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SphereLight::SetRadius(float radius) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSphereLightSetRadius(GetRprObject(this), radius)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SpotLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSpotLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SpotLight::SetConeShape(rpr_float iangle, rpr_float oangle) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSpotLightSetConeShape(GetRprObject(this), iangle, oangle)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DirectionalLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDirectionalLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status DirectionalLight::SetShadowSoftnessAngle(rpr_float angle) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprDirectionalLightSetShadowSoftnessAngle(GetRprObject(this), angle)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::SetImage(Image* image) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightSetImage(GetRprObject(this), GetRprObject(image))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::SetIntensityScale(rpr_float intensity_scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightSetIntensityScale(GetRprObject(this), intensity_scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::AttachPortal(Scene* scene, Shape* portal) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightAttachPortal(GetRprObject(scene), GetRprObject(this), GetRprObject(portal))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::DetachPortal(Scene* scene, Shape* portal) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightDetachPortal(GetRprObject(scene), GetRprObject(this), GetRprObject(portal))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::SetEnvironmentLightOverride(EnvironmentOverride overrride, Light* in_light) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightSetEnvironmentLightOverride(GetRprObject(this), overrride, GetRprObject(in_light))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status EnvironmentLight::GetEnvironmentLightOverride(EnvironmentOverride overrride, Light** out_light) {
    rpr_light temp = nullptr;
    RPR_CPPWRAPER_CALL_PREFIX
    rprEnvironmentLightGetEnvironmentLightOverride(GetRprObject(this), overrride, &temp)
    RPR_CPPWRAPER_GET_CALL_SUFFIX
    *out_light = GetContextObjectFromRprApiObject<Light>(temp);
    return RPR_SUCCESS;
}

Status SkyLight::SetTurbidity(rpr_float turbidity) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightSetTurbidity(GetRprObject(this), turbidity)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SkyLight::SetAlbedo(rpr_float albedo) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightSetAlbedo(GetRprObject(this), albedo)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SkyLight::SetScale(rpr_float scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightSetScale(GetRprObject(this), scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SkyLight::SetDirection(rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightSetDirection(GetRprObject(this), x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SkyLight::AttachPortal(Scene* scene, Shape* portal) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightAttachPortal(GetRprObject(scene), GetRprObject(this), GetRprObject(portal))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status SkyLight::DetachPortal(Scene* scene, Shape* portal) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprSkyLightDetachPortal(GetRprObject(scene), GetRprObject(this), GetRprObject(portal))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status IESLight::SetRadiantPower(rpr_float r, rpr_float g, rpr_float b) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprIESLightSetRadiantPower3f(GetRprObject(this), r, g, b)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status IESLight::SetImageFromFile(rpr_char const* imagePath, rpr_int nx, rpr_int ny) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprIESLightSetImageFromFile(GetRprObject(this), imagePath, nx, ny)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status IESLight::SetImageFromIESdata(rpr_char const* iesData, rpr_int nx, rpr_int ny) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprIESLightSetImageFromIESdata(GetRprObject(this), iesData, nx, ny)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::GetInfo(ImageInfo image_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageGetInfo(GetRprObject(this), image_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::SetWrap(ImageWrapType type) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageSetWrap(GetRprObject(this), type)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::SetFilter(ImageFilterType type) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageSetFilter(GetRprObject(this), type)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::SetUDIM(rpr_uint tileIndex, Image* image) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageSetUDIM(GetRprObject(this), tileIndex, GetRprObject(image))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::SetGamma(rpr_float type) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageSetGamma(GetRprObject(this), type)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Image::SetMipmapEnabled(rpr_bool enabled) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprImageSetMipmapEnabled(GetRprObject(this), enabled)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Buffer::GetInfo(BufferInfo buffer_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprBufferGetInfo(GetRprObject(this), buffer_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::GetInfo(HeteroVolumeParameter heteroVol_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeGetInfo(GetRprObject(this), heteroVol_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetTransform(float const* transform, rpr_bool transpose) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetTransform(GetRprObject(this), transpose, transform)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetEmissionGrid(Grid* grid) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetEmissionGrid(GetRprObject(this), GetRprObject(grid))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetDensityGrid(Grid* grid) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetDensityGrid(GetRprObject(this), GetRprObject(grid))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetAlbedoGrid(Grid* grid) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetAlbedoGrid(GetRprObject(this), GetRprObject(grid))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetEmissionLookup(rpr_float const* ptr, rpr_uint n) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetEmissionLookup(GetRprObject(this), ptr, n)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetDensityLookup(rpr_float const* ptr, rpr_uint n) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetDensityLookup(GetRprObject(this), ptr, n)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetAlbedoLookup(rpr_float const* ptr, rpr_uint n) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetAlbedoLookup(GetRprObject(this), ptr, n)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetAlbedoScale(rpr_float scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetAlbedoScale(GetRprObject(this), scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetEmissionScale(rpr_float scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetEmissionScale(GetRprObject(this), scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status HeteroVolume::SetDensityScale(rpr_float scale) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprHeteroVolumeSetDensityScale(GetRprObject(this), scale)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Grid::GetInfo(GridParameter grid_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprGridGetInfo(GetRprObject(this), grid_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Curve::GetInfo(CurveParameter curve_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCurveGetInfo(GetRprObject(this), curve_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Curve::SetTransform(float const* transform, rpr_bool transpose) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCurveSetTransform(GetRprObject(this), transpose, transform)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Curve::SetVisibilityFlag(CurveParameter visibilityFlag, rpr_bool visible) {
	return RPR_ERROR_UNSUPPORTED;
    /*RPR_CPPWRAPER_CALL_PREFIX
    rprCurveSetVisibilityFlag(GetRprObject(this), visibilityFlag, visible)
    RPR_CPPWRAPER_CALL_SUFFIX*/
}

Status Curve::SetVisibility(rpr_bool visible) {
	return RPR_ERROR_UNSUPPORTED;
	/*RPR_CPPWRAPER_CALL_PREFIX
    rprCurveSetVisibility(GetRprObject(this), visible)
    RPR_CPPWRAPER_CALL_SUFFIX*/
}

Status Curve::SetMaterial(MaterialNode* material) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCurveSetMaterial(GetRprObject(this), GetRprObject(material))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status FrameBuffer::GetInfo(FramebufferInfo info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprFrameBufferGetInfo(GetRprObject(this), info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status FrameBuffer::Clear() {
    RPR_CPPWRAPER_CALL_PREFIX
    rprFrameBufferClear(GetRprObject(this))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status FrameBuffer::FillWithColor(rpr_float r, rpr_float g, rpr_float b, rpr_float a) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprFrameBufferFillWithColor(GetRprObject(this), r, g, b, a)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status FrameBuffer::SaveToFile(rpr_char const* file_path) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprFrameBufferSaveToFile(GetRprObject(this), file_path)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status FrameBuffer::SetLPE(const char* lpe) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprFrameBufferSetLPE(GetRprObject(this), lpe)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, MaterialNode* in_input_node) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputNByKey(GetRprObject(this), in_input, GetRprObject(in_input_node))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, rpr_float in_value_x, rpr_float in_value_y, rpr_float in_value_z, rpr_float in_value_w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputFByKey(GetRprObject(this), in_input, in_value_x, in_value_y, in_value_z, in_value_w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, rpr_uint in_value) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputUByKey(GetRprObject(this), in_input, in_value)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, Image* image) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputImageDataByKey(GetRprObject(this), in_input, GetRprObject(image))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, Light* light) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputLightDataByKey(GetRprObject(this), in_input, GetRprObject(light))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::SetInput(MaterialNodeInput in_input, Buffer* buffer) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeSetInputBufferDataByKey(GetRprObject(this), in_input, GetRprObject(buffer))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::GetInfo(MaterialNodeInfo in_info, size_t in_size, void* in_data, size_t* out_size) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeGetInfo(GetRprObject(this), in_info, in_size, in_data, out_size)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status MaterialNode::GetInputInfo(rpr_int in_input_idx, MaterialNodeInputInfo in_info, size_t in_size, void* in_data, size_t* out_size) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprMaterialNodeGetInputInfo(GetRprObject(this), in_input_idx, in_info, in_size, in_data, out_size)
    RPR_CPPWRAPER_CALL_SUFFIX
}

MaterialXNode::MaterialXNode(
    Context& ctx,
    rpr_material_node* nodes,
    rpr_uint numNodes,
    rpr_image* images,
    rpr_uint numImages,
    rpr_uint rootNodeIdx)
    : MaterialNode(ctx, nodes[rootNodeIdx])
    , m_auxiliaryNodes(nodes)
    , m_numAuxiliaryNodes(numNodes)
    , m_auxiliaryImages(images)
    , m_numAuxiliaryImages(numImages) {
    m_auxiliaryNodes[rootNodeIdx] = nullptr;
}

MaterialXNode::~MaterialXNode() {
    for (rpr_uint i = 0; i < m_numAuxiliaryNodes; ++i) {
        if (m_auxiliaryNodes[i]) {
            rprObjectDelete(m_auxiliaryNodes[i]);
        }
    }
    for (rpr_uint i = 0; i < m_numAuxiliaryImages; ++i) {
        if (m_auxiliaryImages[i]) {
            rprObjectDelete(m_auxiliaryImages[i]);
        }
    }
    rprLoadMaterialX_free(m_auxiliaryNodes, m_auxiliaryImages);
}

Status PostEffect::SetParameter(rpr_char const* name, rpr_uint x) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPostEffectSetParameter1u(GetRprObject(this), name, x)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status PostEffect::SetParameter(rpr_char const* name, rpr_float x) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPostEffectSetParameter1f(GetRprObject(this), name, x)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status PostEffect::SetParameter(rpr_char const* name, rpr_float x, rpr_float y, rpr_float z) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPostEffectSetParameter3f(GetRprObject(this), name, x, y, z)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status PostEffect::SetParameter(rpr_char const* name, rpr_float x, rpr_float y, rpr_float z, rpr_float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPostEffectSetParameter4f(GetRprObject(this), name, x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status PostEffect::GetInfo(PostEffectInfo info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprPostEffectGetInfo(GetRprObject(this), info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, FrameBuffer* input) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInputFb(GetRprObject(this), inputName, GetRprObject(input))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, Composite* input) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInputC(GetRprObject(this), inputName, GetRprObject(input))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, Lut* input) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInputLUT(GetRprObject(this), inputName, GetRprObject(input))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, float x, float y, float z, float w) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInput4f(GetRprObject(this), inputName, x, y, z, w)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, rpr_uint value) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInput1u(GetRprObject(this), inputName, value)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::SetInput(const rpr_char* inputName, MaterialNodeArithmeticOperation op) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeSetInputOp(GetRprObject(this), inputName, op)
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::Compute(FrameBuffer* fb) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeCompute(GetRprObject(this), GetRprObject(fb))
    RPR_CPPWRAPER_CALL_SUFFIX
}

Status Composite::GetInfo(CompositeInfo composite_info, size_t size, void* data, size_t* size_ret) {
    RPR_CPPWRAPER_CALL_PREFIX
    rprCompositeGetInfo(GetRprObject(this), composite_info, size, data, size_ret)
    RPR_CPPWRAPER_CALL_SUFFIX
}

} // namespace rpr
