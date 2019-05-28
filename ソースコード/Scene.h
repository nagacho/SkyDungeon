//========================================================
//	SCENE
//========================================================	 
//	�쐬���F2019/3/11(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef _SCENE_H_
#define	_SCENE_H_

// �O���錾
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

	void ChangeScenePattern(ScenePattern* pScenePattern);		// �s����ԕύX

};

#endif