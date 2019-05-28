//========================================================
//	DirectX9
//========================================================	 
//	�쐬���F2018/4/24(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef _DIRECTX9_H_
#define _DIRECTX9_H_
#include <d3d9.h>
#include <d3dx9.h>

// �O���錾
class Object2D;

// �N���X
class DirectX9
{
private:
	static LPDIRECT3DDEVICE9 m_D3DDevice;
	static HWND m_hWnd;

	// �t�B�[�h�o�b�N�u���[
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

	// �t�B�[�h�o�b�N�u���[
	static void BlurDraw();
	//static LPDIRECT3DTEXTURE9* GetTexture1() { return &m_BlurTexture1; }
	//static LPDIRECT3DTEXTURE9* GetTexture2() { return &m_BlurTexture2; }
	//static LPDIRECT3DSURFACE9* GetSurface1() { return &m_BackBufferSurface; }

	// �J����
	static void SetView(D3DXMATRIX view){ m_D3DDevice->SetTransform(D3DTS_VIEW, &view); }
	static void SetProjection(D3DXMATRIX projection){ m_D3DDevice->SetTransform(D3DTS_PROJECTION, &projection); }

	// ���C�g
	static void SetLight(D3DLIGHT9 light){ m_D3DDevice->SetLight(0, &light); }

	// �}�e���A��
	static void SetMaterial(D3DMATERIAL9 material){ m_D3DDevice->SetMaterial(&material); }

	// �A���t�@�u�����f�B���O
	static void SetAlphaBlending();

	// �e�N�X�`�������_�����O�֘A
	static void SetTexRenderDefault();
	static void SetTexRenderAlpha();
	static void SetTexRenderSeal();

	//�f�o�C�X�̎擾
	static LPDIRECT3DDEVICE9 GetDevice(void) { return m_D3DDevice; }

	// �n���h���̎擾
	static HWND GetHandle(void) { return m_hWnd; }

};


#endif