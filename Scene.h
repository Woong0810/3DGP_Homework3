//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include "Player.h"

#define MAX_LIGHTS			16 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

struct LIGHT
{
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff;
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int						m_nType;
	float					m_fRange;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
	int						m_nLights;
};

class CTerrainObject;

enum GAME_SCENE_MODE
{
	GAME_SCENE_START = 0,
	GAME_SCENE_MENU,
	GAME_SCENE_TUTORIAL,
	GAME_SCENE_LEVEL1,
	GAME_SCENE_LEVEL2,
	GAME_SCENE_LEVEL3
};

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void BuildDefaultLightsAndMaterials();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();

	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	bool ProcessInput(UCHAR *pKeysBuffer);
	bool IsLevelPlaying() const { return(m_nSceneMode >= GAME_SCENE_TUTORIAL); }
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();

	CPlayer						*m_pPlayer = NULL;
	CTerrainObject				*m_pTerrain = NULL;

public:
	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;

	CGameObject					**m_ppGameObjects = NULL;
	int							m_nGameObjects = 0;

	LIGHT						*m_pLights = NULL;
	int							m_nLights = 0;

	XMFLOAT4					m_xmf4GlobalAmbient;

	ID3D12Resource				*m_pd3dcbLights = NULL;
	LIGHTS						*m_pcbMappedLights = NULL;

	float						m_fElapsedTime = 0.0f;

private:
	CGameObject *CreateTextObject(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList, const char *pstrFileName, const XMFLOAT3& xmf3Position, float fScale);
	void SetSceneMode(GAME_SCENE_MODE nSceneMode);
	void ResetLevel1();
	void ClampPlayerToTerrain();
	bool IsVisibleObject(int nObject) const;
	bool IsStartTitleHover(int x, int y) const;
	bool IsStartNameHover(int x, int y) const;
	bool IsMenuStartHover(int x, int y, int *pnMenuItem = NULL) const;
	bool IsMenuEndHover(int x, int y) const;

	GAME_SCENE_MODE			m_nSceneMode = GAME_SCENE_START;
	bool						m_bTitleHovered = false;
	bool						m_bNameHovered = false;
	bool						m_bNameExploding = false;
	float						m_fNameExplosionElapsedTime = 0.0f;
	float						m_fModeElapsedTime = 0.0f;
	float						m_fTitleHoverRotation = 0.0f;
	float						m_fNameHoverRotation = 0.0f;
	XMFLOAT4X4				m_xmf4x4StartTitleBaseTransform;
	XMFLOAT4X4				m_xmf4x4StartNameBaseTransform;
	bool						m_bMenuStartHovered[4] = { false, false, false, false };
	bool						m_bMenuEndHovered = false;
	float						m_fMenuStartHoverRotation[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float						m_fMenuEndHoverRotation = 0.0f;
	XMFLOAT4X4				m_xmf4x4MenuStartBaseTransforms[4];
	XMFLOAT4X4				m_xmf4x4MenuEndBaseTransform;
};
