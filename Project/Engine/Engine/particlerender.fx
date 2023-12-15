#ifndef _PARTICLERENDER
#define _PARTICLERENDER

#include "value.fx"
#include "struct.fx"

// ====================
// ParticleRenderShader
// Mesh     : PointMesh
// Domain   : Opaque
// Blend    : AlphaBlend
// DepthStencil : Less
// Rasterizer : CULL_NONE


// Parameter
#define PARTICLE_INDEX  g_int_1
#define POS_INHERIT     g_int_0
#define AnimTime        g_float_0
// ====================

StructuredBuffer<tParticle> ParticleBuffer : register(t16);

struct VTX_IN
{       
    uint InstID : SV_InstanceID;
};

struct VTX_OUT
{ 
    uint InstID : SV_InstanceID;
};


VTX_OUT VS_ParticleRender(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;    
   
    output.InstID = _in.InstID;       
    
    return output;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : SV_InstanceID;
};

// 1 ���� ������ 6���� ����
[maxvertexcount(6)]
void GS_ParticleRender(point VTX_OUT _in[1], inout TriangleStream<GS_OUT> _output)
{
    uint ID = _in[0].InstID;
    
    // �ش� �ν��Ͻ��� ��Ȱ��ȭ��, ��� ������ ���� ==> �ƹ��ϵ� ����
    if (0 == ParticleBuffer[ID].Alive)
        return;
    
     // ��ƼŬ ������ ������ġ ���ϱ�
    float3 vParticleWorldPos = (float3) 0.f;
    
    
    if (POS_INHERIT)
    {
        vParticleWorldPos = g_matWorld._41_42_43 + ParticleBuffer[ID].vPos;
    }
    else
    {
        vParticleWorldPos = ParticleBuffer[ID].vPos;
    }
    
    // World ��ǥ�� View, Proj ���� ��ȯ
    float4 vViewPos = mul(float4(vParticleWorldPos, 1.f), g_matView);
    float3 vScale = ParticleBuffer[ID].vScale;
    
    // ViewSpace ���� Point �� Rect �� ����
    // 0 --- 1
    // |  \  |
    // 3 --- 2
    float3 arrRect[4] =
    {
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y + vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x + vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z),
        float3(vViewPos.x - vScale.x / 2.f, vViewPos.y - vScale.y / 2.f, vViewPos.z)
    };
    
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition = mul(float4(arrRect[i], 1.f), g_matProj);
        output[i].InstID = _in[0].InstID;
    }
    
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
        
            
    _output.Append(output[0]);
    _output.Append(output[2]);
    _output.Append(output[3]);
    _output.RestartStrip();
    
    _output.Append(output[2]);
    _output.Append(output[0]);
    _output.Append(output[1]);
    _output.RestartStrip();
}


float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;    
    
    if (AnimTime>=0.f && AnimTime< 0.1f)
    {
        if (g_btex_0)
        {
            vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        }
    }
    else if (AnimTime>=0.1f && AnimTime <0.2f)
    {
        if (g_btex_1)
        {
            vOutColor = g_tex_1.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        }
    }
    else if (AnimTime >= 0.2f && AnimTime < 0.3f)
    {
        if (g_btex_2)
        {
            vOutColor = g_tex_2.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        }
    }
    else if (AnimTime >= 0.3f && AnimTime < 0.4f)
    {
        if (g_btex_3)
        {
            vOutColor = g_tex_3.Sample(g_sam_0, _in.vUV) * ParticleBuffer[_in.InstID].vColor;
        }
    }

    else
    {
        vOutColor = ParticleBuffer[_in.InstID].vColor;
    }
    
    return vOutColor;
}

#endif