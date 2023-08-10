#include "Pyramid.h"

#define H_WARNMSG(hr, text) if (FAILED(hr)) MessageBox( NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Pyramid::Pyramid(ID3D11Device* i_d3dDevice, ID3D11DeviceContext* i_context, D3D_PRIMITIVE_TOPOLOGY drawMode)
{

	AdvVertex vertices[5] = {
		{ XMFLOAT3(0.0f,  1.5f,  0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  0.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  0.0f,  1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  0.0f,  1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) }
	};
	// пирамидка состоит из 6 треугольников =>  18 индексов вершин
    WORD indices[18] = {	// индексы массива vertices[], по которым строятся треугольники
        0,2,1,	/* Треугольник 1 = vertices[0], vertices[2], vertices[3] */
        0,3,4,	/* Треугольник 2 = vertices[0], vertices[3], vertices[4] */
        0,1,3,	/* и т. д. */
        0,4,2,
        1,2,3,
        2,4,3, };


	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA bufData;
	setBuffer(&bd, sizeof(AdvVertex) * 5, D3D11_BIND_VERTEX_BUFFER);
	setBufferData(&bufData, vertices);
	HRESULT hr = i_d3dDevice->CreateBuffer(&bd, &bufData, &vertexBuffer);
	H_WARNMSG(hr, L"Ошибка инициализации вершинного буфера");

	setBuffer(&bd, sizeof(WORD) * 18, D3D11_BIND_INDEX_BUFFER);
	setBufferData(&bufData, indices);
	hr = i_d3dDevice->CreateBuffer(&bd, &bufData, &indexBuffer);
	H_WARNMSG(hr, L"Ошибка инициализации индексного буфера");
	
	unsigned int stride = sizeof(AdvVertex);
	unsigned int offset = 0;

	i_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	i_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	i_context->IASetPrimitiveTopology(drawMode);

	setBuffer(&bd, sizeof(XMMATRIX) * 3, D3D11_BIND_CONSTANT_BUFFER);
	hr = i_d3dDevice->CreateBuffer(&bd, NULL, &constBuffer);
	H_WARNMSG(hr, L"Ошибка инициализации константного буфера");
}

void Pyramid::move(float x, float y, float z)
{

}


 void Pyramid::render(ID3D11DeviceContext* i_context)
{
	i_context->VSSetShader(vShader, NULL, 0);
	i_context->PSSetShader(pShader, NULL, 0);
	i_context->VSSetConstantBuffers(0, 1, &constBuffer); // загрузка буфера в шейдер
	
	i_context->DrawIndexed(18, 0, 0);
}

void Pyramid::setBuffer(D3D11_BUFFER_DESC* bd, int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage)
{
	ZeroMemory(bd, sizeof(*bd));
	bd->Usage = usage;
	bd->ByteWidth = byteWidth;
	bd->BindFlags = bindBuffer; // - тип буфера буфер вершин
	bd->CPUAccessFlags = 0;
}

void Pyramid::setBufferData(D3D11_SUBRESOURCE_DATA* o_bdata, void* w)
{
	ZeroMemory(o_bdata, sizeof(*o_bdata));
	o_bdata->pSysMem = w;
}