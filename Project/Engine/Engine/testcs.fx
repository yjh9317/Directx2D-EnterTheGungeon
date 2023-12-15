#ifndef _TESTCS
#define _TESTCS

#include "value.fx"



// �׷� ������ ����
// �ִ� ���� 1024
// SV_GroupThreadID     : �׷� �������� ������ �ε���
// SV_GroupIndex        : �׷� �������� ������ �ε����� 1���� ������ ��ȯ
// SV_GroupID           : �����尡 ���� �׷��� �ε���
// SV_DispatchThreadID  : ��� �����带 �������� ���� ���� �ε���

RWTexture2D<float4> g_RWTex_0 : register(u0);

#define WIDTH   g_int_0
#define HEIGHT  g_int_1
#define Color   g_vec4_0

[numthreads(32, 32, 1)]
void CS_Test(int3 _id : SV_DispatchThreadID)
{
    if (_id.x < 0 || WIDTH <= _id.x || _id.y < 0 || HEIGHT <= _id.y)
        return;    
    
    g_RWTex_0[_id.xy] = Color;
}

#endif
