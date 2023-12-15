#pragma once


#define SINGLE(TYPE) friend class CSingleton<TYPE>;\
					 private:\
						TYPE();\
						~TYPE();

#define DT CTimeMgr::GetInst()->GetDT()

#define KEY_TAP(key) KEY_STATE::TAP == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_PRESSED(key) KEY_STATE::PRESSED == CKeyMgr::GetInst()->GetKeyState(key)
#define KEY_AWAY(key) KEY_STATE::AWAY == CKeyMgr::GetInst()->GetKeyState(key)

#define DEVICE CDevice::GetInst()->GetDevice().Get()
#define CONTEXT CDevice::GetInst()->GetDeviceContext().Get()

#define MAX_LAYER 32

#define SAFE_DELETE(p) if(nullptr != p) delete p; p = nullptr;
#define CLONE(type) virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) virtual type* Clone() { return nullptr; }


#define DEGREE_TO_RADIAN(degree) ((PI/180)*(degree)) // 60�й� -> ȣ����
#define RADIAN_TO_DEGREE(radian) ((180/PI)*(radian)) // ȣ���� -> 60�й�
#define ONE_RADIAN 57.2957951;


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;


enum class SCENE_STATE
{
	PLAY,
	STOP,
	PAUSE,
};

enum class RES_TYPE
{
	PREFAB,
	MESHDATA,
	MATERIAL,
	GRAPHICS_SHADER,
	COMPUTE_SHADER,
	MESH,
	TEXTURE,
	SOUND,	
	SCENEFILE,
	END,
};

enum class CB_TYPE
{
	TRANSFORM,		// b0
	SCALAR_PARAM,	// b1
	ANIM2D,			// b2
	GLOBAL,			// b3

	//GLOBAL,			// b3
	END,
};

// Rasterizer option
enum class RS_TYPE
{
	CULL_BACK,	// Default
	CULL_FRONT,
	CULL_NONE, 
	WIRE_FRAME, 
	END,
};

// DepthStencil Option
enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,

	GREATER,
	GREATER_EQUAL,

	NO_TEST,	// �����׽�Ʈ ���� ����, ���̴� ���
	NO_WRITE,	// �����׽�Ʈ ����, ���̸� ������� ����

	NO_TEST_NO_WRITE, // �����׽�Ʈ ���� ����, ���� ������� ����
	END,
};

// BlendState Option
enum class BS_TYPE
{
	DEFAULT,		// SrcRGB * (1) + DestRGB * (0)
	ALPHA_BLEND,	// SrcRGB * (SrcAlpha) + DestRGB * (1 - SrcAlpha)
	END,
};

// ���̴��� ������ ������ ���� �з�
enum class SHADER_DOMAIN
{
	DOMAIN_FORWARD,	// ������
	DOMAIN_MASKED,	// ������, ����
	DOMAIN_TRANSLUCENT,	// ������
	DOMAIN_POSTPROCESS, // �� ó��
};


enum class COMPONENT_TYPE
{
	TRANSFORM,	// ��ġ, ũ��, ȸ�� (Location)

	CAMERA,		// ȭ���� ��� ī�޶� ����

	COLLIDER2D, // 2D �浹ü
	COLLIDER3D, // 3D �浹ü

	ANIMATOR2D, // 2D Sprite Animation
	ANIMATOR3D, // 3D Bone Skinning Animation

	LIGHT2D,
	LIGHT3D,

	BOUNDINGBOX, // Picking, FrustumCulling

	// renderer
	MESHRENDER,		// Mesh Renderer
	TILEMAP,		// 2D Tile Map Renderer
	PARTICLESYSTEM, // ���� ������
	LANDSCAPE,		// ���� ������
	DECAL,			// ��Į ������Ʈ
	
	END,

	// Update
	SCRIPT,			// ����
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
	END,
};



enum class SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1, 
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

	END,
};

enum class TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,

	TEX_CUBE_0,
	TEX_CUBE_1,
		
	TEX_ARR_0,
	TEX_ARR_1,

	END,
};


enum PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,	

	NO_PS = VS | HS | DS | GS,
	ALL = VS | HS | DS | GS | PS,
};


enum class EVENT_TYPE
{
	CREATE_OBJ,				// lParam : Object Adress, wParam : Layer Index
	DELETE_OBJ,				// lParam : Object Adress
	ADD_CHILD,				// lParam : Parent Object, wParam : Child Object
	DISCONNECT_PARENT,		// lParam : Object Adress
	SET_CAMEAR_INDEX,		// lParam : Camera Component Adress, wParam : Camera Change Index

	ACTIVATE_OBJECT,		// lParam : Object Adress
	DEACTIVATE_OBJECT,		// lParam : Object Adress

	ACTIVATE_COMPONENT,		// lParam : Component Adress
	DEACTIVATE_COMOPNENT,   // lParam : Component Adress

	SCENE_CHANGE,			// lParam : Next Stage Enum
	CHANGE_AI_STATE,		// lParam : FSM Adress, wParam : Next State Type
	CHANGE_PLAYER_STATE,	// lParam : FSM Adress, wParam : Next State Type

	DELETE_RES,				// lParam : ResAdress

	END,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// �¾�
	POINT,			// ����, ȶ��
	SPOT,			// ������
};

enum class MON_STATE
{
	START,
	IDLE,
	TRACE,
	ATK,
	HIT,
	SKILL1,
	SKILL2,
	SKILL3,
	SKILL4,
	SKILL5,
	DEAD,
};


enum class PLAYER_TYPE
{
	IDLE,
	WALK,
	ROLL,
	FALL,
	HIT,
	DEAD,
};
