#include "World.h"


World::World()
{
	worldMatrix = XMMatrixIdentity();

}

void World::UpdateMatrixes(ID3D11DeviceContext* i_context, XMMATRIX* view, XMMATRIX* perspective, ID3D11Buffer* cBuffer)
{
	static double time = 0.0f;
	static DWORD starttime = 0;
	DWORD curtime = GetTickCount64();
	if (starttime == 0)
		starttime = curtime;
	time = (curtime - starttime) / 1000.f;

	worldMatrix = XMMatrixRotationY(time);

	ConstantBuffer bd;
	bd.world = XMMatrixTranspose(worldMatrix);
	bd.view = XMMatrixTranspose(*view);
	bd.proj = XMMatrixTranspose(*perspective);

	
	/*char c_w[240] = { 0 }, c_v[240] = { 0 }, c_p[240] = { 0 };
	XMMATRIX* p;
	p = &bd.world;
	float bw[4][4] = {p->_11, p->_12, p->_13, p->_14,
					  p->_21, p->_22, p->_23, p->_24,
					  p->_31, p->_32, p->_33, p->_34,
					  p->_41, p->_42, p->_43, p->_44};
	p = &bd.view;
	float bv[4][4] = { p->_11, p->_12, p->_13, p->_14,
					  p->_21, p->_22, p->_23, p->_24,
					  p->_31, p->_32, p->_33, p->_34,
					  p->_41, p->_42, p->_43, p->_44 };
	p = &bd.proj;
	float bp[4][4] = { p->_11, p->_12, p->_13, p->_14,
					  p->_21, p->_22, p->_23, p->_24,
					  p->_31, p->_32, p->_33, p->_34,
					  p->_41, p->_42, p->_43, p->_44 };
	
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) {

			sprintf_s(c_w, 240, "%.2f ", bw[i][j]);
			sprintf_s(c_v, 240, "%.2f ", bv[i][j]);
			sprintf_s(c_p, 240, "%.2f ", bp[i][j]);
		}
		//sprintf_s(c_w, 12, "\n");
		//sprintf_s(c_v, 12, "\n");
		//sprintf_s(c_p, 12, "\n");
	}

	char displaymatrixes[360];
	sprintf_s(displaymatrixes, 120, "%s", c_w);
	sprintf_s(displaymatrixes, 120, "%s", c_v);
	sprintf_s(displaymatrixes, 120, "%s", c_p);
	
	MessageBoxA(NULL, c_w, "Проверка матриц", MB_OK | MB_ICONEXCLAMATION);*/

	// загружаем структуру bd в константный буфер
	i_context->UpdateSubresource(cBuffer, 0, NULL, &bd, 0, 0);
	// загружаем в константный буфер

}

XMMATRIX* World:: getWorldMatrix()
{
	return &worldMatrix;
}