//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

static const int UI_TITLE_OBJECT = 0;
static const int UI_NAME_OBJECT = 1;
static const int UI_TUTORIAL_OBJECT = 2;
static const int UI_LEVEL1_OBJECT = 3;
static const int UI_LEVEL2_OBJECT = 4;
static const int UI_LEVEL3_OBJECT = 5;
static const int UI_TUTORIAL_START_OBJECT = 6;
static const int UI_LEVEL1_START_OBJECT = 7;
static const int UI_LEVEL2_START_OBJECT = 8;
static const int UI_LEVEL3_START_OBJECT = 9;
static const int UI_END_OBJECT = 10;
static const int UI_MENU_START_FIRST_OBJECT = UI_TUTORIAL_START_OBJECT;
static const int UI_MENU_START_COUNT = 4;
static const int WORLD_OBJECT_START = 11;
static const int WORLD_OBJECT_COUNT = 8;
static const int TOTAL_SCENE_OBJECTS = WORLD_OBJECT_START + WORLD_OBJECT_COUNT;

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);

	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 1000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(30.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.001f, 0.0001f);
	m_pLights[1].m_bEnable = true;
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[1].m_xmf3Position = XMFLOAT3(-50.0f, 20.0f, -5.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true;
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_pLights[3].m_bEnable = true;
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 600.0f;
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(50.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

CGameObject *CScene::CreateTextObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const XMFLOAT3& xmf3Position, float fScale)
{
	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64] = { 0 };
	CGameObject *pTextModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, pstrFileName, &nMeshesInHierarchy, pnMaterialsInHierarchy);
	if (pTextModel) pTextModel->m_xmf4x4Transform = Matrix4x4::Identity();

	CGameObject *pTextObject = new CGameObject();
	pTextObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pTextObject->SetChild(pTextModel, true);
	pTextObject->SetScale(fScale, fScale, fScale);
	pTextObject->Rotate(0.0f, 180.0f, 0.0f);
	pTextObject->SetPosition(xmf3Position);
	return(pTextObject);
}

void CScene::SetSceneMode(GAME_SCENE_MODE nSceneMode)
{
	m_nSceneMode = nSceneMode;
	m_fModeElapsedTime = 0.0f;
	if (m_nSceneMode < GAME_SCENE_TUTORIAL)
	{
		::ReleaseCapture();
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}
	if (m_nSceneMode == GAME_SCENE_LEVEL1) ResetLevel1();
	if (m_nSceneMode != GAME_SCENE_START)
	{
		m_bTitleHovered = false;
		m_bNameHovered = false;
		m_bNameExploding = false;
		m_fNameExplosionElapsedTime = 0.0f;
		m_fTitleHoverRotation = 0.0f;
		m_fNameHoverRotation = 0.0f;
	}
	if (m_nSceneMode != GAME_SCENE_MENU)
	{
		for (int i = 0; i < UI_MENU_START_COUNT; i++)
		{
			m_bMenuStartHovered[i] = false;
			m_fMenuStartHoverRotation[i] = 0.0f;
		}
		m_bMenuEndHovered = false;
		m_fMenuEndHoverRotation = 0.0f;
	}
}

void CScene::ResetLevel1()
{
	if (!m_pPlayer) return;
	m_pPlayer->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	m_pPlayer->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

bool CScene::IsVisibleObject(int nObject) const
{
	if (m_nSceneMode == GAME_SCENE_START) return((nObject == UI_TITLE_OBJECT) || (nObject == UI_NAME_OBJECT));
	if (m_nSceneMode == GAME_SCENE_MENU) return((nObject >= UI_TUTORIAL_OBJECT) && (nObject <= UI_END_OBJECT));
	return(nObject >= WORLD_OBJECT_START);
}

bool CScene::IsStartTitleHover(int x, int y) const
{
	return((x >= 220) && (x <= 1060) && (y >= 180) && (y <= 320));
}

bool CScene::IsStartNameHover(int x, int y) const
{
	return((x >= 420) && (x <= 860) && (y >= 385) && (y <= 540));
}

bool CScene::IsMenuStartHover(int x, int y, int *pnMenuItem) const
{
	static const RECT rcMenuStarts[UI_MENU_START_COUNT] =
	{
		{ 760, 80, 1060, 170 },
		{ 760, 210, 1060, 300 },
		{ 760, 340, 1060, 430 },
		{ 760, 470, 1060, 560 }
	};

	for (int i = 0; i < UI_MENU_START_COUNT; i++)
	{
		if ((x >= rcMenuStarts[i].left) && (x <= rcMenuStarts[i].right) && (y >= rcMenuStarts[i].top) && (y <= rcMenuStarts[i].bottom))
		{
			if (pnMenuItem) *pnMenuItem = i;
			return(true);
		}
	}
	return(false);
}

bool CScene::IsMenuEndHover(int x, int y) const
{
	return((x >= 540) && (x <= 740) && (y >= 585) && (y <= 700));
}
void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);

	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);

	BuildDefaultLightsAndMaterials();

	m_nGameObjects = TOTAL_SCENE_OBJECTS;
	m_ppGameObjects = new CGameObject*[m_nGameObjects];
	for (int i = 0; i < m_nGameObjects; i++) m_ppGameObjects[i] = NULL;

	m_ppGameObjects[UI_TITLE_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/3DGameProgramming1.bin", XMFLOAT3(0.0f, 30.0f, 0.0f), 20.0f);
	m_ppGameObjects[UI_NAME_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/MyName.bin", XMFLOAT3(0.0f, -25.0f, 0.0f), 30.0f);
	m_xmf4x4StartTitleBaseTransform = m_ppGameObjects[UI_TITLE_OBJECT]->m_xmf4x4Transform;
	m_xmf4x4StartNameBaseTransform = m_ppGameObjects[UI_NAME_OBJECT]->m_xmf4x4Transform;
	m_ppGameObjects[UI_TUTORIAL_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Tutorial.bin", XMFLOAT3(-75.0f, 45.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL1_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Level_1.bin", XMFLOAT3(-75.0f, 20.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL2_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Level_2.bin", XMFLOAT3(-75.0f, -5.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL3_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Level_3.bin", XMFLOAT3(-75.0f, -30.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_TUTORIAL_START_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Start.bin", XMFLOAT3(45.0f, 45.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL1_START_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Start.bin", XMFLOAT3(45.0f, 20.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL2_START_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Start.bin", XMFLOAT3(45.0f, -5.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_LEVEL3_START_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Start.bin", XMFLOAT3(45.0f, -30.0f, 0.0f), 11.0f);
	m_ppGameObjects[UI_END_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/End.bin", XMFLOAT3(-10.0f, -55.0f, 0.0f), 11.0f);
	for (int i = 0; i < UI_MENU_START_COUNT; i++) m_xmf4x4MenuStartBaseTransforms[i] = m_ppGameObjects[UI_MENU_START_FIRST_OBJECT + i]->m_xmf4x4Transform;
	m_xmf4x4MenuEndBaseTransform = m_ppGameObjects[UI_END_OBJECT]->m_xmf4x4Transform;

	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64];
	CGameObject *pApacheModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Apache.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CApacheObject* pApacheObject = new CApacheObject();
	pApacheObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(+130.0f, 0.0f, 160.0f);
	pApacheObject->SetScale(1.5f, 1.5f, 1.5f);
	pApacheObject->Rotate(0.0f, 90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 0] = pApacheObject;

	pApacheObject = new CApacheObject();
	pApacheObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pApacheObject->SetChild(pApacheModel, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetPosition(-75.0f, 0.0f, 80.0f);
	pApacheObject->SetScale(1.5f, 1.5f, 1.5f);
	pApacheObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 1] = pApacheObject;

	nMeshesInHierarchy = 0;
	CGameObject *pGunshipModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Gunship.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CGunshipObject* pGunshipObject = new CGunshipObject();
	pGunshipObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pGunshipObject->SetChild(pGunshipModel, true);
	pGunshipObject->OnInitialize();
	pGunshipObject->SetPosition(135.0f, 40.0f, 220.0f);
	pGunshipObject->SetScale(8.5f, 8.5f, 8.5f);
	pGunshipObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 2] = pGunshipObject;

	nMeshesInHierarchy = 0;
	CGameObject *pSuperCobraModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/SuperCobra.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CSuperCobraObject* pSuperCobraObject = new CSuperCobraObject();
	pSuperCobraObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pSuperCobraObject->SetChild(pSuperCobraModel, true);
	pSuperCobraObject->OnInitialize();
	pSuperCobraObject->SetPosition(95.0f, 50.0f, 50.0f);
	pSuperCobraObject->SetScale(4.5f, 4.5f, 4.5f);
	pSuperCobraObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 3] = pSuperCobraObject;

	nMeshesInHierarchy = 0; 
	CGameObject *pMi24Model = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Mi24.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CMi24Object* pMi24Object = new CMi24Object();
	pMi24Object->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pMi24Object->SetChild(pMi24Model, true);
	pMi24Object->OnInitialize();
	pMi24Object->SetPosition(-95.0f, 50.0f, 50.0f);
	pMi24Object->SetScale(4.5f, 4.5f, 4.5f);
	pMi24Object->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 4] = pMi24Object;

	nMeshesInHierarchy = 0;
	CGameObject* pHummerModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Hummer.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CHummerObject* pHummerObject = new CHummerObject();
	pHummerObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pHummerObject->SetChild(pHummerModel);
	pHummerObject->OnInitialize();
	pHummerObject->SetPosition(260.0f, 0.0f, 150.0f);
	pHummerObject->SetScale(18.0f, 18.0f, 18.0f);
	pHummerObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 5] = pHummerObject;

	nMeshesInHierarchy = 0;
	CGameObject* pAbramsModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/M26.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CM26Object* pTankObject = new CM26Object();
	pTankObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pTankObject->SetChild(pAbramsModel);
	pTankObject->OnInitialize();
	pTankObject->SetPosition(260.0f, 0.0f, 150.0f);
	pTankObject->SetScale(18.0f, 18.0f, 18.0f);
	pTankObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 6] = pTankObject;

	nMeshesInHierarchy = 0;
	CGameObject* pEllenModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Ellen.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CGameObject* pEllenObject = new CGameObject();
	pEllenObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pEllenObject->SetChild(pEllenModel);
	pEllenObject->OnInitialize();
	pEllenObject->SetPosition(70.0f, -15.0f, 10.0f);
	pEllenObject->SetScale(28.0f, 28.0f, 28.0f);
	pEllenObject->Rotate(0.0f, 180.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 7] = pEllenObject;

	CreateShaderVariables(pd3dDevice, pd3dCommandList);
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

/***/	ReleaseShaderVariables();

	if (m_ppGameObjects)
	{
		for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->Release();
		delete[] m_ppGameObjects;
	}

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[4];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[1].Descriptor.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[3].Constants.Num32BitValues = 1;
	pd3dRootParameters[3].Constants.ShaderRegister = 3; //Framework Constants
	pd3dRootParameters[3].Constants.RegisterSpace = 0;
	pd3dRootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256ÀÇ ¹è¼ö
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);

	if (m_nSceneMode < GAME_SCENE_TUTORIAL)
	{
		XMFLOAT4 xmf4UiAmbient = XMFLOAT4(5.0f, 5.0f, 5.0f, 1.0f);
		int nUiLights = 0;
		::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &xmf4UiAmbient, sizeof(XMFLOAT4));
		::memcpy(&m_pcbMappedLights->m_nLights, &nUiLights, sizeof(int));
		return;
	}

	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}

	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->ReleaseShaderVariables();
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->ReleaseUploadBuffers();
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	if ((m_nSceneMode == GAME_SCENE_START) && (nMessageID == WM_MOUSEMOVE))
	{
		m_bTitleHovered = IsStartTitleHover(x, y);
		m_bNameHovered = IsStartNameHover(x, y);
		return(true);
	}
	if ((m_nSceneMode == GAME_SCENE_MENU) && (nMessageID == WM_MOUSEMOVE))
	{
		int nHoveredMenuItem = -1;
		IsMenuStartHover(x, y, &nHoveredMenuItem);
		for (int i = 0; i < UI_MENU_START_COUNT; i++) m_bMenuStartHovered[i] = (i == nHoveredMenuItem);
		m_bMenuEndHovered = IsMenuEndHover(x, y);
		return(true);
	}
	if (nMessageID != WM_LBUTTONUP) return(false);
	if ((m_nSceneMode == GAME_SCENE_START) && IsStartNameHover(x, y))
	{
		m_bNameExploding = true;
		m_fNameExplosionElapsedTime = 0.0f;
		return(true);
	}
	if (m_nSceneMode == GAME_SCENE_MENU)
	{
		int nSelectedMenuItem = -1;
		if (IsMenuStartHover(x, y, &nSelectedMenuItem))
		{
			SetSceneMode((GAME_SCENE_MODE)(GAME_SCENE_TUTORIAL + nSelectedMenuItem));
			return(true);
		}
		if (IsMenuEndHover(x, y))
		{
			::PostQuitMessage(0);
			return(true);
		}
	}
	return(m_nSceneMode < GAME_SCENE_TUTORIAL);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (nMessageID == WM_KEYUP)
	{
		if ((wParam == VK_ESCAPE) && (m_nSceneMode >= GAME_SCENE_TUTORIAL))
		{
			SetSceneMode(GAME_SCENE_MENU);
			return(true);
		}
		if ((wParam == VK_END) && (m_nSceneMode == GAME_SCENE_MENU))
		{
			::PostQuitMessage(0);
			return(true);
		}
		return(m_nSceneMode < GAME_SCENE_TUTORIAL);
	}

	return(m_nSceneMode < GAME_SCENE_TUTORIAL);
}
bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(m_nSceneMode < GAME_SCENE_TUTORIAL);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	m_fElapsedTime = fTimeElapsed;
	m_fModeElapsedTime += fTimeElapsed;

	if (m_nSceneMode == GAME_SCENE_START)
	{
		if (m_ppGameObjects[UI_TITLE_OBJECT])
		{
			m_fTitleHoverRotation = (m_bTitleHovered) ? (m_fTitleHoverRotation + 150.0f * fTimeElapsed) : 0.0f;
			m_ppGameObjects[UI_TITLE_OBJECT]->m_xmf4x4Transform = m_xmf4x4StartTitleBaseTransform;
			if (m_bTitleHovered) m_ppGameObjects[UI_TITLE_OBJECT]->Rotate(0.0f, m_fTitleHoverRotation, 0.0f);
			else m_ppGameObjects[UI_TITLE_OBJECT]->UpdateTransform(NULL);
		}

		if (m_ppGameObjects[UI_NAME_OBJECT])
		{
			if (m_bNameExploding)
			{
				m_fNameExplosionElapsedTime += fTimeElapsed;
				float fExplosionRatio = m_fNameExplosionElapsedTime / 0.45f;
				if (fExplosionRatio > 1.0f) fExplosionRatio = 1.0f;
				float fShake = sinf(fExplosionRatio * XM_2PI * 6.0f) * (1.0f - fExplosionRatio);
				m_ppGameObjects[UI_NAME_OBJECT]->m_xmf4x4Transform = m_xmf4x4StartNameBaseTransform;
				m_ppGameObjects[UI_NAME_OBJECT]->Rotate(360.0f * fExplosionRatio, 1080.0f * fExplosionRatio, 540.0f * fExplosionRatio);
				m_ppGameObjects[UI_NAME_OBJECT]->SetScale(1.0f + (3.5f * fExplosionRatio), 1.0f + (3.5f * fExplosionRatio), 1.0f + (3.5f * fExplosionRatio));
				m_ppGameObjects[UI_NAME_OBJECT]->SetPosition(28.0f * fShake, -25.0f + (70.0f * fExplosionRatio), 0.0f);
				if (m_fNameExplosionElapsedTime >= 0.45f) SetSceneMode(GAME_SCENE_MENU);
			}
			else
			{
				m_fNameHoverRotation = (m_bNameHovered) ? (m_fNameHoverRotation + 150.0f * fTimeElapsed) : 0.0f;
				m_ppGameObjects[UI_NAME_OBJECT]->m_xmf4x4Transform = m_xmf4x4StartNameBaseTransform;
				if (m_bNameHovered) m_ppGameObjects[UI_NAME_OBJECT]->Rotate(0.0f, m_fNameHoverRotation, 0.0f);
				else m_ppGameObjects[UI_NAME_OBJECT]->UpdateTransform(NULL);
			}
		}
		return;
	}

	if (m_nSceneMode == GAME_SCENE_MENU)
	{
		for (int i = 0; i < UI_MENU_START_COUNT; i++)
		{
			CGameObject *pStartObject = m_ppGameObjects[UI_MENU_START_FIRST_OBJECT + i];
			if (pStartObject)
			{
				m_fMenuStartHoverRotation[i] = (m_bMenuStartHovered[i]) ? (m_fMenuStartHoverRotation[i] + 150.0f * fTimeElapsed) : 0.0f;
				pStartObject->m_xmf4x4Transform = m_xmf4x4MenuStartBaseTransforms[i];
				if (m_bMenuStartHovered[i]) pStartObject->Rotate(0.0f, m_fMenuStartHoverRotation[i], 0.0f);
				else pStartObject->UpdateTransform(NULL);
			}
		}
		if (m_ppGameObjects[UI_END_OBJECT])
		{
			m_fMenuEndHoverRotation = (m_bMenuEndHovered) ? (m_fMenuEndHoverRotation + 150.0f * fTimeElapsed) : 0.0f;
			m_ppGameObjects[UI_END_OBJECT]->m_xmf4x4Transform = m_xmf4x4MenuEndBaseTransform;
			if (m_bMenuEndHovered) m_ppGameObjects[UI_END_OBJECT]->Rotate(0.0f, m_fMenuEndHoverRotation, 0.0f);
			else m_ppGameObjects[UI_END_OBJECT]->UpdateTransform(NULL);
		}
		return;
	}

	for (int i = WORLD_OBJECT_START; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i]) m_ppGameObjects[i]->Animate(fTimeElapsed, NULL);
	}

	if (m_pLights && m_pPlayer)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}
void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if (m_nSceneMode < GAME_SCENE_TUTORIAL)
	{
		pCamera->GenerateViewMatrix(XMFLOAT3(0.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	}

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i] && IsVisibleObject(i))
		{
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera, m_ppGameObjects[i]->m_ppd3dcbInstancingGameObjects, m_ppGameObjects[i]->m_ppcbMappedInstancingGameObjects);
		}
	}
}