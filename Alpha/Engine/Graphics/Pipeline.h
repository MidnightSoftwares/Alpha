#pragma once

#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"

class Pipeline final
{
private:
    Pipeline(GraphicsDevice* graphicsDevice,
        D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
        const VertexShader* vertexShader,
        ID3D11RasterizerState* rasterizerState,
        const PixelShader* pixelShader,
        ID3D11DepthStencilState* depthStencilState);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    D3D_PRIMITIVE_TOPOLOGY mPrimitiveTopology;
    const VertexShader* mVertexShader;
    ID3D11RasterizerState* mRasterizerState;
    const PixelShader* mPixelShader;
    ID3D11DepthStencilState* mDepthStencilState;

private:
    friend class Graphics;
};

using PipelineDeleter = std::function<void(Pipeline*)>;
using PipelinePtr = std::unique_ptr<Pipeline, PipelineDeleter>;
