/**********************************************************************
Copyright (C)2017 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

*   Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include "float3.h"

namespace RadeonProRender
{
    class float4
    {
    public:
        float4(float xx = 0.f, float yy = 0.f, float zz = 0.f, float ww = 0.f) : x(xx), y(yy), z(zz), w(ww) {}

        float& operator [](int i)       { return *(&x + i); }
        float  operator [](int i) const { return *(&x + i); }
        float4 operator-() const        { return float4(-x, -y, -z, -w); }

        float3 xyz() const              { return float3(x,y,z); }

        float4& operator += (float4 const& o) { x+=o.x; y+=o.y; z+=o.z; w+=o.w; return *this;}
        float4& operator -= (float4 const& o) { x-=o.x; y-=o.y; z-=o.z; w-=o.w; return *this;}
        float4& operator *= (float4 const& o) { x*=o.x; y*=o.y; z*=o.z; w*=o.w; return *this;}
        float4& operator /= (float4 const& o) { x/=o.x; y/=o.y; z/=o.z; w/=o.w; return *this;}
        float4& operator *= (float c) { x*=c; y*=c; z*=c; w*=c; return *this;}
        float4& operator /= (float c) { float cinv = 1.f/c; x*=cinv; y*=cinv; z*=cinv; w*=cinv; return *this;}
        friend std::ostream& operator<<(std::ostream& os, const float4& o);

        float x, y, z, w;
    };


    inline float4 operator+(float4 const& v1, float c) { return float4(v1.x+c, v1.y+c, v1.z+c, v1.w+c); }
    inline float4 operator-(float4 const& v1, float c) { return float4(v1.x-c, v1.y-c, v1.z-c, v1.w-c); }
    inline float4 operator*(float4 const& v1, float c) { return float4(v1.x*c, v1.y*c, v1.z*c, v1.w*c); }
    inline float4 operator/(float4 const& v1, float c) { return float4(v1.x/c, v1.y/c, v1.z/c, v1.w/c); }


    inline std::ostream& operator<<(std::ostream& os, const float4& o)
    {
        os << "[" << o.x << ", " << o.y << ", " << o.z << ", " << o.w <<  "]";
        return os;
    }

    inline float4 operator+(float4 const& v1, float4 const& v2)
    {
        float4 res = v1;
        return res+=v2;
    }

    inline float4 operator-(float4 const& v1, float4 const& v2)
    {
        float4 res = v1;
        return res-=v2;
    }

    inline float4 operator*(float4 const& v1, float4 const& v2)
    {
        float4 res = v1;
        return res*=v2;
    }

    inline float4 operator/(float4 const& v1, float4 const& v2)
    {
        float4 res = v1;
        return res/=v2;
    }

    inline float4 operator*(float c, float4 const& v1)
    {
        return operator*(v1, c);
    }

}