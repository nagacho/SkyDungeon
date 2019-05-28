#include "main.h"
#include "DirectX9.h"
#include "manager.h"
#include "debug.h"
#include "FPSCounter.h"


// ���������[�N���m�p---------------------------------------------------
#include"crtdbg.h"
#define malloc(x) _malloc_dbg(x, _NORMAL_BLOCK, __FILE__ ,__LINE__)
#define new	::new(_NORMAL_BLOCK, __FILE__, __LINE__)
//----------------------------------------------------------------------

//=======================================
// �v���g�^�C�v�錾
//=======================================
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );	//�߂�l window���L���� �N���X�ō쐬�������O( , �ǂ�ȃ��b�Z�[�W��, ����msg�̕t���̏��)

bool/*HRESULT*/ Initialize( HINSTANCE hInstance, HWND hWnd, BOOL bWindow);	// �E�B���h�E���[�h���t���X�N���[����
void Finalize(void);	// �I������
void Update(void);	// �X�V����
void Draw(void);	// �`�揈��

//=======================================
// �O���[�o���ϐ�
//=======================================
bool g_hWindow;
FPSCounter* g_fpsCounter;

//==============================================================================
// �֐����FAPIENTRY WinMain
// ���e�@�F�E�C���h�E��ʍ쐬
// �����@�FHINSTANCE hInstance
// �����@�FHINSTANCE hPrevInstance
// �����@�FLPSTR lpCmdLime
// �����@�Fint nShowCmd
// �߂�l�Fint
//==============================================================================
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLime, int nShowCmd )
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLime);

	// ���������[�N����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF);
	// CRT���������[�N�ӏ����o
	//_CrtSetBreakAlloc(2615);

	//�\���̂��̇@ �\���̕ϐ��̏������œ���� *����* �ł����݂̗ʂ����Ȃ�
	WNDCLASSEX wcex = 
	{
		sizeof ( WNDCLASSEX ),		// �\���̂̃T�C�Y,�\���̂̃T�C�Y�ɑ��N���X�������Ă��邱�Ƃ�����
		CS_VREDRAW | CS_HREDRAW,	// CS_VREDRAW == �����ACS_REDRAW == �����Bwindow�̃T�C�Y�ω��ɑΉ����čĕ`�������
		WndProc,					// window�v���V�[�W�� �֐��̃|�C���^�^�B�֐������ꂾ�����A�h���X�ɂȂ�B�E�B���h�E��ł̔��������C�x���g�̏���
		0,							// �g��Ȃ� �N���X�\���̂̒ǉ��̈� �o�C�g�P�ʂŗ\��
		0,							// �g��Ȃ� �E�B���h�E�\���̂̒ǉ��̈� �o�C�g�P�ʂŗ\��
		hInstance,					// �d�v�I�I WinMain���� �������̂悤�Ȃ��� �C���X�^���X�̃n���h��
		NULL,						// �A�v���P�[�V�����̃A�C�R�� 
		LoadCursor( NULL, IDC_ARROW ),	// �J�[�\���̕ύX
		( HBRUSH )( COLOR_WINDOW+1 ),	// �N���C�A���g�̈�̃f�t�H���g�̔w�i�F�̐ݒ�
		NULL,						// �N���X���j���[�n���h��
		CLASS_NAME,					// �p�[�c�� �N���X�� ���͂Ȃ�ł��悢
		NULL							// �~�j�A�C�R���̐ݒ�	
	};
	
	RegisterClassEx( &wcex );			// window�N���X�̓o�^

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// �N���C�A���g�̈�̃T�C�Y�ݒ�
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	int w_width, w_height = 0;							
	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	
	RECT dr;											
	DWORD bStyle = (WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX) ^ WS_THICKFRAME;

	AdjustWindowRect( &wr, bStyle, false );	// �N���C�A���g�T�C�Y�����߂�֐�
	w_width  = (wr.right - wr.left);		// �N���C�A���g�T�C�Y�̕�
	w_height = ( wr.bottom - wr.top);		// �N���C�A���g�T�C�Y�̍���
	GetWindowRect( GetDesktopWindow(), &dr);

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// �E�B���h�E����
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	HWND hWnd = CreateWindowEx( 0,CLASS_NAME, WINDOW_NAME, /*WS_POPUP*/bStyle, CW_USEDEFAULT, CW_USEDEFAULT, w_width, w_height, NULL ,NULL, hInstance, NULL );
	ShowWindow( hWnd, nShowCmd );		// �\��
	UpdateWindow( hWnd );				// �X�V

	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	// �Q�[�����[�v�̍쐬 	��ShouWindow�̌�I�I�I�I
	//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
	DWORD dwCurrentTime = 0;				// ���݂̎���
	MSG msg;

	if( !Initialize( hInstance, hWnd, true ) )	// �Q�[��������						
	{
		EMSG("DirectX9�̏������Ɏ��s���܂���");
		return -1;
	}

	timeBeginPeriod(1);		// ����\��ݒ�

	do{
		// ���b�Z�[�W�ҋ@
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE))		
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{

			dwCurrentTime = timeGetTime();		// 1�~���b�P�ʂŎ擾
			Update();	// �X�V����
			Draw();		// �`�揈��
		}

	}while( msg.message != WM_QUIT);

	Finalize();			// �Q�[���I��
	timeEndPeriod(1);	// ����\��߂�
	return (int)msg.wParam;

}

//==============================================================================
// �֐����FCALLBACK WndProc
// ���e�@�F���b�Z�[�W�Ǘ�
// �����@�FHWND hWnd
// �����@�FUINT uMsg
// �����@�FWPARAM wParam
// �����@�FLPARAM lParam
// �߂�l�Fint
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
			PostQuitMessage( 0 );		//WM_QUIT���b�Z�[�W�̑��M
			break;

		case WM_KEYDOWN:				// �L�[�������ꂽ��
			switch(wParam)
			{
				case VK_ESCAPE:			//�G�X�P�[�v�{�^��
					nID = MessageBox( hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
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

	return DefWindowProc( hWnd, uMsg, wParam, lParam ); //�d�v�I�I ��{�͂������������Ă���� ���ʂȓ�����case��return���g��������ʂ��Ȃ��悤�ɂ���

}

//==============================================================================
// �֐����FInitialize
// ���e�@�Fmain������
// �����@�FHINSTANCE hInstance	// 
// �����@�FHWND hWnd			// �e�n���h��
// �����@�FBOOL bWindow			// �E�B���h�E���[�h���t���X�N���[����
//==============================================================================
bool Initialize( HINSTANCE hInstance, HWND hWnd, BOOL bWindow)	
{
	// FPS�p�J�E���^�[
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
// �֐����FFinalize
// ���e�@�Fmain�I��
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
// �֐����FUpdate
// ���e�@�Fmain�I��
//==============================================================================
void Update(void)	
{
	DirectX9::Update();
	CManager::Update();
}

//==============================================================================
// �֐����FDraw
// ���e�@�Fmain�I��
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
		Debug::String2(10, 10, "FPS�F %f ", FPSCounter::GetFPS());
	}

	DirectX9::ScreenSwap();
	
}



