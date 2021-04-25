#include "stdafx.h"
#include "Scene.h"

default_random_engine dre;
//uniform_int_distribution<float> uid(10.0f, 30.0f);
//uniform_int_distribution<float> speed(15.0f, 25.0f);

CScene::CScene()
{
}

CScene::~CScene()
{
}

inline float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	static int cnt;
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'A':
			for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++)
			{
				CExplosiveObject *pExplosiveObject = (CExplosiveObject *)(*iter);
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;
		case VK_CONTROL:
			if (cnt < 30)
			{
				m_pPlayer->m_fOldPosition = m_pPlayer->GetPosition().z;			// �߻��������� �÷��̾� ��ġ�� OldPosition������ ����.
				m_ppBullets.reserve(30);										// ����ũ�� �̸� �̻��� ��ü 30����ŭ �Ҵ�
				CCubeMesh *pBulletCubeMesh = new CCubeMesh(0.5f, 0.5f, 3.5f);
				pBulletCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.5f, 0.5f, 4.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
				CGameObject *Bullet = new CbulletObject();
				Bullet->SetMesh(pBulletCubeMesh);
				Bullet->SetColor(RGB(0, 0, 0));
				Bullet->SetPosition(m_pPlayer->GetPosition());					// �÷��̾� ��ġ���� ����
				Bullet->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
				Bullet->SetRotationSpeed(900.06f);
				((CbulletObject*)Bullet)->m_xmlook = m_pPlayer->m_xmf3Look;		// �̻��� ��ü�� �躤�Ϳ� �÷��̾��� �躤�� ����.
				Bullet->SetMovingDirection(((CbulletObject*)Bullet)->m_xmlook);	// �̻��� ���⿡ ���� �����߾��� �÷��̾��� �躤�� ����.
				Bullet->SetMovingSpeed(40.f);
				m_ppBullets.emplace_back(Bullet);
				++cnt;
				if (cnt == 29)
					cnt = 0;
			}
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 500.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);
	pWallCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 0, 0));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	auto p = m_pObjects.begin();

	if ((*p)) m_pObjects.clear();

	if (m_pWallsObject) delete m_pWallsObject;

	m_ppBullets.clear();
}

void CScene::CreateObjectPerTime(float fElapsedTime)
{
	m_fCreateTimes += fElapsedTime;   // ���� ��ü �����ϰ� �ٸ����� ��ü �����ϴ� �ð��� ��� ����
	m_fRCreateTimes += fElapsedTime;  // ���� ��ü �����ϴ� �ð��� ��� ����

	if (m_fCreateTimes >= 1.0f)		  // ��ü���� 1�ʰ� �Ѿ�����
	{
		CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);
		pObjectCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(2.0f, 2.0f, 2.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		CGameObject* pObject = new CCubeObject();
		pObject = new CExplosiveObject();
		pObject->SetMesh(pObjectCubeMesh);
		pObject->SetPosition(rand() % 90 - 45, rand() % 90 - 45, m_pPlayer->GetPosition().z + m_pPlayer->m_xmf3Look.z * 100.0f); // �÷��̾��� 100m�տ��� ����
		pObject->SetRotationAxis(XMFLOAT3(rand() % 1 + 1, rand() % 1 + 1, 0.0f));			// ȸ���� ����
		pObject->SetRotationSpeed(90.06f);
		pObject->SetMovingDirection(XMFLOAT3(RandF(0, 2), RandF(0, 2), RandF(0, 2)));
		pObject->SetMovingSpeed(RandF(10, 20));

		int Case = RandF(1, 4);							//������ �����ϱ� ���� ���尪 ����
		cout << Case << endl;
		if (Case == 1)
			pObject->SetColor(RGB(128, 0, 255));		// 1�϶� 
		else if (Case == 2)
			pObject->SetColor(RGB(0, 255, 0));			// 2�϶�
		else if (Case == 3)
			pObject->SetColor(RGB(200, 0, 128));		// 3�϶�


		if (pObject->GetPosition().z > 0)
			m_pObjects.emplace_back(pObject);
		else
		{
			pObject = nullptr;
			delete pObject;
		}
		m_fCreateTimes = 0.0f;
	}

	if (m_fRCreateTimes >= 10.0f)
	{
		cout << "������ ����" << endl;
		CCubeMesh *pObjectCubeMesh = new CCubeMesh(6.0f, 6.0f, 6.0f);
		pObjectCubeMesh->SetOOBB(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(3.0f, 3.0f, 3.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
		CGameObject* pObject = new CCubeObject();

		pObject = new CExplosiveObject();
		pObject->SetMesh(pObjectCubeMesh);
		pObject->SetPosition((RandF(-45, 45)), RandF(-45, 45), m_pPlayer->GetPosition().z + m_pPlayer->m_xmf3Look.z * 100.0f);
		pObject->SetRotationAxis(XMFLOAT3(rand() % 1 + 1, rand() % 1 + 1, 0.0f));
		pObject->SetRotationSpeed(90.06f);
		pObject->SetMovingDirection(XMFLOAT3(RandF(0, 2), RandF(0, 2), RandF(0, 2)));
		pObject->SetMovingSpeed(15.0f);
		pObject->SetColor(RGB(255, 0, 0));

		if (pObject->GetPosition().z > 0)
			m_pObjects.emplace_back(pObject);
		else
		{
			pObject = nullptr;
			delete pObject;
		}
		m_fRCreateTimes = 0;
	}

}

void CScene::CheckObjectByObjectCollisions()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++) (*iter)->m_pObjectCollided = NULL;
	for (auto iter1 = m_pObjects.begin(); iter1 != m_pObjects.end(); iter1++)
	{
		for (auto iter2 = iter1 + 1; iter2 != m_pObjects.end(); iter2++)
		{
			if ((*iter1)->m_xmOOBB.Intersects((*iter2)->m_xmOOBB))
			{
				(*iter1)->m_pObjectCollided = (*iter2);
				(*iter2)->m_pObjectCollided = (*iter1);
			}
		}
	}
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++)
	{
		if ((*iter)->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = (*iter)->m_xmf3MovingDirection;
			float fMovingSpeed = (*iter)->m_fMovingSpeed;
			(*iter)->m_xmf3MovingDirection = (*iter)->m_pObjectCollided->m_xmf3MovingDirection;
			(*iter)->m_fMovingSpeed = (*iter)->m_pObjectCollided->m_fMovingSpeed;
			(*iter)->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			(*iter)->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			(*iter)->m_pObjectCollided->m_pObjectCollided = NULL;
			(*iter)->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains((*iter)->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = (*iter)->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&(*iter)->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&(*iter)->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS:
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = (*iter)->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&(*iter)->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&(*iter)->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case CONTAINS:
			break;
		}
	}
}

void CScene::CheckObjectByBulletCollisions()
{
	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++) (*iter)->m_pObjectCollided = NULL; // ������Ʈ���� �浹���� �ʱ�ȭ
	for (auto iter1 = m_pObjects.begin(); iter1 != m_pObjects.end();) {    // ������Ʈ�� ���� ���Ϳ� �����ϴ� iterator
		for (auto iter2 = m_ppBullets.begin(); iter2 != m_ppBullets.end(); ) { // �Ѿ��� ���� ���Ϳ� �����ϴ� iterator
			if ((*iter1)->m_xmOOBB.Intersects((*iter2)->m_xmOOBB))  // �浹�� �Ͼ�ٸ�
			{
				delete (*iter2);									// 
				iter2 = m_ppBullets.erase(iter2);					// �Ѿ��� ����Ű�� iterator�� ����

				CExplosiveObject *pExplosiveObject = (CExplosiveObject *)(*iter1);		// �ͽ��÷��� ����
				pExplosiveObject->m_bBlowingUp = true;									// �ͽ��÷��� Ű��
			}
			else { // �浹�� �Ͼ�� �ʴ´ٸ� iterator�� ���� index�� ����ִ� �Ѿ��� ����Ű�� �Ѵ�.
				++iter2;
			}
		}
		++iter1; // �浹�� �Ͼ�� �ʴ´ٸ� iterator�� ���� index�� ����ִ� ������Ʈ�� ����Ű�� �Ѵ�.
	}
}

void CScene::CheckPlayerByObjectCollisions()
{
	m_pPlayer->Collision();

	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); ++iter)
	{
		if ((*iter)->m_xmOOBB.Contains(m_pPlayer->m_xmSphere) == INTERSECTS)
		{
			cout << "�÷��̾� ���� ���" << endl;
			m_bPlayerDie = true;
			break;
		}
	}
}

void CScene::Animate(float fElapsedTime)
{

	m_pWallsObject->Animate(fElapsedTime);

	for (auto iter1 = m_pObjects.begin(); iter1 != m_pObjects.end(); ++iter1)
	{
		if (m_pPlayer->m_xmf3Look.z > 0) // �÷��̾ ������ �� ��
		{
			if ((*iter1)->GetPosition().z < m_pPlayer->GetPosition().z)
			{
			//	cout << "�պ��� ������ �ڿ� ������" << endl;
				m_pPlayer->SetColor(RGB(255, 192, 203));
			}
			else
			{
			//	cout << "�պ��� ������ �ڿ� ������" << endl;
				m_pPlayer->SetColor(RGB(0, 0, 255));
			}
		}
		if (m_pPlayer->m_xmf3Look.z <= 0) // �÷��̾ �ڷ� �� ��
		{
			if ((*iter1)->GetPosition().z > m_pPlayer->GetPosition().z)
			{
			//	cout << " �ں��� ������ �ڿ� �� ����" << endl;
				m_pPlayer->SetColor(RGB(255, 192, 203));
				break;
			}
			else
			{
			//	cout << "�ں��� ���� �� �ڿ� �� ����" << endl;
				m_pPlayer->SetColor(RGB(0, 0, 255));
				continue;
			}
		}
	}

	//�浹��
	if (m_pWallsObject->m_xmOOBB.Contains(XMLoadFloat3(&m_pPlayer->m_xmf3Position)) == DISJOINT && m_pPlayer->GetPosition().z < 1000 || m_pPlayer->GetPosition().z > 1000 || m_pPlayer->GetPosition().z < 0)
	{
		m_pPlayer->m_bCollisioncheck = true; //�浹üũ�� �Һ��� true
		m_pPlayer->SetColor(RGB(255, 0, 0)); //���������� ǥ��
		m_pPlayer->Move(m_pPlayer->dwDirection, -4.00f);	// �ݴ�������� 4m��ŭ �ðܳ�����
	}
	else
	{
		if (m_pPlayer->m_bCollisioncheck)
		{
			m_fColorchangeTime += fElapsedTime;
			if (m_fColorchangeTime > 1.5)				    // �浹�ϰ� 1.5�ʰ� ������ 
			{
				cout << "���� �ٲ���" << endl;
				m_pPlayer->SetColor(RGB(0, 0, 255));		// ��� ����
				m_fColorchangeTime = 0;
				m_pPlayer->m_bCollisioncheck = false;
			}
		}
	}


	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end();) {  // ������Ʈ�� ����Ű�� iterator
		(*iter)->Animate(fElapsedTime);

		CExplosiveObject *pExplosiveObject = (CExplosiveObject *)(*iter);
		if (pExplosiveObject->m_bBlowEnd) {                           // ������Ʈ�� m_bBlowEnd�� ture�̸�
			delete (*iter);											 // ��ü�� ����Ű�� iterator ����
			iter = m_pObjects.erase(iter);						     // ��ü ����
		}
		else
			++iter;                                                  // �ƴѰ�� iter�� ����
	}

	for (auto p = m_ppBullets.begin(); p != m_ppBullets.end();)      // �Ѿ� ����Ű�� iterator 
	{
		((CGameObject*)(*p))->Animate(fElapsedTime);
		if ((*p)->GetPosition().z - 100.0f > m_pPlayer->m_fOldPosition) // �Ѿ� ��ȿ��Ÿ� 100m�� �Ǹ� ����
		{
			delete (*p);
			p = m_ppBullets.erase(p);
		}
		else
			++p;														// 100m�� ���������� iter�� ����
	}

	CheckObjectByWallCollisions();
	CheckObjectByBulletCollisions();
	CheckObjectByObjectCollisions();
	CheckPlayerByObjectCollisions();

	if (abs(m_pPlayer->GetPosition().z + 100.0f * m_pPlayer->m_xmf3Look.z) > 100.0f)
	{
		CreateObjectPerTime(fElapsedTime);
	}
}

void CScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);

	for (auto iter = m_pObjects.begin(); iter != m_pObjects.end(); iter++) (*iter)->Render(hDCFrameBuffer, pCamera);

	for (auto p = m_ppBullets.begin(); p != m_ppBullets.end(); ++p)
	{
		(*p)->Render(hDCFrameBuffer, pCamera);
	}
}
