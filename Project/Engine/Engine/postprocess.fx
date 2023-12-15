#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"
#include "func.fx"


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// =====================================
// PostProcess Shader
// Domain       : Post Process
// Mesh         : RectMesh
// Blend        : Default
// DepthStencil : NoTest NoWrite

#define PostProcessTarget   g_tex_0
#define IsBind              g_btex_0
#define DistortionTime      g_float_0
#define DistortionDist      g_float_1

// =====================================


VTX_OUT VS_PostProcess(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;    
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);    
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_PostProcess(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       

    // _in.vPosition; ÇÈ¼¿ ÁÂÇ¥
    float2 vScreenUV = _in.vPosition.xy / vResolution;

    
    
    
    if (IsBind)
    {       
        ////1 
        //vScreenUV.y += sin(vScreenUV.x * 3.141592f * 20.f + (fAccTime * 4.f)) * 0.01;
        ////vScreenUV -= 0.5;
                
        //_in.vUV.x += fAccTime * 0.1f;
        //vScreenUV += (g_noise_cloud.Sample(g_sam_0, _in.vUV).rg - 0.5f) * 0.03f;
        //vScreenUV = saturate(vScreenUV);
                
        //vOutColor = PostProcessTarget.Sample(g_sam_1, vScreenUV);
        
        float t = clamp(fAccTime / 6., 0., 1.);

        float2 coords = vScreenUV;
        float2 dir = coords - float2(0.5,0.5);
	
        float dist = distance(coords, float2(0.5,0.5));
        float2 offset = dir * (sin(dist * 10. - DistortionTime * 20.) + 0.5) / 20.f;
        if (dist < DistortionDist)
            discard;
        
        
        //dir * (sin(dist * 80. - fAccTime * 15.) + .5) / 100.f;
        
        float2 texCoord = coords + offset;
        float4 diffuse = PostProcessTarget.Sample(g_sam_1, texCoord);
        float4 mixin = PostProcessTarget.Sample(g_sam_1, texCoord);

        vOutColor = mixin * t + diffuse * (1. - t);
        
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }    
    return vOutColor;
}




#endif

        
//float t = clamp(fAccTime / 10., 0., 1.);

//float2 coords = vScreenUV;
//float2 dir = coords - float2(0.5, 0.5);
	
//float dist = distance(coords, float2(0.5, 0.5));
//float2 offset = dir * (sin(dist * 10. - DistortionTime * 30.) + 1.5) / 50.f;
        
        
//        //dir * (sin(dist * 80. - fAccTime * 15.) + .5) / 100.f;
        
//float2 texCoord = coords + offset;
//float4 diffuse = PostProcessTarget.Sample(g_sam_1, texCoord);
//float4 mixin = PostProcessTarget.Sample(g_sam_1, texCoord);

//        vOutColor = mixin * t + diffuse * (1. - t);