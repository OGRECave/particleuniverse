//-----------------------------------------------------------------------------------------------------------------------------------------------
// Vertex shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
void mainVP( 
   float4 position     : POSITION, 
   out float4 outPos   : POSITION, 
   out float2 outDepth : TEXCOORD0, 

   uniform float4x4 worldViewProj,
   uniform float4 texelOffsets, 
   uniform float4 depthRange,
   uniform float depthScale)
{ 
    outPos = mul(worldViewProj, position); 
    outPos.xy += texelOffsets.zw * outPos.w;
    outDepth.x = depthScale * (outPos.z - depthRange.x) * depthRange.w;
    outDepth.y = outPos.w;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// Fragment shader
//-----------------------------------------------------------------------------------------------------------------------------------------------
void mainFP( 
   float2 depth      : TEXCOORD0, 
   out float4 result : COLOR) 
    
{ 
    float finalDepth = depth.x; 
    result = float4(finalDepth, finalDepth, finalDepth, 1); 
} 

