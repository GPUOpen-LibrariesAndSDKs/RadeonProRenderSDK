/*****************************************************************************\
*
*  Module Name    RadeonProRender_Metal.h
*  Project        AMD Radeon ProRender Metal Interop API
*
*  Description    Radeon ProRender Metal Interop header
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_METAL_H
#define __RADEONPRORENDER_METAL_H

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif


#include "RadeonProRender.h"

/* rpr_context_properties  */
#define RPR_METAL_DEVICE 0x6602 
#define RPR_METAL_COMMAND_QUEUE 0x6603 

typedef void * rpr_metal_device;
typedef void * rpr_metal_command_queue;
#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_METAL_H  */
