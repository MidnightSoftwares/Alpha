#pragma once

#include "VertexShader.h"
#include "PixelShader.h"

class Pipeline final
{
public:
    enum Error
    {
        CreateVertexShaderFailed,
        CreatePixelShaderFailed,
        CreateInputLayoutFailed
    };

private:
    Pipeline(std::unique_ptr<VertexShader> vertexShader, std::unique_ptr<PixelShader> pixelShader);

public:
    Pipeline(const Pipeline& pipeline) = delete;
    Pipeline(Pipeline&& pipeline) = delete;

    Pipeline& operator=(const Pipeline& pipeline) = delete;
    Pipeline& operator=(Pipeline&& pipeline) = delete;

    ~Pipeline() = default;

public:
    static Expected<std::unique_ptr<Pipeline>, Error> Create(ID3D11Device* device,
        D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
        const std::wstring& vertexFilePath, const std::wstring& pixelFilePath);

public:
    VertexShader* VertexShader() const;
    PixelShader* PixelShader() const;

private:
    std::unique_ptr<::VertexShader> mVertexShader;
    std::unique_ptr<::PixelShader> mPixelShader;
};
