//------------------------------------------------
// �O���[�o���ϐ�
//------------------------------------------------
float4x4	g_view;					// �J�����ϊ��s��
float4x4	g_projection;			// �v���W�F�N�V����

// �e�N�X�`��
bool g_tex;
texture g_texture;

// �}�e���A��
float4 g_diffuse_mat;
float4 g_ambient_mat;
float4 g_specular_mat;
float4 g_emmisive_mat;

// ��
float4		g_diffuse;					// �f�B�t���[�Y
//float4		g_emmisive;				// �G�~�b�V�u
float4		g_ambient;					// ����
float4		g_specular;					// �X�y�L�����[��
float3		g_light_dir;				// ���s�����̕���

// �v���C���[�̍��W
float4		g_player_pos;

// �\����
struct VS_IN
{
	float3 pos		: POSITION;		// ���[�J�����W
	float3 normal	: NORMAL;		// �@��
	float4 color	: COLOR0;		// ���_�J���[
	float2 tex		: TEXCOORD0;	// UV���W
	float3 worldpos : TEXCOORD1;	// ���[���h���W
	float3 rotation : TEXCOORD2;	// ��]
	float3 scale	: TEXCOORD3;	// �傫��
};

struct VS_OUT
{
	float4 pos		: POSITION;		// �o�͍��W
	float4 color	: COLOR0;		// �o�̓J���[
	float2 tex		: TEXCOORD0;	// �o��UV
	float3 normal	: TEXCOORD1;	// ���[���h�ϊ���@���o��
	float3 posforps	: TEXCOORD2;	// ���[���h�ϊ��㒸�_���W�o��
};

struct PS_IN
{
	float4 color	: COLOR0;
	float2 tex		: TEXCOORD0;	//�s�N�Z����UV���W
	float3 normal	: TEXCOORD1;	// ���[���h�ϊ���@��
	float3 posforps	: TEXCOORD2;	// ���[���h�ϊ��㒸�_���W
};

//�T���v���[�ϐ��̍쐬
sampler Sampler1 = sampler_state {
	Texture = g_texture;

	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};
//------------------------------------------------
// ���_�V�F�[�_
//------------------------------------------------
void VS(	VS_IN vIn,
			out VS_OUT vOut
)
{
	float4x4 MtxTrans = { 1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		vIn.worldpos.x, vIn.worldpos.y, vIn.worldpos.z, 1.0f
	};

	float4x4 MtxScale = { vIn.scale.x,0.0f       ,0.0f       ,0.0f,
		0.0f       ,vIn.scale.y,0.0f       ,0.0f,
		0.0f       ,0.0f       ,vIn.scale.z,0.0f,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};

	float4x4 MtxRotX = { 1.0f, 0.0f                ,0.0f               ,0.0f,
		0.0f, cos(vIn.rotation.x) ,sin(vIn.rotation.x),0.0f,
		0.0f, -sin(vIn.rotation.x),cos(vIn.rotation.x),0.0f,
		0.0f, 0.0f                ,0.0f               ,1.0f
	};
	//
	//float4x4 MtxRotY = { cos(vIn.rotation.y), 0.0f, -sin(vIn.rotation.y), 0.0f,
	//					 0.0f              , 1.0f, 0.0f               , 0.0f,
	//					 sin(vIn.rotation.y), 0.0f, cos(vIn.rotation.y) , 0.0f,
	//					 0.0f              , 0.0f, 0.0f               , 1.0f
	//};
	//
	float4x4 MtxRotZ = { cos(vIn.rotation.z) ,sin(vIn.rotation.z),0.0f, 0.0f,
		-sin(vIn.rotation.z),cos(vIn.rotation.z),0.0f, 0.0f,
		0.0f               ,0.0f              ,1.0f, 0.0f,
		0.0f               ,0.0f              ,0.0f, 1.0f
	};

	float4x4 world = mul(MtxScale, MtxRotZ);
	world = mul(world, MtxRotX);
	//world = mul(world, MtxRotY);
	world = mul(world, MtxTrans);

	// ���W�ϊ�
	vOut.pos = mul(float4(vIn.pos, 1.0f), world);		// ���_���W�i���[�J�����W�n�j�����[���h���W�n�ɕϊ�	
	vOut.posforps = vOut.pos;
	vOut.pos = mul(vOut.pos, g_view);					// ���_���W�i���[���h���W�n�j���J�������W�n�ɕϊ�
	vOut.pos = mul(vOut.pos, g_projection);				// ���_���W�i�J�������W�n�j���X�N���[�����W�n�ɕϊ�

	// ���_�J���[�o��
	vOut.color = vIn.color;

	//�e�N�X�`�����W�̏o��
	vOut.tex = vIn.tex;

	// �@�������[���h�ϊ����Đ��K��
	vOut.normal = normalize(mul(vIn.normal, (float3x3)world));		// N

}



//------------------------------------------------
// �s�N�Z���V�F�[�_
//------------------------------------------------
void PS(	PS_IN pIn,
		out float4 out_color : COLOR0
)
{

	float3 N;	// ���[���h�ϊ��ϖ@��
	float3 L;	// ���̃x�N�g��

	// ���[���h��Ԃł̖@���x�N�g��
	N = normalize(pIn.normal);

	// ���z���𐳋K��
	float3 playerpos_xyz = g_player_pos.xyz;
	playerpos_xyz.y += 2.0f;
	float3 sun_light_dir = pIn.posforps - playerpos_xyz;

	// ���C�g�̉e���͈�
	if (sun_light_dir.x * sun_light_dir.x + sun_light_dir.z * sun_light_dir.z >= 3.0f * 3.0f)
	{
		L = 0.0f;
	}
	else
	{
		L = normalize(-sun_light_dir);
	}

	// �o�̓J���[�����߂�
	float4 diffuse = g_ambient*pIn.color + g_diffuse * pIn.color * max(0.0f, dot(L, N));

	// �F�̏o��
	if (!g_tex)
	{	
		out_color = pIn.color;//���̂܂܏o��
	}
	else
	{
		//�T���v���[����e�N�Z�����擾
		float4 texcol = tex2D(Sampler1, pIn.tex);
	
		//���_�J���[�ƃu�����h
		out_color = saturate(diffuse * texcol);//�e�N�Z���F���o��
	}
	
}

//------------------------------------------------
// �e�N�j�b�N�i�V�F�[�_�̓����j
//------------------------------------------------
technique BasicTec
{
	pass P0
	{
		VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
	}
}
