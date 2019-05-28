#include "main.h"
#include "DirectX9.h"
#include "ImGUIManager.h"
#include "input.h"
#include "Debug.h"
#include "FPSCounter.h"

//=================================
// ���̉�
//=================================
LPDIRECT3DDEVICE9 DirectX9::m_D3DDevice = nullptr;
HWND DirectX9::m_hWnd;

// �t�B�[�h�o�b�N�u���[�p�ϐ�
LPDIRECT3DTEXTURE9 DirectX9::m_BlurTexture1;
LPDIRECT3DSURFACE9 DirectX9::m_BlurSurface1;
LPDIRECT3DTEXTURE9 DirectX9::m_BlurTexture2;
LPDIRECT3DSURFACE9 DirectX9::m_BlurSurface2;
LPDIRECT3DSURFACE9 DirectX9::m_BackBufferSurface; 
Object2D* DirectX9::m_Blur2D;


//==============================================================================
// �֐����FInitialize
// ���e�@�FDirectX9������
// �����@�FHINSTANCE hInstance
// �����@�FHWND hWnd
// �����@�FBOOL bWindow
//==============================================================================
bool DirectX9::Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{	
	HRESULT hr;
	m_hWnd = hWnd;

	// �C���^�[�t�F�[�X�̎擾
	LPDIRECT3D9 d3d  =nullptr;
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == nullptr) { EMSG("DirectX9�@�C���^�[�t�F�[�X�쐬���s"); return false; }

	// �f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE d3ddm;
	hr = d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(hr)) { EMSG("DirectX9 �f�B�X�v���C���[�h�̎擾���s"); return false; }

	// �o�b�t�@�̍쐬(�X���b�v�`�F�[��)
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// �J���[RGBA32bit
	d3dpp.BackBufferCount			 = 1;							// ����ʉ�������H
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		//�X���b�v�G�t�F�N�g
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// ��{�I��true�ɂ���@�f���M���ɓ������ăt���b�v
	d3dpp.AutoDepthStencilFormat	 = /*D3DFMT_D16*/D3DFMT_D24S8;	//
	d3dpp.Windowed					 = bWindow;						// window�ɂ���̂��t���X�N���[����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// �b�� ONE���ƈ��I������Ƃ������b�Z�[�W��������ĕ`�� ���ǂꂭ�炢�ŕ`��ł���̂����������Ƃ���IMMEDIATE
	d3dpp.PresentationInterval		 = D3DPRESENT_INTERVAL_DEFAULT;	//

	// �f�o�C�X�̐���
	hr = d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_D3DDevice);
	if (FAILED(hr)) { EMSG("DirectX9 �f�o�C�X�쐬���s"); return false;}
	SAFE_RELEASE(d3d);

	//	�����_�X�e�[�g�p�����[�^�̐ݒ�

	//	�A���t�@�u�����f�B���O�̐ݒ�
	SetAlphaBlending();

	//	�@�����K���̐ݒ�i�s��ɉe�����Ȃ��Ȃ�j
	m_D3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// �e�N�X�`���u�����f�B���O�i�f�t�H���g�j
	SetTexRenderAlpha();

	// �e�N�X�`�����b�s���O���[�h�̎w��i�e�N�X�`��UV�͈̔�0.0�`1.0�𒴂����Ƃ��ǂ̂悤�ȏ��������邩�j
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// WRAP�͔����̈Ӗ��@// CLAMP�͒[���������΂��@// MIRROR
	m_D3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);									
	
	//	�e�N�X�`���t�B���^�[�̃Z�b�g
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);// ���ۂ̃T�C�Y�����������Ȃ������ɂ�����t�B���^�����O (���`�A���`�G�C���A�V���O)	   
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);// ���ۂ̃T�C�Y�����傫���Ȃ������ɂ�����t�B���^�����O
	m_D3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
				
	//	�Q�[��������
	Debug::InitializeString();
	CInput::InitializeKeyboard(hInstance, hWnd);

	// ImGUI
	//ImGUIManager::Initializeialize(hWnd,  DirectX9::GetDevice());

	// �t�B�[�h�o�b�N�u���[
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
// �֐����FFinalize
// ���e�@�FDirectX9�I��
//==============================================================================
void DirectX9::Finalize(void)
{
	// �t�B�[�h�o�b�N�u���[
	//m_BackBufferSurface->Release();
	//m_BlurSurface1->Release();
	//m_BlurSurface2->Release();
	//m_BlurTexture1->Release();
	//m_BlurTexture2->Release();

	// ImGUI
	//ImGUIManager::Finalize();

	// �Q�[���I��
	CInput::FinalizeKeyboard();
	Debug::FinalizeString();

	// �f�o�C�X���
	SAFE_RELEASE(m_D3DDevice);

	
}

//==============================================================================
// �֐����FUpdate
// ���e�@�FDirectX9�X�V
//==============================================================================
void DirectX9::Update(void)
{
	CInput::UpdateKeyboard();	// �L�[�{�[�h�X�V
	//ImGUIManager::Update();
}

//=====================================================================================
// �֐����FScreenBegin()
// ���e�@�F��ʂ��J���[�ŃN���A�[
// �߂�l�Fbool 
//=====================================================================================
bool DirectX9::ScreenBegin()
{
	return (m_D3DDevice->BeginScene()) ? true : false;
}

//=====================================================================================
// �֐����FScreenEnd()
// ���e�@�F�o�b�N�o�b�t�@�ւ̕`��I��
//=====================================================================================
void DirectX9::ScreenEnd()
{
	m_D3DDevice->EndScene();
}

//=====================================================================================
// �֐����FScreenClear()
// ���e�@�F��ʂ��J���[�ŃN���A�[
//=====================================================================================
void DirectX9::ScreenClear()
{
	// �J�E���g�A�͈̓N���A�i����͑S��ʁj�A�F�̂��ƁA���F�ŃN���A�AZBUFFER�̃N���A�l�A�X�e���V���o�b�t�@�[�̃N���A�[
	m_D3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
}

//=====================================================================================
// �֐����FScreenSwap()
// ���e�@�F��ʂ��J���[�ŃN���A�[
//=====================================================================================
void DirectX9::ScreenSwap()
{
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����i�t���b�v�j
	m_D3DDevice->Present(NULL, NULL, NULL, NULL);// present �͑���@Draw�̍Ō�ɌĂ�
}

//=====================================================================================
// �֐����FBlurDraw()
// ���e�@�F�t�B�[�h�o�b�N�u���[�̎���
// �����@�F����
// �߂�l�Fvoid 
//=====================================================================================
void DirectX9::BlurDraw()
{
	////�`��^�[�Q�b�g�̕ύX
	//m_D3DDevice->SetRenderTarget(0, m_BlurSurface1);
	//
	//// ��ʃN���A
	//ScreenClear();
	//
	//// �`��J�n�i�j
	//if (SUCCEEDED(ScreenBegin()))
	//{
	//	// �S�Ă̕`��
	//	CManager::Draw();
	//
	//	// �����ɂQD�|���S���`��
	//	m_Blur2D->SetColor(D3DCOLOR_RGBA(255, 255, 255, 240));
	//	m_Blur2D->Draw2(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1366.0f + 0.0f, 768.0f + 0.0f), m_BlurTexture2);
	//
	//	m_D3DDevice->EndScene();	// �o�b�N�o�b�t�@�ւ̕`��I��
	//
	//	// �^�[�Q�b�g�̕ύX
	//	m_D3DDevice->SetRenderTarget(0, m_BackBufferSurface);
	//
	//	// ��ʃN���A
	//	ScreenClear();
	//
	//	// �`��J�n�i�j
	//	if (SUCCEEDED(ScreenBegin()))
	//	{
	//		m_Blur2D->SetScale(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
	//		m_Blur2D->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//	
	//		// 2D�|���S���`��		// �S��ʁA���Q�T�T
	//		m_Blur2D->Draw2(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR2(1366.0f, 768.0f), m_BlurTexture1);
	//	
	//		m_D3DDevice->EndScene();
	//	}
	//}
	//
	//// ����ւ�
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
// �A���t�@�u�����f�B���O�̐ݒ�
//================================================
void DirectX9::SetAlphaBlending()
{
	m_D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ��1�����A���t�@�̃^�C�v    ��2���� �g�����ǂ����̔��f
	m_D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// ��
	m_D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// ��
}

//================================================
// �e�N�X�`�������_�����O�̐ݒ�i�f�t�H���g�j
//================================================
void DirectX9::SetTexRenderDefault()
{
	// �F�̃u�����h�@�@�@�@�o�� = �e�N�X�`���̐F�@�~�@�|���S���̐F�@/ 255       // �|���S���J���[�����Ȃ�e�N�X�`���̐F�����̂܂܎g�p 
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// �|���Z����
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// �|���S���̐F

	// �����x�̃u�����h
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �e�N�X�`���̓����x
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);	// ����g�p
}

//================================================
// �e�N�X�`�������_�����O�̐ݒ�i�����x�u�����h�L�j(�����r�b�g�ɂ��g����)
//================================================
void DirectX9::SetTexRenderAlpha()
{
	// �F�̃u�����h�@�@�@�@�o�� = �e�N�X�`���̐F�@�~�@�|���S���̐F�@/ 255       // �|���S���J���[�����Ȃ�e�N�X�`���̐F�����̂܂܎g�p 
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);		// �|���Z����
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);		// �|���S���̐F

	// �����x�̃u�����h
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �e�N�X�`���̓����x
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ����g�p
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);	// �|���S���̓����x
}

//================================================
// �e�N�X�`�������_�����O�̐ݒ�i�V�[���̂悤�Ɂj
//================================================
void DirectX9::SetTexRenderSeal()
{
	// �B�|���S���̏�ɓ\����@�@�@�@�o�� = �e�N�X�`���̐F�~���@�{�@�|���S���̐F�~(�P�|��)
	// �F�̃u�����h
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);			// �e�N�X�`���̐F
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);	// �A���t�@�u�����f�B���O
	m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);			// �|���S���̐F
																					// �����x�̃u�����h
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);	// ����g�p
	m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// �e�N�X�`���̓����x
}

//================================================
// �e�N�X�`�������_�����O�̐ݒ�i�X�|�b�g���C�g�j
//================================================
//void SetTexRenderSpotLight()
//{
//	// �ǂɃX�|�b�g���C�g�Q���o�[�W����(�r��)
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);	
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// �e�N�X�`���̓����x
//	//m_D3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
//
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);		// �e�N�X�`���̐F
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BLENDTEXTUREALPHA);
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2,D3DTA_CURRENT );		// �e�N�X�`���̓����x
//
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
//	//m_D3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);		// �e�N�X�`���̓����x
//}




