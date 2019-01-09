#include "VertexShader.h"

VertexShader::VertexShader(::GraphicsDevice* graphicsDevice, ID3D11InputLayout* inputLayout,
    ID3D11VertexShader* handle):
    mGraphicsDevice{graphicsDevice},
    mInputLayout{inputLayout},
    mHandle{handle}
{
}

GraphicsDevice* VertexShader::GraphicsDevice() const
{
    return mGraphicsDevice;
}
