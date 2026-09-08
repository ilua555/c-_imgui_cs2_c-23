module;
#define WIN32_LEAN_AND_MEAN
#define STB_IMAGE_IMPLEMENTATION
#include <Windows.h>
#include <d3d11.h>
#include "stb_image/stb_image.h"
export module conveer_photo;



export auto LoadPhoto(ID3D11Device* pDevice, unsigned char* massiv_photo_bytes, int len_massiv_bytes, int* x, int* y, int* chanels_in_file) -> ID3D11ShaderResourceView* {
	unsigned char* pixels = stbi_load_from_memory(massiv_photo_bytes, len_massiv_bytes, x, y, chanels_in_file, 4);
	if (pixels == nullptr) return nullptr;
	D3D11_TEXTURE2D_DESC desc{};
    desc.Width = *x;
    desc.Height = *y;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subres{};
    subres.pSysMem = pixels;
    subres.SysMemPitch = *x * 4;
   
    ID3D11Texture2D* pTexture2D = nullptr;
    HRESULT hrTexture = pDevice->CreateTexture2D(&desc, &subres, &pTexture2D);
    if (FAILED(hrTexture)) return nullptr;

    stbi_image_free(pixels);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc_shader{};
    desc_shader.Format = desc.Format;
    desc_shader.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc_shader.Texture2D.MipLevels = 1;

    ID3D11ShaderResourceView* ShaderResourse = nullptr;
    HRESULT hrShader = pDevice->CreateShaderResourceView(pTexture2D, &desc_shader, &ShaderResourse);
    pTexture2D->Release();

    if (ShaderResourse == nullptr || FAILED(hrShader)) return nullptr;


    return ShaderResourse;
}