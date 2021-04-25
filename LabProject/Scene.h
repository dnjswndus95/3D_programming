#pragma once

#include "GameObject.h"
#include "Player.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

	CPlayer						*m_pPlayer = NULL;

	int							m_nObjects = 0;
	//CGameObject				**m_ppObjects = NULL;
	vector<CGameObject*>		m_pObjects;
	vector<CGameObject*>		m_ppBullets;
	CWallsObject				*m_pWallsObject = NULL;

	XMFLOAT4X4					m_xmf4x4World;

	float						m_fBackColorChangeTime = 0.0f;
	float						m_fColorchangeTime = 0.0f;
	float						m_fCreateTimes = 0;
	float						m_fRCreateTimes = 0;
	bool						m_bPlayerDie = false;

	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void CheckObjectByObjectCollisions();
	virtual void CheckObjectByWallCollisions();
	virtual void CheckObjectByBulletCollisions();
	virtual void CheckPlayerByObjectCollisions();

	virtual void CreateObjectPerTime(float fElapsedTime);

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera *pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

