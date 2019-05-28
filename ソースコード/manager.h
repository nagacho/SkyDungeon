//========================================================
//	ゲームマネージャー
//========================================================	 
//	作成日：2018/05/08(火)
//	作成者：Ryoya Nagata
//========================================================
#ifndef	_MANAGER_H_
#define	_MANAGER_H_
#include "main.h"

// 定数
static const int STAIR_MAX = 5;

// 前方宣言
class Scene;
class Fade;

// クラス
class CManager
{
private:
	static Scene* m_Scene;
	static Fade* m_Fade;
public:
	CManager(){}
	~CManager(){}

	static void Initialize();
	static void Finalize();
	static void Update();
	static void Draw();
};

#endif


