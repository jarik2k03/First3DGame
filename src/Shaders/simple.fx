
Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

cbuffer ConstantBuffer : register (b0) // b0 - индекс буфера
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VS_INPUT {
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
};

struct PS_OUTPUT // Входящие данные пиксельного шейдера
{
  float4 pos : SV_POSITION; // Позиция пикселя в проекции (экранная)
  float2 tex : TEXCOORD0; // Координаты текстуры по tu, tv
};

PS_OUTPUT VS_Out(VS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0; // создали нашу структуру
	output.pos = mul(input.pos, world); // трансформация позиции вершины в матрицу мира
	output.pos = mul(output.pos, view); // в матрицу вида
	output.pos = mul(output.pos, proj); // в матрицу проекции
	output.tex = input.tex;
	return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  return dirt.Sample(sample_linear_mip, input.tex);
}
