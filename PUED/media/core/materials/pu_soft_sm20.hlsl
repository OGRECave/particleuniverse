//-----------------------------------------------------------------------------------------------------------------------
// This script is part of the Particle Universe Media Pack product.
//
// Copyright (c) 2008 Henry van Merode
//
// Usage of this program is licensed under the terms of the Particle Universe Commercial License.
// You can find a copy of the Commercial License in the Particle Universe package.
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
void mainVP(
    float4 iPosition          : POSITION,
    float4 iColor             : COLOR,
    float2 iUV                : TEXCOORD,

    out float4 oPosition      : POSITION,
    out float4 oColor         : COLOR,
    out float2 oVertexDepth   : TEXCOORD0,
    out float2 oUVDepth       : TEXCOORD1,
    out float2 oUV            : TEXCOORD2,

    uniform float4x4 worldViewProj,
    uniform float4 texelOffsets,
    uniform float4 depthRange,
    uniform float depthScale)
{
    // Calculate uv of the vertex
    oPosition = mul(worldViewProj, iPosition);
    oUVDepth = oPosition.xy / oPosition.w;
    oUVDepth.x = 0.5 * (oUVDepth.x + 1.0);
    oUVDepth.y = 0.5 * (-oUVDepth.y + 1.0);

    // Calculate depth of the vertex
    oPosition.xy += texelOffsets.zw * oPosition.w;
    oVertexDepth.x = depthScale * (oPosition.z - depthRange.x) * depthRange.w;

    // Set output values
    oColor = iColor;
    oUV = iUV;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// Fragment shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
float4 mainFP(
    float4 color               : COLOR,
    float2 vertexDepth         : TEXCOORD0,
    float2 uvDepth             : TEXCOORD1,
    float2 uv                  : TEXCOORD2,

    uniform sampler2D depthMap : register(s0),
    uniform sampler2D tex      : register(s1),

    uniform float contrastPower,
    uniform float scale,
    uniform float delta) : COLOR
{
    // Determine depth of the map of the vertex coordinate
    float depthFromMap = tex2D(depthMap, uvDepth).r;

    // Determine alpha factor
    float diff = vertexDepth.x - depthFromMap;

    // Do not render what's behind the objects
    if (diff > 0.0 && diff < delta)
    {
        discard;
    }
    float contrast = 1.0 - exp2(-2*pow(2.0*saturate(-diff * scale), contrastPower));
    if (diff < 0.0)
    {
        color *= float4(1.0, 1.0, 1.0, contrast);
    }

    // Recalculate the colour
    float4 output = tex2D(tex, uv);
    output *= color;
    return output;
}
