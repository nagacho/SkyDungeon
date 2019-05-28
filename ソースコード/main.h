//========================================================
//	���C��
//========================================================	 
//	�쐬���F2018/4/13(��)
//	�쐬�ҁFRyoya Nagata
//========================================================
#ifndef	_MAIN_H_
#define	_MAIN_H_
#include <d3d9.h>
#include <d3dx9.h>

//======================
// �}�N����`
//======================
#define WIN32_LEAN_AND_MEAN													// �w�b�_�[���炠�܂�g���Ȃ��֐����Ȃ�
#define EMSG(x)	MessageBox (NULL, x, "�G���[",MB_OK);						// ���b�Z�[�W�G���[
#define SAFE_RELEASE(x)	if(x != nullptr){ (x)->Release(); (x) = nullptr; }	// �����[�X
#define SAFE_DELETE(x) if(x != nullptr){ delete x; (x) = nullptr; }			// ���S��delete
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// FVF�̐錾�@RHW�͊O���I�@D3DFVF_XYZRHW �@���ǉ���D3DFVF_NORMAL

#define CLASS_NAME	"sample"
#define WINDOW_NAME	"�E�B���h�E�̕\��"

//======================
// �e���v���[�g�֐�
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
// �萔
//=======================
static const int SCREEN_WIDTH = 1366;				// ��ʂ̕�
static const int SCREEN_HEIGHT = 768;				// ��ʂ̍���


//====================================
// ���_�\����
//====================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nvec;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
}VERTEX_3D;


#endif