/**********************************************************************
Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/

#ifndef __RADEONPRORENDER_D3D12_H
#define __RADEONPRORENDER_D3D12_H

#ifdef __cplusplus
extern "C" {
#endif

/* rpr_creation_flags */
#define RPR_CREATION_FLAGS_ENABLE_D3D12_INTEROP (1<<12)

/* rpr_framebuffer_properties */
#define RPR_D3D12_IMAGE_OBJECT 0x6001

/* rpr_context_properties names */
#define RPR_CONTEXT_CREATEPROP_D3D12_INTEROP_INFO 0x6101

struct D3D12SemaphoreInfo
{
    void *fence;
    void *counter;
};

struct D3D12InteropInfo
{
    void* devices;
    D3D12SemaphoreInfo* framebuffers_release_semaphores;
    uint32_t device_count;
    uint32_t main_device_index;
    uint32_t frames_in_flight;
};

#ifdef __cplusplus
}
#endif

#endif  /*__RADEONPRORENDER_D3D12_H  */
