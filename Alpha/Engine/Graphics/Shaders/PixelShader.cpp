#include "PixelShader.h"
#include "../../Core/Logger.h"

#include <d3dcompiler.h>

PixelShader::PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> handle) :
    mHandle{std::move(handle)}
{
}

Expected<std::unique_ptr<PixelShader>, PixelShader::Error> PixelShader::Create(ID3D11Device* device,
    const std::wstring& filePath)
{
    // Read file to buffer
    Microsoft::WRL::ComPtr<ID3D10Blob> buffer;

    auto hr = D3DReadFileToBlob(filePath.c_str(), buffer.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"D3DReadFileToBlob: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Shader file path: " + filePath);
        return ReadFileToBufferFailed;
    }

    // Create shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> handle;

    hr = device->CreatePixelShader(buffer->GetBufferPointer(), buffer->GetBufferSize(),
        nullptr, handle.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreatePixelShader: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Shader file path: " + filePath);
        return CreateShaderFailed;
    }

    // Create pixel shader instance
    return std::unique_ptr<PixelShader>{
        new PixelShader{
            std::move(handle)
        }
    };
}

ID3D11PixelShader* PixelShader::Handle() const
{
    return mHandle.Get();
}
