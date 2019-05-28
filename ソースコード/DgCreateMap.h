//========================================================
//	ダンジョンマップ自動生成
//========================================================	 
//	作成日：2018/08/14(火)
//	作成者：Ryoya Nagata
//========================================================
#ifndef	_DGCREATEMAP_H_
#define	_DGCREATEMAP_H_
#include "main.h"
#include<list>

static const int AREA_NUM = 8;			// マップ分割数
static const int MAP_WIDTH = 50;		// マップ幅
static const int MAP_HEIGHT = 50;	    // マップ高さ


typedef enum 
{
	NOTYPE = -1,	// 何もない
	WALL,			// 壁
	STREET,			// 道
	ROOM,			// 部屋
	STAIRS,			// 階段
	MAPTYPE_MAX
}MAPTYPE;

//========================================
// エリアクラス
//========================================
class Area
{
private:

public:
	D3DXVECTOR2 start = D3DXVECTOR2(0.0f, 0.0f);
	D3DXVECTOR2 end = D3DXVECTOR2(0.0f, 0.0f);

	int GetWidth() { return (int)(end.x - start.x); }
	int GetHeight() { return (int)(end.y - start.y); }
	int GetAllSize() { return (int)(GetWidth() * GetHeight()); }
};

//===============================
// 関数オブジェクト
//===============================
typedef struct
{
	Area* MaximizeArea;
	int num;
}AREA_LIST;

class MaxArea
{
private:
	AREA_LIST Max_area;
	int counter;
	bool bOnce = false;

public:
	MaxArea()
	{
		Max_area.MaximizeArea = new Area();
		Max_area.num = -1;
		counter = -1;
	}
	void operator()(Area* target)
	{
		int a = target->GetAllSize() - Max_area.MaximizeArea->GetAllSize();
		counter++;
		if (a >= 0)
		{
			if (!bOnce)
			{
				delete Max_area.MaximizeArea;
				bOnce = true;
			}
			Max_area.MaximizeArea = target;
			Max_area.num = counter;
		}
	}
	AREA_LIST GetMaxArea() { return Max_area; }

};



//=========================================
// 分割クラス
//=========================================
class Root
{
public:
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;
};

//=========================================
// 部屋クラス
//=========================================
class Room
{
public:
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;

	int GetWidth() { return (int)(end.x - start.x); }
	int GetHeight() { return (int)(end.y - start.y); }
	int GetAllSize() { return (int)(GetWidth() * GetHeight()); }

	D3DXVECTOR2 ConnectPoint;	// 通路を繋げるときに使う座標 
	D3DXVECTOR2 CrossLoad[4];	// 部屋から通路を伸ばすときに使う交点
	bool GateExist[4];			// 部屋から見て方向に通路を作るかどうか
	int MaxGate = 0;			// 部屋から伸びている通路の最大数
	int GateNum = 0;			// 部屋から伸びている通路の数
};


//===============================
// マップ自動生成クラス
//===============================
class DgCreateMap
{
private:
	D3DXVECTOR2 m_Scale;
	static int m_Map[MAP_HEIGHT][MAP_WIDTH];		// 読み込み用
	std::list<Area*> areaList;
	std::list<Room*> roomList;
	std::list<Root*> rootList;

	static void RegistArea(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end);
	static void RegistRoot(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end);
	static void RegistRoom(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end);
	static void WriteRootToMap(D3DXVECTOR2 start, D3DXVECTOR2 end);
	static void WriteRoomToMap(D3DXVECTOR2 start, D3DXVECTOR2 end);
	static void WriteLoadToMap(D3DXVECTOR2 start, D3DXVECTOR2 end, int mode);
	static void DeleteLine(D3DXVECTOR2 start, D3DXVECTOR2 end);

public: 
	DgCreateMap() {}
	~DgCreateMap() {}

	void SetMap(int height, int width, int value) { m_Map[height][width] = value; }
	int GetMap(int height, int width) { return m_Map[height][width]; }
	static DgCreateMap* Create();
	void Release();

	
};



#endif


