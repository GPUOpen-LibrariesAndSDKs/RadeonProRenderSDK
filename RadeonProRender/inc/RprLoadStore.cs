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

//using FireRender.AMD.RenderEngine.Common;
//using FireRender.Types;
//using FireRender.Types.Common;
using System;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace FireRender.AMD.RenderEngine.Core
{
public static class RprLoadStore
{
private const string dllName = "RprLoadStore64";
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
public struct RprsAnimationStruct // _rprs_animation
{
    public uint Structsize;
    public IntPtr Groupname;
    public uint Movementtype;
    public uint Interpolationtype;
    public uint Nbtimekeys;
    public uint Nbtransformvalues;
    public IntPtr Timekeys;
    public IntPtr Transformvalues;
};
public const uint RPRLOADSTORE_PARAMETER_TYPE_UNDEF = 0x0 ;
public const uint RPRLOADSTORE_PARAMETER_TYPE_INT = 0x1 ;
public const uint RPRLOADSTORE_PARAMETER_TYPE_FLOAT = 0x2 ;

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

public const uint RPRLOADSTORE_EXPORTFLAG_EXTERNALFILES = (1 << 0) ;
public const uint RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_1 = (1 << 1) ;
public const uint RPRLOADSTORE_EXPORTFLAG_COMPRESS_IMAGE_LEVEL_2 = (1 << 2) ;
public const uint RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_NORMALS = (1 << 3) ;
public const uint RPRLOADSTORE_EXPORTFLAG_COMPRESS_FLOAT_TO_HALF_UV = (1 << 4) ;
public const uint RPRLOADSTORE_EXPORTFLAG_EMBED_FILE_IMAGES_USING_OBJECTNAME = (1 << 5) ;
public const uint RPRLOADSTORE_EXPORTFLAG_USE_IMAGE_CACHE = (1 << 6) ;
public const uint RPRLOADSTORE_EXPORTFLAG_ONLY_EXPORT_ATTACHED_RENDER_LAYERS = (1 << 7) ;

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

[DllImport(dllName)] static extern Status rprsExport(IntPtr rprsFileName, IntPtr context, IntPtr scene, int extraCustomParam_int_number, IntPtr extraCustomParam_int_names, IntPtr extraCustomParam_int_values, int extraCustomParam_float_number, IntPtr extraCustomParam_float_names, IntPtr extraCustomParam_float_values, uint exportFlags, IntPtr rprsCtx);
public static Status sExport(IntPtr rprsFileName, IntPtr context, IntPtr scene, int extraCustomParam_int_number, IntPtr extraCustomParam_int_names, IntPtr extraCustomParam_int_values, int extraCustomParam_float_number, IntPtr extraCustomParam_float_names, IntPtr extraCustomParam_float_values, uint exportFlags, IntPtr rprsCtx)
{
return rprsExport(rprsFileName, context, scene, extraCustomParam_int_number, extraCustomParam_int_names, extraCustomParam_int_values, extraCustomParam_float_number, extraCustomParam_float_names, extraCustomParam_float_values, exportFlags, rprsCtx);
}
[DllImport(dllName)] static extern Status rprsxExport(IntPtr rprsFileName, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr scene, int extraCustomParam_int_number, IntPtr extraCustomParam_int_names, IntPtr extraCustomParam_int_values, int extraCustomParam_float_number, IntPtr extraCustomParam_float_names, IntPtr extraCustomParam_float_values, uint exportFlags, IntPtr rprsCtx);
public static Status sxExport(IntPtr rprsFileName, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr scene, int extraCustomParam_int_number, IntPtr extraCustomParam_int_names, IntPtr extraCustomParam_int_values, int extraCustomParam_float_number, IntPtr extraCustomParam_float_names, IntPtr extraCustomParam_float_values, uint exportFlags, IntPtr rprsCtx)
{
return rprsxExport(rprsFileName, context, contextX__NOT_USED_ANYMORE, scene, extraCustomParam_int_number, extraCustomParam_int_names, extraCustomParam_int_values, extraCustomParam_float_number, extraCustomParam_float_names, extraCustomParam_float_values, exportFlags, rprsCtx);
}

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

[DllImport(dllName)] static extern Status rprsImport(IntPtr rprsFileName, IntPtr context, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx);
public static Status sImport(IntPtr rprsFileName, IntPtr context, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx)
{
return rprsImport(rprsFileName, context, materialSystem, scene, useAlreadyExistingScene, rprsCtx);
}
[DllImport(dllName)] static extern Status rprsxImport(IntPtr rprsFileName, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx);
public static Status sxImport(IntPtr rprsFileName, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx)
{
return rprsxImport(rprsFileName, context, contextX__NOT_USED_ANYMORE, materialSystem, scene, useAlreadyExistingScene, rprsCtx);
}
[DllImport(dllName)] static extern Status rprsImportFromData(IntPtr data, long dataSize, IntPtr context, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx);
public static Status sImportFromData(IntPtr data, long dataSize, IntPtr context, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx)
{
return rprsImportFromData(data, dataSize, context, materialSystem, scene, useAlreadyExistingScene, rprsCtx);
}
[DllImport(dllName)] static extern Status rprsxImportFromData(IntPtr data, long dataSize, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx);
public static Status sxImportFromData(IntPtr data, long dataSize, IntPtr context, IntPtr contextX__NOT_USED_ANYMORE, IntPtr materialSystem, IntPtr scene, bool useAlreadyExistingScene, IntPtr rprsCtx)
{
return rprsxImportFromData(data, dataSize, context, contextX__NOT_USED_ANYMORE, materialSystem, scene, useAlreadyExistingScene, rprsCtx);
}

/** 
*
* if 'rprsFileName' is embedding some OCIO configuration, then we need to unpack them before render.
* This operation is NOT done inside rprsImport, so it should be called before or after rprsImport.
* This function also sets the RPR context RPR_CONTEXT_OCIO_CONFIG_PATH parameter to 'basePath'.  rprsImport doesn't set this parameter.
*
*/

[DllImport(dllName)] static extern Status rprsBuildOCIOFiles(IntPtr rprsFileName, IntPtr context, IntPtr basePath, IntPtr rprsCtx);
public static Status sBuildOCIOFiles(IntPtr rprsFileName, IntPtr context, IntPtr basePath, IntPtr rprsCtx)
{
return rprsBuildOCIOFiles(rprsFileName, context, basePath, rprsCtx);
}

//
// rprsGetExtraCustomParam**** can be called after an rprsImport. they give the 'extraCustomParam' that were given as argument to the rprsExport function and stored inside the RPRS file.
// Using the function with the 'Ex' suffix must be use if the rprsImport was using a RPRS_context.
//

[DllImport(dllName)] static extern Status rprsGetExtraCustomParam_int(IntPtr name, IntPtr value);
public static Status sGetExtraCustomParam_int(IntPtr name, IntPtr value)
{
return rprsGetExtraCustomParam_int(name, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamEx_int(IntPtr rprsCtx, IntPtr name, IntPtr value);
public static Status sGetExtraCustomParamEx_int(IntPtr rprsCtx, IntPtr name, IntPtr value)
{
return rprsGetExtraCustomParamEx_int(rprsCtx, name, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParam_float(IntPtr name, IntPtr value);
public static Status sGetExtraCustomParam_float(IntPtr name, IntPtr value)
{
return rprsGetExtraCustomParam_float(name, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamEx_float(IntPtr rprsCtx, IntPtr name, IntPtr value);
public static Status sGetExtraCustomParamEx_float(IntPtr rprsCtx, IntPtr name, IntPtr value)
{
return rprsGetExtraCustomParamEx_float(rprsCtx, name, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamIndex_int(int index, IntPtr value);
public static Status sGetExtraCustomParamIndex_int(int index, IntPtr value)
{
return rprsGetExtraCustomParamIndex_int(index, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamIndexEx_int(IntPtr rprsCtx, int index, IntPtr value);
public static Status sGetExtraCustomParamIndexEx_int(IntPtr rprsCtx, int index, IntPtr value)
{
return rprsGetExtraCustomParamIndexEx_int(rprsCtx, index, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamIndex_float(int index, IntPtr value);
public static Status sGetExtraCustomParamIndex_float(int index, IntPtr value)
{
return rprsGetExtraCustomParamIndex_float(index, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamIndexEx_float(IntPtr rprsCtx, int index, IntPtr value);
public static Status sGetExtraCustomParamIndexEx_float(IntPtr rprsCtx, int index, IntPtr value)
{
return rprsGetExtraCustomParamIndexEx_float(rprsCtx, index, value);
}
[DllImport(dllName)] static extern int rprsGetNumberOfExtraCustomParam();
public static void  sGetNumberOfExtraCustomParam()
{
Check(rprsGetNumberOfExtraCustomParam());
}
[DllImport(dllName)] static extern int rprsGetNumberOfExtraCustomParamEx(IntPtr rprsCtx);
public static void  sGetNumberOfExtraCustomParamEx(IntPtr rprsCtx)
{
Check(rprsGetNumberOfExtraCustomParamEx(rprsCtx));
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamNameSize(int index, IntPtr nameSizeGet);
public static Status sGetExtraCustomParamNameSize(int index, IntPtr nameSizeGet)
{
return rprsGetExtraCustomParamNameSize(index, nameSizeGet);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamNameSizeEx(IntPtr rprsCtx, int index, IntPtr nameSizeGet);
public static Status sGetExtraCustomParamNameSizeEx(IntPtr rprsCtx, int index, IntPtr nameSizeGet)
{
return rprsGetExtraCustomParamNameSizeEx(rprsCtx, index, nameSizeGet);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamName(int index, IntPtr nameGet, int nameGetSize);
public static Status sGetExtraCustomParamName(int index, IntPtr nameGet, int nameGetSize)
{
return rprsGetExtraCustomParamName(index, nameGet, nameGetSize);
}
[DllImport(dllName)] static extern Status rprsGetExtraCustomParamNameEx(IntPtr rprsCtx, int index, IntPtr nameGet, int nameGetSize);
public static Status sGetExtraCustomParamNameEx(IntPtr rprsCtx, int index, IntPtr nameGet, int nameGetSize)
{
return rprsGetExtraCustomParamNameEx(rprsCtx, index, nameGet, nameGetSize);
}
[DllImport(dllName)] static extern int rprsGetExtraCustomParamType(int index);
public static void  sGetExtraCustomParamType(int index)
{
Check(rprsGetExtraCustomParamType(index));
}
[DllImport(dllName)] static extern int rprsGetExtraCustomParamTypeEx(IntPtr rprsCtx, int index);
public static void  sGetExtraCustomParamTypeEx(IntPtr rprsCtx, int index)
{
Check(rprsGetExtraCustomParamTypeEx(rprsCtx, index));
}

// 
// rprsListImported*** can be called after an rprsImport. those functions will list each RPR objects created during the import.
// rprsListImportedMaterialX si deprecated and may be removed in the future.
// Using the function with the 'Ex' suffix must be use if the rprsImport was using a RPRS_context.
//

[DllImport(dllName)] static extern Status rprsListImportedCameras(IntPtr Cameras, int sizeCameraBytes, IntPtr numberOfCameras);
public static Status sListImportedCameras(IntPtr Cameras, int sizeCameraBytes, IntPtr numberOfCameras)
{
return rprsListImportedCameras(Cameras, sizeCameraBytes, numberOfCameras);
}
[DllImport(dllName)] static extern Status rprsListImportedCamerasEx(IntPtr rprsCtx, IntPtr Cameras, int sizeCameraBytes, IntPtr numberOfCameras);
public static Status sListImportedCamerasEx(IntPtr rprsCtx, IntPtr Cameras, int sizeCameraBytes, IntPtr numberOfCameras)
{
return rprsListImportedCamerasEx(rprsCtx, Cameras, sizeCameraBytes, numberOfCameras);
}
[DllImport(dllName)] static extern Status rprsListImportedMaterialX(IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes);
public static Status sListImportedMaterialX(IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes)
{
return rprsListImportedMaterialX(MaterialNodes, sizeMaterialNodeBytes, numberOfMaterialNodes);
}
[DllImport(dllName)] static extern Status rprsListImportedMaterialXEx(IntPtr rprsCtx, IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes);
public static Status sListImportedMaterialXEx(IntPtr rprsCtx, IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes)
{
return rprsListImportedMaterialXEx(rprsCtx, MaterialNodes, sizeMaterialNodeBytes, numberOfMaterialNodes);
}
[DllImport(dllName)] static extern Status rprsListImportedMaterialNodes(IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes);
public static Status sListImportedMaterialNodes(IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes)
{
return rprsListImportedMaterialNodes(MaterialNodes, sizeMaterialNodeBytes, numberOfMaterialNodes);
}
[DllImport(dllName)] static extern Status rprsListImportedMaterialNodesEx(IntPtr rprsCtx, IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes);
public static Status sListImportedMaterialNodesEx(IntPtr rprsCtx, IntPtr MaterialNodes, int sizeMaterialNodeBytes, IntPtr numberOfMaterialNodes)
{
return rprsListImportedMaterialNodesEx(rprsCtx, MaterialNodes, sizeMaterialNodeBytes, numberOfMaterialNodes);
}
[DllImport(dllName)] static extern Status rprsListImportedLights(IntPtr Lights, int sizeLightBytes, IntPtr numberOfLights);
public static Status sListImportedLights(IntPtr Lights, int sizeLightBytes, IntPtr numberOfLights)
{
return rprsListImportedLights(Lights, sizeLightBytes, numberOfLights);
}
[DllImport(dllName)] static extern Status rprsListImportedLightsEx(IntPtr rprsCtx, IntPtr Lights, int sizeLightBytes, IntPtr numberOfLights);
public static Status sListImportedLightsEx(IntPtr rprsCtx, IntPtr Lights, int sizeLightBytes, IntPtr numberOfLights)
{
return rprsListImportedLightsEx(rprsCtx, Lights, sizeLightBytes, numberOfLights);
}
[DllImport(dllName)] static extern Status rprsListImportedShapes(IntPtr Shapes, int sizeShapeBytes, IntPtr numberOfShapes);
public static Status sListImportedShapes(IntPtr Shapes, int sizeShapeBytes, IntPtr numberOfShapes)
{
return rprsListImportedShapes(Shapes, sizeShapeBytes, numberOfShapes);
}
[DllImport(dllName)] static extern Status rprsListImportedShapesEx(IntPtr rprsCtx, IntPtr Shapes, int sizeShapeBytes, IntPtr numberOfShapes);
public static Status sListImportedShapesEx(IntPtr rprsCtx, IntPtr Shapes, int sizeShapeBytes, IntPtr numberOfShapes)
{
return rprsListImportedShapesEx(rprsCtx, Shapes, sizeShapeBytes, numberOfShapes);
}
[DllImport(dllName)] static extern Status rprsListImportedCurves(IntPtr Curves, int sizeCurveBytes, IntPtr numberOfCurves);
public static Status sListImportedCurves(IntPtr Curves, int sizeCurveBytes, IntPtr numberOfCurves)
{
return rprsListImportedCurves(Curves, sizeCurveBytes, numberOfCurves);
}
[DllImport(dllName)] static extern Status rprsListImportedCurvesEx(IntPtr rprsCtx, IntPtr Curves, int sizeCurveBytes, IntPtr numberOfCurves);
public static Status sListImportedCurvesEx(IntPtr rprsCtx, IntPtr Curves, int sizeCurveBytes, IntPtr numberOfCurves)
{
return rprsListImportedCurvesEx(rprsCtx, Curves, sizeCurveBytes, numberOfCurves);
}
[DllImport(dllName)] static extern Status rprsListImportedPostEffects(IntPtr PostEffects, int sizePostEffectsBytes, IntPtr numberOfPostEffects);
public static Status sListImportedPostEffects(IntPtr PostEffects, int sizePostEffectsBytes, IntPtr numberOfPostEffects)
{
return rprsListImportedPostEffects(PostEffects, sizePostEffectsBytes, numberOfPostEffects);
}
[DllImport(dllName)] static extern Status rprsListImportedPostEffectsEx(IntPtr rprsCtx, IntPtr PostEffects, int sizePostEffectsBytes, IntPtr numberOfPostEffects);
public static Status sListImportedPostEffectsEx(IntPtr rprsCtx, IntPtr PostEffects, int sizePostEffectsBytes, IntPtr numberOfPostEffects)
{
return rprsListImportedPostEffectsEx(rprsCtx, PostEffects, sizePostEffectsBytes, numberOfPostEffects);
}
[DllImport(dllName)] static extern Status rprsListImportedHeteroVolumes(IntPtr HeteroVolumes, int sizeHeteroVolumesBytes, IntPtr numberOfHeteroVolumes);
public static Status sListImportedHeteroVolumes(IntPtr HeteroVolumes, int sizeHeteroVolumesBytes, IntPtr numberOfHeteroVolumes)
{
return rprsListImportedHeteroVolumes(HeteroVolumes, sizeHeteroVolumesBytes, numberOfHeteroVolumes);
}
[DllImport(dllName)] static extern Status rprsListImportedHeteroVolumesEx(IntPtr rprsCtx, IntPtr HeteroVolumes, int sizeHeteroVolumesBytes, IntPtr numberOfHeteroVolumes);
public static Status sListImportedHeteroVolumesEx(IntPtr rprsCtx, IntPtr HeteroVolumes, int sizeHeteroVolumesBytes, IntPtr numberOfHeteroVolumes)
{
return rprsListImportedHeteroVolumesEx(rprsCtx, HeteroVolumes, sizeHeteroVolumesBytes, numberOfHeteroVolumes);
}
[DllImport(dllName)] static extern Status rprsListImportedGrids(IntPtr Grids, int sizeGridsBytes, IntPtr numberOfGrids);
public static Status sListImportedGrids(IntPtr Grids, int sizeGridsBytes, IntPtr numberOfGrids)
{
return rprsListImportedGrids(Grids, sizeGridsBytes, numberOfGrids);
}
[DllImport(dllName)] static extern Status rprsListImportedGridsEx(IntPtr rprsCtx, IntPtr Grids, int sizeGridsBytes, IntPtr numberOfGrids);
public static Status sListImportedGridsEx(IntPtr rprsCtx, IntPtr Grids, int sizeGridsBytes, IntPtr numberOfGrids)
{
return rprsListImportedGridsEx(rprsCtx, Grids, sizeGridsBytes, numberOfGrids);
}
[DllImport(dllName)] static extern Status rprsListImportedBuffers(IntPtr Buffers, int sizeBuffersBytes, IntPtr numberOfBuffers);
public static Status sListImportedBuffers(IntPtr Buffers, int sizeBuffersBytes, IntPtr numberOfBuffers)
{
return rprsListImportedBuffers(Buffers, sizeBuffersBytes, numberOfBuffers);
}
[DllImport(dllName)] static extern Status rprsListImportedBuffersEx(IntPtr rprsCtx, IntPtr Buffers, int sizeBuffersBytes, IntPtr numberOfBuffers);
public static Status sListImportedBuffersEx(IntPtr rprsCtx, IntPtr Buffers, int sizeBuffersBytes, IntPtr numberOfBuffers)
{
return rprsListImportedBuffersEx(rprsCtx, Buffers, sizeBuffersBytes, numberOfBuffers);
}
[DllImport(dllName)] static extern Status rprsListImportedImages(IntPtr Images, int sizeImageBytes, IntPtr numberOfImages);
public static Status sListImportedImages(IntPtr Images, int sizeImageBytes, IntPtr numberOfImages)
{
return rprsListImportedImages(Images, sizeImageBytes, numberOfImages);
}
[DllImport(dllName)] static extern Status rprsListImportedImagesEx(IntPtr rprsCtx, IntPtr Images, int sizeImageBytes, IntPtr numberOfImages);
public static Status sListImportedImagesEx(IntPtr rprsCtx, IntPtr Images, int sizeImageBytes, IntPtr numberOfImages)
{
return rprsListImportedImagesEx(rprsCtx, Images, sizeImageBytes, numberOfImages);
}

//
// rprsExportCustomList and rprsImportCustomList are deprecated and not supported anymore.
//

[DllImport(dllName)] static extern Status rprsExportCustomList(IntPtr rprsFileName, int materialNode_number, IntPtr materialNode_list, int camera_number, IntPtr camera_list, int light_number, IntPtr light_list, int shape_number, IntPtr shape_list, int image_number, IntPtr image_list, IntPtr rprsCtx);
public static Status sExportCustomList(IntPtr rprsFileName, int materialNode_number, IntPtr materialNode_list, int camera_number, IntPtr camera_list, int light_number, IntPtr light_list, int shape_number, IntPtr shape_list, int image_number, IntPtr image_list, IntPtr rprsCtx)
{
return rprsExportCustomList(rprsFileName, materialNode_number, materialNode_list, camera_number, camera_list, light_number, light_list, shape_number, shape_list, image_number, image_list, rprsCtx);
}
[DllImport(dllName)] static extern Status rprsImportCustomList(IntPtr rprsFileName, IntPtr context, IntPtr materialSystem, IntPtr materialNode_number, IntPtr materialNode_list, IntPtr camera_number, IntPtr camera_list, IntPtr light_number, IntPtr light_list, IntPtr shape_number, IntPtr shape_list, IntPtr image_number, IntPtr image_list, IntPtr rprsCtx);
public static Status sImportCustomList(IntPtr rprsFileName, IntPtr context, IntPtr materialSystem, IntPtr materialNode_number, IntPtr materialNode_list, IntPtr camera_number, IntPtr camera_list, IntPtr light_number, IntPtr light_list, IntPtr shape_number, IntPtr shape_list, IntPtr image_number, IntPtr image_list, IntPtr rprsCtx)
{
return rprsImportCustomList(rprsFileName, context, materialSystem, materialNode_number, materialNode_list, camera_number, camera_list, light_number, light_list, shape_number, shape_list, image_number, image_list, rprsCtx);
}




// In classic usage, rprsExport only saves 1 camera ( the camera attached to scene )
// however, user may need to store more cameras. You can do that with this function.
// Just call rprsAddExtraCamera for each extra camera, then call rprsExport.
// internally, the list of extra cameras will be reset as soon as you call rprsExport
// return RPR_SUCCESS if success.

[DllImport(dllName)] static extern Status rprsAddExtraCamera(IntPtr extraCam);
public static Status sAddExtraCamera(IntPtr extraCam)
{
return rprsAddExtraCamera(extraCam);
}
[DllImport(dllName)] static extern Status rprsAddExtraCameraEx(IntPtr rprsCtx, IntPtr extraCam);
public static Status sAddExtraCameraEx(IntPtr rprsCtx, IntPtr extraCam)
{
return rprsAddExtraCameraEx(rprsCtx, extraCam);
}




// Some custom 'extra' parameters can be attached to shape and saved inside the RPRS file.
// Call rprsAddExtraShapeParameter before the rprsExport call to add those parameters in the exporter.
// Call rprsGetExtraShapeParameter after the rprsImport call in order to read those parameters.

[DllImport(dllName)] static extern Status rprsAddExtraShapeParameter(IntPtr shape, IntPtr parameterName, int value);
public static Status sAddExtraShapeParameter(IntPtr shape, IntPtr parameterName, int value)
{
return rprsAddExtraShapeParameter(shape, parameterName, value);
}
[DllImport(dllName)] static extern Status rprsAddExtraShapeParameterEx(IntPtr rprsCtx, IntPtr shape, IntPtr parameterName, int value);
public static Status sAddExtraShapeParameterEx(IntPtr rprsCtx, IntPtr shape, IntPtr parameterName, int value)
{
return rprsAddExtraShapeParameterEx(rprsCtx, shape, parameterName, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraShapeParameter(IntPtr shape, IntPtr parameterName, IntPtr value);
public static Status sGetExtraShapeParameter(IntPtr shape, IntPtr parameterName, IntPtr value)
{
return rprsGetExtraShapeParameter(shape, parameterName, value);
}
[DllImport(dllName)] static extern Status rprsGetExtraShapeParameterEx(IntPtr rprsCtx, IntPtr shape, IntPtr parameterName, IntPtr value);
public static Status sGetExtraShapeParameterEx(IntPtr rprsCtx, IntPtr shape, IntPtr parameterName, IntPtr value)
{
return rprsGetExtraShapeParameterEx(rprsCtx, shape, parameterName, value);
}

// rprsExportToXML is for Debugging usecase only.
// This function converts an RPRS file ( created with rprsExport ) into an XML representing this file.
// The binary buffers ( textures, geometry ... ) are not exported.
// This XML file is not designed to be imported.
// Arguments:
// 'rprsFilePath' : input RPRS file.
// 'xmlFileOut' : name of the output XML file.

[DllImport(dllName)] static extern Status rprsExportToXML(IntPtr rprsFilePath, IntPtr xmlFileOut);
public static Status sExportToXML(IntPtr rprsFilePath, IntPtr xmlFileOut)
{
return rprsExportToXML(rprsFilePath, xmlFileOut);
}
[DllImport(dllName)] static extern Status rprsExportToXMLEx(IntPtr rprsCtx, IntPtr rprsFilePath, IntPtr xmlFileOut);
public static Status sExportToXMLEx(IntPtr rprsCtx, IntPtr rprsFilePath, IntPtr xmlFileOut)
{
return rprsExportToXMLEx(rprsCtx, rprsFilePath, xmlFileOut);
}

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

[DllImport(dllName)] static extern Status rprsAssignShapeToGroup(IntPtr shape, IntPtr groupName);
public static Status sAssignShapeToGroup(IntPtr shape, IntPtr groupName)
{
return rprsAssignShapeToGroup(shape, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignShapeToGroupEx(IntPtr rprsCtx, IntPtr shape, IntPtr groupName);
public static Status sAssignShapeToGroupEx(IntPtr rprsCtx, IntPtr shape, IntPtr groupName)
{
return rprsAssignShapeToGroupEx(rprsCtx, shape, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignLightToGroup(IntPtr light, IntPtr groupName);
public static Status sAssignLightToGroup(IntPtr light, IntPtr groupName)
{
return rprsAssignLightToGroup(light, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignLightToGroupEx(IntPtr rprsCtx, IntPtr light, IntPtr groupName);
public static Status sAssignLightToGroupEx(IntPtr rprsCtx, IntPtr light, IntPtr groupName)
{
return rprsAssignLightToGroupEx(rprsCtx, light, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignCameraToGroup(IntPtr camera, IntPtr groupName);
public static Status sAssignCameraToGroup(IntPtr camera, IntPtr groupName)
{
return rprsAssignCameraToGroup(camera, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignCameraToGroupEx(IntPtr rprsCtx, IntPtr camera, IntPtr groupName);
public static Status sAssignCameraToGroupEx(IntPtr rprsCtx, IntPtr camera, IntPtr groupName)
{
return rprsAssignCameraToGroupEx(rprsCtx, camera, groupName);
}
[DllImport(dllName)] static extern Status rprsAssignParentGroupToGroup(IntPtr groupChild, IntPtr groupParent);
public static Status sAssignParentGroupToGroup(IntPtr groupChild, IntPtr groupParent)
{
return rprsAssignParentGroupToGroup(groupChild, groupParent);
}
[DllImport(dllName)] static extern Status rprsAssignParentGroupToGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr groupParent);
public static Status sAssignParentGroupToGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr groupParent)
{
return rprsAssignParentGroupToGroupEx(rprsCtx, groupChild, groupParent);
}
[DllImport(dllName)] static extern Status rprsSetTransformGroup(IntPtr groupChild, IntPtr matrixComponents);
public static Status sSetTransformGroup(IntPtr groupChild, IntPtr matrixComponents)
{
return rprsSetTransformGroup(groupChild, matrixComponents);
}
[DllImport(dllName)] static extern Status rprsSetTransformGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr matrixComponents);
public static Status sSetTransformGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr matrixComponents)
{
return rprsSetTransformGroupEx(rprsCtx, groupChild, matrixComponents);
}
[DllImport(dllName)] static extern Status rprsGetTransformGroup(IntPtr groupChild, IntPtr matrixComponents);
public static Status sGetTransformGroup(IntPtr groupChild, IntPtr matrixComponents)
{
return rprsGetTransformGroup(groupChild, matrixComponents);
}
[DllImport(dllName)] static extern Status rprsGetTransformGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr matrixComponents);
public static Status sGetTransformGroupEx(IntPtr rprsCtx, IntPtr groupChild, IntPtr matrixComponents)
{
return rprsGetTransformGroupEx(rprsCtx, groupChild, matrixComponents);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromShape(IntPtr shape, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromShape(IntPtr shape, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromShape(shape, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromShapeEx(IntPtr rprsCtx, IntPtr shape, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromShapeEx(IntPtr rprsCtx, IntPtr shape, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromShapeEx(rprsCtx, shape, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromCamera(IntPtr camera, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromCamera(IntPtr camera, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromCamera(camera, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromCameraEx(IntPtr rprsCtx, IntPtr camera, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromCameraEx(IntPtr rprsCtx, IntPtr camera, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromCameraEx(rprsCtx, camera, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromLight(IntPtr light, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromLight(IntPtr light, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromLight(light, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromLightEx(IntPtr rprsCtx, IntPtr light, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromLightEx(IntPtr rprsCtx, IntPtr light, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromLightEx(rprsCtx, light, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromGroup(IntPtr groupChild, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromGroup(IntPtr groupChild, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromGroup(groupChild, size, groupName, size_ret);
}
[DllImport(dllName)] static extern Status rprsGetParentGroupFromGroupEx(IntPtr rprsCtx, IntPtr groupChild, long size, IntPtr groupName, IntPtr size_ret);
public static Status sGetParentGroupFromGroupEx(IntPtr rprsCtx, IntPtr groupChild, long size, IntPtr groupName, IntPtr size_ret)
{
return rprsGetParentGroupFromGroupEx(rprsCtx, groupChild, size, groupName, size_ret);
}

// Create a RPRS Context. This object can be used to store additional data ( animation, parameters ... ) for an Export.
// And it can be used to read them back after an Import.
// From 2.02.7 RPR SDK it's advised to use the RPRS_context. meaning that  rprsExport/rprsImport  should not give a nullptr to the rprsCtx argument.
//    and function with the 'Ex' suffix should be used.

[DllImport(dllName)] static extern Status rprsCreateContext(out IntPtr out_rprsCtx);
public static Status sCreateContext(out IntPtr out_rprsCtx)
{
return rprsCreateContext(out out_rprsCtx);
}

// Delete a RPRS Context:
// the rprsCtx object and any data hold by it are destroyed (pointers become invalid).

[DllImport(dllName)] static extern Status rprsDeleteContext(IntPtr rprsCtx);
public static Status sDeleteContext(IntPtr rprsCtx)
{
return rprsDeleteContext(rprsCtx);
}

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

[DllImport(dllName)] static extern Status rprsReleaseImportedData();
public static Status sReleaseImportedData()
{
return rprsReleaseImportedData();
}

// void* contextX__NOT_USED_ANYMORE can be set to null  - not used anymore.
// Delete RPR objects created by last rprsImport : camera, material_node, shape, light, image, post_effect, hetero_volume, buffer, curve
// do NOT any delete rpr_context, rpr_scene and the RPRS_context

[DllImport(dllName)] static extern Status rprsDeleteListImportedObjects(IntPtr contextX__NOT_USED_ANYMORE);
public static Status sDeleteListImportedObjects(IntPtr contextX__NOT_USED_ANYMORE)
{
return rprsDeleteListImportedObjects(contextX__NOT_USED_ANYMORE);
}
[DllImport(dllName)] static extern Status rprsDeleteListImportedObjectsEx(IntPtr rprsCtx);
public static Status sDeleteListImportedObjectsEx(IntPtr rprsCtx)
{
return rprsDeleteListImportedObjectsEx(rprsCtx);
}

//rprs_animation_movement_type

public const uint RPRS_ANIMATION_MOVEMENTTYPE_TRANSLATION = 0x1 ;
public const uint RPRS_ANIMATION_MOVEMENTTYPE_ROTATION = 0x2 ;
public const uint RPRS_ANIMATION_MOVEMENTTYPE_SCALE = 0x3 ;

// structSize : size of this struct in Byte (internally used to identify if different versions)
// interpolationType : unused for now - set it to 0
//
// example : if the animation has 2 FLOAT3 defining translation at time 0.5 and 3.0  for a translation along y axis , we have  :
//
//nbTimeKeys = 2
//nbTransformValues = 2 
//timeKeys        = { 0.5 , 3.0 }
//transformValues = { 0.0,0.0,0.0,  0.0,1.0,0.0,  }


// rprsGetAnimation can be called after a rprsImport call. This function gives dedails on each animations stored in the RPRS file.
// return null if not animation exists for animIndex
// Using the function with the 'Ex' suffix must be used if the rprsImport was using a RPRS_context.

[DllImport(dllName)] static extern IntPtr rprsGetAnimation(int animIndex);
public static IntPtr sGetAnimation(int animIndex)
{
return rprsGetAnimation(animIndex);
}
[DllImport(dllName)] static extern IntPtr rprsGetAnimationEx(IntPtr rprsCtx, int animIndex);
public static IntPtr sGetAnimationEx(IntPtr rprsCtx, int animIndex)
{
return rprsGetAnimationEx(rprsCtx, animIndex);
}

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

[DllImport(dllName)] static extern Status rprsAddAnimation(IntPtr anim);
public static Status sAddAnimation(IntPtr anim)
{
return rprsAddAnimation(anim);
}
[DllImport(dllName)] static extern Status rprsAddAnimationEx(IntPtr rprsCtx, IntPtr anim);
public static Status sAddAnimationEx(IntPtr rprsCtx, IntPtr anim)
{
return rprsAddAnimationEx(rprsCtx, anim);
}

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

}
}
