
Texture2D dirt : register(t0);

cbuffer ConstantBuffer : register (b0) // b0 - индекс буфера
{
	matrix world;
	matrix view;
	matrix proj;
}

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 coords : TEXCOORD0;
};

VS_OUTPUT VS_Out(float4 pos : POSITION, float2 coord : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0; // создали нашу структуру
	output.pos = mul(pos, world); // трансформация позиции вершины в матрицу мира
	output.pos = mul(output.pos, view); // в матрицу вида
	output.pos = mul(output.pos, proj); // в матрицу проекции
	output.coords = coord;
	// Оставляем координаты точки без изменений
	return output;
}

float2 PS_Out(VS_OUTPUT input) : SV_Target
{
	return input.coords;
}
