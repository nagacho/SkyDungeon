//========================================================
//	DirectX9
//========================================================	 
//	作成日：2018/4/24(火)
//	作成者：Ryoya Nagata
//========================================================
#ifndef _DIRECTX9_H_
#define _DIRECTX9_H_
#include <d3d9.h>
#include <d3dx9.h>

// 前方宣言
class Object2D;

// クラス
class DirectX9
{
private:
	static LPDIRECT3DDEVICE9 m_D3DDevice;
	static HWND m_hWnd;

	// フィードバックブラー
	static LPDIRECT3DTEXTURE9 m_BlurTexture1;
	static LPDIRECT3DSURFACE9 m_BlurSurface1;
	static LPDIRECT3DTEXTURE9 m_BlurTexture2;
	static LPDIRECT3DSURFACE9 m_BlurSurface2;
	static LPDIRECT3DSURFACE9 m_BackBufferSurface;
	static Object2D* m_Blur2D;

public:
	DirectX9() { ; }
	~DirectX9() { ; }

	static bool Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Finalize(void);
	static void Update(void);

	static void ScreenClear();
	static bool ScreenBegin();
	static void ScreenEnd();
	static void ScreenSwap();

	// フィードバックブラー
	static void BlurDraw();
	//static LPDIRECT3DTEXTURE9* GetTexture1() { return &m_BlurTexture1; }
	//static LPDIRECT3DTEXTURE9* GetTexture2() { return &m_BlurTexture2; }
	//static LPDIRECT3DSURFACE9* GetSurface1() { return &m_BackBufferSurface; }

	// カメラ
	static void SetView(D3DXMATRIX view){ m_D3DDevice->SetTransform(D3DTS_VIEW, &view); }
	static void SetProjection(D3DXMATRIX projection){ m_D3DDevice->SetTransform(D3DTS_PROJECTION, &projection); }

	// ライト
	static void SetLight(D3DLIGHT9 light){ m_D3DDevice->SetLight(0, &light); }

	// マテリアル
	static void SetMaterial(D3DMATERIAL9 material){ m_D3DDevice->SetMaterial(&material); }

	// アルファブレンディング
	static void SetAlphaBlending();

	// テクスチャレンダリング関連
	static void SetTexRenderDefault();
	static void SetTexRenderAlpha();
	static void SetTexRenderSeal();

	//デバイスの取得
	static LPDIRECT3DDEVICE9 GetDevice(void) { return m_D3DDevice; }

	// ハンドルの取得
	static HWND GetHandle(void) { return m_hWnd; }

};


#endif