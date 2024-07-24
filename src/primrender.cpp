#include "primrender.h"

#include <cassert>

PrimitiveRenderer::PrimitiveRenderer() {		
	D3DXMatrixIdentity(&identity);
}

void PrimitiveRenderer::Begin(D3DXMATRIX* viewMatrix) {
	d3d9device = *(IDirect3DDevice9**)0x982bdc;

	if (!d3d9device)
		return;

	HRESULT res = S_OK;
	res |= d3d9device->CreateStateBlock(D3DSBT_ALL, &prevStateBlock);
	
	res |= d3d9device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);	

	res |= d3d9device->SetTransform(D3DTS_PROJECTION, &identity);
	res |= d3d9device->SetTransform(D3DTS_VIEW, viewMatrix);
	res |= d3d9device->SetTransform(D3DTS_WORLD, &identity);

	res |= d3d9device->SetPixelShader(NULL);
	res |= d3d9device->SetVertexShader(NULL);
	res |= d3d9device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	res |= d3d9device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	res |= d3d9device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	res |= d3d9device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	res |= d3d9device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	res |= d3d9device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
	res |= d3d9device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	res |= d3d9device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	res |= d3d9device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	res |= d3d9device->SetRenderState(D3DRS_ZENABLE, TRUE);
	res |= d3d9device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	res |= d3d9device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	res |= d3d9device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	res |= d3d9device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	res |= d3d9device->SetRenderState(D3DRS_CLIPPING, TRUE);
	res |= d3d9device->SetRenderState(D3DRS_LIGHTING, FALSE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	res |= d3d9device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	res |= d3d9device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	res |= d3d9device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	res |= d3d9device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	res |= d3d9device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	assert(res == S_OK);
}

void PrimitiveRenderer::End()
{
	if (!d3d9device)
		return;

	prevStateBlock->Apply();
	prevStateBlock->Release();
}

void PrimitiveRenderer::DrawLine(D3DXCOLOR color, int count, ...)
{	
	if (!d3d9device)
		return;

	D3DCOLOR c = ((int)(color.a * 255.f)) << 24 | ((int)(color.r * 255.f)) << 16 | ((int)(color.g * 255.f) << 8) | ((int)(color.b * 255.f));

	struct Vertex {
		float x, y, z;
		D3DCOLOR col;
	};
	va_list args;
	va_start(args, count);

	Vertex vertices[64];
	if (count > 64)
		count = 64;
	for (int i = 0; i < count; i++) {
		D3DXVECTOR4 vtx = va_arg(args, D3DXVECTOR4);
		vertices[i] = { vtx.x, vtx.y, vtx.z, c };
	}

	va_end(args);

	HRESULT res = S_OK;	
	res |= d3d9device->DrawPrimitiveUP(D3DPT_LINESTRIP, count - 1, (void*)vertices, sizeof(Vertex));

	assert(res == S_OK);
}

void PrimitiveRenderer::DrawRect(D3DXCOLOR color, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c, D3DXVECTOR3 d)
{
	if (!d3d9device)
		return;

	D3DCOLOR col = ((int)(color.a * 255.f)) << 24 | ((int)(color.r * 255.f)) << 16 | ((int)(color.g * 255.f) << 8) | ((int)(color.b * 255.f));

	struct Vertex {
		float x, y, z;
		D3DCOLOR col;
	};

	Vertex vertices[6] = {
		{ a.x, a.y, a.z, col },
		{ b.x, b.y, b.z, col },
		{ c.x, c.y, c.z, col },
		{ c.x, c.y, c.z, col },
		{ d.x, d.y, d.z, col },
		{ a.x, a.y, a.z, col },
	};

	HRESULT res = S_OK;
	res |= d3d9device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, (void*)vertices, sizeof(Vertex));

	assert(res == S_OK);
}

void PrimitiveRenderer::DrawRect(D3DXCOLOR color, D3DXVECTOR4 a, D3DXVECTOR4 b, D3DXVECTOR4 c, D3DXVECTOR4 d) {
	DrawRect(color, D3DXVECTOR3(a.x, a.y, a.z), D3DXVECTOR3(b.x, b.y, b.z), D3DXVECTOR3(c.x, c.y, c.z), D3DXVECTOR3(d.x, d.y, d.z));
}
