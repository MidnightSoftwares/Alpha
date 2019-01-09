#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class PixelShader final
{
private:
    PixelShader(GraphicsDevice* graphicsDevice, ID3D11PixelShader* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    ID3D11PixelShader* mHandle;

private:
    friend class Graphics;
};

using PixelShaderDeleter = std::function<void(PixelShader*)>;
using PixelShaderPtr = std::unique_ptr<PixelShader, PixelShaderDeleter>;
