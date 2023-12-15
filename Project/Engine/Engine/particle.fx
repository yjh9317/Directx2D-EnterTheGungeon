#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

// =======================
// Particle Update Shader
#define MaxThreadCount      g_int_0
#define MaxParticleCount    g_int_1

#define CreateRange         g_float_0
#define WorldPos            float3(g_float_1, g_float_2, g_float_3)

#define MinLifeTime         g_vec2_0.x
#define MaxLifeTime         g_vec2_0.y

#define MinSpeed            g_vec2_1.x
#define MaxSpeed            g_vec2_1.y

#define StartColor          g_vec4_0
#define EndColor            g_vec4_1
#define StartScale          g_vec4_2
#define EndScale            g_vec4_3


// =======================

RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<tParticleData> DataBuffer : register(u1);

[numthreads(32, 1, 1)]
void CS_Particle(int3 _id : SV_DispatchThreadID)
{   
    if (MaxParticleCount < _id.x)
        return;
          
    // ��ƼŬ�� ��Ȱ��ȭ �Ǿ��ִٸ�...
    if ( 0 == ParticleBuffer[_id.x].Alive)
    {
        int iOriginValue = DataBuffer[0].iAliveCount;
        
        // Alive ��ȸ�� ������ �ݺ�
        while (0 < iOriginValue)
        {            
            int iInputValue = iOriginValue - 1;
            int iOutValue = 0;
    
            InterlockedCompareExchange(DataBuffer[0].iAliveCount, iOriginValue, iInputValue, iOutValue);
    
            // Ȱ��ȭ ������ ���, Alive �� True �� ����
            if (iOriginValue == iOutValue)
            {
                ParticleBuffer[_id.x].Alive = 1;
                break;
            }
            
            iOriginValue = DataBuffer[0].iAliveCount;
        }
        
        
        // Ȱ��ȭ �� ��ƼŬ�� �ʱⰪ�� �����ϰ� �ο�
        if(ParticleBuffer[_id.x].Alive)
        {
            // ������ ��ġ �ο�
            float2 vSampleUV = float2(((float) _id.x / (float) MaxThreadCount) + fAccTime, fAccTime);
            vSampleUV.y += sin(vSampleUV.x * 3.141592f * 2.f) * 0.5f;
            float3 vOut = GaussianSample(g_noise_cloud, vSampleUV).rgb;
                        
            float3 vRandom =
            {
                GaussianSample(g_noise_01, vSampleUV + float2(0.1f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.2f, 0.f)).r
                , GaussianSample(g_noise_01, vSampleUV + float2(0.3f, 0.f)).r
            };                        
            
            ParticleBuffer[_id.x].vPos = WorldPos + (vOut - 0.5f) * CreateRange;
            ParticleBuffer[_id.x].vScale = float3(10.f, 10.f, 1.f);
            ParticleBuffer[_id.x].vDir = normalize(vRandom - 0.5f);
                        
            ParticleBuffer[_id.x].m_fCurTime = 0.f;
            ParticleBuffer[_id.x].m_fMaxTime = MinLifeTime + (MaxLifeTime - MinLifeTime) * vOut.r;
        }
    }
    
    // Ȱ��ȭ �� ��ƼŬ
    else
    {
        // �����ð� ����
        ParticleBuffer[_id.x].m_fCurTime += fDT;
        
        // �ִ� �����ð��� �Ѿ ���
        if ( ParticleBuffer[_id.x].m_fMaxTime <= ParticleBuffer[_id.x].m_fCurTime)
        {
            ParticleBuffer[_id.x].Alive = 0;
            return;
        }
                
        float fLifeRatio = ParticleBuffer[_id.x].m_fCurTime / ParticleBuffer[_id.x].m_fMaxTime;
        
        float fSpeed = MinSpeed + (MaxSpeed - MinSpeed) * fLifeRatio;        
        float4 vColor = StartColor + (EndColor - StartColor) * fLifeRatio;
        float3 vScale = StartScale + (EndScale - StartScale) * fLifeRatio;
        
        
        ParticleBuffer[_id.x].vPos += ParticleBuffer[_id.x].vDir * fSpeed * fDT;        
        ParticleBuffer[_id.x].vColor = vColor;        
        ParticleBuffer[_id.x].vScale = vScale;
    }
}

#endif