//========================================================
//	�V�[���̃p�^�[��
//========================================================	 
//	�쐬���F2019/02/11(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef _SCENEPATTERN_H_
#define _SCENEPATTERN_H_

// �O���錾
class Scene;

class ScenePattern
{
public:
	virtual ~ScenePattern() {}
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update(Scene* pScene) = 0;
	virtual void Draw() = 0;
};

#endif