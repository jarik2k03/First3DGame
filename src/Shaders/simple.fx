Texture2D dirt : register(t0);
SamplerState sample_linear_mip : register(s0);

struct Block {
  half1x2 r[4], rc[2];
};
cbuffer Counter : register(b5) {
  int counter;
  int csize;
  int carea;
}
cbuffer ChunkPosBuffer : register(b4) {
  int visible_blocks[4096];
}

cbuffer PositionBuffer : register(b0) { // b0 - индекс буфера
  matrix world;
}
cbuffer LightBuffer : register(b1) { // b1 - индекс буфера
  float4 dir;
  float4 src_color;
  float4 dest_color;
}

cbuffer ViewBuffer : register(b2) { // b2 - матрицы камеры
  matrix view;
}
cbuffer ProjBuffer : register(b3) { // b3 - матрицы камеры
  matrix proj;
}

cbuffer temp : register(b12) {
  struct Block be;
}

struct VS_INPUT {
  float4 pos : POSITION;
  float2 tex : TEXCOORD0;
  float3 norm : NORMAL;
};

struct PS_OUTPUT // Входящие данные пиксельного шейдера
{
  float4 pos : SV_POSITION; // Позиция пикселя в проекции (экранная)
  float2 tex : TEXCOORD0; // Координаты текстуры по tu, tv
  float3 norm : TEXCOORD1;
};

float4x4 CreateTranslationMatrix(float3 translation) {
  float4x4 translationMatrix = (float4x4)0; // Инициализация нулевой матрицы
  // Создание матрицы переноса
  translationMatrix[0][0] = 1.0f;
  translationMatrix[1][1] = 1.0f;
  translationMatrix[2][2] = 1.0f;
  translationMatrix[3][3] = 1.0f;
  translationMatrix[3][0] = translation.x;
  translationMatrix[3][1] = translation.y;
  translationMatrix[3][2] = translation.z;
  return translationMatrix;
}

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
  float x = visible_blocks[counter / csize % csize] / 4;
  float y = visible_blocks[counter % csize] / 4;
  float z = visible_blocks[counter / carea] / 4;

  float4x4 worlld = CreateTranslationMatrix(float3(x, y, z));
  PS_OUTPUT output = (PS_OUTPUT)0; // создали нашу структуру
  output.pos = mul(input.pos, worlld); // трансформация позиции вершины в матрицу мира
  output.pos = mul(output.pos, view); // в матрицу вида
  output.pos = mul(output.pos, proj); // в матрицу проекции
  output.norm = mul(input.norm, worlld);
  output.tex = input.tex;
  return output;
}

float4 PS_Out(PS_OUTPUT input) : SV_Target {
  float4 color = 0.35f;
  color += saturate(dot(dir, input.norm) * src_color);
  color *= dirt.Sample(sample_linear_mip, input.tex);

  float4 c = (float)counter / 255;
  return color;
}

float4 PS_Light(PS_OUTPUT input) : SV_Target {
  return src_color * 2 * (dirt.Sample(sample_linear_mip, input.tex));
}
