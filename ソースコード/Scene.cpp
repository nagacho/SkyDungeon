#include "Scene.h"
#include "SceneTitle.h"

// 実体化
ScenePattern* Scene::m_Scene_pattern = nullptr;

//=====================================================================================
// 関数名：Initialize
// 内容　：シーン初期化
//=====================================================================================
void Scene::Initialize()
{
	m_Scene_pattern = new SceneTitle;
	m_Scene_pattern->Initialize();
}

//=====================================================================================
// 関数名：Finalize
// 内容　：シーン終了
//=====================================================================================
void Scene::Finalize()
{
	m_Scene_pattern->Finalize();
	delete m_Scene_pattern;
}

//=====================================================================================
// 関数名：Update
// 内容　：シーン更新
//=====================================================================================
void Scene::Update()
{
	m_Scene_pattern->Update(this);
}

//=====================================================================================
// 関数名：Draw
// 内容　：シーン描画
//=====================================================================================
void Scene::Draw()
{
	m_Scene_pattern->Draw();
}

//=====================================================================================
// 関数名：ChangeScenePattern
// 内容　：シーンパターン変更
// 引数　：ScenePattern* pScenePattern
//=====================================================================================
void Scene::ChangeScenePattern(ScenePattern* pScenePattern)
{
	m_Scene_pattern->Finalize();
	delete m_Scene_pattern;

	m_Scene_pattern = pScenePattern;

	m_Scene_pattern->Initialize();
}
