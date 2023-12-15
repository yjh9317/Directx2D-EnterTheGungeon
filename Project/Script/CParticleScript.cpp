#include "pch.h"
#include "CParticleScript.h"

CParticleScript::CParticleScript()
	:CScript((int)SCRIPT_TYPE::PARTICLESCRIPT)
	, m_fTime(0.f)
{
}

CParticleScript::~CParticleScript()
{
}


void CParticleScript::update()
{
	m_fTime += DT;

	if (m_fTime > 2.f)
		GetOwner()->Destroy();
}

