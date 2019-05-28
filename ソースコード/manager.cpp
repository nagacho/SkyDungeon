#include "manager.h"
#include "Scene.h"
#include "fade.h"
#include"light.h"



//=====================
// 実体化
//=====================
Scene* CManager::m_Scene = nullptr;
Fade* CManager::m_Fade = nullptr;

//=====================================================================================
// 関数名：Initialize
// 内容　：初期化
//=====================================================================================
void CManager::Initialize()
{
	// ライト初期化
	CLight::Initialize();

	// シーン初期化
	m_Scene->Initialize();

	// フェード
	m_Fade = Fade::Create();
}

//=====================================================================================
// 関数名：Finalizeer
// 内容　：終了
//=====================================================================================
void CManager::Finalize()
{
	// ライト解放
	CLight::Finalize();

	// シーン解放
	m_Scene->Finalize();
	delete m_Scene;

	// フェード終了
	m_Fade->Finalize();
	delete m_Fade;

	
}

//=====================================================================================
// 関数名：Update
// 内容　：更新
//=====================================================================================
void CManager::Update()
{
	// シーン更新
	m_Scene->Update();

	// フェード更新
	m_Fade->Update();

	

	// ゲームシーン切り替え
	//if (m_Num == 2)
	//{
	//	// フェード
	//	if (DgMapManager::GetFloor() == STAIR_MAX)
	//	{
	//		m_Fade->Begin(new SceneResult);
	//	}
	//}
	//
	//if (m_Fade->CheckType() == m_Fade->FADE_WAIT)
	//{
	//	// 更新ストップ
	//	if (!SceneGame::GetNoUpdate())
	//	{
	//		// 2Dポリゴン,3Dポリゴン,モデル,ビルボードの更新
	//		Object::UpdateAll();
	//	}
	//
	//	if (m_Num == 0)
	//	{
	//		// フェード
	//		if (CInput::GetKeyboardTrigger(DIK_RETURN))
	//		{
	//			m_Fade->Begin(new SceneTitle);
	//		}
	//	}
	//	else if (m_Num == 1)
	//	{
	//		// フェード
	//		if (CInput::GetKeyboardTrigger(DIK_RETURN))
	//		{
	//			m_Fade->Begin(new SceneGame);
	//		}
	//	}
	//}

}

//=====================================================================================
// 関数名：Draw
// 内容　：描画
//=====================================================================================
void CManager::Draw()
{

	// シーン描画
	m_Scene->Draw();

	// フェード描画
	m_Fade->Draw();

}
