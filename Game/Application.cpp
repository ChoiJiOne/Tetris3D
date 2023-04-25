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
#include "StaticMesh.h"
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

	std::vector<Vertex::PositionColor> vertices = {
		Vertex::PositionColor(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		Vertex::PositionColor(DirectX::XMFLOAT3(+0.0f, +1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
		Vertex::PositionColor(DirectX::XMFLOAT3(+1.0f, -1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
	};

	std::vector<uint32_t> indices = { 0, 1, 2 };

	std::unique_ptr<StaticMesh> mesh = std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices);

	while (!bIsDone)
	{
		InputManager::Get().Tick();

		RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
		RenderManager::Get().SetWindowViewport();

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