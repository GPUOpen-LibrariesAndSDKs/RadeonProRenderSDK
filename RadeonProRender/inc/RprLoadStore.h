/*****************************************************************************\
*
*  Module Name    RprLoadStore.h
*  Project        RRP Load Store library
*
*  Description    RRP load store library interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RPRLOADSTORE_H
#define __RPRLOADSTORE_H


#include "RadeonProRender.h"

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif


#define RPRLOADSTORE_PARAMETER_TYPE_UNDEF 0x0 
#define RPRLOADSTORE_PARAMETER_TYPE_INT 0x1 
#define RPRLOADSTORE_PARAMETER_TYPE_FLOAT 0x2 

typedef void * RPRS_context;


/** 
*  RPRLOADSTORE_EXPORTFLAG_EXTERNALFILES : buffers above a certain size will be externalized in a file
*  RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_1 : image data will be lossless compressed during export.
*  RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_2 : image data will be lossy compressed during export.
*     COMPRESS_IMAGE_LEVEL_1 and COMPRESS_IMAGE_LEVEL_2 can't be used together - using those flag will increase the time to import/export the RPRS file
*  RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_NORMALS : store vertices normals as HALF instead of FLOAT
*  RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_UV : store vertices UVs as HALF instead of FLOAT
*
*  RPRLOADSTORE_EXPORTFLAG_EMBED_FILE_IMAGES_USING_OBJECTNAME : if enabled, the Exporter assumes that names of all rpr_image ( set by rprObjectSetName ) 
*          represent file paths of the original images. Instead of using the image data from RPR, the Exporter will simply use the data from the file.
*          If the file path is not found, then data from RPR is used to export the image.
*          Advantage of using this flag is that we ensure to attach the original image files into the RPRS, instead of using an image compressed/modified by RPR.
*          For Unicode, you can encode the name with UTF-8
*  
*  RPRLOADSTORE_EXPORTFLAG_USE_IMAGE_CACHE : If enabled Exporter tries to use the native image cache compiled by Northstar during rendering.
*                                            ( Those cache files are usually in the cache folder, named *.ns.bin", 1 file per rpr_image )
*                                            If the cache file is found, export uses this cached data in priority and ignores COMPRESS_IMAGE_LEVEL_1, COMPRESS_IMAGE_LEVEL_2, EMBED_FILE_IMAGES_USING_OBJECTNAME for this image only.
*                                            If the cache file doesn't exist, it does a classic image export.
*                                            Advantages: Export and Import will be faster as we directly manage compiled image.
*                                            Drawbacks: image parameters (like color space) can't be changed in an imported RPRS file that used this flag.
*
*  RPRLOADSTORE_EXPORTFLAG_ONLY_EXPORT_ATTACHED_RENDER_LAYERS : If enabled, only the shape and lights attached to the selected Render Layer list will be exported in the RPRS file.
*                                                               The selected Render Layers list is defined by rprContextAttachRenderLayer / rprContextDetachRenderLayer
*                                                               Attach/Detach Render Layers to shapes/lights with  rprShapeAttachRenderLayer, rprLightAttachRenderLayer, rprShapeDetachRenderLayer, rprLightDetachRenderLayer.
*
*/
#define RPRLOADSTORE_EXPORTFLAG_EXTERNALFILES (1 << 0) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_1 (1 << 1) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_2 (1 << 2) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_NORMALS (1 << 3) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_UV (1 << 4) 
#define RPRLOADSTORE_EXPORTFLAG_EMBED_FILE_IMAGES_USING_OBJECTNAME (1 << 5) 
#define RPRLOADSTORE_EXPORTFLAG_USE_IMAGE_CACHE (1 << 6) 
#define RPRLOADSTORE_EXPORTFLAG_ONLY_EXPORT_ATTACHED_RENDER_LAYERS (1 << 7) 


/** 
* export an RPR scene to an RPRS file
* rprsExport and rprsxExport are pretty much the same functions.
* rprsxExport is now deprecated and may be removed in the future.
*
* exportFlags : new argument from 1.324 RPR SDK.  Set it to 0 if don't use it.
*               flags using  RPRLOADSTORE_EXPORTFLAG_* defines
*
* rprsFileName : UTF-8 can be used for UNICODE
*
* rprsCtx : new argument introduced in 2.02.6 API. can be NULL.
*           This context is created/deleted with  rprsCreateContext/rprsDeleteContext.
*           It can be used to store additional data for the Export
*
*/
extern RPR_API_ENTRY rpr_status rprsExport(char const * rprsFileName, rpr_context context, rpr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags, RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsxExport(char const * rprsFileName, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags, RPRS_context rprsCtx);


/** 
*
* Import and build a RPR scene from RPRS a file ( or buffer if rprsImportFromData is used )
*
* rprsImport and rprsxImport are pretty much the same functions.
* rprsxImport is now deprecated and may be removed in the future.
*
* rprsImportFromData and rprsxImportFromData are pretty much the same functions.
* rprsxImportFromData is now deprecated and may be removed in the future.
*
* Warning : *FromData doesn't support old RPRS import ( version before October 2016 ) 
*
* rprsFileName : UTF-8 can be used for UNICODE
*
* rprsCtx : new argument introduced in 2.02.6 API. can be NULL.
*           This context is created/deleted with  rprsCreateContext/rprsDeleteContext.
*           It can be used to read additional data after an Import.
*
*/
extern RPR_API_ENTRY rpr_status rprsImport(char const * rprsFileName, rpr_context context, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene, RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsxImport(char const * rprsFileName, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene, RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsImportFromData(rpr_uchar const * data, size_t dataSize, rpr_context context, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene, RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsxImportFromData(rpr_uchar const * data, size_t dataSize, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene, RPRS_context rprsCtx);


/** 
*
* if 'rprsFileName' is embedding some OCIO configuration, then we need to unpack them before render.
* This operation is NOT done inside rprsImport, so it should be called before or after rprsImport.
* This function also sets the RPR context RPR_CONTEXT_OCIO_CONFIG_PATH parameter to 'basePath'.  rprsImport doesn't set this parameter.
*
*/
extern RPR_API_ENTRY rpr_status rprsBuildOCIOFiles(char const * rprsFileName, rpr_context context, char const * basePath, RPRS_context rprsCtx);


//
// rprsGetExtraCustomParam**** can be called after an rprsImport. they give the 'extraCustomParam' that were given as argument to the rprsExport function and stored inside the RPRS file.
// Using the function with the 'Ex' suffix must be use if the rprsImport was using a RPRS_context.
//
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParam_int(char const * name, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamEx_int(RPRS_context rprsCtx, char const * name, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParam_float(char const * name, float * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamEx_float(RPRS_context rprsCtx, char const * name, float * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndex_int(int index, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndexEx_int(RPRS_context rprsCtx, int index, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndex_float(int index, float * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndexEx_float(RPRS_context rprsCtx, int index, float * value);
extern RPR_API_ENTRY rpr_int rprsGetNumberOfExtraCustomParam();
extern RPR_API_ENTRY rpr_int rprsGetNumberOfExtraCustomParamEx(RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamNameSize(int index, int * nameSizeGet);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamNameSizeEx(RPRS_context rprsCtx, int index, int * nameSizeGet);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamName(int index, char * nameGet, int nameGetSize);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamNameEx(RPRS_context rprsCtx, int index, char * nameGet, int nameGetSize);
extern RPR_API_ENTRY rpr_int rprsGetExtraCustomParamType(int index);
extern RPR_API_ENTRY rpr_int rprsGetExtraCustomParamTypeEx(RPRS_context rprsCtx, int index);


// 
// rprsListImported*** can be called after an rprsImport. those functions will list each RPR objects created during the import.
// rprsListImportedMaterialX si deprecated and may be removed in the future.
// Using the function with the 'Ex' suffix must be use if the rprsImport was using a RPRS_context.
//
extern RPR_API_ENTRY rpr_status rprsListImportedCameras(rpr_camera * Cameras, int sizeCameraBytes, int * numberOfCameras);
extern RPR_API_ENTRY rpr_status rprsListImportedCamerasEx(RPRS_context rprsCtx, rpr_camera * Cameras, int sizeCameraBytes, int * numberOfCameras);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialX(void * * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialXEx(RPRS_context rprsCtx, void * * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialNodes(rpr_material_node * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialNodesEx(RPRS_context rprsCtx, rpr_material_node * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedLights(rpr_light * Lights, int sizeLightBytes, int * numberOfLights);
extern RPR_API_ENTRY rpr_status rprsListImportedLightsEx(RPRS_context rprsCtx, rpr_light * Lights, int sizeLightBytes, int * numberOfLights);
extern RPR_API_ENTRY rpr_status rprsListImportedShapes(rpr_shape * Shapes, int sizeShapeBytes, int * numberOfShapes);
extern RPR_API_ENTRY rpr_status rprsListImportedShapesEx(RPRS_context rprsCtx, rpr_shape * Shapes, int sizeShapeBytes, int * numberOfShapes);
extern RPR_API_ENTRY rpr_status rprsListImportedCurves(rpr_curve * Curves, int sizeCurveBytes, int * numberOfCurves);
extern RPR_API_ENTRY rpr_status rprsListImportedCurvesEx(RPRS_context rprsCtx, rpr_curve * Curves, int sizeCurveBytes, int * numberOfCurves);
extern RPR_API_ENTRY rpr_status rprsListImportedPostEffects(rpr_post_effect * PostEffects, int sizePostEffectsBytes, int * numberOfPostEffects);
extern RPR_API_ENTRY rpr_status rprsListImportedPostEffectsEx(RPRS_context rprsCtx, rpr_post_effect * PostEffects, int sizePostEffectsBytes, int * numberOfPostEffects);
extern RPR_API_ENTRY rpr_status rprsListImportedHeteroVolumes(rpr_hetero_volume * HeteroVolumes, int sizeHeteroVolumesBytes, int * numberOfHeteroVolumes);
extern RPR_API_ENTRY rpr_status rprsListImportedHeteroVolumesEx(RPRS_context rprsCtx, rpr_hetero_volume * HeteroVolumes, int sizeHeteroVolumesBytes, int * numberOfHeteroVolumes);
extern RPR_API_ENTRY rpr_status rprsListImportedGrids(rpr_grid * Grids, int sizeGridsBytes, int * numberOfGrids);
extern RPR_API_ENTRY rpr_status rprsListImportedGridsEx(RPRS_context rprsCtx, rpr_grid * Grids, int sizeGridsBytes, int * numberOfGrids);
extern RPR_API_ENTRY rpr_status rprsListImportedBuffers(rpr_buffer * Buffers, int sizeBuffersBytes, int * numberOfBuffers);
extern RPR_API_ENTRY rpr_status rprsListImportedBuffersEx(RPRS_context rprsCtx, rpr_buffer * Buffers, int sizeBuffersBytes, int * numberOfBuffers);
extern RPR_API_ENTRY rpr_status rprsListImportedImages(rpr_image * Images, int sizeImageBytes, int * numberOfImages);
extern RPR_API_ENTRY rpr_status rprsListImportedImagesEx(RPRS_context rprsCtx, rpr_image * Images, int sizeImageBytes, int * numberOfImages);


//
// rprsExportCustomList and rprsImportCustomList are deprecated and not supported anymore.
//
extern RPR_API_ENTRY rpr_status rprsExportCustomList(char const * rprsFileName, int materialNode_number, rpr_material_node* materialNode_list, int camera_number, rpr_camera* camera_list, int light_number, rpr_light* light_list, int shape_number, rpr_shape* shape_list, int image_number, rpr_image* image_list, RPRS_context rprsCtx);
extern RPR_API_ENTRY rpr_status rprsImportCustomList(char const * rprsFileName, rpr_context context, rpr_material_system materialSystem, int*  materialNode_number, rpr_material_node* materialNode_list, int*  camera_number, rpr_camera* camera_list, int*  light_number, rpr_light* light_list, int*  shape_number, rpr_shape* shape_list, int*  image_number, rpr_image* image_list, RPRS_context rprsCtx);




// In classic usage, rprsExport only saves 1 camera ( the camera attached to scene )
// however, user may need to store more cameras. You can do that with this function.
// Just call rprsAddExtraCamera for each extra camera, then call rprsExport.
// internally, the list of extra cameras will be reset as soon as you call rprsExport
// return RPR_SUCCESS if success.
extern RPR_API_ENTRY rpr_status rprsAddExtraCamera(rpr_camera extraCam);
extern RPR_API_ENTRY rpr_status rprsAddExtraCameraEx(RPRS_context rprsCtx, rpr_camera extraCam);




// Some custom 'extra' parameters can be attached to shape and saved inside the RPRS file.
// Call rprsAddExtraShapeParameter before the rprsExport call to add those parameters in the exporter.
// Call rprsGetExtraShapeParameter after the rprsImport call in order to read those parameters.
extern RPR_API_ENTRY rpr_status rprsAddExtraShapeParameter(rpr_shape shape, const rpr_char * parameterName, rpr_int value);
extern RPR_API_ENTRY rpr_status rprsAddExtraShapeParameterEx(RPRS_context rprsCtx, rpr_shape shape, const rpr_char * parameterName, rpr_int value);
extern RPR_API_ENTRY rpr_status rprsGetExtraShapeParameter(rpr_shape shape, const rpr_char * parameterName, rpr_int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraShapeParameterEx(RPRS_context rprsCtx, rpr_shape shape, const rpr_char * parameterName, rpr_int * value);


// rprsExportToXML is for Debugging usecase only.
// This function converts an RPRS file ( created with rprsExport ) into an XML representing this file.
// The binary buffers ( textures, geometry ... ) are not exported.
// This XML file is not designed to be imported.
// Arguments:
// 'rprsFilePath' : input RPRS file.
// 'xmlFileOut' : name of the output XML file.
extern RPR_API_ENTRY rpr_status rprsExportToXML(char const * rprsFilePath, char const * xmlFileOut);
extern RPR_API_ENTRY rpr_status rprsExportToXMLEx(RPRS_context rprsCtx, char const * rprsFilePath, char const * xmlFileOut);


// Extra feature :  a shape hierarchy can be saved inside the RPRS
//
// -- Usage for Export :
// Before the call of rprsExport, assign shape to group names, example :
// rprsAssignShapeToGroup(shape_thumb, "hand");
// rprsAssignShapeToGroup(shape_littleFinger, "hand");
// rprsAssignShapeToGroup(shape_nose, "head");
// ...
// also defines the groups parents, example :
// rprsAssignParentGroupToGroup("hand", "arm");
// rprsAssignParentGroupToGroup("arm", "body");
// rprsAssignParentGroupToGroup("head", "body");
// ...
// the groups with no parent will be at the root of the scene.
// the call order of  rprsAssignShapeToGroup  and  rprsAssignParentGroupToGroup  doesn't matter
//
// Note that for rprsSetTransformGroup and rprsGetTransformGroup, matrixComponents is an array of 10 floats in the following order: 
//       0-2:Translation, 3-6:RotationQuaternion,  7-9:Scale
//
// then, call rprsExport. Internally this will export the hierarchy to the RPRS, and clean the group list for next export.
// 
// -- Usage for Import
// After the call of rprsImport the parent groups of shapes and child groups can be get, example :
// char* groupName = new char[512];
// rprsGetParentGroupFromShape(rprshape, 512, groupName, NULL);
// 
// or
// char* groupName = new char[512];
// rprsGetParentGroupFromGroup("hand", 512, groupName, NULL);
// 
// if the shape/folder has not parent, the empty string is returned in groupName
// 
// those functions return RPR_SUCCESS if success
// 
extern RPR_API_ENTRY rpr_status rprsAssignShapeToGroup(rpr_shape shape, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignShapeToGroupEx(RPRS_context rprsCtx, rpr_shape shape, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignLightToGroup(rpr_light light, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignLightToGroupEx(RPRS_context rprsCtx, rpr_light light, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignCameraToGroup(rpr_camera camera, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignCameraToGroupEx(RPRS_context rprsCtx, rpr_camera camera, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignParentGroupToGroup(const rpr_char * groupChild, const rpr_char * groupParent);
extern RPR_API_ENTRY rpr_status rprsAssignParentGroupToGroupEx(RPRS_context rprsCtx, const rpr_char * groupChild, const rpr_char * groupParent);
extern RPR_API_ENTRY rpr_status rprsSetTransformGroup(const rpr_char * groupChild, const float * matrixComponents);
extern RPR_API_ENTRY rpr_status rprsSetTransformGroupEx(RPRS_context rprsCtx, const rpr_char * groupChild, const float * matrixComponents);
extern RPR_API_ENTRY rpr_status rprsGetTransformGroup(const rpr_char * groupChild, float * matrixComponents);
extern RPR_API_ENTRY rpr_status rprsGetTransformGroupEx(RPRS_context rprsCtx, const rpr_char * groupChild, float * matrixComponents);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromShape(rpr_shape shape, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromShapeEx(RPRS_context rprsCtx, rpr_shape shape, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromCamera(rpr_camera camera, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromCameraEx(RPRS_context rprsCtx, rpr_camera camera, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromLight(rpr_light light, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromLightEx(RPRS_context rprsCtx, rpr_light light, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromGroup(const rpr_char * groupChild, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromGroupEx(RPRS_context rprsCtx, const rpr_char * groupChild, size_t size, rpr_char * groupName, size_t * size_ret);


// Create a RPRS Context. This object can be used to store additional data ( animation, parameters ... ) for an Export.
// And it can be used to read them back after an Import.
// From 2.02.7 RPR SDK it's advised to use the RPRS_context. meaning that  rprsExport/rprsImport  should not give a nullptr to the rprsCtx argument.
//    and function with the 'Ex' suffix should be used.
extern RPR_API_ENTRY rpr_status rprsCreateContext(RPRS_context * out_rprsCtx);


// Delete a RPRS Context:
// the rprsCtx object and any data hold by it are destroyed (pointers become invalid).
extern RPR_API_ENTRY rpr_status rprsDeleteContext(RPRS_context rprsCtx);


// when rprsImport/rprsxImport is called, some buffers will be allocated and kept in memory for future getters.
// for example, getters for animations data.
// calling this function will clean all data - and make value from getters unavailable.
// 
// a typical usage is :
// - first call rprsImport/rprsxImport(...)
// - call all the getters you need :  rprsGetParentGroupFromShape , rprs_GetAnimation ....
// - then call rprsReleaseImportedData();
// - all pointers returned by getters become undefined - and musn't be used anymore.
// - render the scene.
//
// Notes:
//  - this function will be automatically called internally at the very beginning of each call of rprsImport/rprsxImport
//    make sure to not use the pointer from getters from a previous Import.
//
//  - IMPORTANT: if using RPRS_context (which is recommended) this function is useless and deprecated. 
//               For RPRS_context, release data with rprsDeleteContext.
//
extern RPR_API_ENTRY rpr_status rprsReleaseImportedData();


// void* contextX__NOT_USED_ANYMORE can be set to null  - not used anymore.
// Delete RPR objects created by last rprsImport : camera, material_node, shape, light, image, post_effect, hetero_volume, buffer, curve
// do NOT any delete rpr_context, rpr_scene and the RPRS_context
extern RPR_API_ENTRY rpr_status rprsDeleteListImportedObjects(void * contextX__NOT_USED_ANYMORE);
extern RPR_API_ENTRY rpr_status rprsDeleteListImportedObjectsEx(RPRS_context rprsCtx);

typedef rpr_uint rprs_animation_movement_type;


//rprs_animation_movement_type
#define RPRS_ANIMATION_MOVEMENTTYPE_TRANSLATION 0x1 
#define RPRS_ANIMATION_MOVEMENTTYPE_ROTATION 0x2 
#define RPRS_ANIMATION_MOVEMENTTYPE_SCALE 0x3 


// structSize : size of this struct in Byte (internally used to identify if different versions)
// interpolationType : unused for now - set it to 0
//
// example : if the animation has 2 FLOAT3 defining translation at time 0.5 and 3.0  for a translation along y axis , we have  :
//
//nbTimeKeys = 2
//nbTransformValues = 2 
//timeKeys        = { 0.5 , 3.0 }
//transformValues = { 0.0,0.0,0.0,  0.0,1.0,0.0,  }
struct __rprs_animation
{
    rpr_uint structSize;
    char * groupName;
    rprs_animation_movement_type movementType;
    rpr_uint interpolationType;
    rpr_uint nbTimeKeys;
    rpr_uint nbTransformValues;
    float * timeKeys;
    float * transformValues;
};
typedef __rprs_animation _rprs_animation;


typedef _rprs_animation rprs_animation;


// rprsGetAnimation can be called after a rprsImport call. This function gives dedails on each animations stored in the RPRS file.
// return null if not animation exists for animIndex
// Using the function with the 'Ex' suffix must be used if the rprsImport was using a RPRS_context.
extern RPR_API_ENTRY const rprs_animation * rprsGetAnimation(int animIndex);
extern RPR_API_ENTRY const rprs_animation * rprsGetAnimationEx(RPRS_context rprsCtx, int animIndex);


// rprsAddAnimation can be called before the rprsExport call. It adds information about animation in the scene. those informations are stored in the RPRS file.
// 1 rprsAddAnimation call per animation.
//
// before 2.01.6 SDK : make sure the pointers specified inside rprs_animation structure  ( groupName2, timeKeys, transformValues) stay available from this call to the rprsExport call.
//                     after that, they won't be used anymore by the LoadStore library.
// from   2.01.6 SDK : RPRS library will copy the animation buffers internally until the rprsExport call. So pointers given to rprsAddAnimation don't need to be kept by the API user.
//
// Using the function with the 'Ex' suffix must be used if the rprsExport will use a RPRS_context.
//
// return RPR_SUCCESS if success.
extern RPR_API_ENTRY rpr_status rprsAddAnimation(const rprs_animation * anim);
extern RPR_API_ENTRY rpr_status rprsAddAnimationEx(RPRS_context rprsCtx, const rprs_animation * anim);


//
//
// FRS is no longer supported by the API.  just replace  FRS* by RPRS*
//
//
// #define FRLOADSTORE_PARAMETER_TYPE_UNDEF 0x0 
// #define FRLOADSTORE_PARAMETER_TYPE_INT 0x1 
// #define FRLOADSTORE_PARAMETER_TYPE_FLOAT 0x2 
// #define FRLOADSTORE_EXPORTFLAG_EXTERNALFILES (1 << 0) 
// extern RPR_API_ENTRY int frsExport(char const * frsFileName, fr_context context, fr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags);
// extern RPR_API_ENTRY int frsxExport(char const * frsFileName, fr_context context, void * contextX, fr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags);
// extern RPR_API_ENTRY int frsImport(char const * frsFileName, fr_context context, fr_material_system materialSystem, fr_scene * scene, bool useAlreadyExistingScene);
// extern RPR_API_ENTRY int frsxImport(char const * frsFileName, fr_context context, void * contextX, fr_material_system materialSystem, fr_scene * scene, bool useAlreadyExistingScene);
// extern RPR_API_ENTRY int frsGetExtraCustomParam_int(char const * name, int * value);
// extern RPR_API_ENTRY int frsGetExtraCustomParam_float(char const * name, float * value);
// extern RPR_API_ENTRY int frsGetExtraCustomParamIndex_int(int index, int * value);
// extern RPR_API_ENTRY int frsGetExtraCustomParamIndex_float(int index, float * value);
// extern RPR_API_ENTRY int frsGetNumberOfExtraCustomParam();
// extern RPR_API_ENTRY int frsGetExtraCustomParamNameSize(int index, int * nameSizeGet);
// extern RPR_API_ENTRY int frsGetExtraCustomParamName(int index, char * nameGet, int nameGetSize);
// extern RPR_API_ENTRY int frsGetExtraCustomParamType(int index);
// extern RPR_API_ENTRY int frsListImportedCameras(void * * Cameras, int sizeCameraBytes, int * numberOfCameras);
// extern RPR_API_ENTRY int frsListImportedMaterialX(void * * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
// extern RPR_API_ENTRY int frsListImportedMaterialNodes(void * * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
// extern RPR_API_ENTRY int frsListImportedLights(void * * Lights, int sizeLightBytes, int * numberOfLights);
// extern RPR_API_ENTRY int frsListImportedShapes(void * * Shapes, int sizeShapeBytes, int * numberOfShapes);
// extern RPR_API_ENTRY int frsListImportedPostEffects(void * * PostEffects, int sizePostEffectsBytes, int * numberOfPostEffects);
// extern RPR_API_ENTRY int frsListImportedHeteroVolumes(void * * HeteroVolumes, int sizeHeteroVolumesBytes, int * numberOfHeteroVolumes);
// extern RPR_API_ENTRY int frsListImportedGrids(void * * Grids, int sizeGridsBytes, int * numberOfGrids);
// extern RPR_API_ENTRY int frsListImportedBuffers(void * * Buffers, int sizeBuffersBytes, int * numberOfBuffers);
// extern RPR_API_ENTRY int frsListImportedImages(void * * Images, int sizeImageBytes, int * numberOfImages);
// extern RPR_API_ENTRY int frsExportCustomList(char const * frsFileName, int materialNode_number, fr_material_node* materialNode_list, int camera_number, fr_camera* camera_list, int light_number, fr_light* light_list, int shape_number, fr_shape* shape_list, int image_number, fr_image* image_list);
// extern RPR_API_ENTRY int frsImportCustomList(char const * frsFileName, void * context, fr_material_system materialSystem, int*  materialNode_number, fr_material_node* materialNode_list, int*  camera_number, fr_camera* camera_list, int*  light_number, fr_light* light_list, int*  shape_number, fr_shape* shape_list, int*  image_number, fr_image* image_list);
// extern RPR_API_ENTRY int frsExportToXML(char const * frsFileNameBinary, char const * frsFileNameAscii);
// extern RPR_API_ENTRY int frsDeleteListImportedObjects(void * contextX);
#ifdef __cplusplus
}
#endif

#endif  /*__RPRLOADSTORE_H  */
