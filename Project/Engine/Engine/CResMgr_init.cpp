#include "pch.h"
#include "CResMgr.h"

void CResMgr::init()
{
	InitSound();

	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineShader();
	CreateEngineMaterial();

	CreateEngineComputeShader();
}

void CResMgr::update()
{
	CSound::g_pFMOD->update();
}

void CResMgr::CreateEngineMesh()
{
	CMesh* pMesh = nullptr;

	vector<Vtx>		vecVtx;
	vector<UINT>	vecIdx;

	Vtx v;

	// ==========
	// Point Mesh	
	// ==========
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"PointMesh", pMesh, true);
	vecVtx.clear();
	vecIdx.clear();


	// ========
	// RectMesh
	// 0 --- 1
	// |  \  |
	// 3 --- 2
	// ========
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.2f, 1.f, 0.2f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.2f, 0.2f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);
	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	
	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh, true);
	vecIdx.clear();

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh_LineStrip", pMesh, true);
	vecVtx.clear(); vecIdx.clear();

	// ==========
	// CircleMesh
	// ==========
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	UINT iSliceCount = 40;
	float fRadius = 0.5f;
	float fAngleStep = XM_2PI / float(iSliceCount);

	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3( fRadius * cosf(fAngleStep * (float)i) , fRadius * sinf(fAngleStep * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}


	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh, true);
	vecIdx.clear();

	// CicleMesh_LineStrip
	for (UINT i = 1; i <= iSliceCount + 1; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_LineStrip", pMesh, true);
	vecVtx.clear(); vecIdx.clear();


	// Cube

	// Sphere

	// Cylinder

	// Frustum


}

void CResMgr::CreateEngineTexture()
{
	Ptr<CTexture> pNoise01 = Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png", true);
	Ptr<CTexture> pNoise02 = Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png", true);
	Ptr<CTexture> pNoiseCloud = Load<CTexture>(L"texture\\noise\\noise_cloud.jpg", L"texture\\noise\\noise_cloud.jpg", true);

	pNoise01->UpdateData(PIPELINE_STAGE::ALL, 70);
	pNoise01->UpdateData_CS(70, true);

	pNoise02->UpdateData(PIPELINE_STAGE::ALL, 71);
	pNoise02->UpdateData_CS(71, true);

	pNoiseCloud->UpdateData(PIPELINE_STAGE::ALL, 72);
	pNoiseCloud->UpdateData_CS(72, true);

	g_global.vNoiseResolution = Vec2(pNoise01->Width(), pNoise01->Height());
}

void CResMgr::CreateEngineShader()
{
	MakeInputLayoutInfo();
		
	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
		
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
		
	pShader->AddScalarParamInfo(L"Mask Limit", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParamInfo(L"OutputTex 0", TEX_PARAM::TEX_0);
	pShader->AddTexParamInfo(L"OutputTex 1", TEX_PARAM::TEX_1);
	pShader->AddTexParamInfo(L"OutputTex 2", TEX_PARAM::TEX_2);

	pShader->AddScalarParamInfo(L"TestParam", SCALAR_PARAM::INT_0);


	AddRes<CGraphicsShader>(L"Std2DShader", pShader, true);


	// Std2DAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2DAlpha");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DAlpha");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
		
	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader, true);


	// PaperBurn Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\paperburn.fx", "VS_PaperBurn");
	pShader->CreatePixelShader(L"shader\\paperburn.fx", "PS_PaperBurn");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddScalarParamInfo(L"Burn Strength", SCALAR_PARAM::FLOAT_0);
	pShader->AddTexParamInfo(L"OutputTex", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"PaperBurnShader", pShader, true);


	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);		

	pShader->AddTexParamInfo(L"TileMapAtlas", TEX_PARAM::TEX_0);

	AddRes<CGraphicsShader>(L"TileMapShader", pShader, true);


	// Collider2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Collider2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Collider2D");
		
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->AddScalarParamInfo(L"IsCollision", SCALAR_PARAM::INT_0);

	AddRes<CGraphicsShader>(L"Collider2DShader", pShader, true);


	// Particle Render Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"Shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"Shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"Shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_TRANSLUCENT);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader, true);

	// PostProcess Shader
	pShader = new CGraphicsShader;

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_PostProcess");	
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_PostProcess");
	
	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->AddTexParamInfo(L"OutputTex 0", TEX_PARAM::TEX_0);
	pShader->AddScalarParamInfo(L"Time", SCALAR_PARAM::FLOAT_0);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader, true);

	//Player Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\playershader.fx", "VS_PlayerShader");
	pShader->CreatePixelShader(L"shader\\playershader.fx", "PS_PlayerShader");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->AddScalarParamInfo(L"DamagedCheck", SCALAR_PARAM::INT_0);
	AddRes<CGraphicsShader>(L"PlayerShader", pShader, true);

	//Monster Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\monstershader.fx", "VS_MonsterShader");
	pShader->CreatePixelShader(L"shader\\monstershader.fx", "PS_MonsterShader");

	pShader->SetShaderDomain(SHADER_DOMAIN::DOMAIN_MASKED);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);


	pShader->AddScalarParamInfo(L"IceCheck", SCALAR_PARAM::INT_0);
	AddRes<CGraphicsShader>(L"MonsterShader", pShader, true);

}

void CResMgr::CreateEngineMaterial()
{
	wstring strContent = CPathMgr::GetInst()->GetContentPath();

	CMaterial* pMtrl = nullptr;

	// Std2DMtrl ����
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));	
	AddRes<CMaterial>(L"material\\Std2DMtrl.mtrl", pMtrl);
	
	

	// Std2DAlphaBlend
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"material\\Std2DAlphaBlendMtrl.mtrl", pMtrl);
	

	// PaperBurnMtrl	
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PaperBurnShader"));
	AddRes<CMaterial>(L"material\\PaperBurnMtrl.mtrl", pMtrl);	
	

	// TileMapMtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"material\\TileMapMtrl.mtrl", pMtrl);	

	// Collider2DMtrl 
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Collider2DShader"));
	AddRes<CMaterial>(L"material\\Collider2DMtrl.mtrl", pMtrl);	

	// Particle Render Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl", pMtrl);
	
	// PostProcess Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, FindRes<CTexture>(L"PostProcessTex"));
	AddRes<CMaterial>(L"material\\PostProcessMtrl.mtrl", pMtrl);	

	// PlayerShader Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PlayerShader"));
	AddRes<CMaterial>(L"material\\PlayerShader.mtrl", pMtrl);

	// MonsterShader Mtrl
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"MonsterShader"));
	AddRes<CMaterial>(L"material\\MonsterShader.mtrl", pMtrl);
}

#include "CTestShader.h"
#include "CParticleUpdateShader.h"

void CResMgr::CreateEngineComputeShader()
{
	CComputeShader* pCS = nullptr;

	// TestShader
	pCS = new CTestShader;
	pCS->CreateComputeShader(L"Shader\\testcs.fx", "CS_Test");
	AddRes<CComputeShader>(L"TestCS", pCS, true);

	// Particle Update Shader
	pCS = new CParticleUpdateShader;
	pCS->CreateComputeShader(L"Shader\\particle.fx", "CS_Particle");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pCS, true);
}


void CResMgr::MakeInputLayoutInfo()
{
	// ���� �Է� ���� (InputLayout ����)	
	UINT iOffset = 0;
	D3D11_INPUT_ELEMENT_DESC tInputDesc = {};

	tInputDesc.SemanticName = "POSITION";	// Semantic �̸�
	tInputDesc.SemanticIndex = 0;			// �ߺ� �̸��� ��� �ε����� ����
	tInputDesc.InputSlot = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 12;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName = "COLOR";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 16;

	CGraphicsShader::AddInputLayout(tInputDesc);

	tInputDesc.SemanticName = "TEXCOORD";
	tInputDesc.SemanticIndex = 0;
	tInputDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tInputDesc.InstanceDataStepRate = 0;
	tInputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	tInputDesc.AlignedByteOffset = iOffset;
	iOffset += 8;

	CGraphicsShader::AddInputLayout(tInputDesc);

}


void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32�� ä�� ����
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

