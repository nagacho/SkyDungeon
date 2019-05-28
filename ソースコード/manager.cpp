#include "manager.h"
#include "Scene.h"
#include "fade.h"
#include"light.h"



//=====================
// ���̉�
//=====================
Scene* CManager::m_Scene = nullptr;
Fade* CManager::m_Fade = nullptr;

//=====================================================================================
// �֐����FInitialize
// ���e�@�F������
//=====================================================================================
void CManager::Initialize()
{
	// ���C�g������
	CLight::Initialize();

	// �V�[��������
	m_Scene->Initialize();

	// �t�F�[�h
	m_Fade = Fade::Create();
}

//=====================================================================================
// �֐����FFinalizeer
// ���e�@�F�I��
//=====================================================================================
void CManager::Finalize()
{
	// ���C�g���
	CLight::Finalize();

	// �V�[�����
	m_Scene->Finalize();
	delete m_Scene;

	// �t�F�[�h�I��
	m_Fade->Finalize();
	delete m_Fade;

	
}

//=====================================================================================
// �֐����FUpdate
// ���e�@�F�X�V
//=====================================================================================
void CManager::Update()
{
	// �V�[���X�V
	m_Scene->Update();

	// �t�F�[�h�X�V
	m_Fade->Update();

	

	// �Q�[���V�[���؂�ւ�
	//if (m_Num == 2)
	//{
	//	// �t�F�[�h
	//	if (DgMapManager::GetFloor() == STAIR_MAX)
	//	{
	//		m_Fade->Begin(new SceneResult);
	//	}
	//}
	//
	//if (m_Fade->CheckType() == m_Fade->FADE_WAIT)
	//{
	//	// �X�V�X�g�b�v
	//	if (!SceneGame::GetNoUpdate())
	//	{
	//		// 2D�|���S��,3D�|���S��,���f��,�r���{�[�h�̍X�V
	//		Object::UpdateAll();
	//	}
	//
	//	if (m_Num == 0)
	//	{
	//		// �t�F�[�h
	//		if (CInput::GetKeyboardTrigger(DIK_RETURN))
	//		{
	//			m_Fade->Begin(new SceneTitle);
	//		}
	//	}
	//	else if (m_Num == 1)
	//	{
	//		// �t�F�[�h
	//		if (CInput::GetKeyboardTrigger(DIK_RETURN))
	//		{
	//			m_Fade->Begin(new SceneGame);
	//		}
	//	}
	//}

}

//=====================================================================================
// �֐����FDraw
// ���e�@�F�`��
//=====================================================================================
void CManager::Draw()
{

	// �V�[���`��
	m_Scene->Draw();

	// �t�F�[�h�`��
	m_Fade->Draw();

}
