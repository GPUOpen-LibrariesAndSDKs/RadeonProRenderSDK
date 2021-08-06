/*****************************************************************************\
*
*  Module Name    ProRenderGLTF.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender GLTF Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

#ifndef __RPRGLTF_H
#define __RPRGLTF_H


#include <RadeonProRender.h>
#include <RprLoadStore.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gltf_status error codes */
#define GLTF_SUCCESS 0
#define GLTF_ERROR_EXPORT -1
#define GLTF_ERROR_IMPORT -2
#define GLTF_ERROR_LOAD -3
#define GLTF_ERROR_SAVE -4


//
// export flags
//
// if flag enabled, export to a binary GLTF
#define RPRGLTF_EXPORTFLAG_GLB (1 << 0) 
// if enabled, do NOT flip texture Y when exporting ( see RPRGLTF_IMPORTFLAG_NO_TEXTUREFLIPV for more details )
#define RPRGLTF_EXPORTFLAG_NO_TEXTUREFLIPV (1 << 1) 
// if enabled, the Exporter assumes that names of all rpr_image ( set by rprObjectSetName ) represent file paths of the original images.
//             Instead of using the image data from RPR, the Exporter will simply use the data from the file.
//             If the file path is not found, then data from RPR is used to export the image.
//             Advantage of using this flag is that we ensure to attach the original image files into the GLTF, instead of using an image compressed/modified by RPR.
//             For Unicode, you can encode the name with UTF-8
#define RPRGLTF_EXPORTFLAG_COPY_IMAGES_USING_OBJECTNAME (1 << 2) 
// if flag enabled, export lights with both RPR and KHR extensions.
#define RPRGLTF_EXPORTFLAG_KHR_LIGHT (1 << 3) 
// if flag enabled, the Exporter may try to create images matching the GLTF PBR material spec.
//                  For example, it takes both Metallic and Roughness images from RPR, and combine them into a single PbrMetallicRoughness.
//					This process should improve the rendering of the GLTF on non-RPR renderer. But it will slow-down the export process as it needs to process & generate new images.
#define RPRGLTF_EXPORTFLAG_BUILD_STD_PBR_IMAGES (1 << 4) 

//
// import flags :
//
// for GLTF, the upper left corner of the image is the uv=(0,0).  for RPR, the bottom left corner of the image is the uv=(0,0).
// So by default (if we don't use this parameter) the transformation (u,v) -> (u,1-v) will be applied to meshes UV.
// if you enable this flag, the UV is NOT flipped.
#define RPRGLTF_IMPORTFLAG_NO_TEXTUREFLIPV (1 << 0) 


typedef int gltf_status;

struct RprGltfImportCallbacks
{
    void (*shapeCustomAttributeHandlerI)(rpr_shape shape, const char *attribute_name, rpr_int value);
};

/* Imports a gltf file from disk and imports the data into a Radeon ProRender context and associated API objects.
@param filename         The path to the gltf file loaded. (For Unicode, you can encode the name with UTF-8)
@param context          The pre-initialized Radeon ProRender context handle to create API objects from.
@param materialSystem   The pre-initialized Radeon ProRender material system handle to create API objects from.
@param uberMatContext   The pre-initialized Radeon ProRender uber material system context handle to create API objects from.
@param scene            The scene at gltTF::scene is loaded and stored in this handle.
@param callbacks        Callbacks to be used during scene parsing - set it to NULL if not used.
@param importFlags      see RPRGLTF_IMPORTFLAG_* for more details - set it to 0 if not used.
@param rprsCtx          new argument introduced in 2.02.6 API. can be NULL.
                        This context is created/deleted with  rprsCreateContext/rprsDeleteContext.
						It can be used to store additional data for the Export
@return                 GLTF_SUCCESS if success, GLTF_ERROR_IMPORT or GLTF_ERROR_LOAD otherwise.
*/
extern int rprImportFromGLTF(const char* filename, rpr_context context, rpr_material_system materialSystem, rpr_scene* scene,
    RprGltfImportCallbacks *callbacks, unsigned int importFlags, RPRS_context rprsCtx);

/* Exports a list of Radeon ProRender scenes to a gltf file on disk.
@param filename         The path to the gltf file to export. (For Unicode, you can encode the name with UTF-8)
@param context          The pre-initialized Radeon ProRender context handle to export API objects from.
@param materialSystem   NOT USED - can be NULL
@param uberMatContext   The pre-initialized Radeon ProRender uber material system context handle to export API objects from.
@param scenes           All exported scenes to be written out to the gltf file.
@param exportFlags      (new parameter from RPR SDK 1.325) - see RPRGLTF_EXPORTFLAG_* for more details - set it to 0 if not used.
@param rprsCtx          new argument introduced in 2.02.6 API. can be NULL.
                        This context is created/deleted with  rprsCreateContext/rprsDeleteContext.
						It can be used to read additional data after an Import.
@return                 GLTF_SUCCESS if success, GLTF_ERROR_EXPORT or GLTF_ERROR_SAVE otherwise.
*/
extern int rprExportToGLTF(const char* filename, rpr_context context, rpr_material_system materialSystem_NOT_USED, const rpr_scene* scenes, size_t sceneCount, unsigned int exportFlags, RPRS_context rprsCtx);


/*
After the rprImportFromGLTF call,  rprGLTF_ListImported_*  can be called to have the list of rpr objects created during the Import

EXAMPLE :
int nbOfObjs = 0;
rprGLTF_ListImported_Images(0,0,&nbOfObjs);
rpr_image* objs = new rpr_image[nbOfObjs];
rprGLTF_ListImported_Images(objs,nbOfObjs*sizeof(rpr_image),0);
for(int i=0; i<nbOfObjs; i++)
	//iterate in the list
delete[] objs;

returns RPR_SUCCESS if success
*/
extern int rprGLTF_ListImported_Shapes(rpr_shape * Shapes, int sizeShapeBytes, int * numberOfShapes);
extern int rprGLTF_ListImported_Lights(rpr_light * Lights, int sizeLightsBytes, int * numberOfLights);
extern int rprGLTF_ListImported_Images(rpr_image * Images, int sizeImagesBytes, int * numberOfImages);
extern int rprGLTF_ListImported_Cameras(rpr_camera * Cameras, int sizeCamerasBytes, int * numberOfCameras);
extern int rprGLTF_ListImported_MaterialNodes(rpr_material_node * MaterialNodes, int sizeMaterialNodesBytes, int * numberOfMaterialNodes);
extern int rprGLTF_ListImported_PostEffects(rpr_post_effect * PostEffects, int sizePostEffectsBytes, int * numberOfPostEffects);
extern int rprGLTF_ListImported_HeteroVolumes(rpr_hetero_volume * HeteroVolumes, int sizeHeteroVolumesBytes, int * numberOfHeteroVolumes);
extern int rprGLTF_ListImported_Buffer(rpr_buffer * Buffers, int sizeShapeBytes, int * numberOfBuffers);


// Getters for Extra-Attribute. Call them just AFTER the rprImportFromGLTF call.
// rprGLTF_AddExtra*** is used for setting thoses attributes.
extern int rprGLTF_GetImportedExtraShapeAttribute(rpr_shape shape, char* name, int& outParam);
extern int rprGLTF_GetImportedExtraLightAttribute(rpr_light light, char* name, int& outParam);


/*
Extra feature :  a shape hierarchy can be saved inside the GLTF

-- Usage for Export :
Before the call of rprExportToGLTF, assign shape to group names, example :
rprGLTF_AssignShapeToGroup(shape_thumb, "hand");
rprGLTF_AssignShapeToGroup(shape_littleFinger, "hand");
rprGLTF_AssignShapeToGroup(shape_nose, "head");
...
also defines the groups parents, example :
rprGLTF_AssignParentGroupToGroup("hand", "arm");
rprGLTF_AssignParentGroupToGroup("arm", "body");
rprGLTF_AssignParentGroupToGroup("head", "body");
...
the groups with no parent will be at the root of the scene.
the call order of  rprGLTF_AssignShapeToGroup  and  rprGLTF_AssignParentGroupToGroup  doesn't matter

Note that for rprGLTF_SetTransformGroup and rprGLTF_GetTransformGroup, matrixComponents is an array of 10 floats in the following order: 
       0-2:Translation, 3-6:RotationQuaternion,  7-9:Scale

then, call rprExportToGLTF. Internally this will export the hierarchy to the GLTF, and clean the group list for next export.

-- Usage for Import
After the call of rprImportFromGLTF. the parent groups of shapes and child groups can be get, example :
char* groupName = new char[512];
rprGLTF_GetParentGroupFromShape(rprshape, 512, groupName, NULL);

or
char* groupName = new char[512];
rprGLTF_GetParentGroupFromGroup("hand", 512, groupName, NULL);

if the shape/folder has not parent, the empty string is returned in groupName

these functions return RPR_SUCCESS if success
*/
extern int rprGLTF_AssignShapeToGroup(rpr_shape shape, const rpr_char* groupName);
extern int rprGLTF_AssignCameraToGroup(rpr_camera camera, const rpr_char* groupName);
extern int rprGLTF_AssignLightToGroup(rpr_light light, const rpr_char* groupName);
extern int rprGLTF_AssignParentGroupToGroup(const rpr_char* groupChild, const rpr_char* groupParent);
extern int rprGLTF_SetTransformGroup(const rpr_char* groupChild, const float* matrixComponents);
extern int rprGLTF_GetTransformGroup(const rpr_char * groupChild, float * matrixComponents);
extern int rprGLTF_GetParentGroupFromShape(rpr_shape shape, size_t size, rpr_char* groupName, size_t* size_ret);
extern int rprGLTF_GetParentGroupFromCamera(rpr_camera camera, size_t size, rpr_char* groupName, size_t* size_ret);
extern int rprGLTF_GetParentGroupFromLight(rpr_light light, size_t size, rpr_char* groupName, size_t* size_ret);
extern int rprGLTF_GetParentGroupFromGroup(const rpr_char* groupChild, size_t size, rpr_char* groupName, size_t* size_ret);


// when rprImportFromGLTF is called, some buffers will be allocated and kept in memory for future getters.
// for example, getters for animations data, Extra-Parameters ...
// calling this function will clean all data - and make value from getters unavailable.
//
// a typical usage is :
// - first call rprImportFromGLTF(...)
// - call all the getters you need :  rprGLTF_GetParentGroupFromShape , rprGLTF_GetAnimation , rprGLTF_GetExtraParameterInt ....
// - then call rprGLTF_ReleaseImportedData();
// - all pointers returned by getters become undefined - and musn't be used anymore.
// - render the scene.
//
// important: this function will be automatically called internally at the very beginning of each call of rprImportFromGLTF
//            make sure to not use the pointer from getters from a previous Import.
extern int rprGLTF_ReleaseImportedData();



// 'contextX' is optionnal
// Delete objects created by last rprImportFromGLTF call : camera, material_node, shape, light, image, post_effect, hetero_volume, buffer, curve
// do NOT delete context and scene
// if you call this function, you have to make sure that you didn't call rprObjectDelete on any of the objects imported by last rprImportFromGLTF
// A typical usage is to call this function, at the very end of the scene rendering, when you don't want to use the scene anymore and want to clean all RPR objects properly.
extern int rprGLTF_DeleteListImportedObjects();


typedef rpr_uint rprgltf_animation_movement_type;

//rprgltf_animation_movement_type
#define RPRGLTF_ANIMATION_MOVEMENTTYPE_TRANSLATION 0x1
#define RPRGLTF_ANIMATION_MOVEMENTTYPE_ROTATION 0x2
#define RPRGLTF_ANIMATION_MOVEMENTTYPE_SCALE 0x3

struct _rprgltf_animation
{
	unsigned int structSize; // size of this struct in Byte (internally used to identify if different versions)

	char* groupName;

	rprgltf_animation_movement_type movementType;

	rpr_uint interpolationType; // unused for now.

	// example : if the animation has 2 FLOAT3 defining translation at time 0.5 and 3.0  for a translation along y axis , we have  :
	//
	//nbTimeKeys = 2
	//nbTransformValues = 2   ( for easier developpement of animation rendering, let's agree to have nbTimeKeys = nbTransformValues in most of the cases, whenever it's possible )
	//timeKeys        = { 0.5 , 3.0 }
	//transformValues = { 0.0,0.0,0.0,  0.0,1.0,0.0,  }

	unsigned int nbTimeKeys;
	unsigned int nbTransformValues;

	float* timeKeys;
	float* transformValues;

};
typedef _rprgltf_animation rprgltf_animation;

//return null if not animation exists for animIndex
extern const rprgltf_animation* rprGLTF_GetAnimation(int animIndex);

// before 2.01.6 SDK : make sure the pointers specified inside rprgltf_animation structure  ( groupName2, timeKeys, transformValues) stay available from this call to the rprExportToGLTF call.
//                     after that, they won't be used anymore by gltf library.
// from   2.01.6 SDK : GLTF library will copy the animation buffers internally until the rprExportToGLTF call. So pointers given to rprGLTF_AddAnimation don't need to be kept by the API user.
// return RPR_SUCCESS if success.
extern int rprGLTF_AddAnimation(const rprgltf_animation* anim);

// In classic usage, rprExportToGLTF only saves 1 camera ( the camera attached to scene )
// however, user may need to store more cameras. You can do that with this function.
// Just call rprGLTF_AddExtraCamera for each extra camera, then call rprExportToGLTF.
// internally, the list of extra cameras will be reset as soon as you call rprExportToGLTF
// return RPR_SUCCESS if success.
extern int rprGLTF_AddExtraCamera(rpr_camera extraCam);

// Using this function user can assign any custom parameters for the given shape/light.
// Call them just BEFORE the rprExportToGLTF call.
// At the moment we support only int type, but it may be enhanced in the future
// rprGLTF_GetImportedExtra*** can be used to get those parameters.
extern int rprGLTF_AddExtraShapeParameter(rpr_shape shape, const rpr_char* parameterName, int value);
extern int rprGLTF_AddExtraLightParameter(rpr_light light, const rpr_char* parameterName, int value);


// === Store of Extra Parameters ===
// API user can store extra int, float, buffers... inside the GLTF.
// Those parameters won't be used by any features of the importer. However the API user can access those value after an rprImportFromGLTF.
//
// Call rprGLTF_AddExtraParameter* before calling rprExportToGLTF
// As soon as rprExportToGLTF is called, all the lists of Extra-Parameter are stored into the .GLTF, and those lists are also deleted for the next Export.
//
// Example:
// rprGLTF_AddExtraParameterInt("var",1);
// rprExportToGLTF("fileA.gltf"...); // fileA will contains the "var"
// rprExportToGLTF("fileB.gltf"...); // fileB won't contain any ExtraParameter
//
// Return RPR_SUCCESS if success.
//
extern rpr_status rprGLTF_AddExtraParameterInt(const rpr_char* parameterName, rpr_int v);
extern rpr_status rprGLTF_AddExtraParameterFloat(const rpr_char* parameterName, rpr_float x);
extern rpr_status rprGLTF_AddExtraParameterFloat2(const rpr_char* parameterName, rpr_float x, rpr_float y);
extern rpr_status rprGLTF_AddExtraParameterFloat4(const rpr_char* parameterName, rpr_float x, rpr_float y, rpr_float z, rpr_float w);
extern rpr_status rprGLTF_AddExtraParameterString(const rpr_char* parameterName, const rpr_char* value);
// for rprGLTF_AddExtraParameterBuffer, the buffer will be copied and stored inside the GLTF library.
// so API user doesn't need to keep the 'buffer' valid.
// internally, the stored Extra-Parameter Buffers will be deleted automatically during the rprExportToGLTF call.
extern rpr_status rprGLTF_AddExtraParameterBuffer(const rpr_char* parameterName, const rpr_uchar* buffer, size_t bufferSizeByte);




// === Get of Extra Parameters ===
// call those after calling rprImportFromGLTF
//
// for String and Buffer, it can be done in 2 calls:
// - first call with bufferOut and bufferOut_SizeByte = 0  -->  sizeOut returns the size needed
// - allocate a buffer of 'sizeOut' bytes.
// - second call with the allocated bufferOut
//
// as soon as rprGLTF_ReleaseImportedData() is called  - ( or if another rprImportFromGLTF() is called ) - it's no longer possible to GetExtraParameter* of the imported gltf.
//
extern rpr_status rprGLTF_GetExtraParameterInt(const rpr_char* parameterName, rpr_int* v);
extern rpr_status rprGLTF_GetExtraParameterFloat(const rpr_char* parameterName, rpr_float* x);
extern rpr_status rprGLTF_GetExtraParameterFloat2(const rpr_char* parameterName, rpr_float* x, rpr_float* y);
extern rpr_status rprGLTF_GetExtraParameterFloat4(const rpr_char* parameterName, rpr_float* x, rpr_float* y, rpr_float* z, rpr_float* w);
extern rpr_status rprGLTF_GetExtraParameterString(const rpr_char* parameterName, rpr_char* bufferOut, size_t bufferOut_SizeByte, size_t* sizeOut);
extern rpr_status rprGLTF_GetExtraParameterBuffer(const rpr_char* parameterName, rpr_uchar* bufferOut, size_t bufferOut_SizeByte, size_t* sizeOut);



#ifdef __cplusplus
}
#endif

#endif // __RPRGLTF_H
