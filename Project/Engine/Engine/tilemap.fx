#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tTileData> TileDataBuffer : register(t16);

// ==============
// TileMap Shader
// Domain : Mask
// Blend : Default
// Depth : LESS
#define TileCountX g_int_0
#define TileCountY g_int_1
#define SliceSizeUV g_vec2_0
// ==============

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

VTX_OUT VS_TileMap(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VTX_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;    
    
    // 0~1 �� UV �� NxM Ÿ�� ũ��� Ȯ���Ѵ�
    float2 vUV = _in.vUV * float2(TileCountX, TileCountY);// * SliceSizeUV + LeftTopUV;
    
    // �� Ÿ�� ��ġ�� ���������� ������ ���ؼ� �������� ���Ѵ�.
    // �ش� �ȼ��� ��ġ�ϴ� Ÿ���� ��, �� ������ �˾� �� �� �ִ�.
    int2 iTileRowCol = floor(vUV);
    
    // ���� Ÿ�� ��� ������ ������ �ε����� ��ȯ(Ÿ�� ������ �迭�� �����ϱ� ����)
    int TileDataIdx = iTileRowCol.y * TileCountX + iTileRowCol.x;
    
    // Ÿ�� �����Ϳ� �����ϸ�, �� Ÿ�� ���� �»�� UV ���� ����ִ�.
    // ���� �̹��� ������ �ȵ� Ÿ���̸� ����
    if (-1 == TileDataBuffer[TileDataIdx].iImgIdx)
        discard;
    
    float2 vLeftTopUV = TileDataBuffer[TileDataIdx].vLTUV;
    
    // 0~1 �� UV �� NxM Ÿ�� ũ��� Ȯ�� UV �� �Ҽ��� �κ��� ���ø� �뵵�� ����Ѵ�.
    float2 vImgUV = frac(vUV);
    
    // ���� Sample ��ġ UV �� ����Ѵ�.
    float2 vSampleUI = vLeftTopUV + vImgUV * SliceSizeUV;
    
    vOutColor = g_tex_0.Sample(g_sam_1, vSampleUI);
    
    if (vOutColor.a <= 0.f)
    {
        discard;
    }
    
    return vOutColor;
}








#endif