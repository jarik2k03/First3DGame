
Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

cbuffer ConstantBuffer : register (b0) // b0 - ������ ������
{
  matrix world;
}

cbuffer CamBuffer : register(b2) // b2 - ������� ������
{
  matrix view;
  matrix proj;
}

cbuffer LightBuffer : register(b1) // b1 - ������ ������
{
  float4 dir;
  float4 src_color;
  float4 dest_color;
}

struct VS_INPUT {
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 norm: NORMAL;
};

struct PS_OUTPUT // �������� ������ ����������� �������
{
  float4 pos : SV_POSITION; // ������� ������� � �������� (��������)
  float2 tex : TEXCOORD0; // ���������� �������� �� tu, tv
  float3 norm : TEXCOORD1;
};

PS_OUTPUT VS_Out(VS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0; // ������� ���� ���������
	output.pos = mul(input.pos, world); // ������������� ������� ������� � ������� ����
	output.pos = mul(output.pos, view); // � ������� ����
	output.pos = mul(output.pos, proj); // � ������� ��������
	output.norm = mul(input.norm, world);
	output.tex = input.tex;
	return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  float4 color = 0.35f;
  color += saturate(dot(dir / 64, input.norm) * src_color);
  color *= dirt.Sample(sample_linear_mip, input.tex);
  return color;
}

float4 PS_Light(PS_OUTPUT input) : SV_Target {
  return src_color * 2 * ( dirt.Sample(sample_linear_mip, input.tex));
}
