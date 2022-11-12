#include "BNS_RenderToTexture.h"

#include <exception>
#include <iostream>

#include "BNS_GraphicsEngine.h"

BNS_RenderToTexture::BNS_RenderToTexture(int textureWidth, int textureHeight)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hr;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	hr = BNS_GraphicsEngine::get()->getRenderSystem()->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
	if (FAILED(hr))
	{
		throw std::exception("BNS_RenderToTexture not created successfully");
	}
	// Setup the description of the render target view.
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	hr = BNS_GraphicsEngine::get()->getRenderSystem()->GetDevice()->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(hr))
	{
		throw std::exception("BNS_RenderToTexture not created successfully");
	}

	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hr = BNS_GraphicsEngine::get()->getRenderSystem()->GetDevice()->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
	if (FAILED(hr))
	{
		throw std::exception("BNS_RenderToTexture not created successfully");
	}
}

BNS_RenderToTexture::BNS_RenderToTexture(const BNS_RenderToTexture&)
{
}

BNS_RenderToTexture::~BNS_RenderToTexture()
{
	m_shaderResourceView->Release();
	m_shaderResourceView = nullptr;

	m_renderTargetView->Release();
	m_renderTargetView = nullptr;

	m_renderTargetTexture->Release();
	m_renderTargetTexture = nullptr;
}

ID3D11ShaderResourceView* BNS_RenderToTexture::GetShaderResourceView()
{
	return m_shaderResourceView;
}