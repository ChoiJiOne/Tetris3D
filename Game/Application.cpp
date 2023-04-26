#define _CRTDBG_MAP_ALLOC

#include <SDL2/SDL.h>

#include <crtdbg.h>
#include <memory>

#include "CommandLine.h"
#include "CrashHandler.h"
#include "ColorNoEffectShader.h"
#include "GameTimer.h"
#include "TextureNoEffectShader.h"
#include "Window.h"
#include "InputManager.h"
#include "StringHelper.hpp"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "Vertex.h"


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
	std::unique_ptr<TextureNoEffectShader> shaderEffect = std::make_unique<TextureNoEffectShader>(
		RenderManager::Get().GetDevice(),
		shaderSourcePath + L"TextureNoEffectVS.hlsl",
		shaderSourcePath + L"TextureNoEffectPS.hlsl"
	);

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(6.0f, 6.0f, -6.0f, 0.0f);
	DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	shaderEffect->SetWorldMatrix(DirectX::XMMatrixIdentity());
	shaderEffect->SetViewMatrix(DirectX::XMMatrixLookAtLH(eye, at, up));
	shaderEffect->SetProjectionMatrix(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, window->GetAspectRatio(), 0.01f, 1000.0f));

	std::string texturePath = CommandLine::GetValue("Content");
	std::unique_ptr<Texture2D> texture = std::make_unique<Texture2D>(
		RenderManager::Get().GetDevice(),
		texturePath + "RedBlock.png"
	);

	std::vector<Vertex::PositionUV> vertices = {
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),

		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		Vertex::PositionUV(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
	};

	std::vector<uint32_t> indices = {
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22,
	};

	std::unique_ptr<StaticMesh> mesh = std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices);

	GameTimer gameTimer;
	gameTimer.Reset();

	RenderManager::Get().SetAlphaBlend(true);
	RenderManager::Get().SetDepthBuffer(true);
	RenderManager::Get().SetFillMode(true);

	while (!bIsDone)
	{
		InputManager::Get().Tick();
		gameTimer.Tick();

		RenderManager::Get().BeginFrame(1.0f, 1.0f, 1.0f, 1.0f);
		RenderManager::Get().SetWindowViewport();

		shaderEffect->SetTexture(texture.get());
		shaderEffect->Bind(RenderManager::Get().GetContext());
		mesh->Draw(RenderManager::Get().GetContext());

		RenderManager::Get().EndFrame();
	}

	mesh.reset();
	shaderEffect.reset();

	RenderManager::Get().Cleanup();
	InputManager::Get().Cleanup();

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