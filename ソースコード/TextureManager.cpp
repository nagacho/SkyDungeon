#include<locale.h>
#include "TextureManager.h"
#include"DirectX9.h"

//==========================
// ���̉�
//==========================
std::map<std::string, LPDIRECT3DTEXTURE9> CTexture::m_MapTexture;
std::map<std::string, D3DXIMAGE_INFO> CTexture::m_MapTextureInfo;
LPDIRECT3DTEXTURE9 CTexture::m_pTexture;
D3DXIMAGE_INFO CTexture::m_PicInfo;

//=====================================================================================
// �֐����FLoadTexture(std::string TexName)
// ���e�@�F�e�N�X�`���̓ǂݍ���
// �����@�Fstd::string TexName�@:�@�e�N�X�`����
// �߂�l�Fvoid
//=====================================================================================
void CTexture::LoadTexture(std::string TexName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = DirectX9::GetDevice();
	
	// �e�N�X�`���𒲂ׂ�
	TexTable::const_iterator it = m_MapTexture.find(TexName);

	// �e�N�X�`���[��������Ȃ�������
	if (it == m_MapTexture.end())
	{
		HRESULT hr;
		std::string fullpath = operator+(TEX_PATH, TexName);
		D3DXGetImageInfoFromFile(fullpath.c_str(), &m_PicInfo);

		hr = D3DXCreateTextureFromFile(device, fullpath.c_str(), &m_pTexture);
		if (FAILED(hr))
		{
			// �e�N�X�`���[�̓ǂݍ��݂̎��s���̃��b�Z�[�W
			EMSG("�e�N�X�`�����ǂݍ��߂Ȃ�����");
			return;
		}

		// map�Ƀe�N�X�`���|�C���^�̓o�^
		m_MapTexture.insert(TexTable::value_type(TexName, m_pTexture));
		m_MapTextureInfo.insert((TexTableInfo::value_type(TexName, m_PicInfo)));
	}
}

//=====================================================================================
// �֐����FGetTexture(std::string TexName)
// ���e�@�F�f�o�C�X�n��
// �����@�Fstd::string TexName�@:�@�e�N�X�`����
// �߂�l�FLPDIRECT3DTEXTURE9
//=====================================================================================
LPDIRECT3DTEXTURE9 CTexture::GetTexture(std::string TexName)
{
	if (m_MapTexture[TexName] == nullptr)
	{
		return m_MapTexture["white.png"];
	}
	else
	{
		return m_MapTexture[TexName];
	}
	
}

//=====================================================================================
// �֐����FRelease(std::string TexName)
// ���e�@�F�e�N�X�`���̉��
// �����@�Fstd::string TexName�@:�@�e�N�X�`����
// �߂�l�Fvoid
//=====================================================================================
void CTexture::Release(std::string TexName)
{
	SAFE_RELEASE(m_MapTexture[TexName]);

	// �e�N�X�`���𒲂ׂ�
	TexTable::const_iterator it = m_MapTexture.find(TexName);
	if (it != m_MapTexture.end())
	{
		m_MapTexture.erase(it);
	}
	
	TexTableInfo::const_iterator it2 = m_MapTextureInfo.find(TexName);
	if (it2 != m_MapTextureInfo.end())
	{
		m_MapTextureInfo.erase(it2);
	}
	
}

//=====================================================================================
// �֐����FReleaseAll()
// ���e�@�F�e�N�X�`���S�Ẳ��
// �����@�F�Ȃ�
// �߂�l�Fvoid
//=====================================================================================
void CTexture::ReleaseAll()
{
	SAFE_RELEASE(m_pTexture);

	// map���̉��
	for (auto itr = m_MapTexture.begin(); itr != m_MapTexture.end(); itr++)
	{
		SAFE_RELEASE(itr->second);
	}

	m_MapTexture.clear();
	m_MapTextureInfo.clear();
}

//=====================================================================================
// �֐����FGetPictureinfo(std::string TexName)
// ���e�@�F�e�N�X�`�����̎擾
// �����@�Fstd::string TexName�@�F�@�e�N�X�`����
// �߂�l�FD3DXIMAGE_INFO
//=====================================================================================
D3DXIMAGE_INFO CTexture::GetPictureinfo(std::string TexName)
{
	return m_MapTextureInfo[TexName];
}


//=====================================================================================
// �֐����FLoadTexture(std::string TexName)
// ���e�@�F�e�N�X�`���̓ǂݍ���
// �����@�Fstd::string TexName�@				// �e�N�X�`����
// �����Q�FIDirect3DTexture9* CreateTexture		// ������e�N�X�`�������֐��̃|�C���^
// �߂�l�Fvoid
//=====================================================================================
void CTexture::LoadTextureFromCreateTexture(std::string TexName, TEX_INFO_DATA* CreateTextureData)
{
	// �e�N�X�`���𒲂ׂ�
	TexTable::const_iterator it = m_MapTexture.find(TexName);

	// �e�N�X�`���[��������Ȃ�������
	if (it == m_MapTexture.end())
	{
		// map�ɓo�^
		LPDIRECT3DTEXTURE9 pTexture = CreateTextureData->pTex;
		D3DXIMAGE_INFO PicInfo = CreateTextureData->TexInfo;
		m_MapTexture.insert(TexTable::value_type(TexName, pTexture));
		m_MapTextureInfo.insert((TexTableInfo::value_type(TexName, PicInfo)));
	}

	
}


//=====================================================================================
// �֐����FTEX_INFO_DATA* CreateTextureFromText(IDirect3DDevice9* dev, const char* text,const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color)
// ���e�@�F������e�N�X�`���̍쐬
// �����@�FIDirect3DDevice9* dev,		//DirectX�̃f�o�C�X�B
// �����Q�Fconst char* text,			//�`�悷�镶����B
// �����R�Fconst char* font,			//�e�L�X�g�Ɏg�p�����t�H���g( Font )�B
// �����S�Fint fontWeight,				//�t�H���g( Font )�̑����B
// �����T�Fint fontSize,				//�t�H���g( Font )�̃T�C�Y�B
// �����U�Fbool italic,					//�C�^���b�N�����ɂ��邩�ǂ����B
// �����V�Fint lineSpacing,				//������
// �����W�Fint color					//�t�H���g( Font )�̐F�B
// �߂�l�FTEX_INFO_DATA*
//=====================================================================================
TEX_INFO_DATA* CTexture::CreateTextureFromText(IDirect3DDevice9* dev, const char* text, const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color)
{
	//InitializeFont
	HDC hdc;
	HFONT oldFont;

	{
		// �t�H���g�̐���
		//int fontWeight = FW_BOLD;
		//  �t�H���g�̍���,���ϕ�,��������̕�����X���Ƃ̊p�x,�x�[�X���C����X���Ƃ̊p�x,�����̑���,�C�^���b�N,�A���_�[���C��,�X�g���C�N�A�E�g,�����Z�b�g,�o�͐��x,�N���b�s���O���x,�o�͕i��,�t�H���g�̃s�b�`�ƃt�@�~���[,�t�H���g�̏��̖�
		LOGFONTA lf = { fontSize, 0, 0, 0, fontWeight, italic, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "�l�r �o�S�V�b�N" };
		for (int i = 0; lf.lfFaceName[i == 0 ? 0 : (i - 1)] != '\0'&& i < 32; ++i)
		{
			lf.lfFaceName[i] = font[i];
		}

		HFONT hFont = CreateFontIndirectA(&lf);

		if (hFont == NULL){ return 0;}

		// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
		hdc = GetDC(NULL);
		oldFont = (HFONT)SelectObject(hdc, hFont);	// �ȑO�̓����^�̑I�����ꂽ�I�u�W�F�N�g��Ԃ��B
	}


	// ������̗v�f�������߂�
	int strNum = 0;
	for (; text[strNum] != '\0'; ++strNum);

	FONT *f = new FONT[strNum];

	const int gradFlag = GGO_GRAY4_BITMAP;	// GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	int grad = 0;							// �K���̍ő�l

	switch (gradFlag) 
	{
		case GGO_GRAY2_BITMAP: 
			grad = 4; 
			break;
		case GGO_GRAY4_BITMAP: 
			grad = 16; 
			break;
		case GGO_GRAY8_BITMAP: 
			grad = 64; 
			break;
	}
	if (grad == 0) { return nullptr; }

	TEXTMETRIC tm;
	GetTextMetricsA(hdc, &tm);
	GLYPHMETRICS gm;
	CONST MAT2 mat = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };

	UINT texWidth = 0;
	UINT texHeight = 0;

	setlocale(LC_ALL, "japanese");
	size_t wcResultNum;							//�ϊ����ꂽ������
	wchar_t *wcstr = new wchar_t[strNum + 1];	//�ϊ����ꂽ���C�h��������i�[����o�b�t�@�̃A�h���X(�ϊ���)
	size_t wcNum = strNum + 1;					//wcstr �o�b�t�@�̃T�C�Y
	mbstowcs_s(&wcResultNum, wcstr, wcNum, text, strNum);
	strNum = wcResultNum;

	for (int i = 0; i<strNum; ++i)
	{
		UINT code = (UINT)wcstr[i];
		DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
		f[i].mono = new BYTE[size];
		f[i].width = (gm.gmBlackBoxX + 3) / 4 * 4;
		f[i].height = gm.gmBlackBoxY;
		f[i].offsetX = gm.gmptGlyphOrigin.x;
		f[i].offsetY = tm.tmAscent - gm.gmptGlyphOrigin.y;
		texWidth += f[i].width + f[i].offsetX;
		texHeight = gm.gmBlackBoxY + f[i].offsetY>texHeight ? gm.gmBlackBoxY + f[i].offsetY : texHeight;
		GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, f[i].mono, &mat);
	}
	texWidth += lineSpacing*(strNum - 1);

	// �e�N�X�`���쐬
	IDirect3DTexture9 *tex = nullptr;
	dev->CreateTexture(texWidth, texHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL);

	D3DXIMAGE_INFO info;
	info.Width = texWidth;
	info.Height = texHeight;

	// �e�N�X�`���Ƀt�H���g�r�b�g�}�b�v������������
	D3DLOCKED_RECT lockedRect;
	tex->LockRect(0, &lockedRect, NULL, 0);			// ���b�N
	DWORD *pTexBuf = (DWORD*)lockedRect.pBits;		// �e�N�X�`���������ւ̃|�C���^

	int tmpWidth = 0;
	for (int i = 0; i < strNum; ++i) {
		for (int y = 0; y < f[i].height; y++) {
			for (int x = 0; x < f[i].width; x++) {
				DWORD alpha = f[i].mono[y * f[i].width + x] * 255 / grad;
				pTexBuf[(texWidth*(y + f[i].offsetY)) + tmpWidth + x + f[i].offsetX + (lineSpacing*i)] = (alpha << 24) | color;
			}
		}
		tmpWidth += f[i].width;
		delete[] f[i].mono;
	}

	tex->UnlockRect(0);  // �A�����b�N

	/*for (int i = 0; i < strNum; i++)
	{
		delete[] f[i];
		delete[] wcstr[i];
	}*/
	

	//UnintFont
	{
		// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���͂�������Ȃ��̂ŉ��
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);
	}

	// �f�[�^�̊i�[
	TEX_INFO_DATA* data = new TEX_INFO_DATA;
	data->pTex = tex;
	data->TexInfo = info;

	return data;

	
}



