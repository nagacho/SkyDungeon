//------------------------------------------------
// グローバル変数
//------------------------------------------------
float4x4 	g_world;					// ワールド行列
float4x4	g_view;						// カメラ変換行列
float4x4	g_projection;				// プロジェクション

// テクスチャ
bool g_tex;
texture g_texture;

// マテリアル
float4 g_diffuse_mat;
float4 g_ambient_mat;
float4 g_specular_mat;
float4 g_emmisive_mat;
bool g_mat;

// 光
float4		g_diffuse;					// ディフューズ
float4		g_ambient;					// 環境光
float4		g_specular;					// スペキュラー光
float3		g_light_dir;				// 平行光源の方向
//float4		g_emmisive;				// エミッシブ

// プレイヤーの座標
float4		g_player_pos;

// 構造体
struct VS_IN
{
	float3 pos		: POSITION;		// ローカル座標
	float3 normal	: NORMAL;		// 法線
	float4 color	: COLOR0;		// 頂点カラー
	float2 tex		: TEXCOORD0;	// UV座標
};

struct VS_OUT
{
	float4 pos		: POSITION;		// 出力座標
	float4 color	: COLOR0;		// 出力カラー
	float2 tex		: TEXCOORD0;	// 出力UV
	float3 normal	: TEXCOORD1;	// ワールド変換後法線出力
	float3 posforps	: TEXCOORD2;	// ワールド変換後頂点座標出力
};

struct PS_IN
{
	float4 color	: COLOR0;
	float2 tex		: TEXCOORD0;	//ピクセルのUV座標
	float3 normal	: TEXCOORD1;	// ワールド変換後法線
	float3 posforps	: TEXCOORD2;	// ワールド変換後頂点座標
};

//サンプラー変数の作成
sampler Sampler1 = sampler_state {
	Texture = g_texture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};
//------------------------------------------------
// 頂点シェーダ
//------------------------------------------------
void VS(	VS_IN vIn,
		out VS_OUT vOut
)
{
	// 座標変換
	vOut.pos = mul(float4(vIn.pos, 1.0f), g_world);		// 頂点座標（ローカル座標系）をワールド座標系に変換	
	vOut.posforps = vOut.pos;
	vOut.pos = mul(vOut.pos, g_view);					// 頂点座標（ワールド座標系）をカメラ座標系に変換
	vOut.pos = mul(vOut.pos, g_projection);				// 頂点座標（カメラ座標系）をスクリーン座標系に変換

	// 頂点カラー出力
	vOut.color = vIn.color;

	//テクスチャ座標の出力
	vOut.tex = vIn.tex;

	// 法線をワールド変換して正規化
	vOut.normal = normalize(mul(vIn.normal, (float3x3)g_world));		// N
}



//------------------------------------------------
// ピクセルシェーダ
//------------------------------------------------
void PS(	PS_IN pIn,
		out float4 out_color : COLOR0
)
{
	float3 N;	// ワールド変換済法線
	float3 L;	// 光のベクトル

	// ワールド空間での法線ベクトル
	N = normalize(pIn.normal);

	// 太陽光を正規化
	float3 playerpos_xyz = g_player_pos.xyz;
	playerpos_xyz.y += 2.0f;
	float3 sun_light_dir = pIn.posforps - playerpos_xyz;

	// ライトの影響範囲
	if (sun_light_dir.x * sun_light_dir.x + sun_light_dir.z * sun_light_dir.z >= 3.0f * 3.0f)
	{
		L = 0.0f;
	}
	else
	{
		L = normalize(-sun_light_dir);
	}

	// 出力カラーを決める
	float4 diffuse;
	if (g_mat)
	{
		diffuse = g_ambient * g_ambient_mat + g_diffuse * g_diffuse_mat * max(0.0f, dot(L, N));
	}
	else
	{
		diffuse = g_ambient * pIn.color + g_diffuse * pIn.color * max(0.0f, dot(L, N));
	}
	

	// 色の出力
	if (!g_tex)
	{
		out_color = diffuse;//そのまま出力
	}
	else
	{
		//サンプラーからテクセルを取得
		float4 texcol = tex2D(Sampler1, pIn.tex);

		//頂点カラーとブレンド
		out_color = saturate(diffuse * texcol);//テクセル色を出力
	}
}

//------------------------------------------------
// テクニック（シェーダの入口）
//------------------------------------------------
technique BasicTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
