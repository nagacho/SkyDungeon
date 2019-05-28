#include "main.h"
#include "DirectX9.h"
#include "manager.h"
#include "debug.h"
#include "FPSCounter.h"


// メモリリーク検知用---------------------------------------------------
#include"crtdbg.h"
#define malloc(x) _malloc_dbg(x, _NORMAL_BLOCK, __FILE__ ,__LINE__)
#define new	::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//----------------------------------------------------------------------

//=======================================
// プロトタイプ宣言
//=======================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	//戻り値 window特有言語 クラスで作成した名前( , どんなメッセージか, そのmsgの付随の情報)

bool/*HRESULT*/ Initialize( HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// ウィンドウモードかフルスクリーンか
void Finalize(void);	// 終了処理
void Update(void);	// 更新処理
void Draw(void);	// 描画処理

//=======================================
// グローバル変数
//=======================================
bool g_hWindow;
FPSCounter* g_fpsCounter;

//==============================================================================
// 関数名：APIENTRY WinMain
// 内容　：ウインドウ画面作成
// 引数　：HINSTANCE hInstance
// 引数　：HINSTANCE hPrevInstance
// 引数　：LPSTR lpCmdLime
// 引数　：int nShowCmd
// 戻り値：int
//==============================================================================
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLime, int nShowCmd )
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLime);

	// メモリリーク調査
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF);
	// CRTメモリリーク箇所検出
	//_CrtSetBreakAlloc(2615);

	//構造体その① 構造体変数の初期化で入れる *特徴* 打ち込みの量が少ない
	WNDCLASSEX wcex = 
	{
		sizeof ( WNDCLASSEX ),		// 構造体のサイズ,構造体のサイズに第一クラスを持ってくることが多い
		CS_VREDRAW | CS_HREDRAW,	// CS_VREDRAW == 垂直、CS_REDRAW == 水平。windowのサイズ変化に対応して再描画をする
		WndProc,					// windowプロシージャ 関数のポインタ型。関数名それだけがアドレスになる。ウィンドウ上での発生したイベントの処理
		0,							// 使わない クラス構造体の追加領域 バイト単位で予約
		0,							// 使わない ウィンドウ構造体の追加領域 バイト単位で予約
		hInstance,					// 重要！！ WinMainから 整理券のようなもの インスタンスのハンドル
		NULL,						// アプリケーションのアイコン 
		LoadCursor( NULL, IDC_ARROW ),	// カーソルの変更
		( HBRUSH )( COLOR_WINDOW+1 ),	// クライアント領域のデフォルトの背景色の設定
		NULL,						// クラスメニューハンドル
		CLASS_NAME,					// パーツ名 クラス名 実はなんでもよい
		NULL							// ミニアイコンの設定	
	};
	
	RegisterClassEx( &wcex );			// windowクラスの登録

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// クライアント領域のサイズ設定
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	int w_width, w_height = 0;							
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	
	RECT dr;											
	DWORD bStyle = (WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX) ^ WS_THICKFRAME;

	AdjustWindowRect( &wr, bStyle, false );	// クライアントサイズを求める関数
	w_width  = (wr.right - wr.left);		// クライアントサイズの幅
	w_height = ( wr.bottom - wr.top);		// クライアントサイズの高さ
	GetWindowRect( GetDesktopWindow(), &dr);

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// ウィンドウ生成
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	HWND hWnd = CreateWindowEx( 0,CLASS_NAME, WINDOW_NAME, /*WS_POPUP*/bStyle, CW_USEDEFAULT, CW_USEDEFAULT, w_width, w_height, NULL ,NULL, hInstance, NULL );
	ShowWindow( hWnd, nShowCmd );		// 表示
	UpdateWindow( hWnd );				// 更新

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// ゲームループの作成 	※ShouWindowの後！！！！
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	DWORD dwCurrentTime = 0;				// 現在の時間
	MSG msg;

	if( !Initialize( hInstance, hWnd, true ) )	// ゲーム初期化						
	{
		EMSG("DirectX9の初期化に失敗しました");
		return -1;
	}

	timeBeginPeriod(1);		// 分解能を設定

	do{
		// メッセージ待機
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))		
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{

			dwCurrentTime = timeGetTime();		// 1ミリ秒単位で取得
			Update();	// 更新処理
			Draw();		// 描画処理
		}

	}while( msg.message != WM_QUIT);

	Finalize();			// ゲーム終了
	timeEndPeriod(1);	// 分解能を戻す
	return (int)msg.wParam;

}

//==============================================================================
// 関数名：CALLBACK WndProc
// 内容　：メッセージ管理
// 引数　：HWND hWnd
// 引数　：UINT uMsg
// 引数　：WPARAM wParam
// 引数　：LPARAM lParam
// 戻り値：int
//==============================================================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	// 
	/*if (ImGUIManager::WndProc(hWnd, uMsg, wParam, lParam)) {
		return true;
	}*/

	int nID;

	switch ( uMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );		//WM_QUITメッセージの送信
			break;

		case WM_KEYDOWN:				// キーが押されたら
			switch(wParam)
			{
				case VK_ESCAPE:			//エスケープボタン
					nID = MessageBox( hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
					if(nID == IDYES)
					{
						DestroyWindow(hWnd);
						break;
					}
					break;
			}

		default:
		break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam ); //重要！！ 基本はこいつが処理してくれる 特別な動きはcaseでreturnを使いこいつを通さないようにする

}

//==============================================================================
// 関数名：Initialize
// 内容　：main初期化
// 引数　：HINSTANCE hInstance	// 
// 引数　：HWND hWnd			// 親ハンドル
// 引数　：BOOL bWindow			// ウィンドウモードかフルスクリーンか
//==============================================================================
bool Initialize( HINSTANCE hInstance, HWND hWnd, BOOL bWindow)	
{
	// FPS用カウンター
	g_fpsCounter = FPSCounter::Create(8);

	if (DirectX9::Initialize(hInstance, hWnd, bWindow))
	{
		CManager::Initialize();
		return true;
	}
	else
	{
		return false;
	}
}

//==============================================================================
// 関数名：Finalize
// 内容　：main終了
//==============================================================================
void Finalize(void)	
{
	if (g_fpsCounter != nullptr)
	{
		delete g_fpsCounter;
		g_fpsCounter = nullptr;
	}
	CManager::Finalize();
	DirectX9::Finalize();
}

//==============================================================================
// 関数名：Update
// 内容　：main終了
//==============================================================================
void Update(void)	
{
	DirectX9::Update();
	CManager::Update();
}

//==============================================================================
// 関数名：Draw
// 内容　：main終了
//==============================================================================
void Draw(void)	
{
	DirectX9::ScreenClear();

	if (SUCCEEDED(DirectX9::ScreenBegin()))
	{
		CManager::Draw();
		DirectX9::ScreenEnd();
		//ImGUIManager::Render();
		FPSCounter::UpdateFPS();
		Debug::String2(10, 10, "FPS： %f ", FPSCounter::GetFPS());
	}

	DirectX9::ScreenSwap();
	
}



