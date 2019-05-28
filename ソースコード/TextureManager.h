//==========================================================
//	TextureManager.h
//----------------------------------------------------------
//
//
//	2018/4/24(火)						Ryoya Nagata
//==========================================================
#ifndef _TEXMANAGE_H_
#define _TEXMANAGE_H_
#include "main.h"
#include <map>
#include <vector>

typedef std::map < std::string, LPDIRECT3DTEXTURE9 > TexTable;		//mapの定義
typedef std::map < std::string, D3DXIMAGE_INFO > TexTableInfo;		//mapの定義

static const std::string TEX_PATH = "data\\texture\\";

//--------------------------
// フォント構造体
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


// クラス
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
	static void LoadTexture(std::string TexName);					// 読み込み
	static LPDIRECT3DTEXTURE9 GetTexture(std::string TexName);		// テクスチャ描画用
	static D3DXIMAGE_INFO GetPictureinfo(std::string TexName);		// テクスチャの情報

	static void LoadTextureFromCreateTexture(std::string TexName, TEX_INFO_DATA* CreateTextureData);
	static TEX_INFO_DATA* CreateTextureFromText(IDirect3DDevice9* dev, const char* text, const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color);

};



#endif