//========================================================
//	メイン
//========================================================	 
//	作成日：2018/4/13(金)
//	作成者：Ryoya Nagata
//========================================================
#ifndef	_MAIN_H_
#define	_MAIN_H_
#include <d3d9.h>
#include <d3dx9.h>

//======================
// マクロ定義
//======================
#define WIN32_LEAN_AND_MEAN													// ヘッダーからあまり使われない関数を省く
#define EMSG(x)	MessageBox (NULL, x, "エラー",MB_OK);						// メッセージエラー
#define SAFE_RELEASE(x)	if(x != nullptr){ (x)->Release(); (x) = nullptr; }	// リリース
#define SAFE_DELETE(x) if(x != nullptr){ delete x; (x) = nullptr; }			// 安全にdelete
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// FVFの宣言　RHWは外す！　D3DFVF_XYZRHW 法線追加時D3DFVF_NORMAL

#define CLASS_NAME	"sample"
#define WINDOW_NAME	"ウィンドウの表示"

//======================
// テンプレート関数
//======================
template<class T>
void SafeDelete(T* ptr)
{
	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}


//=======================
// 定数
//=======================
static const int SCREEN_WIDTH = 1366;				// 画面の幅
static const int SCREEN_HEIGHT = 768;				// 画面の高さ


//====================================
// 頂点構造体
//====================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nvec;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}VERTEX_3D;


#endif