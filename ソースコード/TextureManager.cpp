#include<locale.h>
#include "TextureManager.h"
#include"DirectX9.h"

//==========================
// 実体化
//==========================
std::map<std::string, LPDIRECT3DTEXTURE9> CTexture::m_MapTexture;
std::map<std::string, D3DXIMAGE_INFO> CTexture::m_MapTextureInfo;
LPDIRECT3DTEXTURE9 CTexture::m_pTexture;
D3DXIMAGE_INFO CTexture::m_PicInfo;

//=====================================================================================
// 関数名：LoadTexture(std::string TexName)
// 内容　：テクスチャの読み込み
// 引数　：std::string TexName　:　テクスチャ名
// 戻り値：void
//=====================================================================================
void CTexture::LoadTexture(std::string TexName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 device = DirectX9::GetDevice();
	
	// テクスチャを調べる
	TexTable::const_iterator it = m_MapTexture.find(TexName);

	// テクスチャーが見つからなかったら
	if (it == m_MapTexture.end())
	{
		HRESULT hr;
		std::string fullpath = operator+(TEX_PATH, TexName);
		D3DXGetImageInfoFromFile(fullpath.c_str(), &m_PicInfo);

		hr = D3DXCreateTextureFromFile(device, fullpath.c_str(), &m_pTexture);
		if (FAILED(hr))
		{
			// テクスチャーの読み込みの失敗時のメッセージ
			EMSG("テクスチャが読み込めなかった");
			return;
		}

		// mapにテクスチャポインタの登録
		m_MapTexture.insert(TexTable::value_type(TexName, m_pTexture));
		m_MapTextureInfo.insert((TexTableInfo::value_type(TexName, m_PicInfo)));
	}
}

//=====================================================================================
// 関数名：GetTexture(std::string TexName)
// 内容　：デバイス渡す
// 引数　：std::string TexName　:　テクスチャ名
// 戻り値：LPDIRECT3DTEXTURE9
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
// 関数名：Release(std::string TexName)
// 内容　：テクスチャの解放
// 引数　：std::string TexName　:　テクスチャ名
// 戻り値：void
//=====================================================================================
void CTexture::Release(std::string TexName)
{
	SAFE_RELEASE(m_MapTexture[TexName]);

	// テクスチャを調べる
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
// 関数名：ReleaseAll()
// 内容　：テクスチャ全ての解放
// 引数　：なし
// 戻り値：void
//=====================================================================================
void CTexture::ReleaseAll()
{
	SAFE_RELEASE(m_pTexture);

	// map内の解放
	for (auto itr = m_MapTexture.begin(); itr != m_MapTexture.end(); itr++)
	{
		SAFE_RELEASE(itr->second);
	}

	m_MapTexture.clear();
	m_MapTextureInfo.clear();
}

//=====================================================================================
// 関数名：GetPictureinfo(std::string TexName)
// 内容　：テクスチャ情報の取得
// 引数　：std::string TexName　：　テクスチャ名
// 戻り値：D3DXIMAGE_INFO
//=====================================================================================
D3DXIMAGE_INFO CTexture::GetPictureinfo(std::string TexName)
{
	return m_MapTextureInfo[TexName];
}


//=====================================================================================
// 関数名：LoadTexture(std::string TexName)
// 内容　：テクスチャの読み込み
// 引数　：std::string TexName　				// テクスチャ名
// 引数２：IDirect3DTexture9* CreateTexture		// 文字列テクスチャ生成関数のポインタ
// 戻り値：void
//=====================================================================================
void CTexture::LoadTextureFromCreateTexture(std::string TexName, TEX_INFO_DATA* CreateTextureData)
{
	// テクスチャを調べる
	TexTable::const_iterator it = m_MapTexture.find(TexName);

	// テクスチャーが見つからなかったら
	if (it == m_MapTexture.end())
	{
		// mapに登録
		LPDIRECT3DTEXTURE9 pTexture = CreateTextureData->pTex;
		D3DXIMAGE_INFO PicInfo = CreateTextureData->TexInfo;
		m_MapTexture.insert(TexTable::value_type(TexName, pTexture));
		m_MapTextureInfo.insert((TexTableInfo::value_type(TexName, PicInfo)));
	}

	
}


//=====================================================================================
// 関数名：TEX_INFO_DATA* CreateTextureFromText(IDirect3DDevice9* dev, const char* text,const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color)
// 内容　：文字列テクスチャの作成
// 引数　：IDirect3DDevice9* dev,		//DirectXのデバイス。
// 引数２：const char* text,			//描画する文字列。
// 引数３：const char* font,			//テキストに使用されるフォント( Font )。
// 引数４：int fontWeight,				//フォント( Font )の太さ。
// 引数５：int fontSize,				//フォント( Font )のサイズ。
// 引数６：bool italic,					//イタリック文字にするかどうか。
// 引数７：int lineSpacing,				//文字間
// 引数８：int color					//フォント( Font )の色。
// 戻り値：TEX_INFO_DATA*
//=====================================================================================
TEX_INFO_DATA* CTexture::CreateTextureFromText(IDirect3DDevice9* dev, const char* text, const char* font, int fontWeight, int fontSize, bool italic, int lineSpacing, int color)
{
	//InitializeFont
	HDC hdc;
	HFONT oldFont;

	{
		// フォントの生成
		//int fontWeight = FW_BOLD;
		//  フォントの高さ,平均幅,文字送りの方向とX軸との角度,ベースラインとX軸との角度,文字の太さ,イタリック,アンダーライン,ストライクアウト,文字セット,出力精度,クリッピング精度,出力品質,フォントのピッチとファミリー,フォントの書体名
		LOGFONTA lf = { fontSize, 0, 0, 0, fontWeight, italic, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "ＭＳ Ｐゴシック" };
		for (int i = 0; lf.lfFaceName[i == 0 ? 0 : (i - 1)] != '\0'&& i < 32; ++i)
		{
			lf.lfFaceName[i] = font[i];
		}

		HFONT hFont = CreateFontIndirectA(&lf);

		if (hFont == NULL){ return 0;}

		// デバイスにフォントを持たせないとGetGlyphOutline関数はエラーとなる
		hdc = GetDC(NULL);
		oldFont = (HFONT)SelectObject(hdc, hFont);	// 以前の同じ型の選択されたオブジェクトを返す。
	}


	// 文字列の要素数を求める
	int strNum = 0;
	for (; text[strNum] != '\0'; ++strNum);

	FONT *f = new FONT[strNum];

	const int gradFlag = GGO_GRAY4_BITMAP;	// GGO_GRAY2_BITMAP or GGO_GRAY4_BITMAP or GGO_GRAY8_BITMAP
	int grad = 0;							// 階調の最大値

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
	size_t wcResultNum;							//変換された文字数
	wchar_t *wcstr = new wchar_t[strNum + 1];	//変換されたワイド文字列を格納するバッファのアドレス(変換先)
	size_t wcNum = strNum + 1;					//wcstr バッファのサイズ
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

	// テクスチャ作成
	IDirect3DTexture9 *tex = nullptr;
	dev->CreateTexture(texWidth, texHeight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL);

	D3DXIMAGE_INFO info;
	info.Width = texWidth;
	info.Height = texHeight;

	// テクスチャにフォントビットマップ情報を書き込み
	D3DLOCKED_RECT lockedRect;
	tex->LockRect(0, &lockedRect, NULL, 0);			// ロック
	DWORD *pTexBuf = (DWORD*)lockedRect.pBits;		// テクスチャメモリへのポインタ

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

	tex->UnlockRect(0);  // アンロック

	/*for (int i = 0; i < strNum; i++)
	{
		delete[] f[i];
		delete[] wcstr[i];
	}*/
	

	//UnintFont
	{
		// デバイスコンテキストとフォントハンドルはもういらないので解放
		SelectObject(hdc, oldFont);
		ReleaseDC(NULL, hdc);
	}

	// データの格納
	TEX_INFO_DATA* data = new TEX_INFO_DATA;
	data->pTex = tex;
	data->TexInfo = info;

	return data;

	
}



