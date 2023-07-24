
#
#
# DEMO of RPR Rendering with Python Binding.
#
#


import sys
import os

binFolder = ""
pluginFile = ""
if sys.platform == "linux":
    binFolder = "binUbuntu20"
    pluginFile = "libNorthstar64.so"
elif sys.platform == "win32":
    binFolder = "binWin64"
    pluginFile = "Northstar64.dll"
else:
    binFolder = "binMacOS"
    pluginFile = "libNorthstar64.dylib"


# add the RPR Python library folder in order to import the rpr,rprs,rprgltf modules.
if sys.platform == "win32":
    sys.path.insert(1, '../../python/build/Release')
else:
	sys.path.insert(1, '../../python/build')

# we need to add the RPR DLLs
if sys.platform == "win32":
    os.add_dll_directory(  os.path.abspath(os.path.dirname(__file__) + '../../../RadeonProRender/' + binFolder )    )

	
import rpr
import rprs
import ctypes
import numpy as np


def RPRCHECK(retCode):
    if ( retCode != rpr.Status.SUCCESS ):
        print(f"RPR ERROR: {retCode}")
        exit()


print("-- script starts --");


imageOutFile = [ "OUT_PYTHON_1.png" ,  "OUT_PYTHON_2.png"  ,  "OUT_PYTHON_3.png" ,  "OUT_PYTHON_4.png" ]



plugin_id = rpr.RegisterPlugin((str("../../RadeonProRender/"+binFolder+"/"+pluginFile)))
if ( plugin_id == -1 ):
    print(f"RPR ERROR: plugin not found.")
    exit()


# remove previous generated image files
for i in imageOutFile:
    if os.path.exists(i):
        os.remove(i)


# CONTEXT CREATION
print("RPR context creating...");
flagsCtx = 0
flagsCtx |= int(rpr.CreationFlags.ENABLE_GPU0)

# with Northstar, add this flag to render with OpenCL instead of HIP :
#flagsCtx |= int(rpr.CreationFlags.ENABLE_OPENCL)

newCtx = rpr.Context()
ctxInfo1 = rpr.PyMalloc("../../hipbin")
pluginLists__myarray =    np.array([plugin_id], dtype=np.int32)  
RPRCHECK( rpr.CreateContext(0x00300102  ,  pluginLists__myarray   , 1  , flagsCtx ,   [   rpr.Int64ToPvoid(rpr.ContextInfo.PRECOMPILED_BINARY_PATH) ,   ctxInfo1.GetMem(0)  ,  rpr.Int64ToPvoid(0)   ] , "" ,  newCtx ) )
ctxInfo1.Free(); ctxInfo1=0;
RPRCHECK( rpr.ContextSetActivePlugin(newCtx, plugin_id) )
print("RPR context created.");

matsys = rpr.MaterialSystem()
RPRCHECK( rpr.ContextCreateMaterialSystem(newCtx, 0, matsys) )


# CREATE SCENE
rprScene = rpr.Scene()
RPRCHECK( rpr.ContextCreateScene(newCtx, rprScene) )
RPRCHECK( rpr.ContextSetScene( newCtx , rprScene ) )


# CREATE CAMERA
rprCamera = rpr.Camera()
RPRCHECK( rpr.ContextCreateCamera(  newCtx , rprCamera  ) )
RPRCHECK( rpr.CameraLookAt(rprCamera  , 0, 5, 20,   0, 1, 0,    0, 1, 0  ) )
RPRCHECK( rpr.CameraSetFocalLength(rprCamera, 75.0) )
RPRCHECK( rpr.SceneSetCamera(rprScene,   rprCamera) )






# CREATE SHAPE

cube_data = np.array([  -1.0, 1.0, -1.0,    0.0, 1.0, 0.0,    0.0, 0.0  ,
					   1.0, 1.0, -1.0,    0.0, 1.0, 0.0,    1.0, 0.0  ,
					    1.0, 1.0, 1.0,     0.0, 1.0, 0.0,    1.0, 1.0  ,
					    -1.0, 1.0, 1.0,    0.0, 1.0, 0.0,    0.0, 1.0   ,

					    -1.0, -1.0, -1.0,  0.0, -1.0, 0.0,     0.0, 0.0    ,
					    1.0, -1.0, -1.0,   0.0, -1.0, 0.0,     1.0, 0.0    ,
					     1.0, -1.0, 1.0,    0.0, -1.0, 0.0,     1.0, 1.0   ,
					    -1.0, -1.0, 1.0,   0.0, -1.0, 0.0,     0.0, 1.0    ,

					    -1.0, -1.0, 1.0,   -1.0, 0.0, 0.0,     0.0, 0.0   ,
					   -1.0, -1.0, -1.0,  -1.0, 0.0, 0.0,     1.0, 0.0   ,
					     -1.0, 1.0, -1.0,   -1.0, 0.0, 0.0,     1.0, 1.0    ,
					    -1.0, 1.0, 1.0,    -1.0, 0.0, 0.0,     0.0, 1.0   ,

					    1.0, -1.0, 1.0,     1.0, 0.0, 0.0,    0.0, 0.0    ,
					    1.0, -1.0, -1.0,    1.0, 0.0, 0.0,    1.0, 0.0   ,
					     1.0, 1.0, -1.0,     1.0, 0.0, 0.0,    1.0, 1.0    ,
					    1.0, 1.0, 1.0,      1.0, 0.0, 0.0,    0.0, 1.0   ,

					    -1.0, -1.0, -1.0,    0.0, 0.0, -1.0 ,    0.0, 0.0   ,
					    1.0, -1.0, -1.0,     0.0, 0.0, -1.0 ,    1.0, 0.0   ,
					     1.0, 1.0, -1.0,      0.0, 0.0, -1.0,     1.0, 1.0   ,
					    -1.0, 1.0, -1.0,     0.0, 0.0, -1.0,     0.0, 1.0    ,

					     -1.0, -1.0, 1.0,   0.0, 0.0, 1.0,     0.0, 0.0    ,
					     1.0, -1.0, 1.0,    0.0, 0.0, 1.0,     1.0, 0.0    ,
					    1.0, 1.0, 1.0,     0.0, 0.0, 1.0,     1.0, 1.0   ,
					    -1.0, 1.0, 1.0,    0.0, 0.0, 1.0,     0.0, 1.0   

					 ], dtype=np.float32)


indices = np.array([
    3,1,0,
    2,1,3,

    6,4,5,
    7,4,6,

    11,9,8,
    10,9,11,

    14,12,13,
    15,12,14,

    19,17,16,
    18,17,19,

    22,20,21,
    23,20,22
    ], dtype=np.int32)

num_face_vertices = np.array([3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3], dtype=np.int32)

rprCube  = rpr.Shape()
RPRCHECK( rpr.ContextCreateMesh(newCtx,
    cube_data, 0,   24,  8*4,
    cube_data, 3,   24,  8*4,
    cube_data, 6,   24,  8*4,
    indices , 4,
    indices , 4,
    indices , 4,
    num_face_vertices,
    12,
    rprCube ) )

RPRCHECK( rpr.SceneAttachShape(rprScene, rprCube) )

transform = np.array([
            1.0,  0.0,    0.0,   -2.0,
            0,    1.0,    0,     1.0,
            0,    0,     1.0,    0,
            0.0,  0.0,    0,     1
        ], dtype=np.float32)

RPRCHECK( rpr.ShapeSetTransform(rprCube, True, transform) )





# CREATE MATERIAL

diffuseA = rpr.MaterialNode()
diffuseB = rpr.MaterialNode()
uv_node = rpr.MaterialNode()
uv_scaled_node = rpr.MaterialNode()
checker = rpr.MaterialNode()
layered = rpr.MaterialNode()

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.DIFFUSE, diffuseA) )
RPRCHECK( rpr.MaterialNodeSetInputFByKey(diffuseA, rpr.MaterialInput.COLOR, 0.0, 0.5, 1.0, 0.0) )

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.DIFFUSE, diffuseB) )
RPRCHECK( rpr.MaterialNodeSetInputFByKey(diffuseB, rpr.MaterialInput.COLOR, 0.5, 0.20, 1.0, 0.0) )

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.INPUT_LOOKUP, uv_node) )
RPRCHECK( rpr.MaterialNodeSetInputUByKey(uv_node, rpr.MaterialInput.VALUE, rpr.MaterialNodeLookup.UV) )

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.ARITHMETIC, uv_scaled_node) )
RPRCHECK( rpr.MaterialNodeSetInputUByKey(uv_scaled_node, rpr.MaterialInput.OP, rpr.MaterialNodeOp.MUL) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(uv_scaled_node, rpr.MaterialInput.COLOR0, uv_node) )
RPRCHECK( rpr.MaterialNodeSetInputFByKey(uv_scaled_node, rpr.MaterialInput.COLOR1, 0.7, 0.7, 0.0, 0) )

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.CHECKER_TEXTURE, checker) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(checker, rpr.MaterialInput.UV  , uv_scaled_node) )

RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.BLEND, layered) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(layered, rpr.MaterialInput.COLOR0, diffuseA) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(layered, rpr.MaterialInput.COLOR1, diffuseB) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(layered, rpr.MaterialInput.WEIGHT, checker) )

RPRCHECK( rpr.ShapeSetMaterial(rprCube,layered) )



# CREATE A SHAPE

plane_data = np.array([
    -15.0, 0.0, -15.0,    0.0, 1.0, 0.0,    0.0, 1.0,
    -15.0, 0.0,  15.0,    0.0, 1.0, 0.0,    0.0, 0.0,
     15.0, 0.0,  15.0,    0.0, 1.0, 0.0,    1.0, 0.0,
     15.0, 0.0, -15.0,    0.0, 1.0, 0.0,    1.0, 1.0,
     ], dtype=np.float32)


rprPlane = rpr.Shape()
RPRCHECK(  rpr.ContextCreateMesh(newCtx,
    plane_data, 0,   4,  8*4,
    plane_data, 3,   4,  8*4,
    plane_data, 6,   4,  8*4,
    indices , 4,
    indices , 4,
    indices , 4,
    num_face_vertices,
    2,
    rprPlane ) )

RPRCHECK(  rpr.SceneAttachShape(rprScene, rprPlane) )



# CREATE MATERIAL
diffuseC = rpr.MaterialNode()
RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.DIFFUSE, diffuseC) )
RPRCHECK( rpr.MaterialNodeSetInputFByKey(diffuseC, rpr.MaterialInput.COLOR, 1.0, 0.2, 0.0, 0.0) )
RPRCHECK( rpr.ShapeSetMaterial(rprPlane,diffuseC) )






# CREATE A SHAPE

rprPlane2 = rpr.Shape()
RPRCHECK(  rpr.ContextCreateMesh(newCtx,
    plane_data, 0,   4,  8*4,
    plane_data, 3,   4,  8*4,
    plane_data, 6,   4,  8*4,
    indices , 4,
    indices , 4,
    indices , 4,
    num_face_vertices,
    2,
    rprPlane2 ) )

RPRCHECK( rpr.SceneAttachShape(rprScene, rprPlane2) )

transform2 = np.array([
        0.1001,  0.000,  0.000,    2.00,
        0.00,    0.0,   -0.100,      2.000,
        0.00,   0.10,   0.0,        0.800,
        0.0000,  0.0000,    0.00,    1.0
        ], dtype=np.float32)

RPRCHECK( rpr.ShapeSetTransform(rprPlane2, True, transform2) )


# CREATE MATERIAL
diffuse4 = rpr.MaterialNode()
image1 = rpr.Image();
materialImage1 = rpr.MaterialNode()
RPRCHECK( rpr.ContextCreateImageFromFile(newCtx,  "../../Resources/Textures/lead_rusted_Base_Color.jpg" , image1) )
RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.IMAGE_TEXTURE, materialImage1) )
RPRCHECK( rpr.MaterialNodeSetInputImageDataByKey(materialImage1, rpr.MaterialInput.DATA, image1) )
RPRCHECK( rpr.MaterialSystemCreateNode(matsys, rpr.MaterialNodeType.DIFFUSE, diffuse4) )
RPRCHECK( rpr.MaterialNodeSetInputNByKey(diffuse4, rpr.MaterialInput.COLOR, materialImage1) )
RPRCHECK( rpr.ShapeSetMaterial(rprPlane2, diffuse4) )


# CREATE LIGHT
rprLight = rpr.Light()
RPRCHECK( rpr.ContextCreatePointLight(newCtx , rprLight ) )
transform2 = np.array([
            1.0,  0.0,    0.0,   0.0,
            0,    1.0,    0,     8.0,
            0,    0,     1.0,    2.0,
            0.0,  0.0,    0,     1
            ], dtype=np.float32)
RPRCHECK( rpr.LightSetTransform(rprLight, True, transform2 ) )
RPRCHECK( rpr.PointLightSetRadiantPower3f(rprLight, 200, 200, 200) )
RPRCHECK( rpr.SceneAttachLight(  rprScene , rprLight  ) )



# CREATE FRAMEBUFFER
fbFormat = rpr.FramebufferFormat()
fbFormat.num_components = 4
fbFormat.type =  rpr.ComponentType.FLOAT32
fbDesc = rpr.FrameBufferDesc()
fbDesc.fb_width = 800
fbDesc.fb_height = 600
rprFB  = rpr.Framebuffer()
RPRCHECK( rpr.ContextCreateFrameBuffer(newCtx , fbFormat,  fbDesc , rprFB ) )
rprFB2  = rpr.Framebuffer()
RPRCHECK( rpr.ContextCreateFrameBuffer(newCtx , fbFormat, fbDesc , rprFB2 ) )
RPRCHECK( rpr.ContextSetAOV(newCtx , rpr.Aov.COLOR  , rprFB ) )



# SET CONTEXT PARAMETERS
rpr.ContextSetParameterByKey1f(newCtx, rpr.ContextInfo.DISPLAY_GAMMA , 2.2 )
rpr.ContextSetParameterByKey1u(newCtx, rpr.ContextInfo.ITERATIONS, 100)


# RENDER
print("Render...")
RPRCHECK( rpr.FrameBufferClear(rprFB) )
RPRCHECK( rpr.ContextRender(newCtx) )
RPRCHECK( rpr.ContextResolveFrameBuffer(newCtx  ,  rprFB ,   rprFB2  ,  False ) )
RPRCHECK( rpr.FrameBufferSaveToFile(rprFB2 , imageOutFile[0]  ) )
print("Done.")





# remove a light
RPRCHECK( rpr.SceneDetachLight(  rprScene , rprLight  ) )



# CREATE ENVIRONEMENT LIGHT
lightEnv = rpr.Light()
imgEnvLight = rpr.Image();
RPRCHECK( rpr.ContextCreateEnvironmentLight(newCtx, lightEnv) )
RPRCHECK( rpr.ContextCreateImageFromFile(newCtx, "../../Resources/Textures/turning_area_4k.hdr", imgEnvLight) )
RPRCHECK( rpr.EnvironmentLightSetImage(lightEnv, imgEnvLight) )
RPRCHECK( rpr.EnvironmentLightSetIntensityScale(lightEnv, 0.8) )
RPRCHECK( rpr.SceneAttachLight(rprScene, lightEnv) )


# RENDER
print("Render...")
RPRCHECK( rpr.FrameBufferClear(rprFB) )
RPRCHECK( rpr.ContextRender(newCtx) )
RPRCHECK( rpr.ContextResolveFrameBuffer(newCtx  ,  rprFB ,   rprFB2  ,  False ) )
RPRCHECK( rpr.FrameBufferSaveToFile(rprFB2 ,imageOutFile[1]  ) )
print("Done.")


# remove the cube shape
RPRCHECK( rpr.SceneDetachShape(rprScene, rprCube) )




# CREATE A NEW CUBE SHAPE WITH MOTION BLUR
shiftX = 0.3
shiftY = 0.3
cube_NumberOfVertices = 24
numberOfBlurKeyTime = 2

cube_data_2 = np.array([
        
        -1.0, 1.0, -1.0,    0.0, 1.0, 0.0,    0.0, 0.0 ,
        1.0, 1.0, -1.0,    0.0, 1.0, 0.0,    1.0, 0.0 ,
        1.0, 1.0, 1.0 ,    0.0, 1.0, 0.0,    1.0, 1.0 ,
        -1.0, 1.0, 1.0 ,   0.0, 1.0, 0.0,    0.0, 1.0,

        -1.0, -1.0, -1.0 , 0.0, -1.0, 0.0,   0.0, 0.0 ,
        1.0, -1.0, -1.0 ,  0.0, -1.0, 0.0,   1.0, 0.0 ,
        1.0, -1.0, 1.0 ,   0.0, -1.0, 0.0,   1.0, 1.0 ,
        -1.0, -1.0, 1.0 ,  0.0, -1.0, 0.0,   0.0, 1.0 ,

        -1.0, -1.0, 1.0 ,  -1.0, 0.0, 0.0,   0.0, 0.0 ,
        -1.0, -1.0, -1.0 , -1.0, 0.0, 0.0,   1.0, 0.0 ,
        -1.0, 1.0, -1.0 ,  -1.0, 0.0, 0.0,   1.0, 1.0 ,
        -1.0, 1.0, 1.0 ,   -1.0, 0.0, 0.0,   0.0, 1.0 ,

        1.0, -1.0, 1.0 ,   1.0, 0.0, 0.0,    0.0, 0.0 ,
        1.0, -1.0, -1.0 ,  1.0, 0.0, 0.0,    1.0, 0.0 ,
        1.0, 1.0, -1.0 ,   1.0, 0.0, 0.0,    1.0, 1.0 ,
        1.0, 1.0, 1.0 ,    1.0, 0.0, 0.0,    0.0, 1.0 ,

        -1.0, -1.0, -1.0 , 0.0, 0.0, -1.0 ,  0.0, 0.0 ,
        1.0, -1.0, -1.0 ,  0.0, 0.0, -1.0 ,  1.0, 0.0 ,
        1.0, 1.0, -1.0 ,   0.0, 0.0, -1.0,   1.0, 1.0 ,
        -1.0, 1.0, -1.0 ,  0.0, 0.0, -1.0,   0.0, 1.0 ,

        -1.0, -1.0, 1.0 ,  0.0, 0.0, 1.0,    0.0, 0.0 ,
        1.0, -1.0, 1.0 ,   0.0, 0.0,  1.0,   1.0, 0.0 ,
        1.0, 1.0, 1.0 ,    0.0, 0.0, 1.0,    1.0, 1.0 ,
        -1.0, 1.0, 1.0 ,   0.0, 0.0, 1.0,    0.0, 1.0 ,
        

        -1.0+shiftX, 1.0, -1.0,           0.0, 1.0, 0.0,      0.0, 0.0 ,
        1.0+shiftX, 1.0+shiftY, -1.0,    0.0, 1.0, 0.0,      1.0, 0.0 ,
        1.0+shiftX, 1.0+shiftY, 1.0 ,    0.0, 1.0, 0.0,      1.0, 1.0 ,
        -1.0+shiftX, 1.0, 1.0 ,          0.0, 1.0, 0.0,      0.0, 1.0,

        -1.0, -1.0, -1.0 ,               0.0, -1.0, 0.0,     0.0, 0.0 ,
        1.0, -1.0, -1.0 ,                0.0, -1.0, 0.0,     1.0, 0.0 ,
        1.0, -1.0, 1.0 ,                 0.0, -1.0, 0.0,     1.0, 1.0 ,
        -1.0, -1.0, 1.0 ,                0.0, -1.0, 0.0,     0.0, 1.0 ,

        -1.0, -1.0, 1.0 ,               -1.0, 0.0, 0.0,      0.0, 0.0 ,
        -1.0, -1.0, -1.0 ,              -1.0, 0.0, 0.0,      1.0, 0.0 ,
        -1.0+shiftX, 1.0, -1.0 ,        -1.0, 0.0, 0.0,      1.0, 1.0 ,
        -1.0+shiftX, 1.0, 1.0 ,         -1.0, 0.0, 0.0,      0.0, 1.0 ,

        1.0, -1.0, 1.0 ,                1.0, 0.0, 0.0,       0.0, 0.0 ,
        1.0, -1.0, -1.0 ,               1.0, 0.0, 0.0,       1.0, 0.0 ,
        1.0+shiftX, 1.0+shiftY, -1.0 ,  1.0, 0.0, 0.0,       1.0, 1.0 ,
        1.0+shiftX, 1.0+shiftY, 1.0 ,   1.0, 0.0, 0.0,       0.0, 1.0 ,

        -1.0, -1.0, -1.0 ,              0.0, 0.0, -1.0 ,     0.0, 0.0 ,
        1.0, -1.0, -1.0 ,               0.0, 0.0, -1.0 ,     1.0, 0.0 ,
        1.0+shiftX, 1.0+shiftY, -1.0 ,  0.0, 0.0, -1.0,      1.0, 1.0 ,
        -1.0+shiftX, 1.0, -1.0 ,        0.0, 0.0, -1.0,      0.0, 1.0 ,

        -1.0, -1.0, 1.0 ,               0.0, 0.0, 1.0,       0.0, 0.0 ,
        1.0, -1.0, 1.0 ,                0.0, 0.0,  1.0,      1.0, 0.0 ,
        1.0+shiftX, 1.0+shiftY, 1.0 ,   0.0, 0.0, 1.0,       1.0, 1.0 ,
        -1.0+shiftX, 1.0, 1.0 ,         0.0, 0.0, 1.0,       0.0, 1.0 
        
        ], dtype=np.float32)


cube_data_2_texcoords = np.array([cube_data_2])

num_texcoords = np.array([   cube_NumberOfVertices*numberOfBlurKeyTime ], dtype=np.int32)
texcoord_stride = np.array([   8*4 ], dtype=np.int32)
texcoord_indices_  = np.array([ indices ])
tidx_stride_ = np.array([  4 ], dtype=np.int32)

rprCube2 = rpr.Shape()
RPRCHECK( rpr.ContextCreateMeshEx2(
    newCtx,   
    cube_data_2, 0,    cube_NumberOfVertices*numberOfBlurKeyTime,    8*4,
    cube_data_2, 3,    cube_NumberOfVertices*numberOfBlurKeyTime,    8*4,
    np.array([]),0,0,
    1, 
    cube_data_2_texcoords, 6,       
    num_texcoords, texcoord_stride,
    indices, 4,
    indices, 4,
    texcoord_indices_,tidx_stride_,
    num_face_vertices, 12, 
    [ rpr.Mesh.MOTION_DIMENSION ,  numberOfBlurKeyTime , 0  ], 
    rprCube2
    ) )

RPRCHECK( rpr.SceneAttachShape(rprScene, rprCube2) )


transform3 = np.array([
            1.0,  0.0,    0.0,   -2.0,
            0,    1.0,    0,     1.0,
            0,    0,     1.0,    0,
            0.0,  0.0,    0,     1
        ], dtype=np.float32)


RPRCHECK( rpr.ShapeSetTransform(rprCube2, True, transform3) )

RPRCHECK( rpr.ShapeSetMaterial(rprCube2,layered) )

# add some exposure for the motion blur effect
RPRCHECK( rpr.CameraSetExposure(rprCamera, 1.0) )



# RENDER
print("Render...")
RPRCHECK( rpr.FrameBufferClear(rprFB) )
RPRCHECK( rpr.ContextRender(newCtx) )
RPRCHECK( rpr.ContextResolveFrameBuffer(newCtx  ,  rprFB ,   rprFB2  ,  False ) )
RPRCHECK( rpr.FrameBufferSaveToFile(rprFB2 ,imageOutFile[2]  ) )
print("Done.")



# detach previously created shapes and light before importing a scene file.
RPRCHECK( rpr.SceneDetachShape(rprScene, rprCube2) )
RPRCHECK( rpr.SceneDetachShape(rprScene, rprPlane) )
RPRCHECK( rpr.SceneDetachShape(rprScene, rprPlane2) )
RPRCHECK( rpr.SceneDetachLight(rprScene, lightEnv) )


# import RPRS file
RPRS_Ctx = rprs.Context()
RPRCHECK( rprs.CreateContext(RPRS_Ctx) )
RPRCHECK( rprs.Import("../../Resources/Meshes/matball.rprs", newCtx,  matsys ,rprScene, True, RPRS_Ctx ) )


# RENDER
print("Render...")
RPRCHECK( rpr.FrameBufferClear(rprFB) )
rpr.ContextSetParameterByKey1u(newCtx, rpr.ContextInfo.ITERATIONS, 100)
RPRCHECK( rpr.ContextRender(newCtx) )
RPRCHECK( rpr.ContextResolveFrameBuffer(newCtx  ,  rprFB ,   rprFB2  ,  False ) )
RPRCHECK( rpr.FrameBufferSaveToFile(rprFB2 ,imageOutFile[3]  ) )
print("Done.")




# DELETE RPR OBJECTS
RPRCHECK( rprs.DeleteListImportedObjectsEx( RPRS_Ctx ) );
RPRCHECK( rprs.DeleteContext(RPRS_Ctx) ); RPRS_Ctx=0;
RPRCHECK( rpr.ObjectDelete( lightEnv ) ); lightEnv=0;
RPRCHECK( rpr.ObjectDelete( imgEnvLight ) ); imgEnvLight=0;
RPRCHECK( rpr.ObjectDelete( matsys ) ); matsys=0;
RPRCHECK( rpr.ObjectDelete( diffuseA ) ); diffuseA=0;
RPRCHECK( rpr.ObjectDelete( diffuseB ) ); diffuseB=0;
RPRCHECK( rpr.ObjectDelete( uv_node ) ); uv_node=0;
RPRCHECK( rpr.ObjectDelete( uv_scaled_node ) ); uv_scaled_node=0;
RPRCHECK( rpr.ObjectDelete( checker ) ); checker=0;
RPRCHECK( rpr.ObjectDelete( layered ) ); layered=0;
RPRCHECK( rpr.ObjectDelete( diffuse4 ) ); diffuse4=0;
RPRCHECK( rpr.ObjectDelete( image1 ) ); image1=0;
RPRCHECK( rpr.ObjectDelete( materialImage1 ) ); materialImage1=0;
RPRCHECK( rpr.ObjectDelete( diffuseC ) ); diffuseC=0;
RPRCHECK( rpr.ObjectDelete( rprCube2 ) ); rprCube2=0;
RPRCHECK( rpr.ObjectDelete( rprPlane2 ) ); rprPlane2=0;
RPRCHECK( rpr.ObjectDelete( rprPlane ) ); rprPlane=0;
RPRCHECK( rpr.ObjectDelete( rprScene ) ); rprScene=0;
RPRCHECK( rpr.ObjectDelete( rprCamera ) ); rprCamera=0;
RPRCHECK( rpr.ObjectDelete( rprCube ) ); rprCube=0;
RPRCHECK( rpr.ObjectDelete( rprLight ) ); rprLight=0;
RPRCHECK( rpr.ObjectDelete( rprFB ) ); rprFB=0;
RPRCHECK( rpr.ObjectDelete( rprFB2 ) ); rprFB2=0;

# context must always be the last deleted object
RPRCHECK( rpr.ObjectDelete( newCtx ) )


print("-- script ends --");


