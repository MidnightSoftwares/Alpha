#pragma once

#include "../../Utils/Expected.h"

#include <d3d11.h>
#include <wrl/client.h>

class VertexShader final
{
public:
    enum Error
    {
        ReadFileToBufferFailed,
        CreateInputLayoutFailed,
        CreateShaderFailed
    };

private:
    VertexShader(Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
        Microsoft::WRL::ComPtr<ID3D11VertexShader> handle);

public:
    VertexShader(const VertexShader& vertexShader) = delete;
    VertexShader(VertexShader&& vertexShader) = delete;

    VertexShader& operator=(const VertexShader& vertexShader) = delete;
    VertexShader& operator=(VertexShader&& vertexShader) = delete;

    ~VertexShader() = default;

public:
    static Expected<std::unique_ptr<VertexShader>, Error> Create(ID3D11Device* device,
        D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
        const std::wstring& filePath);

public:
    ID3D11InputLayout* InputLayout() const;
    ID3D11VertexShader* Handle() const;

private:
    Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mHandle;
};
