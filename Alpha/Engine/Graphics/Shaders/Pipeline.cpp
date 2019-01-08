#include "Pipeline.h"

Pipeline::Pipeline(std::unique_ptr<::VertexShader> vertexShader,
    std::unique_ptr<::PixelShader> pixelShader) :
    mVertexShader{std::move(vertexShader)},
    mPixelShader{std::move(pixelShader)}
{
}

Expected<std::unique_ptr<Pipeline>, Pipeline::Error> Pipeline::Create(ID3D11Device* device,
    D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
    const std::wstring& vertexFilePath, const std::wstring& pixelFilePath)
{
    // Create vertex shader
    auto vertexShaderE = VertexShader::Create(device, layoutDesc, layoutElements, vertexFilePath);

    if (!vertexShaderE.Valid())
    {
        return CreateVertexShaderFailed;
    }

    auto vertexShader = std::move(vertexShaderE.Value());

    // Create pixel shader
    auto pixelShaderE = PixelShader::Create(device, pixelFilePath);

    if (!pixelShaderE.Valid())
    {
        return CreatePixelShaderFailed;
    }

    auto pixelShader = std::move(pixelShaderE.Value());

    // Create pipeline instance
    return std::unique_ptr<Pipeline>{
        new Pipeline{
            std::move(vertexShader),
            std::move(pixelShader)
        }
    };
}

VertexShader* Pipeline::VertexShader() const
{
    return mVertexShader.get();
}

PixelShader* Pipeline::PixelShader() const
{
    return mPixelShader.get();
}
