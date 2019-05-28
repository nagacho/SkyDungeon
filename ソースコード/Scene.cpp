#include "Scene.h"
#include "SceneTitle.h"

// ���̉�
ScenePattern* Scene::m_Scene_pattern = nullptr;

//=====================================================================================
// �֐����FInitialize
// ���e�@�F�V�[��������
//=====================================================================================
void Scene::Initialize()
{
	m_Scene_pattern = new SceneTitle;
	m_Scene_pattern->Initialize();
}

//=====================================================================================
// �֐����FFinalize
// ���e�@�F�V�[���I��
//=====================================================================================
void Scene::Finalize()
{
	m_Scene_pattern->Finalize();
	delete m_Scene_pattern;
}

//=====================================================================================
// �֐����FUpdate
// ���e�@�F�V�[���X�V
//=====================================================================================
void Scene::Update()
{
	m_Scene_pattern->Update(this);
}

//=====================================================================================
// �֐����FDraw
// ���e�@�F�V�[���`��
//=====================================================================================
void Scene::Draw()
{
	m_Scene_pattern->Draw();
}

//=====================================================================================
// �֐����FChangeScenePattern
// ���e�@�F�V�[���p�^�[���ύX
// �����@�FScenePattern* pScenePattern
//=====================================================================================
void Scene::ChangeScenePattern(ScenePattern* pScenePattern)
{
	m_Scene_pattern->Finalize();
	delete m_Scene_pattern;

	m_Scene_pattern = pScenePattern;

	m_Scene_pattern->Initialize();
}
