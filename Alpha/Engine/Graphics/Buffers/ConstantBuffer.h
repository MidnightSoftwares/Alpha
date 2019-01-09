#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class ConstantBuffer final
{
private:
    ConstantBuffer(GraphicsDevice* graphicsDevice, UINT size, ID3D11Buffer* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    UINT mSize;
    ID3D11Buffer* mHandle;

private:
    friend class Graphics;
};

using ConstantBufferDeleter = std::function<void(ConstantBuffer*)>;
using ConstantBufferPtr = std::unique_ptr<ConstantBuffer, ConstantBufferDeleter>;
