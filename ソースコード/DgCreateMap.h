//========================================================
//	�_���W�����}�b�v��������
//========================================================	 
//	�쐬���F2018/08/14(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef	_DGCREATEMAP_H_
#define	_DGCREATEMAP_H_
#include "main.h"
#include<list>

static const int AREA_NUM = 8;			// �}�b�v������
static const int MAP_WIDTH = 50;		// �}�b�v��
static const int MAP_HEIGHT = 50;	    // �}�b�v����


typedef enum 
{
	NOTYPE = -1,	// �����Ȃ�
	WALL,			// ��
	STREET,			// ��
	ROOM,			// ����
	STAIRS,			// �K�i
	MAPTYPE_MAX
}MAPTYPE;

//========================================
// �G���A�N���X
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
// �֐��I�u�W�F�N�g
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
// �����N���X
//=========================================
class Root
{
public:
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;
};

//=========================================
// �����N���X
//=========================================
class Room
{
public:
	D3DXVECTOR2 start;
	D3DXVECTOR2 end;

	int GetWidth() { return (int)(end.x - start.x); }
	int GetHeight() { return (int)(end.y - start.y); }
	int GetAllSize() { return (int)(GetWidth() * GetHeight()); }

	D3DXVECTOR2 ConnectPoint;	// �ʘH���q����Ƃ��Ɏg�����W 
	D3DXVECTOR2 CrossLoad[4];	// ��������ʘH��L�΂��Ƃ��Ɏg����_
	bool GateExist[4];			// �������猩�ĕ����ɒʘH����邩�ǂ���
	int MaxGate = 0;			// ��������L�тĂ���ʘH�̍ő吔
	int GateNum = 0;			// ��������L�тĂ���ʘH�̐�
};


//===============================
// �}�b�v���������N���X
//===============================
class DgCreateMap
{
private:
	D3DXVECTOR2 m_Scale;
	static int m_Map[MAP_HEIGHT][MAP_WIDTH];		// �ǂݍ��ݗp
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


