#define _CRTDBG_MAP_ALLOC

#include <SDL2/SDL.h>

#include <crtdbg.h>
#include <memory>

#include "CommandLine.h"
#include "CrashHandler.h"
#include "ColorNoEffectShader.h"
#include "Window.h"
#include "InputManager.h"
#include "StringHelper.hpp"
#include "RenderManager.h"


struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};


/**
 * @brief 애플리케이션을 초기화하고 실행합니다.
 * 
 * @param argc 명령행 인자의 수입니다.
 * @param argv 명령행 인자입니다.
 *
 * @throws
 * - 초기화에 실패하면 표준 예외를 던집니다.
 * - 실행에 실패하면 표준 예외를 던집니다.
 */
void RunApplication(int32_t argc, char** argv)
{
	CommandLine::Parse(argc, argv);
	CrashHandler::SetCrashDumpFilePath(CommandLine::GetValue("Crash"));

	CHECK(SDL_Init(SDL_INIT_EVERYTHING) == 0, "failed to initialize SDL2...");
	
	std::unique_ptr<Window> window = std::make_unique<Window>("Tetris3D", 200, 200, 1000, 800, EWindowFlags::SHOWN | EWindowFlags::RESIZABLE);

	bool bIsDone = false;

	InputManager::Get().Setup();
	RenderManager::Get().Setup(window.get());

	InputManager::Get().BindWindowEventAction(EWindowEvent::CLOSE, [&]() { bIsDone = true; });
	InputManager::Get().BindWindowEventAction(EWindowEvent::RESIZED, [&]() { RenderManager::Get().Resize(); });

	std::wstring shaderSourcePath = StringHelper::Convert(CommandLine::GetValue("Shader"));
	std::unique_ptr<ColorNoEffectShader> shaderEffect = std::make_unique<ColorNoEffectShader>(
		RenderManager::Get().GetDevice(),
		shaderSourcePath + L"ColorNoEffectVS.hlsl",
		shaderSourcePath + L"ColorNoEffectPS.hlsl"
	);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -6.0f, 0.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	shaderEffect->SetWorldMatrix(DirectX::XMMatrixIdentity());
	shaderEffect->SetViewMatrix(DirectX::XMMatrixLookAtLH(eye, at, up));
	shaderEffect->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, window->GetAspectRatio(), 0.01f, 1000.0f));

	ID3D11Buffer* vertexBuffer = nullptr;
	int32_t vertexCount = 3;

	ID3D11Buffer* indexBuffer = nullptr;
	int32_t indexCount = 3;

	VertexType vertices[3] = {
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+0.0f, +1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT3(+1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	};

	unsigned int indices[3] = {
		0, 1, 2
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	RenderManager::Get().GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	RenderManager::Get().GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

	while (!bIsDone)
	{
		InputManager::Get().Tick();

		RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
		RenderManager::Get().SetWindowViewport();

		shaderEffect->Bind(RenderManager::Get().GetContext());

		unsigned int stride = sizeof(VertexType);
		unsigned int offset = 0;

		RenderManager::Get().GetContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		RenderManager::Get().GetContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		RenderManager::Get().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		RenderManager::Get().GetContext()->DrawIndexed(indexCount, 0, 0);

		RenderManager::Get().EndFrame();
	}

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	RenderManager::Get().Cleanup();
	InputManager::Get().Cleanup();

	shaderEffect.reset();
	window.reset();
	SDL_Quit();
}


/**
 * @brief 애플리케이션의 진입점입니다.
 *
 * @param argc 명령행 인자의 수입니다.
 * @param argv 명령행 인자입니다.
 *
 * @return 다른 프로그램에 전달할 수 있는 상태 코드를 반환합니다.
 */
int32_t main(int32_t argc, char** argv)
{
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	__try
	{
		RunApplication(argc, argv);
	}
	__except (CrashHandler::DetectApplicationCrash(GetExceptionInformation()))
	{
		CrashHandler::CrashErrorMessageBox();
	}

	return 0;
}