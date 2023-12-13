
Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

cbuffer ConstantBuffer : register (b0) // b0 - ������ ������
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VS_INPUT {
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_OUTPUT // �������� ������ ����������� �������
{
  float4 pos : SV_POSITION; // ������� ������� � �������� (��������)
  float2 tex : TEXCOORD0; // ���������� �������� �� tu, tv
};

PS_OUTPUT VS_Out(VS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0; // ������� ���� ���������
	output.pos = mul(input.pos, world); // ������������� ������� ������� � ������� ����
	output.pos = mul(output.pos, view); // � ������� ����
	output.pos = mul(output.pos, proj); // � ������� ��������
	output.tex = input.tex;
	return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  return dirt.Sample(sample_linear_mip, input.tex);
}
