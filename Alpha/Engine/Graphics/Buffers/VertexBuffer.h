#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class VertexBuffer final
{
private:
    VertexBuffer(GraphicsDevice* graphicsDevice,
        UINT elementSize, UINT elementCount, ID3D11Buffer* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    UINT mElementSize;
    UINT mElementCount;
    ID3D11Buffer* mHandle;

private:
    friend class Graphics;
};

using VertexBufferDeleter = std::function<void(VertexBuffer*)>;
using VertexBufferPtr = std::unique_ptr<VertexBuffer, VertexBufferDeleter>;
