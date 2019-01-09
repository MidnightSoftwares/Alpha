#pragma once

#include <d3d11.h>
#include <functional>

class GraphicsDevice;

class RasterizerState final
{
private:
    RasterizerState(GraphicsDevice* graphicsDevice, ID3D11RasterizerState* handle);

public:
    GraphicsDevice* GraphicsDevice() const;

private:
    ::GraphicsDevice* mGraphicsDevice;
    ID3D11RasterizerState* mHandle;

private:
    friend class Graphics;
};

using RasterizerStateDeleter = std::function<void(RasterizerState*)>;
using RasterizerStatePtr = std::unique_ptr<RasterizerState, RasterizerStateDeleter>;
