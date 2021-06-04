/*****************************************************************************\
*
*  Module Name    RprTools.h
*  Project        AMD Radeon ProRender
*
*  Description    Radeon ProRender Interface header
*
*  Copyright(C) 2017-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/

//
// see readme.txt 
//


#ifndef __RADEONPRORENDERTOOLS_H
#define __RADEONPRORENDERTOOLS_H

enum RPR_TOOLS_OS
{
	RPRTOS_WINDOWS,
	RPRTOS_LINUX,
	RPRTOS_MACOS,
};

//if in a project we don't want to include RadeonProRender.h, and we only need to use  IsDeviceNameWhitelisted()
//then we define RADEONPRORENDERTOOLS_DONTUSERPR 
#ifndef RADEONPRORENDERTOOLS_DONTUSERPR


#include "RadeonProRender.h"


enum RPR_TOOLS_DEVICE
{
	RPRTD_GPU0,
	RPRTD_GPU1,
	RPRTD_GPU2,
	RPRTD_GPU3,
	RPRTD_GPU4,
	RPRTD_GPU5,
	RPRTD_GPU6,
	RPRTD_GPU7,

	RPRTD_GPU8,
	RPRTD_GPU9,
	RPRTD_GPU10,
	RPRTD_GPU11,
	RPRTD_GPU12,
	RPRTD_GPU13,
	RPRTD_GPU14,
	RPRTD_GPU15,

	RPRTD_CPU,
};

enum RPR_TOOLS_COMPATIBILITY
{
	// device is compatible
	RPRTC_COMPATIBLE = 0,     

	// device is incompatible for unknown reason (mostly because error occurs while testing compatibility)
	RPRTC_INCOMPATIBLE_UNKNOWN,      

	// device is incompatible because not tested by Radeon ProRender team.
	RPRTC_INCOMPATIBLE_UNCERTIFIED,      

	// device is incompatible because a Radeon ProRender creation is reporting a UNSUPPORTED error. It could be because device is not OpenCL 1.2 compatible. 
	RPRTC_INCOMPATIBLE_CONTEXT_UNSUPPORTED, 

	// device is incompatible because a Radeon ProRender creation is reporting an error.
	RPRTC_INCOMPATIBLE_CONTEXT_ERROR,
};

// 'rendererDLL' is the path to plugin DLL : example : "Tahoe64.dll"
// 'tahoePluginID' is the ID of a registered plugin from rprRegisterPlugin("Tahoe64.dll")
RPR_TOOLS_COMPATIBILITY rprIsDeviceCompatible(const rpr_char* rendererDLL, RPR_TOOLS_DEVICE device, rpr_char const * cache_path, bool doWhiteListTest, RPR_TOOLS_OS os, rpr_creation_flags additionalflags = (rpr_creation_flags)0);
RPR_TOOLS_COMPATIBILITY rprIsDeviceCompatible(rpr_int tahoePluginID      , RPR_TOOLS_DEVICE device, rpr_char const * cache_path, bool doWhiteListTest, RPR_TOOLS_OS os, rpr_creation_flags additionalflags = (rpr_creation_flags)0);


// get the list of Compatible devices in 'devicesUsed'
//
// 'devicesUsed' input value.
// 'devicesCompatibleOut' output value.
// 'rendererDLL' is the path to plugin DLL : example : "Tahoe64.dll"
// 'tahoePluginID' is the ID of a registered plugin from rprRegisterPlugin("Tahoe64.dll")
//
// example : devicesUsed=RPR_CREATION_FLAGS_ENABLE_GPU0|RPR_CREATION_FLAGS_ENABLE_GPU1|RPR_CREATION_FLAGS_ENABLE_GPU2
//           if GPU1 is incompatible
//           devicesCompatibleOut will be return with  RPR_CREATION_FLAGS_ENABLE_GPU0|RPR_CREATION_FLAGS_ENABLE_GPU2
//
void rprAreDevicesCompatible(const rpr_char* rendererDLL, rpr_char const * cache_path, bool doWhiteListTest, rpr_creation_flags devicesUsed,  rpr_creation_flags* devicesCompatibleOut, RPR_TOOLS_OS os);
void rprAreDevicesCompatible(rpr_int tahoePluginID      , rpr_char const * cache_path, bool doWhiteListTest, rpr_creation_flags devicesUsed,  rpr_creation_flags* devicesCompatibleOut, RPR_TOOLS_OS os);


// Combine 2 framebuffers, into a single one.
// 'destination' must be allocated/freed by api user. the size in byte must be 4*sizeof(float)*width*height
// for sanity check, add the 'destination_sizeByte'
//
// colorAOV and alphaAOV must be normalized ( result of a rprContextResolveFrameBuffer with a normalization )
// classic usage is to use a resolved RPR_AOV_COLOR for colorAOV, and resolved RPR_AOV_OPACITY for alphaAOV 
//
// "CombineRGBAlpha" Composite Algorithm :
// takes the RGB of 'colorAOV' --> sets it to RGB of 'destination'
// takes the R   of 'alphaAOV' --> sets it to A   of 'destination'
//
// Note that this function is a simple single-thread demo. Performance can be improved with parallel computing.
//
rpr_int rprtools_Compositing_CombineRGBAlpha(rpr_framebuffer colorAOV, rpr_framebuffer alphaAOV, float* destination, size_t destination_sizeByte);

#endif

//return TRUE if device is known as unsupported.
//
// example of 'deviceName' :   "AMD FirePro W8000"
//
bool IsDeviceNameWhitelisted(const char* deviceName, RPR_TOOLS_OS os);


#endif
