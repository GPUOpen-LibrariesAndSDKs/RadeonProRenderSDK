#ifndef __RPRGLTF_H
#define __RPRGLTF_H


#include <RadeonProRender.h>

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
@return                 GLTF_SUCCESS if success, GLTF_ERROR_IMPORT or GLTF_ERROR_LOAD otherwise.
*/
extern int rprImportFromGLTF(const char* filename, rpr_context context, rpr_material_system materialSystem, rpr_scene* scene,
    RprGltfImportCallbacks *callbacks, unsigned int importFlags);

/* Exports a list of Radeon ProRender scenes to a gltf file on disk.
@param filename         The path to the gltf file to export. (For Unicode, you can encode the name with UTF-8)
@param context          The pre-initialized Radeon ProRender context handle to export API objects from.
@param materialSystem   NOT USED - can be NULL
@param uberMatContext   The pre-initialized Radeon ProRender uber material system context handle to export API objects from.
@param scenes           All exported scenes to be written out to the gltf file.
@param exportFlags      (new parameter from RPR SDK 1.325) - see RPRGLTF_EXPORTFLAG_* for more details - set it to 0 if not used.
@return                 GLTF_SUCCESS if success, GLTF_ERROR_EXPORT or GLTF_ERROR_SAVE otherwise.
*/
extern int rprExportToGLTF(const char* filename, rpr_context context, rpr_material_system materialSystem_NOT_USED, const rpr_scene* scenes, size_t sceneCount, unsigned int exportFlags);


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

extern int rprGLTF_GetImportedExtraShapeAttribute(rpr_shape shape, char* name, int& outParam);

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
extern int rprGLTF_AssignParentGroupToGroup(const rpr_char* groupChild, const rpr_char* groupParent);
extern int rprGLTF_SetTransformGroup(const rpr_char* groupChild, float* matrixComponents);
extern int rprGLTF_GetParentGroupFromShape(rpr_shape shape, size_t size, rpr_char* groupName, size_t* size_ret);
extern int rprGLTF_GetParentGroupFromCamera(rpr_camera camera, size_t size, rpr_char* groupName, size_t* size_ret);
extern int rprGLTF_GetParentGroupFromGroup(const rpr_char* groupChild, size_t size, rpr_char* groupName, size_t* size_ret);


// when rprImportFromGLTF is called, some buffers will be allocated and kept in memory for future getters.
// for example, getters for animations data.
// calling this function will clean all data - and make value from getters unavailable.
//
// a typical usage is :
// - first call rprImportFromGLTF(...)
// - call all the getters you need :  rprGLTF_GetParentGroupFromShape , rprGLTF_GetAnimation ....
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

// make sure the pointers specified inside rprgltf_animation structure  ( groupName2, timeKeys, transformValues) stay available from this call to the rprExportToGLTF call.
// after that, they won't be used anymore by gltf library.
// return RPR_SUCCESS if success.
extern int rprGLTF_AddAnimation(const rprgltf_animation* anim);

// In classic usage, rprExportToGLTF only saves 1 camera ( the camera attached to scene )
// however, user may need to store more cameras. You can do that with this function.
// Just call rprGLTF_AddExtraCamera for each extra camera, then call rprExportToGLTF.
// internally, the list of extra cameras will be reset as soon as you call rprExportToGLTF
// return RPR_SUCCESS if success.
extern int rprGLTF_AddExtraCamera(rpr_camera extraCam);

// Using this function user can assign any custom parameters for the given shape.
// At the moment we support only int type, but it may be enhanced in the future
extern int rprGLTF_AddExtraShapeParameter(rpr_shape shape, const rpr_char* parameterName, int value);


#ifdef __cplusplus
}
#endif

#endif // __RPRGLTF_H
