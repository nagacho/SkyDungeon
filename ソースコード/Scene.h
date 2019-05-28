//========================================================
//	SCENE
//========================================================	 
//	作成日：2019/3/11(月)
//	作成者：Ryoya Nagata
//========================================================
#ifndef _SCENE_H_
#define	_SCENE_H_

// 前方宣言
class ScenePattern;

class Scene
{
private:
	static ScenePattern* m_Scene_pattern;
public:
	Scene(){}
	~Scene(){}
public:
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void ChangeScenePattern(ScenePattern* pScenePattern);		// 行動状態変更

};

#endif