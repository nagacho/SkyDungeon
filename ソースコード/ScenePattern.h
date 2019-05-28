//========================================================
//	シーンのパターン
//========================================================	 
//	作成日：2019/02/11(月)
//	作成者：Ryoya Nagata
//========================================================
#ifndef _SCENEPATTERN_H_
#define _SCENEPATTERN_H_

// 前方宣言
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