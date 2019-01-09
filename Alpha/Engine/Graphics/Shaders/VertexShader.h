#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class VertexShader final
{
private:
    VertexShader(GraphicsDevice* graphicsDevice,
        ID3D11InputLayout* inputLayout, ID3D11VertexShader* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    ID3D11InputLayout* mInputLayout;
    ID3D11VertexShader* mHandle;

private:
    friend class Graphics;
};

using VertexShaderDeleter = std::function<void(VertexShader*)>;
using VertexShaderPtr = std::unique_ptr<VertexShader, VertexShaderDeleter>;
