cbuffer ConstantBuffer : register (b0) // b0 - ������ ������
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR0; // color0 - ���������� ����, color1 - ���������� ����
};



VS_OUTPUT VS_Out(float4 pos : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output = (VS_OUTPUT)0; // ������� ���� ���������
	output.pos = mul(pos, world); // ������������� ������� ������� � ������� ����
	output.pos = mul(output.pos, view); // � ������� ����
	output.pos = mul(output.pos, proj); // � ������� ��������
	output.color = color;
	// ��������� ���������� ����� ��� ���������
	return output;
}



float4 PS_Out(VS_OUTPUT input) : SV_Target
{
	return input.color;
}
