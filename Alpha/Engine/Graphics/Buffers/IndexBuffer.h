#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class IndexBuffer final
{
private:
    IndexBuffer(GraphicsDevice* graphicsDevice, UINT elementCount, ID3D11Buffer* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

public:
    static const UINT ELEMENT_SIZE = sizeof(unsigned int);

private:
    ::GraphicsDevice* mGraphicsDevice;
    UINT mElementCount;
    ID3D11Buffer* mHandle;

private:
    friend class Graphics;
};

using IndexBufferDeleter = std::function<void(IndexBuffer*)>;
using IndexBufferPtr = std::unique_ptr<IndexBuffer, IndexBufferDeleter>;
