// -----------------------------------------------------------------------------------------------
// This source file is part of the Particle Universe product.
//
// Copyright (c) 2011 Henry van Merode
// 
// Usage of this program is licensed under the terms of the Particle Universe Commercial License.
// You can find a copy of the Commercial License in the Particle Universe package.
// -----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------
// Fragment shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
float4 mainFP (float2 uv                  : TEXCOORD0,
               uniform float3 CameraPos,
               uniform sampler2D tex      : register(s0)) : COLOR
{

    float fineGridScale = 100.0f;
    float mediumGridScale = 10.0f;
    float courseGridScale = 1.0f;

    float4 grid1 = tex2D(tex, fineGridScale * uv);
    float4 grid2 = tex2D(tex, mediumGridScale * uv);
    float4 grid3 = tex2D(tex, courseGridScale * uv);
    float len = length(CameraPos);
    grid3.a *= (0.05 * len - 0.2);
    grid3.a = grid3.a < 0.0f ? 0.0f : grid3.a;
    grid3.a = grid3.a > 0.6f ? 0.6f : grid3.a;
    grid1 = grid1 + grid2 + grid3;
    return grid1;
}
