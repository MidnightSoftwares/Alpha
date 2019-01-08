#include "VertexShader.h"
#include "../../Core/Logger.h"

#include <d3dcompiler.h>

VertexShader::VertexShader(Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
    Microsoft::WRL::ComPtr<ID3D11VertexShader> handle) :
    mInputLayout{std::move(inputLayout)},
    mHandle{std::move(handle)}
{
}

Expected<std::unique_ptr<VertexShader>, VertexShader::Error> VertexShader::Create(
    ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
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

    // Create input layout
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

    hr = device->CreateInputLayout(layoutDesc, layoutElements,
        buffer->GetBufferPointer(), buffer->GetBufferSize(),
        inputLayout.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateInputLayout: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Vertex shader file path: " + filePath);
        return CreateInputLayoutFailed;
    }

    // Create shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> handle;

    hr = device->CreateVertexShader(buffer->GetBufferPointer(), buffer->GetBufferSize(),
        nullptr, handle.GetAddressOf());

    if (FAILED(hr))
    {
        LOG_ERROR(L"ID3D11Device::CreateVertexShader: " + DebugUtils::COMErrorMessage(hr));
        LOG_INFO(L"Shader file path: " + filePath);
        return CreateShaderFailed;
    }

    // Create vertex shader instance
    return std::unique_ptr<VertexShader>{
        new VertexShader{
            std::move(inputLayout),
            std::move(handle)
        }
    };
}

ID3D11InputLayout* VertexShader::InputLayout() const
{
    return mInputLayout.Get();
}

ID3D11VertexShader* VertexShader::Handle() const
{
    return mHandle.Get();
}
