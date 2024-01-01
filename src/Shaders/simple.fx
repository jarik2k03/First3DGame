
Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

cbuffer PositionBuffer : register (b0) {// b0 - индекс буфера
  matrix world;
}
cbuffer LightBuffer : register(b1) {// b1 - индекс буфера
  float4 dir;
  float4 src_color;
  float4 dest_color;
}


cbuffer ViewBuffer : register(b2) {// b2 - матрицы камеры
  matrix view;
}
cbuffer ProjBuffer : register(b3) {// b3 - матрицы камеры 
  matrix proj;
}

struct VS_INPUT {
	float4 pos : POSITION;
	float2 tex : TEXCOORD0;
	float3 norm: NORMAL;
};

struct PS_OUTPUT // Входящие данные пиксельного шейдера
{
  float4 pos : SV_POSITION; // Позиция пикселя в проекции (экранная)
  float2 tex : TEXCOORD0; // Координаты текстуры по tu, tv
  float3 norm : TEXCOORD1;
};

PS_OUTPUT VS_Out(VS_INPUT input) {
	PS_OUTPUT output = (PS_OUTPUT)0; // создали нашу структуру
	output.pos = mul(input.pos, world); // трансформация позиции вершины в матрицу мира
	output.pos = mul(output.pos, view); // в матрицу вида
	output.pos = mul(output.pos, proj); // в матрицу проекции
	output.norm = mul(input.norm, world);
	output.tex = input.tex;
	return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  float4 color = 0.35f;
  color += saturate(dot(dir * 0.01, input.norm) * src_color);
  color *= dirt.Sample(sample_linear_mip, input.tex);
  return color;
}

float4 PS_Light(PS_OUTPUT input) : SV_Target {
  return src_color * 2 * ( dirt.Sample(sample_linear_mip, input.tex));
}
