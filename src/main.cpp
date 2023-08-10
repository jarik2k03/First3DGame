#include <iostream>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <windows.h>
#include "Rendering/Device.h"
#include "Rendering/Window.h"
#include "Rendering/Shaders.h"
#include "Entity/Triangle.h"
#include "Entity/Pyramid.h"
#include "Entity/Camera.h"
#include "World.h"

#define D getDevice() 
#define C getContext()
#define SHADERPATH L"src\\Shaders\\simple.fx"

int main_cycle(Device** myDevice);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Window mainWindow(hInstance, 1280, 720); // задаём туда дескриптор приложения
	Device* d3d = new Device(mainWindow.getWd());
	Shaders* shaderController = new Shaders();
	World* overlord = new World();
	Camera* eye = new Camera(mainWindow.getWd(), XM_PIDIV4);
	
	Pyramid* piramidka = new Pyramid(d3d->D, d3d->C);


	ID3D11VertexShader* v = shaderController->addVertexShader(d3d->D, d3d->C, SHADERPATH, "VS_Out", "vs_5_0");
	ID3D11PixelShader*  p = shaderController->addPixelShader(d3d->D, SHADERPATH, "PS_Out", "ps_5_0");

	piramidka->bindShaders(v, p); 
	//main_cycle(&dxDevice);

	MSG msg = { 0 };
	while (msg.message != WM_QUIT && msg.wParam != VK_ESCAPE)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.wParam == VK_F11)
				d3d->switchFullScreen();
		} else {
			
			overlord->UpdateMatrixes(d3d->C, eye->getProjMatrix(), eye->getViewMatrix(), piramidka->getConstBuffer());
			d3d->renderStart();
			
			piramidka->render(d3d->C);
			
			d3d->renderEnd();
		}
	}

	delete piramidka;
	delete eye;
	delete overlord;
	delete shaderController;
	delete d3d;
	return 0;
}

int main_cycle(Device** myDevice)
{
	return 0;
}

