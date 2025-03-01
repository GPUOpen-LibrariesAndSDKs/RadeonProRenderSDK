
#
#
# DEMO of GLTF Rendering with Python Binding.
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
import rprgltf
import ctypes
import numpy as np


def RPRCHECK(retCode):
    if ( retCode != rpr.Status.SUCCESS ):
        print(f"RPR ERROR: {retCode}")
        exit()


print("-- script starts --");


imageOutFile = [ "OUT_PYTHON_GLTF_1.png"  ]



plugin_id = rpr.RegisterPlugin((str("../../RadeonProRender/" + binFolder+"/"+pluginFile)))
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
RPRCHECK( rpr.CreateContext(rpr.VERSION_MAJOR_MINOR_REVISION  ,  pluginLists__myarray   , 1  , flagsCtx ,   [   rpr.Int64ToPvoid(rpr.ContextInfo.PRECOMPILED_BINARY_PATH) ,   ctxInfo1.GetMem(0)  ,  rpr.Int64ToPvoid(0)   ] , "" ,  newCtx ) )
ctxInfo1.Free(); ctxInfo1=0;
RPRCHECK( rpr.ContextSetActivePlugin(newCtx, plugin_id) )
print("RPR context created.");


matsys = rpr.MaterialSystem()
RPRCHECK( rpr.ContextCreateMaterialSystem(newCtx, 0, matsys) )


# CREATE SCENE
rprScene = rpr.Scene()
RPRCHECK( rpr.ContextCreateScene(newCtx, rprScene) )
RPRCHECK( rpr.ContextSetScene( newCtx , rprScene ) )


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



# import GLTF file
RPRS_Ctx = rprs.Context()
RPRCHECK( rprs.CreateContext(RPRS_Ctx) )
print("GLTF importing...");
RPRCHECK( rprgltf.Import("../../Resources/Meshes/gltf_teapot/cube_floor.gltf", newCtx,  matsys ,rprScene, 0, RPRS_Ctx ) )
print("GLTF import done.");


# RENDER
print("Render...")
RPRCHECK( rpr.FrameBufferClear(rprFB) )
rpr.ContextSetParameterByKey1u(newCtx, rpr.ContextInfo.ITERATIONS, 100)
RPRCHECK( rpr.ContextRender(newCtx) )
RPRCHECK( rpr.ContextResolveFrameBuffer(newCtx  ,  rprFB ,   rprFB2  ,  False ) )
RPRCHECK( rpr.FrameBufferSaveToFile(rprFB2 ,imageOutFile[0]  ) )
print("Done.")



# DELETE RPR OBJECTS
RPRCHECK( rprgltf.DeleteListImportedObjects() );
RPRCHECK( rprs.DeleteContext(RPRS_Ctx) ); RPRS_Ctx=0;
RPRCHECK( rpr.ObjectDelete( matsys ) ); matsys=0;
RPRCHECK( rpr.ObjectDelete( rprScene ) ); rprScene=0;
RPRCHECK( rpr.ObjectDelete( rprFB ) ); rprFB=0;
RPRCHECK( rpr.ObjectDelete( rprFB2 ) ); rprFB2=0;

# context must always be the last deleted object
RPRCHECK( rpr.ObjectDelete( newCtx ) )


print("-- script ends --");


