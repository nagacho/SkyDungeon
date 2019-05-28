#include "main.h"
#include "DirectX9.h"
#include "ImGUIManager.h"
#include "input.h"
#include "Debug.h"
#include "FPSCounter.h"

//=================================
// 実体化
//=================================
LPDIRECT3DDEVICE9 DirectX9::m_D3DDevice = nullptr;
HWND DirectX9::m_hWnd;

// フィードバックブラー用変数
LPDIRECT3DTEXTURE9 DirectX9::m_BlurTexture1;
LPDIRECT3DSURFACE9 DirectX9::m_BlurSurface1;
LPDIRECT3DTEXTURE9 DirectX9::m_BlurTexture2;
LPDIRECT3DSURFACE9 DirectX9::m_BlurSurface2;
LPDIRECT3DSURFACE9 DirectX9::m_BackBufferSurface; 
Object2D* DirectX9::m_Blur2D;


//==============================================================================
// 関数名：Initialize
// 内容　：DirectX9初期化
// 引数　：HINSTANCE hInstance
// 引数　：HWND hWnd
// 引数　：BOOL bWindow
//==============================================================================
bool DirectX9::Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{	
	HRESULT hr;
	m_hWnd = hWnd;

	// インターフェースの取得
	LPDIRECT3D9 d3d  =nullptr;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == nullptr) { EMSG("DirectX9　インターフェース作成失敗"); return false; }

	// ディスプレイモードを取得
	D3DDISPLAYMODE d3ddm;
	hr = d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(hr)) { EMSG("DirectX9 ディスプレイモードの取得失敗"); return false; }

	// バッファの作成(スワップチェーン)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// カラーRGBA32bit
	d3dpp.BackBufferCount			 = 1;							// 後ろ画面何枚いる？
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		//スワップエフェクト
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// 基本的にtrueにする　映像信号に同期してフリップ
	d3dpp.AutoDepthStencilFormat	 = /*D3DFMT_D16*/D3DFMT_D24S8;	//
	d3dpp.Windowed					 = bWindow;						// windowにするのかフルスクリーンか
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// 秒間 ONEだと一回終わったというメッセージがきたら再描画 今どれくらいで描画できるのか試したいときはIMMEDIATE
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;	//

	// デバイスの生成
	hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice);
	if (FAILED(hr)) { EMSG("DirectX9 デバイス作成失敗"); return false;}
	SAFE_RELEASE(d3d);

	//	レンダステートパラメータの設定

	//	アルファブレンディングの設定
	SetAlphaBlending();

	//	法線正規化の設定（行列に影響しなくなる）
	m_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// テクスチャブレンディング（デフォルト）
	SetTexRenderAlpha();

	// テクスチャラッピングモードの指定（テクスチャUVの範囲0.0～1.0を超えたときどのような処理をするか）
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// WRAPは反復の意味　// CLAMPは端を引き延ばす　// MIRROR
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);									
	
	//	テクスチャフィルターのセット
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);// 実際のサイズよりも小さくなった時にかかるフィルタリング (線形アンチエイリアシング)	   
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);// 実際のサイズよりも大きくなった時にかかるフィルタリング
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
				
	//	ゲーム初期化
	Debug::InitializeString();
	CInput::InitializeKeyboard(hInstance, hWnd);

	// ImGUI
	//ImGUIManager::Initializeialize(hWnd,  DirectX9::GetDevice());

	// フィードバックブラー
	//D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture1);
	//m_BlurTexture1->GetSurfaceLevel(0, &m_BlurSurface1);
	//
	//D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture2);
	//m_BlurTexture2->GetSurfaceLevel(0, &m_BlurSurface2);
	//
	//m_D3DDevice->GetRenderTarget(0, &m_BackBufferSurface);
	//
	//m_Blur2D = Object2D::Create(D3DXVECTOR3(-50.0f, -50.0f, 0.0f), "posion.png", 0.0f, 0.0f, 1024.0f, 1024.0f, true);
	
	return true;
}

//==============================================================================
// 関数名：Finalize
// 内容　：DirectX9終了
//==============================================================================
void DirectX9::Finalize(void)
{
	// フィードバックブラー
	//m_BackBufferSurface->Release();
	//m_BlurSurface1->Release();
	//m_BlurSurface2->Release();
	//m_BlurTexture1->Release();
	//m_BlurTexture2->Release();

	// ImGUI
	//ImGUIManager::Finalize();

	// ゲーム終了
	CInput::FinalizeKeyboard();
	Debug::FinalizeString();

	// デバイス解放
	SAFE_RELEASE(m_D3DDevice);

	
}

//==============================================================================
// 関数名：Update
// 内容　：DirectX9更新
//==============================================================================
void DirectX9::Update(void)
{
	CInput::UpdateKeyboard();	// キーボード更新
	//ImGUIManager::Update();
}

//=====================================================================================
// 関数名：ScreenBegin()
// 内容　：画面をカラーでクリアー
// 戻り値：bool 
//=====================================================================================
bool DirectX9::ScreenBegin()
{
	return (m_D3DDevice->BeginScene()) ? true : false;
}

//=====================================================================================
// 関数名：ScreenEnd()
// 内容　：バックバッファへの描画終了
//=====================================================================================
void DirectX9::ScreenEnd()
{
	m_D3DDevice->EndScene();
}

//=====================================================================================
// 関数名：ScreenClear()
// 内容　：画面をカラーでクリアー
//=====================================================================================
void DirectX9::ScreenClear()
{
	// カウント、範囲クリア（今回は全画面）、色のこと、何色でクリア、ZBUFFERのクリア値、ステンシルバッファーのクリアー
	m_D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
}

//=====================================================================================
// 関数名：ScreenSwap()
// 内容　：画面をカラーでクリアー
//=====================================================================================
void DirectX9::ScreenSwap()
{
	// バックバッファとフロントバッファを交代（フリップ）
	m_D3DDevice->Present(NULL, NULL, NULL, NULL);// present は送る　Drawの最後に呼ぶ
}

//=====================================================================================
// 関数名：BlurDraw()
// 内容　：フィードバックブラーの実装
// 引数　：無し
// 戻り値：void 
//=====================================================================================
void DirectX9::BlurDraw()
{
	////描画ターゲットの変更
	//m_D3DDevice->SetRenderTarget(0, m_BlurSurface1);
	//
	//// 画面クリア
	//ScreenClear();
	//
	//// 描画開始（）
	//if (SUCCEEDED(ScreenBegin()))
	//{
	//	// 全ての描画
	//	CManager::Draw();
	//
	//	// ここに２Dポリゴン描画
	//	m_Blur2D->SetColor(D3DCOLOR_RGBA(255, 255, 255, 240));
	//	m_Blur2D->Draw2(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1366.0f + 0.0f, 768.0f + 0.0f), m_BlurTexture2);
	//
	//	m_D3DDevice->EndScene();	// バックバッファへの描画終了
	//
	//	// ターゲットの変更
	//	m_D3DDevice->SetRenderTarget(0, m_BackBufferSurface);
	//
	//	// 画面クリア
	//	ScreenClear();
	//
	//	// 描画開始（）
	//	if (SUCCEEDED(ScreenBegin()))
	//	{
	//		m_Blur2D->SetScale(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	//		m_Blur2D->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//	
	//		// 2Dポリゴン描画		// 全画面、α２５５
	//		m_Blur2D->Draw2(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1366.0f, 768.0f), m_BlurTexture1);
	//	
	//		m_D3DDevice->EndScene();
	//	}
	//}
	//
	//// 入れ替え
	//LPDIRECT3DTEXTURE9 texture;
	//texture = m_BlurTexture1;
	//m_BlurTexture1 = m_BlurTexture2;
	//m_BlurTexture2 = texture;
	//
	//LPDIRECT3DSURFACE9 surface;
	//surface = m_BlurSurface1;
	//m_BlurSurface1 = m_BlurSurface2;
	//m_BlurSurface2 = surface;
}

//================================================
// アルファブレンディングの設定
//================================================
void DirectX9::SetAlphaBlending()
{
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// 第1引数アルファのタイプ    第2引数 使うかどうかの判断
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// α
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// α
}

//================================================
// テクスチャレンダリングの設定（デフォルト）
//================================================
void DirectX9::SetTexRenderDefault()
{
	// 色のブレンド　　　　出力 = テクスチャの色　×　ポリゴンの色　/ 255       // ポリゴンカラーが白ならテクスチャの色をそのまま使用 
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// テクスチャの色
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// 掛け算して
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// ポリゴンの色

	// 透明度のブレンド
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// テクスチャの透明度
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// 上を使用
}

//================================================
// テクスチャレンダリングの設定（透明度ブレンド有）(文字ビットにも使える)
//================================================
void DirectX9::SetTexRenderAlpha()
{
	// 色のブレンド　　　　出力 = テクスチャの色　×　ポリゴンの色　/ 255       // ポリゴンカラーが白ならテクスチャの色をそのまま使用 
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// テクスチャの色
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// 掛け算して
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// ポリゴンの色

	// 透明度のブレンド
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// テクスチャの透明度
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// 上を使用
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// ポリゴンの透明度
}

//================================================
// テクスチャレンダリングの設定（シールのように）
//================================================
void DirectX9::SetTexRenderSeal()
{
	// ③ポリゴンの上に貼る方法　　　出力 = テクスチャの色×α　＋　ポリゴンの色×(１－α)
	// 色のブレンド
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// テクスチャの色
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);	// アルファブレンディング
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// ポリゴンの色
																					// 透明度のブレンド
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);	// 上を使用
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// テクスチャの透明度
}

//================================================
// テクスチャレンダリングの設定（スポットライト）
//================================================
//void SetTexRenderSpotLight()
//{
//	// 壁にスポットライト２ｄバージョン(途中)
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// テクスチャの色
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);	
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// テクスチャの透明度
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// テクスチャの色
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,D3DTA_CURRENT );		// テクスチャの透明度
//
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);		// テクスチャの透明度
//}




