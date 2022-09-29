/*****************************************************************************\
*
*  Module Name    RadeonProRender.hpp
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#pragma once

#ifndef RPR_API_USE_HEADER_V2
#error "RPR C++ wrapper requires v2 RPR headers"
#endif // RPR_API_USE_HEADER_V2
#include "RadeonProRender.h"

#include <exception>
#include <mutex>

// for developers who wants to attach extra data to the RPR Nodes, just define RPR_USE_CUSTOMIZED_NODE and create your own BaseNode_Customized class inside rprCustomizedNode.hpp

#ifdef RPR_USE_CUSTOMIZED_NODE
#include <rprCustomizedNode.hpp>
#endif

/// By default, each call is thread-safe (Context mutex is locked every time)
/// Define RPR_CPPWRAPER_DISABLE_MUTEXLOCK to disable this behavior

namespace rpr {

enum CurveCreationFlags {
    kCurveCreationFlagsNone = 0,
    kCurveCreationFlagTapered = 1 << 0,
};

using Aov = rpr_aov;
using BufferDesc = rpr_buffer_desc;
using BufferInfo = rpr_buffer_info;
using CameraInfo = rpr_camera_info;
using CameraMode = rpr_camera_mode;
using ComponentType = rpr_component_type;
using CompositeInfo = rpr_composite_info;
using CompositeType = rpr_composite_type;
using ContextInfo = rpr_context_info;
using ContextProperties = rpr_context_properties;
using CreationFlags = rpr_creation_flags;
using CurveParameter = rpr_curve_parameter;
using EnvironmentOverride = rpr_environment_override;
using FramebufferDesc = rpr_framebuffer_desc;
using FramebufferFormat = rpr_framebuffer_format;
using FramebufferInfo = rpr_framebuffer_info;
using GridParameter = rpr_grid_parameter;
using HeteroVolumeParameter = rpr_hetero_volume_parameter;
using ImageDesc = rpr_image_desc;
using ImageFilterType = rpr_image_filter_type;
using ImageFormat = rpr_image_format;
using ImageInfo = rpr_image_info;
using ImageWrapType = rpr_image_wrap_type;
using PrimvarInterpolationType = rpr_primvar_interpolation_type;
using LightInfo = rpr_light_info;
using MaterialNodeArithmeticOperation = rpr_material_node_arithmetic_operation;
using MaterialNodeInfo = rpr_material_node_info;
using MaterialNodeInput = rpr_material_node_input;
using MaterialNodeInputInfo = rpr_material_node_input_info;
using MaterialNodeType = rpr_material_node_type;
using ParameterInfo = rpr_parameter_info;
using PostEffectInfo = rpr_post_effect_info;
using PostEffectType = rpr_post_effect_type;
using SceneInfo = rpr_scene_info;
using ShapeInfo = rpr_shape_info;
using SubdivBoundaryInterfopType = rpr_subdiv_boundary_interfop_type;
using Status = rpr_status;

class Context;
class Buffer;
class Camera;
class Composite;
class Curve;
class SphereLight;
class DiskLight;
class DirectionalLight;
class EnvironmentLight;
class FrameBuffer;
class Grid;
class HeteroVolume;
class IESLight;
class Image;
class Light;
class Lut;
class MaterialNode;
class MaterialXNode;
class PointLight;
class PostEffect;
class Scene;
class Shape;
class SkyLight;
class SpotLight;

class Context {
public:
    ~Context();

    // context creator
    static Context* Create(rpr_int apiVersion, rpr_int* pluginIDs, size_t pluginCount, CreationFlags creationFlags, ContextProperties const* properties, rpr_char const* cachePath, Status* out_status = nullptr);

    // object creators
    Scene* CreateScene(Status* out_status = nullptr);
    Shape* CreateShape(rpr_float const* vertices, size_t numVertices, rpr_int vertexStride, rpr_float const* normals, size_t numNormals, rpr_int normalStride, rpr_float const* texcoords, size_t numTexcoords, rpr_int texcoordStride, rpr_int const* vertexIndices, rpr_int vidxStride, rpr_int const* normalIndices, rpr_int nidxStride, rpr_int const* texcoordIndices, rpr_int tidxStride, rpr_int const* numFaceVertices, size_t numFaces, Status* out_status = nullptr);
    Shape* CreateShape(rpr_float const* vertices, size_t numVertices, rpr_int vertexStride, rpr_float const* normals, size_t numNormals, rpr_int normalStride, rpr_int const * perVertexFlag, size_t numPerVertexFlags, rpr_int perVertexFlagStride, rpr_int numberOfTexCoordLayers, rpr_float const ** texcoords, size_t const * numTexcoords, rpr_int const * texcoordStride, rpr_int const* vertexIndices, rpr_int vidxStride, rpr_int const * normalIndices, rpr_int nidxStride, rpr_int const ** texcoordIndices, rpr_int const * tidxStride, rpr_int const * numFaceVertices, size_t numFaces, rpr_mesh_info const * meshProperties, Status* out_status = nullptr);
    Shape* CreateShapeInstance(Shape* prototypeShape, Status* out_status = nullptr);
    Camera* CreateCamera(Status* out_status = nullptr);
    FrameBuffer* CreateFrameBuffer(FramebufferFormat const& format, FramebufferDesc const& fbDesc, Status* out_status = nullptr);
    Image* CreateImage(ImageFormat const& format, ImageDesc const& imageDesc, void const* data, Status* out_status = nullptr);
    Image* CreateImageFromFileMemory(rpr_char const* extension, void const* data, size_t dataSizeByte, Status* out_status = nullptr);
    Image* CreateImageFromFile(rpr_char const* path, Status* out_status = nullptr);
    MaterialNode* CreateMaterialNode(MaterialNodeType type, Status* out_status = nullptr);
    Buffer* CreateBuffer(BufferDesc const& bufferDesc, void const* data, Status* out_status = nullptr);
    HeteroVolume* CreateHeteroVolume(Status* out_status = nullptr);
    Grid* CreateGrid(size_t gridSizeX, size_t gridSizeY, size_t gridSizeZ, void const* indicesList, size_t numberOfIndices, rpr_grid_indices_topology indicesListTopology, void const* gridData, size_t gridDataSizeByte, rpr_uint gridDataTopology___unused, Status* out_status = nullptr);
    Curve* CreateCurve(size_t numControlPoints, float const* controlPointsData, rpr_int controlPointsStride, size_t numIndices, rpr_uint curveCount, rpr_uint const* indicesData, float const* radius, float const* textureUV, rpr_int const* segmentPerCurve, CurveCreationFlags creationFlags, Status* out_status = nullptr);
    PostEffect* CreatePostEffect(PostEffectType type, Status* out_status = nullptr);
    Composite* CreateComposite(CompositeType type, Status* out_status = nullptr);
    Lut* CreateLutFromFile(const rpr_char* fileLutPath, Status* out_status = nullptr);
    SphereLight* CreateSphereLight(Status* out_status = nullptr);
    DiskLight* CreateDiskLight(Status* out_status = nullptr);
    PointLight* CreatePointLight(Status* out_status = nullptr);
    SpotLight* CreateSpotLight(Status* out_status = nullptr);
    DirectionalLight* CreateDirectionalLight(Status* out_status = nullptr);
    EnvironmentLight* CreateEnvironmentLight(Status* out_status = nullptr);
    SkyLight* CreateSkyLight(Status* out_status = nullptr);
    IESLight* CreateIESLight(Status* out_status = nullptr);
    MaterialXNode* CreateMaterialXNode(char const* xmlData, char const* basePath, int imageAlreadyCreated_count, char const** imageAlreadyCreated_paths, rpr::Image** imageAlreadyCreated_list, Status* out_status = nullptr);

    Status SetActivePlugin(rpr_int pluginID);
    Status GetInfo(ContextInfo contextInfo, size_t size, void* data, size_t* size_ret);
    Status GetParameterInfo(int paramIdx, ParameterInfo parameterInfo, size_t size, void* data, size_t* size_ret);
    Status GetAOV(Aov aov, FrameBuffer** out_fb);
    Status SetAOV(Aov aov, FrameBuffer* frameBuffer);
    Status SetAOVindexLookup(rpr_int key, float colorR, float colorG, float colorB, float colorA);
    Status SetCuttingPlane(rpr_int index, float x, float y, float z, float w);
    Status SetScene(Scene* scene);
    Status GetScene(Scene** out_scene);
    Status SetParameter(ContextInfo input, rpr_uint x);
    Status SetParameter(ContextInfo input, float x);
    Status SetParameter(ContextInfo input, float x, float y, float z);
    Status SetParameter(ContextInfo input, float x, float y, float z, float w);
    Status SetParameter(ContextInfo input, rpr_char const* value);
    Status SetParameter(ContextInfo input, void* value);
    Status Render();
    Status AbortRender();
    Status RenderTile(rpr_uint xmin, rpr_uint xmax, rpr_uint ymin, rpr_uint ymax);
    Status ClearMemory();
    Status GetFunctionPtr(rpr_char const* functionName, void** out_functionPtr);
    Status ResolveFrameBuffer(FrameBuffer* srcFrameBuffer, FrameBuffer* dstFrameBuffer, rpr_bool noDisplayGamma);
    Status AttachPostEffect(PostEffect* effect);
    Status DetachPostEffect(PostEffect* effect);
    Status GetAttachedPostEffectCount(rpr_uint* nb);
    Status GetAttachedPostEffect(rpr_uint i, PostEffect** out_effect);

    std::mutex& GetMutex() { return m_apiCallMutex; }

    // Compiler encounters an ambiguous situation when it tries to select SetParameter overload when the second argument is of integral type (e.g. int could be implicitly cast to both rpr_uint and float)
    // We explicitly prefer SetParameter(_, rpr_uint) overload in this situation
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, Status>::type SetParameter(ContextInfo input, T integer) {
        return SetParameter(input, static_cast<rpr_uint>(integer));
    }

private:
    Context() = default;
    Context& GetContext() { return *this; }

private:
    std::mutex m_apiCallMutex; // 1 mutex per context

    rpr_context m_context = nullptr;
    rpr_material_system m_materialSystem = nullptr; // one material system per context is the good design for any project.

    friend rpr_context GetRprObject(rpr::Context* ptr);
};

inline rpr_context GetRprObject(rpr::Context* ptr) {
    return ptr ? ptr->m_context : nullptr;
}

#ifndef RPR_USE_CUSTOMIZED_NODE
class BaseNode_Customized {
};
#endif

template<typename T>
struct RprApiTypeOf;

#define DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(name, rprApiType) \
    template <> \
    struct RprApiTypeOf<name> { using value = rprApiType; };

DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Buffer, rpr_buffer);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Camera, rpr_camera);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Composite, rpr_composite);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Curve, rpr_curve);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(DirectionalLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(EnvironmentLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(FrameBuffer, rpr_framebuffer);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Grid, rpr_grid);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(HeteroVolume, rpr_hetero_volume);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(IESLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Image, rpr_image);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Light, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Lut, rpr_lut);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(MaterialNode, rpr_material_node);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(PointLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(PostEffect, rpr_post_effect);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Scene, rpr_scene);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(Shape, rpr_shape);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(SkyLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(SpotLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(DiskLight, rpr_light);
DEFINE_CONTEXT_OBJECT_RPR_API_TYPE(SphereLight, rpr_light);

class ContextObject : BaseNode_Customized {
public:
    virtual ~ContextObject();

    Status SetName(rpr_char const* name);

    Context& GetContext() { return m_context; }

protected:
    ContextObject(Context& context, void* rprObject) : m_context(context), m_rprObject(rprObject) {}

    template<typename Wrapper>
    friend typename RprApiTypeOf<Wrapper>::value GetRprObject(Wrapper* ptr);

private:
    Context& m_context;
    void* m_rprObject = nullptr;
};

template<typename Wrapper>
typename RprApiTypeOf<Wrapper>::value GetRprObject(Wrapper* ptr) {
    return ptr ? static_cast<typename RprApiTypeOf<Wrapper>::value>(ptr->m_rprObject) : nullptr;
}

class Scene : public ContextObject {
    friend class Context;

public:
    virtual ~Scene() = default;

    Status Clear();
    Status Attach(Shape* shape);
    Status Detach(Shape* shape);
    Status Attach(HeteroVolume* heteroVolume);
    Status Detach(HeteroVolume* heteroVolume);
    Status Attach(Curve* curve);
    Status Detach(Curve* curve);
    Status Attach(Light* light);
    Status Detach(Light* light);
    Status SetEnvironmentLight(Light* light);
    Status GetEnvironmentLight(Light** out_light);
    Status GetInfo(SceneInfo info, size_t size, void* data, size_t* size_ret);
    Status SetBackgroundImage(Image* image);
    Status GetBackgroundImage(Image** out_image);
    Status SetCameraRight(Camera* camera);
    Status GetCameraRight(Camera** out_camera);
    Status SetCamera(Camera* camera);
    Status GetCamera(Camera** out_camera);
private:
    Scene(Context& ctx, rpr_scene obj);
};

class SceneObject : public ContextObject {
public:
    virtual Status SetTransform(float const* transform, rpr_bool transpose) = 0;

protected:
    SceneObject(Context& context, void* rprObject) : ContextObject(context, rprObject) {}
};

class Camera : public SceneObject {
    friend class Context;

public:
    virtual ~Camera() = default;

    Status GetInfo(CameraInfo cameraInfo, size_t size, void* data, size_t* size_ret);
    Status SetFocalLength(float flength);
    Status SetLinearMotion(float x, float y, float z);
    Status SetAngularMotion(float x, float y, float z, float w);
    Status SetFocusDistance(float fdist);
    Status SetSensorSize(float width, float height);
    Status LookAt(float posx, float posy, float posz, float atx, float aty, float atz, float upx, float upy, float upz);
    Status SetFStop(float fstop);
    Status SetApertureBlades(rpr_uint numBlades);
    Status SetExposure(float exposure);
    Status SetMode(CameraMode mode);
    Status SetOrthoWidth(float width);
    Status SetFocalTilt(float tilt);
    Status SetIPD(float ipd);
    Status SetLensShift(float shiftx, float shifty);
    Status SetTiltCorrection(float tiltX, float tiltY);
    Status SetOrthoHeight(float height);
    Status SetNearPlane(float near);
    Status SetFarPlane(float far);
    Status SetPostScale(float scale);

    Status SetTransform(float const* transform, rpr_bool transpose) override;
private:
    Camera(Context& ctx, rpr_camera obj);
};

class Shape : public SceneObject {
    friend class Context;

public:
    virtual ~Shape() = default;

    Status SetVertexValue(rpr_int setIndex, rpr_int const* indices, rpr_float const* values, rpr_int indicesCount);
    Status SetSubdivisionFactor(rpr_uint factor);
    Status SetSubdivisionCreaseWeight(rpr_float factor);
    Status SetSubdivisionBoundaryInterop(SubdivBoundaryInterfopType type);
    Status AutoAdaptSubdivisionFactor(FrameBuffer* framebuffer, Camera* camera, rpr_int factor);
    Status SetDisplacementScale(rpr_float minscale, rpr_float maxscale);
    Status SetObjectGroupID(rpr_uint objectGroupID);
    Status SetObjectID(rpr_uint objectID);
    Status SetLightGroupID(rpr_uint lightGroupID);
    Status SetLayerMask(rpr_uint layerMask);
    Status SetDisplacementMaterial(MaterialNode* materialNode);
    Status SetMaterialFaces(MaterialNode* node, rpr_int const* faceIndices, size_t numFaces);
    Status SetVolumeMaterial(MaterialNode* node);
    Status SetLinearMotion(rpr_float x, rpr_float y, rpr_float z);
    Status SetAngularMotion(rpr_float x, rpr_float y, rpr_float z, rpr_float w);
    Status SetScaleMotion(rpr_float x, rpr_float y, rpr_float z);
    Status SetVisibilityFlag(ShapeInfo visibilityFlag, rpr_bool visible);
    Status SetVisibility(rpr_bool visible);
    Status SetVisibilityInSpecular(rpr_bool visible);
    Status SetShadowCatcher(rpr_bool shadowCatcher);
    Status SetShadowColor(rpr_float r, rpr_float g, rpr_float b);
    Status SetReflectionCatcher(rpr_bool reflectionCatcher);
    Status MarkStatic(rpr_bool isStatic);
    Status GetInfo(ShapeInfo arg1, size_t arg2, void* arg3, size_t* arg4);
    Status SetHeteroVolume(HeteroVolume* heteroVolume);
    Status SetMaterial(MaterialNode* material);
    Status SetPrimvar(rpr_uint key, rpr_float const * data, rpr_uint floatCount, rpr_uint componentCount, PrimvarInterpolationType interop);

    Status SetTransform(float const* transform, rpr_bool transpose) override;

private:
    Shape(Context& ctx, rpr_shape obj);
};

class Light : public SceneObject {
    friend class Context;

public:
    virtual ~Light() = 0; // 'Light' is an abstract class.

    Status SetGroupId(rpr_uint groupId);
    Status GetInfo(LightInfo info, size_t size, void* data, size_t* size_ret);
    Status SetVisibilityFlag(LightInfo visibilityFlag, rpr_bool visible);

    Status SetTransform(float const* transform, rpr_bool transpose) override;

protected:
    Light(Context& ctx, rpr_light obj);
};

class RadiantLight : public Light {
public:
    ~RadiantLight() override = default;

    virtual Status SetRadiantPower(float r, float g, float b) = 0;

protected:
    RadiantLight(Context& ctx, rpr_light obj);
};

class PointLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~PointLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
private:
    PointLight(Context& ctx, rpr_light obj);
};

class SpotLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~SpotLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
    Status SetConeShape(float iangle, float oangle);
private:
    SpotLight(Context& ctx, rpr_light obj);
};

class DiskLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~DiskLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
    Status SetRadius(float radius);
    Status SetAngle(float angle);
    Status SetInnerAngle(float angle);
private:
    DiskLight(Context& ctx, rpr_light obj);
};

class SphereLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~SphereLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
    Status SetRadius(float radius);
private:
    SphereLight(Context& ctx, rpr_light obj);
};

class DirectionalLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~DirectionalLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
    Status SetShadowSoftnessAngle(float angle);
private:
    DirectionalLight(Context& ctx, rpr_light obj);
};

class EnvironmentLight : public Light {
    friend class Context;

public:
    virtual ~EnvironmentLight() = default;

    Status SetImage(Image* image);
    Status SetIntensityScale(float intensityScale);
    Status AttachPortal(Scene* scene, Shape* portal);
    Status DetachPortal(Scene* scene, Shape* portal);
    Status SetEnvironmentLightOverride(EnvironmentOverride overrride, Light* light);
    Status GetEnvironmentLightOverride(EnvironmentOverride overrride, Light** out_light);
private:
    EnvironmentLight(Context& ctx, rpr_light obj);
};

class SkyLight : public Light {
    friend class Context;

public:
    virtual ~SkyLight() = default;

    Status SetTurbidity(float turbidity);
    Status SetAlbedo(float albedo);
    Status SetScale(float scale);
    Status SetDirection(float x, float y, float z);
    Status AttachPortal(Scene* scene, Shape* portal);
    Status DetachPortal(Scene* scene, Shape* portal);
private:
    SkyLight(Context& ctx, rpr_light obj);
};

class IESLight final : public RadiantLight {
    friend class Context;

public:
    virtual ~IESLight() = default;

    Status SetRadiantPower(float r, float g, float b) override;
    Status SetImageFromFile(rpr_char const* imagePath, rpr_int nx, rpr_int ny);
    Status SetImageFromIESdata(rpr_char const* iesData, rpr_int nx, rpr_int ny);
private:
    IESLight(Context& ctx, rpr_light obj);
};

class HeteroVolume : public SceneObject {
    friend class Context;

public:
    virtual ~HeteroVolume() = default;

    Status GetInfo(HeteroVolumeParameter parameter, size_t size, void* data, size_t* size_ret);
    Status SetEmissionGrid(Grid* grid);
    Status SetDensityGrid(Grid* grid);
    Status SetAlbedoGrid(Grid* grid);
    Status SetEmissionLookup(float const* ptr, rpr_uint n);
    Status SetDensityLookup(float const* ptr, rpr_uint n);
    Status SetAlbedoLookup(float const* ptr, rpr_uint n);
    Status SetAlbedoScale(float scale);
    Status SetEmissionScale(float scale);
    Status SetDensityScale(float scale);

    Status SetTransform(float const* transform, rpr_bool transpose) override;

private:
    HeteroVolume(Context& ctx, rpr_hetero_volume obj);
};

class Grid : public ContextObject {
    friend class Context;

public:
    virtual ~Grid() = default;

    Status GetInfo(GridParameter info, size_t size, void* data, size_t* size_ret);

private:
    Grid(Context& ctx, rpr_grid obj);
};

class Curve : public SceneObject {
    friend class Context;

public:
    virtual ~Curve() = default;

    Status GetInfo(CurveParameter info, size_t size, void* data, size_t* size_ret);
    Status SetVisibilityFlag(CurveParameter visibilityFlag, rpr_bool visible);
    Status SetVisibility(rpr_bool visible);
    Status SetMaterial(MaterialNode* material);

    Status SetTransform(float const* transform, rpr_bool transpose) override;

private:
    Curve(Context& ctx, rpr_curve obj);
};

class FrameBuffer : public ContextObject {
    friend class Context;

public:
    virtual ~FrameBuffer() = default;

    Status GetInfo(FramebufferInfo info, size_t size, void* data, size_t* size_ret);
    Status Clear();
    Status FillWithColor(float r, float g, float b, float a);
    Status SaveToFile(rpr_char const* filePath);
    Status SetLPE(const char* lpe);

private:
    FrameBuffer(Context& ctx, rpr_framebuffer obj);
};

class Image : public ContextObject {
    friend class Context;

public:
    virtual ~Image() = default;

    Status GetInfo(ImageInfo imageInfo, size_t size, void* data, size_t* size_ret);
    Status SetWrap(ImageWrapType type);
    Status SetFilter(ImageFilterType type);
    Status SetUDIM(rpr_uint tileIndex, Image* image);
    Status SetGamma(float type);
    Status SetMipmapEnabled(rpr_bool enabled);

private:
    Image(Context& ctx, rpr_image obj);
};

class Buffer : public ContextObject {
    friend class Context;

public:
    virtual ~Buffer() = default;

    Status GetInfo(BufferInfo info, size_t size, void* data, size_t* size_ret);

private:
    Buffer(Context& ctx, rpr_buffer obj);
};

class MaterialNode : public ContextObject {
    friend class Context;

public:
    virtual ~MaterialNode() = default;

    Status SetInput(MaterialNodeInput input, MaterialNode* node);
    Status SetInput(MaterialNodeInput input, float valueX, float valueY, float valueZ, float valueW);
    Status SetInput(MaterialNodeInput input, rpr_uint value);
    Status SetInput(MaterialNodeInput input, Image* image);
    Status SetInput(MaterialNodeInput input, Buffer* buffer);
    Status SetInput(MaterialNodeInput input, Light* light);
    Status GetInfo(MaterialNodeInfo info, size_t size, void* data, size_t* size_ret);
    Status GetInputInfo(rpr_int inputIdx, MaterialNodeInputInfo info, size_t size, void* data, size_t* out_size);

protected:
    MaterialNode(Context& ctx, rpr_material_node obj);
};

class MaterialXNode : public MaterialNode {
public:
    virtual ~MaterialXNode() override;

private:
    friend class Context;
    MaterialXNode(
        Context& ctx,
        rpr_material_node* nodes,
        rpr_uint numNodes,
        rpr_image* images,
        rpr_uint numImages,
        rpr_uint rootNodeIdx);

    rpr_material_node* m_auxiliaryNodes;
    rpr_uint m_numAuxiliaryNodes;
    rpr_image* m_auxiliaryImages;
    rpr_uint m_numAuxiliaryImages;
};

class PostEffect : public ContextObject {
    friend class Context;

public:
    virtual ~PostEffect() = default;

    Status SetParameter(rpr_char const* name, rpr_uint x);
    Status SetParameter(rpr_char const* name, float x);
    Status SetParameter(rpr_char const* name, float x, float y, float z);
    Status SetParameter(rpr_char const* name, float x, float y, float z, float w);
    Status GetInfo(PostEffectInfo info, size_t size, void* data, size_t* size_ret);

private:
    PostEffect(Context& ctx, rpr_post_effect obj);
};

class Composite : public ContextObject {
    friend class Context;

public:
    virtual ~Composite() = default;

    Status SetInput(const rpr_char* name, FrameBuffer* input);
    Status SetInput(const rpr_char* name, Composite* input);
    Status SetInput(const rpr_char* name, Lut* input);
    Status SetInput(const rpr_char* name, float x, float y, float z, float w);
    Status SetInput(const rpr_char* name, rpr_uint value);
    Status SetInput(const rpr_char* name, MaterialNodeArithmeticOperation op);
    Status Compute(FrameBuffer* fb);
    Status GetInfo(CompositeInfo info, size_t size, void* data, size_t* size_ret);

private:
    Composite(Context& ctx, rpr_composite obj);
};

class Lut : public ContextObject {
    friend class Context;

public:
    virtual ~Lut() = default;

    Status CreateFromFile(const rpr_char* fileLutPath);
    Status CreateFromData(const rpr_char* lutData);

private:
    Lut(Context& ctx, rpr_lut obj);
};

} // namespace rpr
