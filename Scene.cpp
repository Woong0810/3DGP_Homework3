//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "Terrain.h"
#include "Bullet.h"
#include "HUD.h"

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
static const int WORLD_OBJECT_COUNT = 5;
static const int LEVEL1_CLEAR_OBJECT = WORLD_OBJECT_START + WORLD_OBJECT_COUNT;
static const int TOTAL_SCENE_OBJECTS = LEVEL1_CLEAR_OBJECT + 1;
static const int HUD_BAR_COUNT = 4;
static const int HUD_PLAYER_BACKGROUND = 0;
static const int HUD_PLAYER_GAUGE = 1;
static const int HUD_ENEMY_BACKGROUND = 2;
static const int HUD_ENEMY_GAUGE = 3;

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::GetClearColor(float pfClearColor[4]) const
{
	const float pfDefaultClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float pfLevel1BattleClearColor[4] = { 0.42f, 0.52f, 0.58f, 1.0f };
	const float *pfSourceClearColor = ((m_nSceneMode == GAME_SCENE_LEVEL1) && !m_bLevel1Cleared) ? pfLevel1BattleClearColor : pfDefaultClearColor;
	for (int i = 0; i < 4; i++) pfClearColor[i] = pfSourceClearColor[i];
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

	const float fStartX = 0.0f;
	const float fStartZ = -120.0f;
	const float fStartAltitude = 60.0f;
	float fTerrainY = (m_pTerrain) ? m_pTerrain->GetHeight(fStartX, fStartZ) : 20.0f;
	XMFLOAT3 xmf3StartPosition = XMFLOAT3(fStartX, fTerrainY + fStartAltitude, fStartZ);

	m_pPlayer->SetPosition(xmf3StartPosition);
	m_pPlayer->SetVelocity(XMFLOAT3(0.0f, 0.0f, 0.0f));
	for (int i = 0; i < m_nProjectiles; i++) if (m_ppProjectiles[i]) m_ppProjectiles[i]->Reset();
	m_fProjectileFireCooldown = 0.0f;
	m_nPlayerHP = m_nPlayerMaxHP;
	m_nLastHitLevel1TargetIndex = -1;
	m_fLastHitTargetDisplayElapsedTime = 0.0f;
	m_bLevel1Cleared = false;
	m_fLevel1ClearElapsedTime = 0.0f;
	if (m_ppGameObjects && m_ppGameObjects[LEVEL1_CLEAR_OBJECT]) m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->m_xmf4x4Transform = m_xmf4x4Level1ClearBaseTransform;
	ResetLevel1Targets();

	char pstrDebug[128];
	sprintf_s(pstrDebug, "Level-1 Reset Player Position: %.2f, %.2f, %.2f\n", xmf3StartPosition.x, xmf3StartPosition.y, xmf3StartPosition.z);
	::OutputDebugStringA(pstrDebug);
}

void CScene::ClampPlayerToTerrain()
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || !m_pPlayer || !m_pTerrain) return;

	const float fMinAltitude = 40.0f;
	XMFLOAT3 xmf3PlayerPosition = m_pPlayer->GetPosition();
	float fTerrainY = m_pTerrain->GetHeight(xmf3PlayerPosition.x, xmf3PlayerPosition.z);
	float fMinimumY = fTerrainY + fMinAltitude;
	if (xmf3PlayerPosition.y < fMinimumY)
	{
		xmf3PlayerPosition.y = fMinimumY;
		m_pPlayer->SetPosition(xmf3PlayerPosition);
	}
}

void CScene::FirePlayerProjectile()
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || !m_pPlayer || (m_fProjectileFireCooldown > 0.0f)) return;

	CProjectileObject *ppProjectilesToFire[2] = { NULL, NULL };
	for (int i = 0; i < m_nProjectiles; i++)
	{
		if (m_ppProjectiles[i] && !m_ppProjectiles[i]->IsActive())
		{
			if (!ppProjectilesToFire[0]) ppProjectilesToFire[0] = m_ppProjectiles[i];
			else
			{
				ppProjectilesToFire[1] = m_ppProjectiles[i];
				break;
			}
		}
	}
	if (!ppProjectilesToFire[0]) return;

	const float fMuzzleRightOffset = 1.6f;
	const float fMuzzleLookOffset = 3.0f;
	const float fMuzzleUpOffset = -0.75f;
	XMFLOAT3 xmf3Look = Vector3::Normalize(m_pPlayer->GetLookVector());
	XMFLOAT3 xmf3Right = Vector3::Normalize(m_pPlayer->GetRightVector());
	XMFLOAT3 xmf3Up = Vector3::Normalize(m_pPlayer->GetUpVector());
	const float fProjectileDownAimOffset = -0.3f;
	XMFLOAT3 xmf3FireDirection = Vector3::Normalize(Vector3::Add(xmf3Look, xmf3Up, fProjectileDownAimOffset));
	XMFLOAT3 xmf3BasePosition = m_pPlayer->GetPosition();

	m_pPlayer->OnPrepareRender();
	CGameObject *pMissileFrame = m_pPlayer->FindFrame("Hellfire_Missile");
	if (pMissileFrame) xmf3BasePosition = pMissileFrame->GetPosition();

	XMFLOAT3 xmf3LeftMuzzlePosition = Vector3::Add(Vector3::Add(Vector3::Add(xmf3BasePosition, xmf3Right, -fMuzzleRightOffset), xmf3Look, fMuzzleLookOffset), xmf3Up, fMuzzleUpOffset);
	XMFLOAT3 xmf3RightMuzzlePosition = Vector3::Add(Vector3::Add(Vector3::Add(xmf3BasePosition, xmf3Right, +fMuzzleRightOffset), xmf3Look, fMuzzleLookOffset), xmf3Up, fMuzzleUpOffset);

	ppProjectilesToFire[0]->Fire(xmf3LeftMuzzlePosition, xmf3FireDirection);
	if (ppProjectilesToFire[1]) ppProjectilesToFire[1]->Fire(xmf3RightMuzzlePosition, xmf3FireDirection);
	m_fProjectileFireCooldown = PROJECTILE_FIRE_COOLDOWN;
}
void CScene::InitializeLevel1Targets()
{
	if (m_pLevel1Targets) delete[] m_pLevel1Targets;

	m_nLevel1Targets = 4;
	m_pLevel1Targets = new SLevel1TargetState[m_nLevel1Targets];
	m_nCurrentLevel1Wave = 1;

	const int nApache1Index = WORLD_OBJECT_START + 0;
	const int nApache2Index = WORLD_OBJECT_START + 1;
	const int nSuperCobraIndex = WORLD_OBJECT_START + 2;
	const int nMi24Index = WORLD_OBJECT_START + 3;

	const XMFLOAT3 xmf3StartPositions[4] =
	{
		XMFLOAT3(-120.0f, 80.0f, 200.0f),
		XMFLOAT3(+120.0f, 80.0f, 220.0f),
		XMFLOAT3(0.0f, 95.0f, 430.0f),
		XMFLOAT3(0.0f, 110.0f, 650.0f)
	};
	const int pnObjectIndices[4] = { nApache1Index, nApache2Index, nSuperCobraIndex, nMi24Index };
	const int pnWaves[4] = { 1, 1, 2, 3 };
	const int pnMaxHPs[4] = { 18, 18, 30, 45 };
	const float pfCollisionRadii[4] = { 8.0f, 8.0f, 8.0f, 9.0f };
	const float pfMoveSpeeds[4] = { 1.1f, 1.0f, 1.35f, 0.65f };
	const float pfMoveRadii[4] = { 22.0f, 24.0f, 28.0f, 18.0f };

	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		XMFLOAT3 xmf3StartPosition = xmf3StartPositions[i];
		float fTerrainY = (m_pTerrain) ? m_pTerrain->GetHeight(xmf3StartPosition.x, xmf3StartPosition.z) : 0.0f;
		xmf3StartPosition.y += fTerrainY;

		m_pLevel1Targets[i].m_nObjectIndex = pnObjectIndices[i];
		m_pLevel1Targets[i].m_nWave = pnWaves[i];
		m_pLevel1Targets[i].m_nMaxHP = pnMaxHPs[i];
		m_pLevel1Targets[i].m_nHP = pnMaxHPs[i];
		m_pLevel1Targets[i].m_bActive = false;
		m_pLevel1Targets[i].m_bDestroying = false;
		m_pLevel1Targets[i].m_fDestroyElapsedTime = 0.0f;
		m_pLevel1Targets[i].m_fCollisionRadius = pfCollisionRadii[i];
		m_pLevel1Targets[i].m_xmf3StartPosition = xmf3StartPosition;
		m_pLevel1Targets[i].m_xmf3BasePosition = xmf3StartPosition;
		m_pLevel1Targets[i].m_fMoveAngle = 0.0f;
		m_pLevel1Targets[i].m_fMoveSpeed = pfMoveSpeeds[i];
		m_pLevel1Targets[i].m_fMoveRadius = pfMoveRadii[i];

		if (m_ppGameObjects[pnObjectIndices[i]])
		{
			m_ppGameObjects[pnObjectIndices[i]]->SetPosition(xmf3StartPosition);
			m_pLevel1Targets[i].m_xmf4x4StartTransform = m_ppGameObjects[pnObjectIndices[i]]->m_xmf4x4Transform;
		}
	}
}

void CScene::ResetLevel1Targets()
{
	m_nCurrentLevel1Wave = 1;
	m_bLevel1Cleared = false;
	m_fLevel1ClearElapsedTime = 0.0f;
	if (!m_pLevel1Targets) return;

	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		SLevel1TargetState& target = m_pLevel1Targets[i];
		target.m_nHP = target.m_nMaxHP;
		target.m_bActive = false;
		target.m_bDestroying = false;
		target.m_fDestroyElapsedTime = 0.0f;
		target.m_fMoveAngle = 0.0f;
		target.m_xmf3BasePosition = target.m_xmf3StartPosition;
		if ((target.m_nObjectIndex >= 0) && (target.m_nObjectIndex < m_nGameObjects) && m_ppGameObjects[target.m_nObjectIndex])
		{
			m_ppGameObjects[target.m_nObjectIndex]->m_xmf4x4Transform = target.m_xmf4x4StartTransform;
			m_ppGameObjects[target.m_nObjectIndex]->SetPosition(target.m_xmf3StartPosition);
		}
	}
	ActivateLevel1Wave(1);
}

void CScene::ActivateLevel1Wave(int nWave)
{
	if (!m_pLevel1Targets) return;

	m_nCurrentLevel1Wave = nWave;
	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		SLevel1TargetState& target = m_pLevel1Targets[i];
		bool bWaveTarget = (target.m_nWave == nWave);
		target.m_bActive = bWaveTarget;
		target.m_bDestroying = false;
		target.m_fDestroyElapsedTime = 0.0f;
		if (bWaveTarget)
		{
			target.m_nHP = target.m_nMaxHP;
			target.m_fMoveAngle = 0.0f;
			target.m_xmf3BasePosition = target.m_xmf3StartPosition;
			if ((target.m_nObjectIndex >= 0) && (target.m_nObjectIndex < m_nGameObjects) && m_ppGameObjects[target.m_nObjectIndex])
			{
				m_ppGameObjects[target.m_nObjectIndex]->m_xmf4x4Transform = target.m_xmf4x4StartTransform;
				m_ppGameObjects[target.m_nObjectIndex]->SetPosition(target.m_xmf3StartPosition);
			}
		}
	}
}

void CScene::UpdateLevel1Targets(float fTimeElapsed)
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || !m_pLevel1Targets) return;

	const float fDestroyDuration = 0.45f;
	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		SLevel1TargetState& target = m_pLevel1Targets[i];
		if (!target.m_bActive) continue;
		if ((target.m_nObjectIndex < 0) || (target.m_nObjectIndex >= m_nGameObjects)) continue;

		CGameObject *pTargetObject = m_ppGameObjects[target.m_nObjectIndex];
		if (!pTargetObject) continue;

		if (target.m_bDestroying)
		{
			target.m_fDestroyElapsedTime += fTimeElapsed;
			float fDestroyRatio = target.m_fDestroyElapsedTime / fDestroyDuration;
			if (fDestroyRatio > 1.0f) fDestroyRatio = 1.0f;

			XMFLOAT3 xmf3DestroyPosition = target.m_xmf3BasePosition;
			xmf3DestroyPosition.y += fDestroyRatio * 8.0f;
			float fDestroyScale = 1.0f + (fDestroyRatio * 0.8f);

			pTargetObject->m_xmf4x4Transform = target.m_xmf4x4StartTransform;
			pTargetObject->SetPosition(xmf3DestroyPosition);
			pTargetObject->SetScale(fDestroyScale, fDestroyScale, fDestroyScale);
			pTargetObject->Rotate(0.0f, 720.0f * target.m_fDestroyElapsedTime, 0.0f);

			if (target.m_fDestroyElapsedTime >= fDestroyDuration)
			{
				target.m_bActive = false;
				target.m_bDestroying = false;
				target.m_fDestroyElapsedTime = 0.0f;
				pTargetObject->m_xmf4x4Transform = target.m_xmf4x4StartTransform;
				pTargetObject->SetPosition(target.m_xmf3StartPosition);
			}
			continue;
		}

		target.m_fMoveAngle += target.m_fMoveSpeed * fTimeElapsed;
		XMFLOAT3 xmf3Position = target.m_xmf3BasePosition;
		if (target.m_nWave == 1)
		{
			xmf3Position.x = target.m_xmf3BasePosition.x + sinf(target.m_fMoveAngle) * target.m_fMoveRadius;
		}
		else if (target.m_nWave == 2)
		{
			xmf3Position.x = target.m_xmf3BasePosition.x + sinf(target.m_fMoveAngle) * target.m_fMoveRadius;
			xmf3Position.z = target.m_xmf3BasePosition.z + cosf(target.m_fMoveAngle * 0.7f) * (target.m_fMoveRadius * 0.35f);
		}
		else
		{
			xmf3Position.x = target.m_xmf3BasePosition.x + sinf(target.m_fMoveAngle) * target.m_fMoveRadius;
			xmf3Position.z = target.m_xmf3BasePosition.z + cosf(target.m_fMoveAngle) * target.m_fMoveRadius;
		}

		pTargetObject->SetPosition(xmf3Position);
		pTargetObject->Rotate(0.0f, target.m_fMoveSpeed * fTimeElapsed * 20.0f, 0.0f);
	}
}

void CScene::UpdateLevel1ClearText()
{
	if (!m_bLevel1Cleared) return;
	if (!m_ppGameObjects || !m_ppGameObjects[LEVEL1_CLEAR_OBJECT]) return;

	const float fClearBaseY = 0.0f;
	const float fClearBobSpeed = 2.5f;
	const float fClearBobAmplitude = 5.0f;
	float fClearY = fClearBaseY + sinf(m_fLevel1ClearElapsedTime * fClearBobSpeed) * fClearBobAmplitude;

	CGameObject *pClearObject = m_ppGameObjects[LEVEL1_CLEAR_OBJECT];
	pClearObject->m_xmf4x4Transform = m_xmf4x4Level1ClearBaseTransform;
	pClearObject->SetPosition(XMFLOAT3(0.0f, fClearY, 0.0f));
}

void CScene::CheckProjectileTargetCollisions()
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || !m_ppProjectiles || !m_pLevel1Targets) return;

	for (int i = 0; i < m_nProjectiles; i++)
	{
		CProjectileObject *pProjectile = m_ppProjectiles[i];
		if (!pProjectile || !pProjectile->IsActive()) continue;

		XMFLOAT3 xmf3ProjectilePosition = pProjectile->GetPosition();
		float fProjectileRadius = pProjectile->GetCollisionRadius();

		for (int j = 0; j < m_nLevel1Targets; j++)
		{
			SLevel1TargetState& target = m_pLevel1Targets[j];
			if (!target.m_bActive || target.m_bDestroying) continue;
			if ((target.m_nObjectIndex < 0) || (target.m_nObjectIndex >= m_nGameObjects)) continue;
			CGameObject *pTargetObject = m_ppGameObjects[target.m_nObjectIndex];
			if (!pTargetObject) continue;

			XMFLOAT3 xmf3TargetPosition = pTargetObject->GetPosition();
			XMFLOAT3 xmf3Difference = Vector3::Subtract(xmf3ProjectilePosition, xmf3TargetPosition);
			float fDistance = Vector3::Length(xmf3Difference);
			float fHitDistance = fProjectileRadius + target.m_fCollisionRadius;
			if (fDistance <= fHitDistance)
			{
				pProjectile->Reset();
				ApplyDamageToLevel1Target(j, 1);
				break;
			}
		}
	}
}

void CScene::ApplyDamageToLevel1Target(int nTargetIndex, int nDamage)
{
	if (!m_pLevel1Targets || (nTargetIndex < 0) || (nTargetIndex >= m_nLevel1Targets)) return;

	SLevel1TargetState& target = m_pLevel1Targets[nTargetIndex];
	if (!target.m_bActive || target.m_bDestroying) return;

	target.m_nHP -= nDamage;
	m_nLastHitLevel1TargetIndex = nTargetIndex;
	m_fLastHitTargetDisplayElapsedTime = 0.0f;
	char pstrDebug[160];
	sprintf_s(pstrDebug, "[Level1] Target hit: index=%d, HP=%d\n", nTargetIndex, target.m_nHP);
	::OutputDebugStringA(pstrDebug);

	if (target.m_nHP <= 0)
	{
		target.m_nHP = 0;
		target.m_bDestroying = true;
		target.m_fDestroyElapsedTime = 0.0f;
		sprintf_s(pstrDebug, "[Level1] Target destroyed: wave=%d, objectIndex=%d\n", target.m_nWave, target.m_nObjectIndex);
		::OutputDebugStringA(pstrDebug);
	}
}

bool CScene::IsCurrentLevel1WaveCleared() const
{
	if (!m_pLevel1Targets) return(false);

	bool bHasCurrentWaveTarget = false;
	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		if (m_pLevel1Targets[i].m_nWave != m_nCurrentLevel1Wave) continue;
		bHasCurrentWaveTarget = true;
		if (m_pLevel1Targets[i].m_bActive || m_pLevel1Targets[i].m_bDestroying) return(false);
	}
	return(bHasCurrentWaveTarget);
}

void CScene::AdvanceLevel1WaveIfNeeded()
{
	if (m_bLevel1Cleared || !IsCurrentLevel1WaveCleared()) return;

	if (m_nCurrentLevel1Wave == 1)
	{
		::OutputDebugStringA("[Level1] Activate Wave 2\n");
		ActivateLevel1Wave(2);
	}
	else if (m_nCurrentLevel1Wave == 2)
	{
		::OutputDebugStringA("[Level1] Activate Wave 3\n");
		ActivateLevel1Wave(3);
	}
	else if (m_nCurrentLevel1Wave == 3)
	{
		m_bLevel1Cleared = true;
		m_fLevel1ClearElapsedTime = 0.0f;
		for (int i = 0; i < m_nLevel1Targets; i++) m_pLevel1Targets[i].m_bActive = false;
		::OutputDebugStringA("[Level1] Mission Clear\n");
	}
}
int CScene::GetHudEnemyTargetIndex() const
{
	if (m_bLevel1Cleared || !m_pLevel1Targets) return(-1);

	if ((m_nLastHitLevel1TargetIndex >= 0) && (m_nLastHitLevel1TargetIndex < m_nLevel1Targets))
	{
		const SLevel1TargetState& lastHitTarget = m_pLevel1Targets[m_nLastHitLevel1TargetIndex];
		if (lastHitTarget.m_bActive || lastHitTarget.m_bDestroying) return(m_nLastHitLevel1TargetIndex);
	}

	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		const SLevel1TargetState& target = m_pLevel1Targets[i];
		if ((target.m_nWave == m_nCurrentLevel1Wave) && (target.m_bActive || target.m_bDestroying)) return(i);
	}
	return(-1);
}

void CScene::UpdateLevel1HudBars()
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || m_bLevel1Cleared || !m_ppHudBars || (m_nHudBars < HUD_BAR_COUNT)) return;
	if (!m_ppHudBars[HUD_PLAYER_BACKGROUND] || !m_ppHudBars[HUD_PLAYER_GAUGE] || !m_ppHudBars[HUD_ENEMY_BACKGROUND] || !m_ppHudBars[HUD_ENEMY_GAUGE]) return;

	const float fHudFullWidth = 150.0f;
	const float fHudHeight = 3.5f;
	const float fHudDepth = 0.5f;
	const float fPlayerHudY = 48.0f;
	const float fEnemyHudY = 41.0f;
	const float fBackgroundZ = 0.0f;
	const float fGaugeZ = -0.35f;

	XMFLOAT3 xmf3HudRight = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3 xmf3HudUp = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 xmf3HudLook = XMFLOAT3(0.0f, 0.0f, 1.0f);

	float fPlayerRatio = (m_nPlayerMaxHP > 0) ? ((float)m_nPlayerHP / (float)m_nPlayerMaxHP) : 0.0f;
	if (fPlayerRatio < 0.0f) fPlayerRatio = 0.0f;
	if (fPlayerRatio > 1.0f) fPlayerRatio = 1.0f;

	int nEnemyTargetIndex = GetHudEnemyTargetIndex();
	float fEnemyRatio = 0.0f;
	if ((nEnemyTargetIndex >= 0) && (nEnemyTargetIndex < m_nLevel1Targets))
	{
		const SLevel1TargetState& target = m_pLevel1Targets[nEnemyTargetIndex];
		fEnemyRatio = (target.m_nMaxHP > 0) ? ((float)target.m_nHP / (float)target.m_nMaxHP) : 0.0f;
	}
	if (fEnemyRatio < 0.0f) fEnemyRatio = 0.0f;
	if (fEnemyRatio > 1.0f) fEnemyRatio = 1.0f;

	float fPlayerGaugeWidth = fHudFullWidth * fPlayerRatio;
	float fEnemyGaugeWidth = fHudFullWidth * fEnemyRatio;
	float fPlayerGaugeCenterX = (-fHudFullWidth * 0.5f) + (fPlayerGaugeWidth * 0.5f);
	float fEnemyGaugeCenterX = (-fHudFullWidth * 0.5f) + (fEnemyGaugeWidth * 0.5f);

	m_ppHudBars[HUD_PLAYER_BACKGROUND]->SetHudTransform(XMFLOAT3(0.0f, fPlayerHudY, fBackgroundZ), xmf3HudRight, xmf3HudUp, xmf3HudLook, fHudFullWidth, fHudHeight, fHudDepth);
	m_ppHudBars[HUD_PLAYER_GAUGE]->SetHudTransform(XMFLOAT3(fPlayerGaugeCenterX, fPlayerHudY, fGaugeZ), xmf3HudRight, xmf3HudUp, xmf3HudLook, fPlayerGaugeWidth, fHudHeight, fHudDepth);
	m_ppHudBars[HUD_ENEMY_BACKGROUND]->SetHudTransform(XMFLOAT3(0.0f, fEnemyHudY, fBackgroundZ), xmf3HudRight, xmf3HudUp, xmf3HudLook, fHudFullWidth, fHudHeight, fHudDepth);
	m_ppHudBars[HUD_ENEMY_GAUGE]->SetHudTransform(XMFLOAT3(fEnemyGaugeCenterX, fEnemyHudY, fGaugeZ), xmf3HudRight, xmf3HudUp, xmf3HudLook, fEnemyGaugeWidth, fHudHeight, fHudDepth);
}

void CScene::RenderLevel1HudBars(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || m_bLevel1Cleared || !m_ppHudBars) return;
	for (int i = 0; i < m_nHudBars; i++)
	{
		if (m_ppHudBars[i])
		{
			m_ppHudBars[i]->UpdateTransform(NULL);
			m_ppHudBars[i]->Render(pd3dCommandList, pCamera, m_ppHudBars[i]->m_ppd3dcbInstancingGameObjects, m_ppHudBars[i]->m_ppcbMappedInstancingGameObjects);
		}
	}
}

void CScene::RenderLevel1HudOverlay(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	if ((m_nSceneMode != GAME_SCENE_LEVEL1) || m_bLevel1Cleared || !pd3dCommandList || !pCamera || !m_ppHudBars) return;

	UpdateLevel1HudBars();

	XMFLOAT3 xmf3OldPosition = pCamera->GetPosition();
	XMFLOAT3 xmf3OldRight = pCamera->GetRightVector();
	XMFLOAT3 xmf3OldUp = pCamera->GetUpVector();
	XMFLOAT3 xmf3OldLook = pCamera->GetLookVector();
	XMFLOAT3 xmf3OldLookAt = pCamera->GetLookAtPosition();

	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->GenerateViewMatrix(XMFLOAT3(0.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	UpdateShaderVariables(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, m_pd3dcbLights->GetGPUVirtualAddress());
	RenderLevel1HudBars(pd3dCommandList, pCamera);

	pCamera->SetPosition(xmf3OldPosition);
	pCamera->GetRightVector() = xmf3OldRight;
	pCamera->GetUpVector() = xmf3OldUp;
	pCamera->GetLookVector() = xmf3OldLook;
	pCamera->SetLookAtPosition(xmf3OldLookAt);
	pCamera->RegenerateViewMatrix();
}
bool CScene::IsActiveLevel1TargetObject(int nObjectIndex) const
{
	if (!m_pLevel1Targets) return(false);
	for (int i = 0; i < m_nLevel1Targets; i++)
	{
		if (m_pLevel1Targets[i].m_nObjectIndex == nObjectIndex) return(m_pLevel1Targets[i].m_bActive);
	}
	return(false);
}
bool CScene::IsVisibleObject(int nObject) const
{
	if (m_nSceneMode == GAME_SCENE_START) return((nObject == UI_TITLE_OBJECT) || (nObject == UI_NAME_OBJECT));
	if (m_nSceneMode == GAME_SCENE_MENU) return((nObject >= UI_TUTORIAL_OBJECT) && (nObject <= UI_END_OBJECT));
	if (m_nSceneMode == GAME_SCENE_LEVEL1)
	{
		if (nObject == LEVEL1_CLEAR_OBJECT) return(m_bLevel1Cleared);
		if (nObject == WORLD_OBJECT_START + 4) return(true);
		if ((nObject >= WORLD_OBJECT_START) && (nObject < WORLD_OBJECT_START + 4)) return(IsActiveLevel1TargetObject(nObject));
		return(false);
	}
	return((nObject >= WORLD_OBJECT_START) && (nObject != LEVEL1_CLEAR_OBJECT));
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
	m_ppGameObjects[LEVEL1_CLEAR_OBJECT] = CreateTextObject(pd3dDevice, pd3dCommandList, "Model/Clear.bin", XMFLOAT3(0.0f, 0.0f, 0.0f), 28.0f);
	m_xmf4x4Level1ClearBaseTransform = m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->m_xmf4x4Transform;
	int nMeshesInHierarchy = 0;
	int pnMaterialsInHierarchy[64];
	CGameObject *pApacheModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Apache.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CGameObject *pApacheModel1 = pApacheModel;
	CApacheObject* pApacheObject = new CApacheObject();
	pApacheObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pApacheObject->SetChild(pApacheModel1, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetScale(1.0f, 1.0f, 1.0f);
	pApacheObject->Rotate(0.0f, 90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 0] = pApacheObject;

	nMeshesInHierarchy = 0;
	CGameObject *pApacheModel2 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Apache.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);
	pApacheObject = new CApacheObject();
	pApacheObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pApacheObject->SetChild(pApacheModel2, true);
	pApacheObject->OnInitialize();
	pApacheObject->SetScale(1.0f, 1.0f, 1.0f);
	pApacheObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 1] = pApacheObject;

	nMeshesInHierarchy = 0;
	CGameObject *pSuperCobraModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/SuperCobra.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CSuperCobraObject* pSuperCobraObject = new CSuperCobraObject();
	pSuperCobraObject->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pSuperCobraObject->SetChild(pSuperCobraModel, true);
	pSuperCobraObject->OnInitialize();
	pSuperCobraObject->SetScale(1.0f, 1.0f, 1.0f);
	pSuperCobraObject->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 2] = pSuperCobraObject;

	nMeshesInHierarchy = 0;
	CGameObject *pMi24Model = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Mi24.bin", &nMeshesInHierarchy, pnMaterialsInHierarchy);

	CMi24Object* pMi24Object = new CMi24Object();
	pMi24Object->CreateShaderVariables(pd3dDevice, pd3dCommandList, nMeshesInHierarchy, pnMaterialsInHierarchy);
	pMi24Object->SetChild(pMi24Model, true);
	pMi24Object->OnInitialize();
	pMi24Object->SetScale(1.0f, 1.0f, 1.0f);
	pMi24Object->Rotate(0.0f, -90.0f, 0.0f);
	m_ppGameObjects[WORLD_OBJECT_START + 3] = pMi24Object;

	int pnTerrainMaterials[1] = { 1 };
	m_pTerrain = new CTerrainObject(pd3dDevice, pd3dCommandList, "HeightMap/Level_1_terrain.raw");
	m_pTerrain->CreateShaderVariables(pd3dDevice, pd3dCommandList, 1, pnTerrainMaterials);
	m_ppGameObjects[WORLD_OBJECT_START + 4] = m_pTerrain;

	InitializeLevel1Targets();

	m_nProjectiles = MAX_PROJECTILES;
	m_ppProjectiles = new CProjectileObject*[m_nProjectiles];
	int pnProjectileMaterials[1] = { 1 };
	for (int i = 0; i < m_nProjectiles; i++)
	{
		m_ppProjectiles[i] = new CProjectileObject(pd3dDevice, pd3dCommandList);
		m_ppProjectiles[i]->CreateShaderVariables(pd3dDevice, pd3dCommandList, 1, pnProjectileMaterials);
	}

	m_nHudBars = HUD_BAR_COUNT;
	m_ppHudBars = new CHudBarObject*[m_nHudBars];
	int pnHudMaterials[1] = { 1 };
	m_ppHudBars[HUD_PLAYER_BACKGROUND] = new CHudBarObject(pd3dDevice, pd3dCommandList, XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f), XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f), XMFLOAT4(0.08f, 0.08f, 0.08f, 1.0f));
	m_ppHudBars[HUD_PLAYER_GAUGE] = new CHudBarObject(pd3dDevice, pd3dCommandList, XMFLOAT4(0.35f, 1.0f, 0.35f, 1.0f), XMFLOAT4(0.20f, 1.0f, 0.25f, 1.0f), XMFLOAT4(0.0f, 0.75f, 0.05f, 1.0f));
	m_ppHudBars[HUD_ENEMY_BACKGROUND] = new CHudBarObject(pd3dDevice, pd3dCommandList, XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f), XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f), XMFLOAT4(0.08f, 0.08f, 0.08f, 1.0f));
	m_ppHudBars[HUD_ENEMY_GAUGE] = new CHudBarObject(pd3dDevice, pd3dCommandList, XMFLOAT4(1.0f, 0.25f, 0.08f, 1.0f), XMFLOAT4(1.0f, 0.28f, 0.06f, 1.0f), XMFLOAT4(0.85f, 0.08f, 0.02f, 1.0f));
	for (int i = 0; i < m_nHudBars; i++) if (m_ppHudBars[i]) m_ppHudBars[i]->CreateShaderVariables(pd3dDevice, pd3dCommandList, 1, pnHudMaterials);
	m_fProjectileFireCooldown = 0.0f;

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

	if (m_ppProjectiles)
	{
		for (int i = 0; i < m_nProjectiles; i++) if (m_ppProjectiles[i]) m_ppProjectiles[i]->Release();
		delete[] m_ppProjectiles;
		m_ppProjectiles = NULL;
		m_nProjectiles = 0;
	}

	if (m_ppHudBars)
	{
		for (int i = 0; i < m_nHudBars; i++) if (m_ppHudBars[i]) m_ppHudBars[i]->Release();
		delete[] m_ppHudBars;
		m_ppHudBars = NULL;
		m_nHudBars = 0;
	}

	if (m_pLevel1Targets)
	{
		delete[] m_pLevel1Targets;
		m_pLevel1Targets = NULL;
		m_nLevel1Targets = 0;
		m_nCurrentLevel1Wave = 1;
	}

	if (m_pLights) delete[] m_pLights;
	m_pTerrain = NULL;
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

	if ((m_nSceneMode < GAME_SCENE_TUTORIAL) || ((m_nSceneMode == GAME_SCENE_LEVEL1) && m_bLevel1Cleared))
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
	for (int i = 0; i < m_nProjectiles; i++) if (m_ppProjectiles[i]) m_ppProjectiles[i]->ReleaseShaderVariables();
	for (int i = 0; i < m_nHudBars; i++) if (m_ppHudBars[i]) m_ppHudBars[i]->ReleaseShaderVariables();
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nGameObjects; i++) if (m_ppGameObjects[i]) m_ppGameObjects[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nProjectiles; i++) if (m_ppProjectiles[i]) m_ppProjectiles[i]->ReleaseUploadBuffers();
	for (int i = 0; i < m_nHudBars; i++) if (m_ppHudBars[i]) m_ppHudBars[i]->ReleaseUploadBuffers();
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
	if ((m_nSceneMode == GAME_SCENE_LEVEL1) && (pKeysBuffer[VK_SPACE] & 0xF0)) FirePlayerProjectile();
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

	if (m_nSceneMode == GAME_SCENE_LEVEL1)
	{
		if (m_fProjectileFireCooldown > 0.0f) m_fProjectileFireCooldown -= fTimeElapsed;
		if (m_nLastHitLevel1TargetIndex >= 0) m_fLastHitTargetDisplayElapsedTime += fTimeElapsed;
		if (m_bLevel1Cleared)
		{
			m_fLevel1ClearElapsedTime += fTimeElapsed;
			UpdateLevel1ClearText();
		}
		for (int i = 0; i < m_nProjectiles; i++) if (m_ppProjectiles[i] && m_ppProjectiles[i]->IsActive()) m_ppProjectiles[i]->Animate(fTimeElapsed, NULL);
		UpdateLevel1Targets(fTimeElapsed);
		CheckProjectileTargetCollisions();
		AdvanceLevel1WaveIfNeeded();
	}

	ClampPlayerToTerrain();

	if (m_pLights && m_pPlayer)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}
}
void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);

	if ((m_nSceneMode < GAME_SCENE_TUTORIAL) || ((m_nSceneMode == GAME_SCENE_LEVEL1) && m_bLevel1Cleared))
	{
		pCamera->GenerateViewMatrix(XMFLOAT3(0.0f, 0.0f, -120.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f));
	}

	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pCamera->UpdateShaderVariables(pd3dCommandList);

	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights

	if ((m_nSceneMode == GAME_SCENE_LEVEL1) && m_bLevel1Cleared)
	{
		if (m_ppGameObjects[LEVEL1_CLEAR_OBJECT])
		{
			m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->UpdateTransform(NULL);
			m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->Render(pd3dCommandList, pCamera, m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->m_ppd3dcbInstancingGameObjects, m_ppGameObjects[LEVEL1_CLEAR_OBJECT]->m_ppcbMappedInstancingGameObjects);
		}
		return;
	}

	for (int i = 0; i < m_nGameObjects; i++)
	{
		if (m_ppGameObjects[i] && IsVisibleObject(i))
		{
			m_ppGameObjects[i]->UpdateTransform(NULL);
			m_ppGameObjects[i]->Render(pd3dCommandList, pCamera, m_ppGameObjects[i]->m_ppd3dcbInstancingGameObjects, m_ppGameObjects[i]->m_ppcbMappedInstancingGameObjects);
		}
	}

	if (m_nSceneMode == GAME_SCENE_LEVEL1)
	{
		for (int i = 0; i < m_nProjectiles; i++)
		{
			if (m_ppProjectiles[i] && m_ppProjectiles[i]->IsActive())
			{
				m_ppProjectiles[i]->Render(pd3dCommandList, pCamera, m_ppProjectiles[i]->m_ppd3dcbInstancingGameObjects, m_ppProjectiles[i]->m_ppcbMappedInstancingGameObjects);
			}
		}
	}
}