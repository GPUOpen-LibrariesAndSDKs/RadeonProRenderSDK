/*****************************************************************************\
*
*  Module Name    RprLoadStore.h
*  Project        RRP Load Store library
*
*  Description    RRP load store library interface header
*
*  Copyright 2019 Advanced Micro Devices, Inc.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
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
*/
#define RPRLOADSTORE_EXPORTFLAG_EXTERNALFILES (1 << 0) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_1 (1 << 1) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_2 (1 << 2) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_NORMALS (1 << 3) 
#define RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_UV (1 << 4) 
#define RPRLOADSTORE_EXPORTFLAG_EMBED_FILE_IMAGES_USING_OBJECTNAME (1 << 5) 


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
*/
extern RPR_API_ENTRY rpr_status rprsExport(char const * rprsFileName, rpr_context context, rpr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags);
extern RPR_API_ENTRY rpr_status rprsxExport(char const * rprsFileName, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_scene scene, int extraCustomParam_int_number, char const * * extraCustomParam_int_names, int const * extraCustomParam_int_values, int extraCustomParam_float_number, char const * * extraCustomParam_float_names, float const * extraCustomParam_float_values, unsigned int exportFlags);


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
*/
extern RPR_API_ENTRY rpr_status rprsImport(char const * rprsFileName, rpr_context context, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene);
extern RPR_API_ENTRY rpr_status rprsxImport(char const * rprsFileName, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene);
extern RPR_API_ENTRY rpr_status rprsImportFromData(rpr_uchar const * data, size_t dataSize, rpr_context context, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene);
extern RPR_API_ENTRY rpr_status rprsxImportFromData(rpr_uchar const * data, size_t dataSize, rpr_context context, void * contextX__NOT_USED_ANYMORE, rpr_material_system materialSystem, rpr_scene * scene, bool useAlreadyExistingScene);


/** 
*
*/
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParam_int(char const * name, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParam_float(char const * name, float * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndex_int(int index, int * value);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamIndex_float(int index, float * value);
extern RPR_API_ENTRY rpr_int rprsGetNumberOfExtraCustomParam();
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamNameSize(int index, int * nameSizeGet);
extern RPR_API_ENTRY rpr_status rprsGetExtraCustomParamName(int index, char * nameGet, int nameGetSize);
extern RPR_API_ENTRY rpr_int rprsGetExtraCustomParamType(int index);


/** 
* rprsListImportedMaterialX si deprecated and may be removed in the future.
*/
extern RPR_API_ENTRY rpr_status rprsListImportedCameras(rpr_camera * Cameras, int sizeCameraBytes, int * numberOfCameras);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialX(void * * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedMaterialNodes(rpr_material_node * MaterialNodes, int sizeMaterialNodeBytes, int * numberOfMaterialNodes);
extern RPR_API_ENTRY rpr_status rprsListImportedLights(rpr_light * Lights, int sizeLightBytes, int * numberOfLights);
extern RPR_API_ENTRY rpr_status rprsListImportedShapes(rpr_shape * Shapes, int sizeShapeBytes, int * numberOfShapes);
extern RPR_API_ENTRY rpr_status rprsListImportedCurves(rpr_curve * Curves, int sizeCurveBytes, int * numberOfCurves);
extern RPR_API_ENTRY rpr_status rprsListImportedPostEffects(rpr_post_effect * PostEffects, int sizePostEffectsBytes, int * numberOfPostEffects);
extern RPR_API_ENTRY rpr_status rprsListImportedHeteroVolumes(rpr_hetero_volume * HeteroVolumes, int sizeHeteroVolumesBytes, int * numberOfHeteroVolumes);
extern RPR_API_ENTRY rpr_status rprsListImportedGrids(rpr_grid * Grids, int sizeGridsBytes, int * numberOfGrids);
extern RPR_API_ENTRY rpr_status rprsListImportedBuffers(rpr_buffer * Buffers, int sizeBuffersBytes, int * numberOfBuffers);
extern RPR_API_ENTRY rpr_status rprsListImportedImages(rpr_image * Images, int sizeImageBytes, int * numberOfImages);



extern RPR_API_ENTRY rpr_status rprsExportCustomList(char const * rprsFileName, int materialNode_number, rpr_material_node* materialNode_list, int camera_number, rpr_camera* camera_list, int light_number, rpr_light* light_list, int shape_number, rpr_shape* shape_list, int image_number, rpr_image* image_list);
extern RPR_API_ENTRY rpr_status rprsImportCustomList(char const * rprsFileName, rpr_context context, rpr_material_system materialSystem, int*  materialNode_number, rpr_material_node* materialNode_list, int*  camera_number, rpr_camera* camera_list, int*  light_number, rpr_light* light_list, int*  shape_number, rpr_shape* shape_list, int*  image_number, rpr_image* image_list);



extern RPR_API_ENTRY rpr_status rprsExportToXML(char const * rprsFileNameBinary, char const * rprsFileNameAscii);


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
extern RPR_API_ENTRY rpr_status rprsAssignCameraToGroup(rpr_camera camera, const rpr_char * groupName);
extern RPR_API_ENTRY rpr_status rprsAssignParentGroupToGroup(const rpr_char * groupChild, const rpr_char * groupParent);
extern RPR_API_ENTRY rpr_status rprsSetTransformGroup(const rpr_char * groupChild, const float * matrixComponents);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromShape(rpr_shape shape, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromCamera(rpr_camera camera, size_t size, rpr_char * groupName, size_t * size_ret);
extern RPR_API_ENTRY rpr_status rprsGetParentGroupFromGroup(const rpr_char * groupChild, size_t size, rpr_char * groupName, size_t * size_ret);


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
// important: this function will be automatically called internally at the very beginning of each call of rprsImport/rprsxImport
//            make sure to not use the pointer from getters from a previous Import.
extern RPR_API_ENTRY rpr_status rprsReleaseImportedData();


// void* contextX__NOT_USED_ANYMORE can be set to null  - not used anymore.
// Delete objects created by last rprsImport : camera, material_node, shape, light, image, post_effect, hetero_volume, buffer, curve
// do NOT delete context and scene
extern RPR_API_ENTRY rpr_status rprsDeleteListImportedObjects(void * contextX__NOT_USED_ANYMORE);

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


//return null if not animation exists for animIndex
extern RPR_API_ENTRY const rprs_animation * rprsGetAnimation(int animIndex);


// make sure the pointers specified inside rprs_animation structure  ( groupName2, timeKeys, transformValues) stay available from this call to the rprsExport call.
// after that, they won't be used anymore by the LoadStore library.
// return RPR_SUCCESS if success.
extern RPR_API_ENTRY rpr_status rprsAddAnimation(const rprs_animation * anim);


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
