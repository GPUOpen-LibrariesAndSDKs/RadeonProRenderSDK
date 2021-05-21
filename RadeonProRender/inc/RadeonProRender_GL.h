/*****************************************************************************\
*
*  Module Name    RadeonProRender_GL.h
*  Project        AMD Radeon ProRender OpenGL Interop API
*
*  Description    Radeon ProRender OpenGL Interop header
*
*  Copyright(C) 2011-2021 Advanced Micro Devices, Inc. All rights reserved.
*
\*****************************************************************************/
#ifndef __RADEONPRORENDER_GL_H
#define __RADEONPRORENDER_GL_H


#include "RadeonProRender.h"

#define RPR_API_ENTRY

#ifdef __cplusplus
extern "C" {
#endif


/* Error codes  */
#define RPR_INVALID_GL_SHAREGROUP_REFERENCE_KHR -1000 

/* rpr_context_properties  */
#define RPR_GL_CONTEXT_KHR 0x2001 
#define RPR_EGL_DISPLAY_KHR 0x2002 
#define RPR_GLX_DISPLAY_KHR 0x2003 
#define RPR_WGL_HDC_KHR 0x2004 
#define RPR_CGL_SHAREGROUP_KHR 0x2005 

typedef unsigned int rpr_GLuint;
typedef int rpr_GLint;
typedef unsigned int rpr_GLenum;
typedef rpr_uint rpr_gl_object_type;
typedef rpr_uint rpr_gl_texture_info;
typedef rpr_uint rpr_gl_platform_info;

extern RPR_API_ENTRY rpr_status rprContextCreateFramebufferFromGLTexture2D(rpr_context context, rpr_GLenum target, rpr_GLint miplevel, rpr_GLuint texture, rpr_framebuffer * out_fb);
#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_GL_H  */
