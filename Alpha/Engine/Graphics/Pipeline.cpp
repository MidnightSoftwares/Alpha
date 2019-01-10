#include "Pipeline.h"

Pipeline::Pipeline(::GraphicsDevice* graphicsDevice,
    D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
    const VertexShader* vertexShader,
    ID3D11RasterizerState* rasterizerState,
    const PixelShader* pixelShader,
    ID3D11DepthStencilState* depthStencilState) :
    mGraphicsDevice{graphicsDevice},
    mPrimitiveTopology{primitiveTopology},
    mVertexShader{vertexShader},
    mRasterizerState{rasterizerState},
    mPixelShader{pixelShader},
    mDepthStencilState{depthStencilState}
{
}

GraphicsDevice* Pipeline::GraphicsDevice() const
{
    return mGraphicsDevice;
}
