/*****************************************************************************\
*
*  Module Name    RadeonProRender_MaterialX.h
*  Project        AMD Radeon ProRender MaterialX API
*
*  Description    Radeon ProRender MaterialX header
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_MATERIALX_H
#define __RADEONPRORENDER_MATERIALX_H


#include "RadeonProRender.h"

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif



  /** @brief Parse a MaterialX XML data, and create the Material graph composed of rpr_material_nodes, and rpr_images
  *
  *  -----> This function is part of the 'Version 1' API - deprecated and replaced by the 'Version 2' API
  *
  * @param xmlData                       null-terminated string of the MaterialX XML data
  * @param resourcePaths and  resourcePathsCount   list of paths used for image loading
  *
  * @param imageAlreadyCreated_count
  * @param imageAlreadyCreated_paths
  * @param imageAlreadyCreated_list
  * We can specify a list of rpr_image that are already loaded. 
  * If rprLoadMaterialX finds any images in the XML belonging to this list it will use it directly instead of creating it with rprContextCreateImageFromFile
  * Those images will not be added in the listImagesOut list.
  * example to add an image in the imageAlreadyCreated list:
  * imageAlreadyCreated_count = 1
  * imageAlreadyCreated_paths[0] = "../../Textures/UVCheckerMap13-1024.png"    // same path specified in the 'value' of the image in the XML
  * imageAlreadyCreated_list[0] = (rpr_image) existing_rpr_image
  * imageAlreadyCreated_paths and imageAlreadyCreated_list must have the same size.
  *
  * @param listNodesOut 
  * @param listImagesOut
  * Thoses 2 buffers are allocated by rprLoadMaterialX, then you should use rprLoadMaterialX_free to free them.
  * they contain the list of rpr_material and rpr_image created by rprLoadMaterialX.
  *
  * @param rootNodeOut         Closure node in the material graph. Index inside listNodesOut. Could be -1 if an error occured.
  *                            This is the material that should be assigned to shape: rprShapeSetMaterial(shape,listNodesOut[rootNodeOut]);
  *
  * This function is NOT traced. However internally it's calling some RPR API to build the graph, those calls are traced.
  */
  extern RPR_API_ENTRY rpr_status rprLoadMaterialX(rpr_context in_context, rpr_material_system in_matsys, char const * xmlData, char const ** incudeData, int includeCount, char const ** resourcePaths, int resourcePathsCount, int imageAlreadyCreated_count, char const ** imageAlreadyCreated_paths, rpr_image * imageAlreadyCreated_list, rpr_material_node ** listNodesOut, rpr_uint * listNodesOut_count, rpr_image ** listImagesOut, rpr_uint * listImagesOut_count, rpr_uint * rootNodeOut, rpr_uint * rootDisplacementNodeOut);


  /** @brief Free the buffers allocated by rprLoadMaterialX
  *
  *  -----> This function is part of the 'Version 1' API - deprecated and replaced by the 'Version 2' API
  *
  * It does NOT call any rprObjectDelete
  * Internally it's doing a simple:
  * delete[] listNodes;
  * delete[] listImages;
  * 
  * This function is NOT traced.
  */
  extern RPR_API_ENTRY rpr_status rprLoadMaterialX_free(rpr_material_node * listNodes, rpr_image * listImages);


  /** @brief  Add resource search path.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  * example:
  * rprMaterialXAddResourceFolder(context, "dependency/");
  * rprMaterialXAddResourceFolder(context, "../imageLib/");
  * rprMaterialXSetFile(material, "materialx.mtlx");
  *  During the parsing of "materialx.mtlx" inside the rprMaterialXSetFile call, the folder path "dependency/" , "../imageLib/" will be used to search any files referenced in the materialX
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXAddResourceFolder(rpr_context in_context, rpr_char const * resourcePath);


  /** @brief  Clean the list created by rprMaterialXAddResourceFolder calls
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXCleanResourceFolder(rpr_context in_context);


  /** @brief  Add a dependency Mtlx file.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  * example:
  * rprMaterialXAddDependencyMtlx(context, "standard_surface.mtlx");
  * rprMaterialXSetFile(material, "materialx.mtlx");
  *  During the parsing of "materialx.mtlx" inside the rprMaterialXSetFile call, standard_surface.mtlx is also parsed and used as a dependancy file.
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXAddDependencyMtlx(rpr_context in_context, rpr_char const * resourcePath);


  /** @brief  Clean the list created by rprMaterialXAddDependencyMtlx calls
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXCleanDependencyMtlx(rpr_context in_context);


  /** @brief  Add a pre-loaded image to the MaterialX creation.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  * example:
  * rprMaterialXAddPreloadedImage(context, "images/back.png" , imgA);
  * rprMaterialXAddPreloadedImage(context, "images/skin.png" , imgB);
  * rprMaterialXSetFile(material, "materialx.mtlx");
  * During the parsing of "materialx.mtlx" inside the rprMaterialXSetFile call, if an image uses the exact path "images/back.png" or "images/skin.png" ,
  *     then rprMaterialXSetFile will use imgA/imgB instead of creation the image itself.
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXAddPreloadedImage(rpr_context in_context, rpr_char const * imgPath, rpr_image img);


  /** @brief  Clean the map created by rprMaterialXAddPreloadedImage calls.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXCleanPreloadedImages(rpr_context in_context);


  /** @brief  Assign the materialX file to a RPR material.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  *  The material must be created as RPR_MATERIAL_NODE_MATX type.
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXSetFile(rpr_material_node material, rpr_char const * xmlPath);


  /** @brief  Same that rprMaterialXSetFile but input a file buffer instead of the file.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  * 'buffer' represents the content of a XML string defining the MaterialX material.
  *          The size of the buffer is defined by 'bufferSize', not by a null-terminated character.
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXSetFileAsBuffer(rpr_material_node material, rpr_char const * buffer, size_t bufferSize);


  /** @brief   Return the Warning/Error messages from the last MaterialX Loading.
  *            This function helps to debug.
  *            If  rprMaterialXSetFile/rprMaterialXSetFileAsBuffer fails, it's a good practice to call rprMaterialXGetLoaderMessages just after.
  *
  *  'size' is the size of allocated 'data' buffer.
  *  'data' can be nullptr if we only want to get 'size_ret'.
  *  'size_ret' is the actual size of the out buffer - can be nullptr.
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXGetLoaderMessages(rpr_context in_context, size_t size, void * data, size_t * size_ret);




  #ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_MATERIALX_H  */
