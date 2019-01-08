#pragma once

#include "../../Utils/Expected.h"

#include <d3d11.h>
#include <wrl/client.h>

class PixelShader final
{
public:
    enum Error
    {
        ReadFileToBufferFailed,
        CreateShaderFailed
    };

private:
    explicit PixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> handle);

public:
    PixelShader(const PixelShader& pixelShader) = delete;
    PixelShader(PixelShader&& pixelShader) = delete;

    PixelShader& operator=(const PixelShader& pixelShader) = delete;
    PixelShader& operator=(PixelShader&& pixelShader) = delete;

    ~PixelShader() = default;

public:
    static Expected<std::unique_ptr<PixelShader>, Error> Create(ID3D11Device* device,
        const std::wstring& filePath);

public:
    ID3D11PixelShader* Handle() const;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mHandle;
};
