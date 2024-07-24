#pragma once
#include <d3d9.h>
#include <d3dx9math.h>

enum class DepthTest {
	Visible, Darken, Invisible
};

class PrimitiveRenderer {

public:
	PrimitiveRenderer();
	void Begin(D3DXMATRIX* viewMatrix);
	void End();
	void DrawLine(D3DXCOLOR color, int count, ...);
	void DrawRect(D3DXCOLOR color, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 d);
	void DrawRect(D3DXCOLOR color, D3DXVECTOR4 a, D3DXVECTOR4 b, D3DXVECTOR4 c, D3DXVECTOR4 d);

private:
	D3DXMATRIX identity;	
	IDirect3DDevice9* d3d9device;
	IDirect3DStateBlock9* prevStateBlock = nullptr;
};
