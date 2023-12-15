#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// ��ǥ	
	Vec4 vColor;	// ����

	Vec2 vUV;

};

typedef Vertex Vtx;



// =======
// TileMap
// =======
struct tTileData
{
	int		iImgIdx;
	Vec2	vLTUV;
	int		TilePass;

	tTileData()
		: iImgIdx(-1)
		, TilePass(0)
	{}
};




// ==========
// Animtion2D
// ==========
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};



// ========
// Particle 
// ========
struct tParticle
{
	Vec3	vPos;
	Vec3	vScale;
	Vec3	vDir;
	Vec4	vColor;

	int		Alive;
	float   m_fCurTime; // ���� �ð�
	float	m_fMaxTime; // �ִ� ���� �ð�
};

struct tParticleData
{
	int		iAliveCount;
	Vec3	vPadding;
};


struct tLightColor
{
	Vec4 vDiff;	// Diffuse Light	���ݻ籤
	Vec4 vSpec; // Specular Light	�ݻ籤
	Vec4 vAmb;  // Ambient Light	ȯ�汤
};

struct tLightInfo
{
	tLightColor color;
	Vec3		vLightDir;	
	int			iLightType;
	Vec3		vWorldPos;
	float		fAngle;
	float		fRange;
	Vec3		vPadding;
};




// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tScalarParam
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int bTex[10];
	int iPadding[2];
};

struct tAnim2D
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;
	Vec2    vOffset;

	int     useAnim2D;
	float   Atlas_Width;
	float   Atlas_Height;
	float	Anim2D_Padding;
};

struct tGlobal
{
	Vec2	vResolution; // RenderTarget Resolution
	Vec2    vNoiseResolution; // NoiseTexture Resolution
	float	fDT;
	float	fAccTime;
	int		Light2DCount;
	int     Light3DCount;
};