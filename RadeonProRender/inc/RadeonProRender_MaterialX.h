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


  /** @brief  Add a dependency Mtlx file. Same than rprMaterialXAddDependencyMtlx, but input a file buffer instead of the file.
  *
  * -----> Note: This function is part of the 'Version 2' MaterialX API that replaces 'Version 1'
  *
  * 'buffer' represents the content of a XML string defining the MaterialX material.
  *          The size of the buffer is defined by 'bufferSize', not by a null-terminated character.
  *
  * example:
  * rprMaterialXAddDependencyMtlxAsBuffer(context, inbuffer, size);
  * rprMaterialXSetFile(material, "materialx.mtlx");
  *  During the parsing of "materialx.mtlx" inside the rprMaterialXSetFile call, 'inbuffer' is also parsed and used as a dependancy file.
  *
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXAddDependencyMtlxAsBuffer(rpr_context in_context, rpr_char const * buffer, size_t bufferSize);


  /** @brief  Clean the list created by rprMaterialXAddDependencyMtlx / rprMaterialXAddDependencyMtlxAsBuffer calls
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


  /** 
  * In MaterialX, Geompropvalue are referenced as strings, example: 
  * input name="geomprop" type="string" value="UVset01"
  * We can map this MaterialX Geompropvalue to a RadeonProRender Primvar, example:
  * rprMaterialXBindGeomPropToPrimvar(context, "UVset01", 100 );
  * In this example, the materialX "UVset01" will be used as the RadeonProRender Primvar key=100 ( created with rprShapeSetPrimvar )
  * Internally this is a map from geompropvalue to key, meaning a geompropvalue only has 1 unique key, but 1 key can have several geompropvalue.
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXBindGeomPropToPrimvar(rpr_context in_context, rpr_char const * geompropvalue, rpr_uint key);


  /** 
  * 
  * function to transform the final UV applied on the shape,
  * call it before rprMaterialXSetFile.
  *
  * example:
  * // the UV set 0 (Base UV) of matx is first translated by (0.7,0.6,0.0) and then scaled by (2,1.5,1)
  * rprMaterialXTexcoord( matx,   0.7f, 0.6f, 0.0f,    2.0f, 1.5f, 1.0f,    0 );
  * rprMaterialXSetFile( matx, "materialx.mtlx");
  * 
  */
  extern RPR_API_ENTRY rpr_status rprMaterialXTexcoord(rpr_material_node material, rpr_float offsetx, rpr_float offsety, rpr_float offsetz, rpr_float scalex, rpr_float scaley, rpr_float scalez, rpr_int uvSet);




  #ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_MATERIALX_H  */
