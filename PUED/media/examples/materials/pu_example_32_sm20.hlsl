//-----------------------------------------------------------------------------------------------------------------------
// This script is part of the Particle Universe Media Pack product.
//
// Copyright (c) 2010 Henry van Merode
//
// Usage of this program is licensed under the terms of the Particle Universe Commercial License.
// You can find a copy of the Commercial License in the Particle Universe package.
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
void mainVP(float4 iPosition : POSITION,
            float2 iUv: TEXCOORD,

            out float4 oPosition : POSITION,
            out float2 oUv : TEXCOORD,
            out float2 oFraction : TEXCOORD1,

            uniform float4x4 worldViewProj,
            uniform float4 lightPosition,
            uniform float radius)
{
    oPosition = mul(worldViewProj, iPosition);
    oUv = iUv;
    float d = distance(iPosition, lightPosition);
    oFraction.x = d / radius;
    if (oFraction.x > 1.0)
    {
        oFraction.x = 1.0;
    }
    oFraction.y = 1 - oFraction.x;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------
// Fragment shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
float4 mainFP (float2 uv : TEXCOORD,
               float2 fraction : TEXCOORD1,

               uniform float4 lightColour,
               uniform sampler2D tex : register(s0)) : COLOR
{
    float4 colour = tex2D (tex, uv.xy); 
    float alpha = colour.a;
    colour = fraction.x * colour + fraction.y * lightColour;
    colour.a = alpha;
    return colour;
}
