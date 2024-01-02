static int block_counter = 0;

Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

struct Block {
  half id;
  half number;
};

cbuffer ChunkPosBuffer : register(b4) {
    half visible_blocks[4096];
}

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
    output.pos = input.pos + world._m30_m31_m32_m33; // трансформация позиции вершины в матрицу мира
	output.pos = mul(output.pos, view); // в матрицу вида
	output.pos = mul(output.pos, proj); // в матрицу проекции
	output.norm = mul(input.norm, world);
	output.tex = input.tex;
	return output;
}

PS_OUTPUT VS_Chunk(VS_INPUT input) {
  float4 position = (1, 112 / 16 % 16, 112 % 16, 112 / 256); // 112 Блок
  PS_OUTPUT output = (PS_OUTPUT)0; // создали нашу структуру
  output.pos = input.pos + position; // трансформация позиции вершины в матрицу мира
  output.pos = mul(output.pos, view); // в матрицу вида
  output.pos = mul(output.pos, proj); // в матрицу проекции
  output.norm = mul(input.norm, world);
  output.tex = input.tex;
  block_counter++;
  return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  float4 color = 0.35f;
  color += saturate(dot(dir * 0.1, input.norm) * src_color);
  color *= dirt.Sample(sample_linear_mip, input.tex);
  return color;
}

float4 PS_Light(PS_OUTPUT input) : SV_Target {
  return src_color * 2 * ( dirt.Sample(sample_linear_mip, input.tex));
}
