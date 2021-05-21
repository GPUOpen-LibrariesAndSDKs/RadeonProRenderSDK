/*****************************************************************************\
*
*  Module Name    RadeonProRender_CL.h
*  Project        AMD Radeon ProRender OpenCL Interop API
*
*  Description    Radeon ProRender OpenCL Interop header
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_CL_H
#define __RADEONPRORENDER_CL_H

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif


#include "RadeonProRender.h"

/* rpr_context_properties  */
#define RPR_CL_CONTEXT 0x3001 
#define RPR_CL_DEVICE 0x3002 
#define RPR_CL_COMMAND_QUEUE 0x3003 

/* rpr_framebuffer_properties */
#define RPR_CL_MEM_OBJECT 0x4001 

typedef void * rpr_cl_context;
typedef void * rpr_cl_device;
typedef void * rpr_cl_command_queue;
typedef void * rpr_cl_mem;
#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_CL_H  */
