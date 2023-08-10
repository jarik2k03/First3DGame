	#include "Shaders.h"

#define H_ERRMSG(hr, text) if (FAILED(hr)){ MessageBox( NULL, text, L"Ошибка шейдеров", MB_OK | MB_ICONHAND); exit(-1);}
#define H_WARNMSG(hr, text) if (FAILED(hr)) MessageBox( NULL, text, L"Ошибка шейдеров", MB_OK | MB_ICONEXCLAMATION);

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
	vertexShaders.clear();
	vertexLayouts.clear();
	pixelShaders.clear();
}
ID3D11VertexShader* Shaders::addVertexShader(ID3D11Device* i_d3dDevice, ID3D11DeviceContext* i_context, LPCWSTR filename, LPCSTR entryPoint, LPCSTR shaderModel)
{
	HRESULT hr = S_OK;
	ID3DBlob* curBlob = NULL;
	hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - функция, описанная в fx
	H_ERRMSG(hr, L"Невозможно скомпилировать файл FX. Пожалуйста, переустановите программу.");
	// создаём шейдер из скомпилированного объекта
	ID3D11VertexShader* vShader;
	hr = i_d3dDevice->CreateVertexShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &vShader);
	H_ERRMSG(hr, L"Не удалость создать вершинный шейдер.");

	int offset = 0;
	D3D11_INPUT_ELEMENT_DESC layout[2] = { // макет вершин
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	int nElements = ARRAYSIZE(layout);
	// создали макет вершин vertexLayout
	ID3D11InputLayout* vLayout;
	hr = i_d3dDevice->CreateInputLayout(layout, nElements, curBlob->GetBufferPointer(), curBlob->GetBufferSize(), &vLayout);
	curBlob->Release();
	H_ERRMSG(hr, L"Не создаётся макет вершин");
	i_context->IASetInputLayout(vLayout);

	vertexShaders.insert(std::make_pair(entryPoint, vShader)); // запись вершинного шейдера в ассоц массив
	vertexLayouts.insert(std::make_pair(entryPoint, vLayout)); // запись вершинного макета в ассоц массив
	return vShader;
}

ID3D11PixelShader* Shaders::addPixelShader(ID3D11Device* i_d3dDevice, LPCWSTR filename, LPCSTR entryPoint, LPCSTR shaderModel)
{
	HRESULT hr = S_OK;
	ID3DBlob* curBlob = NULL;
	hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - функция, описанная в fx
	H_ERRMSG(hr, L"Невозможно скомпилировать файл FX. Пожалуйста, переустановите программу.");
	// создаём шейдер из скомпилированного объекта
	ID3D11PixelShader* pShader;
	hr = i_d3dDevice->CreatePixelShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &pShader);
	H_ERRMSG(hr, L"Не удалось создать пиксельный шейдер.");
	curBlob->Release();

	pixelShaders.insert(std::make_pair(entryPoint, pShader)); // запись пискельного шейдера в ассоц массив
	return pShader;
}

HRESULT Shaders::compileFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** o_blob)
{
	HRESULT hr = S_OK;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	ID3DBlob* errInfo; // список ошибок
	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, shaderFlags, 0, NULL, o_blob, &errInfo, NULL);
	
	if (FAILED(hr)) {
		
		MessageBoxA(NULL, (LPCSTR)errInfo->GetBufferPointer(), (LPCSTR)"Ошибка шейдеров", MB_OK | MB_ICONEXCLAMATION);
		if (errInfo) errInfo->Release();
		exit(-2);
		return hr;
	}
	if (errInfo) errInfo->Release();
	return S_OK;
}

ID3D11VertexShader* Shaders::getVertexShader(LPCSTR entryPoint)
{
	std::map <LPCSTR, ID3D11VertexShader*> ::iterator t = vertexShaders.find(entryPoint);
	return ((t != vertexShaders.end()) ? (t->second) : (NULL));
}

ID3D11InputLayout* Shaders::getVertexLayout(LPCSTR entryPoint)
{
	std::map <LPCSTR, ID3D11InputLayout*> ::iterator t = vertexLayouts.find(entryPoint);
	return ((t != vertexLayouts.end()) ? (t->second) : (NULL));
}


ID3D11PixelShader* Shaders::getPixelShader(LPCSTR entryPoint)
{
	std::map <LPCSTR, ID3D11PixelShader*> ::iterator t = pixelShaders.find(entryPoint);
	return ((t != pixelShaders.end()) ? (t->second) : (NULL));
}