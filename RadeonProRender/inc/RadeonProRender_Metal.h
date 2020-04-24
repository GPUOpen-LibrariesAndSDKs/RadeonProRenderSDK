/*****************************************************************************\
*
*  Module Name    RadeonProRender_Metal.h
*  Project        AMD Radeon ProRender Metal Interop API
*
*  Description    Radeon ProRender Metal Interop header
*
*  Copyright 2011 - 2019 Advanced Micro Devices, Inc.
*
*  All rights reserved.  This notice is intended as a precaution against
*  inadvertent publication and does not imply publication or any waiver
*  of confidentiality.  The year included in the foregoing notice is the
*  year of creation of the work.
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
