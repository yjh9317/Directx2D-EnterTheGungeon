#include "pch.h"
#include "CTimeMgr.h"

#include "CCore.h"


CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llPrevCount{}
	, m_llCurCount{}
	, m_iFPS(0)
	, m_fAddTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// 8 ����Ʈ ������ ���� ����
	// �ʴ� ī���� Ƚ��
	QueryPerformanceFrequency(&m_llFrequency);

	// 8 ����Ʈ ������ ���� ����
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::update()
{
	// ���� ī���� Ȯ��
	QueryPerformanceCounter(&m_llCurCount);

	// ���� ī���� �� ���� ī������ ���� ���̸� �ʴ� ī���ü��� ����� 1�������� ����Ǿ����� �ɸ� �ð��� ���Ѵ�(Delta Seconds)
	m_fDT = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

#ifdef _DEBUG
	 //���� 60 FPS Ȯ��
	if (m_fDT > (1.f / 60.f))
	{
		m_fDT = 1.f / 60.f;
	}
#endif

	// ���� ī���� ���� �����صд�(������ üũ �� Ȱ���ϱ� ���ؼ�)
	m_llPrevCount.QuadPart = m_llCurCount.QuadPart;

	// FPS ī����
	++m_iFPS;

	// �ð� ����
	m_fAddTime += m_fDT;
	
	// �����ð��� 1�ʸ� �Ѵ� ����
	if (m_fAddTime >= 1.f)
	{
		m_fAddTime = m_fAddTime - 1.f; // �ʰ��� ���� ���ܵд�.

		wchar_t szStr[256] = L"";
		wsprintf(szStr, L"FPS : %d", m_iFPS);		
		SetWindowTextW(CCore::GetInst()->GetMainHwnd(), szStr);

		m_iFPS = 0;
	}	

	m_fCurTime += m_fDT;

	g_global.fDT = m_fDT;
	g_global.fAccTime += m_fDT;
}
