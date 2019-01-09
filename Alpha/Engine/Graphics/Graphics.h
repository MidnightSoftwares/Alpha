#pragma once

#include "GraphicsDevice.h"
#include "RasterizerState.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/ConstantBuffer.h"

class Graphics final
{
public:
    Graphics() = delete;

public:
    static GraphicsDevicePtr CreateGraphicsDevice(Window* window);
    // TODO: Replace D3D11_RASTERIZER_DESC* with a custom object.
    static RasterizerStatePtr CreateRasterizerState(GraphicsDevice* graphicsDevice,
        const D3D11_RASTERIZER_DESC* description);
    // TODO: Maybe remove input layout from vertex shader.
    // TODO: Replace D3D11_INPUT_ELEMENT_DESC* with a custom object.
    static VertexShaderPtr CreateVertexShader(GraphicsDevice* graphicsDevice,
        const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
        const std::wstring& filePath);
    static PixelShaderPtr CreatePixelShader(GraphicsDevice* graphicsDevice,
        const std::wstring& filePath);
    // TODO: Replace D3D11_USAGE with custom enum.
    static VertexBufferPtr CreateVertexBuffer(GraphicsDevice* graphicsDevice,
        D3D11_USAGE usage, UINT elementSize, UINT elementCount, const void* data);
    // TODO: Replace D3D11_USAGE with custom enum.
    static IndexBufferPtr CreateIndexBuffer(GraphicsDevice* graphicsDevice,
        D3D11_USAGE usage, UINT elementCount, const unsigned int* data);
    static ConstantBufferPtr CreateConstantBuffer(GraphicsDevice* graphicsDevice,
        UINT size, const void* data);

public:
    // TODO: Remove this after complete refactoring.
    static void TestRender(const GraphicsDevice* graphicsDevice,
        const RasterizerState* rasterizerState,
        const VertexShader* vertexShader, const PixelShader* pixelShader,
        const VertexBuffer* vertexBuffer);
};
