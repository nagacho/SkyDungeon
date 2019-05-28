//========================================================
//	�Q�[���}�l�[�W���[
//========================================================	 
//	�쐬���F2018/05/08(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef	_MANAGER_H_
#define	_MANAGER_H_
#include "main.h"

// �萔
static const int STAIR_MAX = 5;

// �O���錾
class Scene;
class Fade;

// �N���X
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


