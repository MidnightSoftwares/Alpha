#pragma once

#include "GraphicsDevice.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Buffers/ConstantBuffer.h"
#include "Pipeline.h"

class Graphics final
{
public:
    Graphics() = delete;

public:
    static GraphicsDevicePtr CreateGraphicsDevice(Window* window);

    // TODO: Replace D3D11_USAGE with custom enum.
    static VertexBufferPtr CreateVertexBuffer(GraphicsDevice* graphicsDevice,
        D3D11_USAGE usage, UINT elementSize, UINT elementCount, const void* data);
    // TODO: Replace D3D11_USAGE with custom enum.
    static IndexBufferPtr CreateIndexBuffer(GraphicsDevice* graphicsDevice,
        D3D11_USAGE usage, UINT elementCount, const unsigned int* data);
    // TODO: Add usage parameter.
    static ConstantBufferPtr CreateConstantBuffer(GraphicsDevice* graphicsDevice,
        UINT size, const void* data);

    // TODO: Maybe remove input layout from vertex shader.
    // TODO: Replace D3D11_INPUT_ELEMENT_DESC* with a custom object.
    static VertexShaderPtr CreateVertexShader(GraphicsDevice* graphicsDevice,
        const D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutElements,
        const std::wstring& filePath);
    static PixelShaderPtr CreatePixelShader(GraphicsDevice* graphicsDevice,
        const std::wstring& filePath);

    // TODO: Replace D3D11_RASTERIZER_DESC* with a custom object.
    static PipelinePtr CreatePipeline(GraphicsDevice* graphicsDevice,
        D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
        const VertexShader* vertexShader,
        const D3D11_RASTERIZER_DESC* rasterizerDesc,
        const PixelShader* pixelShader);

public:
    // TODO: Remove this after complete refactoring.
    static void TestRender(const GraphicsDevice* graphicsDevice,
        const VertexBuffer* vertexBuffer, const Pipeline* pipeline);

    static void SwapBuffers(const GraphicsDevice* graphicsDevice);
};
