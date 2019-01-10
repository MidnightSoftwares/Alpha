#include "Pipeline.h"

Pipeline::Pipeline(::GraphicsDevice* graphicsDevice,
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
    const VertexShader* vertexShader,
    ID3D11RasterizerState* rasterizerState,
    const PixelShader* pixelShader) :
    mGraphicsDevice{graphicsDevice},
    mPrimitiveTopology{primitiveTopology},
    mVertexShader{vertexShader},
    mRasterizerState{rasterizerState},
    mPixelShader{pixelShader}
{
}

GraphicsDevice* Pipeline::GraphicsDevice() const
{
    return mGraphicsDevice;
}
