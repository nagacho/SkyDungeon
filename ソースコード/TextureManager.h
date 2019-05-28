//==========================================================
//	TextureManager.h
//----------------------------------------------------------
//
//
//	2018/4/24(��)						Ryoya Nagata
//==========================================================
#ifndef _TEXMANAGE_H_
#define _TEXMANAGE_H_
#include "main.h"
#include <map>
#include <vector>

typedef std::map < std::string, LPDIRECT3DTEXTURE9 > TexTable;		//map�̒�`
typedef std::map < std::string, D3DXIMAGE_INFO > TexTableInfo;		//map�̒�`

static const std::string TEX_PATH = "data\\texture\\";

//--------------------------
// �t�H���g�\����
//--------------------------
typedef struct 
{
	BYTE *mono;
	int width;
	int height;
	int offsetX;
	int offsetY;
}FONT;

typedef struct
{
	IDirect3DTexture9* pTex;
	D3DXIMAGE_INFO TexInfo;
}TEX_INFO_DATA;


// �N���X
class CTexture
{
private:
	static std::map<std::string, LPDIRECT3DTEXTURE9> m_MapTexture;
	static std::map<std::string, D3DXIMAGE_INFO> m_MapTextureInfo;
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static D3DXIMAGE_INFO m_PicInfo;
public:
	CTexture(){}
	~CTexture(){}
	static void Release(std::string TexName);
	static void ReleaseAll();
	static void LoadTexture(std::string TexName);					// �ǂݍ���
	static LPDIRECT3DTEXTURE9 GetTexture(std::string TexName);		// �e�N�X�`���`��p
	static D3DXIMAGE_INFO GetPictureinfo(std::string TexName);		// �e�N�X�`���̏��

	static void LoadTextureFromCreateTexture(std::string TexName, TEX_INFO_DATA* CreateTextureData);
	static TEX_INFO_DATA* CreateTextureFromText(IDirect3DDevice9* dev, const char* text, const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color);

};



#endif