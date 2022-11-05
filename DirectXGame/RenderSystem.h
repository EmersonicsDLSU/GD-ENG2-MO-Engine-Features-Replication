#pragma once
#include <d3d11.h>

#include "EnumHandler.h"
#include "Prerequisites.h"

/*
 * Initializes the IDXGI components and setting the graphics pipeline stages.
 * Handles and provides all the functionalities made available by a graphics API
 * like �DirectX�, �Vulkan�, �Metal� and so onhandle and provide all the functionalities
 * made available by a graphics API like �DirectX�, �Vulkan�, �Metal� and so on
*/
class RenderSystem
{
public:
	//Initialize the GraphicsEngine and DirectX 11 Device
	RenderSystem();
	//Release all the resources loaded
	~RenderSystem();
public:
	SwapChainPtr CreateSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr GetImmediateDeviceContext();
	VertexBufferPtr CreateVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, InputLayoutType il_type);
	IndexBufferPtr CreateIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr CreateConstantBuffer(void* buffer, UINT size_buffer);
	VertexShaderPtr CreateVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr CreatePixelShader(const void* shader_byte_code, size_t byte_code_size);
	BlenderPtr CreateBlender();
public:
	bool CompileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool CompilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);

	void ReleaseCompiledShader();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	
private:
	DeviceContextPtr m_imm_device_context;
private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

private:
	// a buffer that contains the compiled vertex_tex shader and its size in the memory
	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;
	friend class Blender;
	friend class InputLayoutManager;
	friend class AppWindow;

};

