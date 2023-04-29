#define _CRTDBG_MAP_ALLOC

#include <SDL2/SDL.h>

#include <crtdbg.h>
#include <memory>

#include "Block.h"
#include "ColorNoEffectShader.h"
#include "CommandLine.h"
#include "ContentManager.h"
#include "CrashHandler.h"
#include "FixCamera.h"
#include "GameTimer.h"
#include "GeometryGenerator.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "StringHelper.hpp"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "Vertex.h"
#include "Window.h"
#include "WorldManager.h"


/**
 * @brief 셰이더 파일을 로딩합니다.
 * 
 * @throws 셰이더 파일 로딩에 실패하면 C++ 표준 예외를 던집니다.
 */
void LoadShader()
{
	std::wstring shaderSourcePath = StringHelper::Convert(CommandLine::GetValue("Shader"));

	ContentManager::Get().AddEffectShader(
		"ColorNoEffectShader",
		std::make_unique<ColorNoEffectShader>(
			RenderManager::Get().GetDevice(),
			shaderSourcePath + L"ColorNoEffectVS.hlsl",
			shaderSourcePath + L"ColorNoEffectPS.hlsl"
		)
	);

	ContentManager::Get().AddEffectShader(
		"TextureNoEffectShader",
		std::make_unique<TextureNoEffectShader>(
			RenderManager::Get().GetDevice(),
			shaderSourcePath + L"TextureNoEffectVS.hlsl",
			shaderSourcePath + L"TextureNoEffectPS.hlsl"
		)
	);
}


/**
 * @brief 텍스처 파일을 로딩합니다.
 * 
 * @throws 텍스처 파일 로딩에 실패하면 C++ 표준 예외를 던집니다.
 */
void LoadTexture()
{
	std::array<std::string, 9> textures = {
		"BlueBlock",
		"CyanBlock",
		"GrayBlock",
		"GreenBlock",
		"MagentaBlock",
		"OrangeBlock",
		"PurpleBlock",
		"RedBlock",
		"YellowBlock",
	};

	std::string texturePath = CommandLine::GetValue("Content");
	for (const auto& texture : textures)
	{
		ContentManager::Get().AddTexture2D(
			texture,
			std::make_unique<Texture2D>(
				RenderManager::Get().GetDevice(),
				StringHelper::Format("%s%s%s", texturePath.c_str(), texture.c_str(), ".png")
			)
		);
	}
}


/**
 * @brief 정적 메시를 로딩합니다.
 *
 * @throws 정적 메시 로딩에 실패하면 C++ 표준 예외를 던집니다.
 */
void LoadStaticMesh()
{
	std::vector<Vertex::PositionUV> vertices;
	std::vector<uint32_t> indices;
	GeometryGenerator::CreateBox(2.0f, 2.0f, 2.0f, vertices, indices);

	ContentManager::Get().AddStaticMesh(
		"Block",
		std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices)
	);
}


/**
 * @brief 게임 오브젝트를 로딩합니다.
 * 
 * @throws 게임 오브젝트 로딩에 실패하면 C++ 표준 예외를 던집니다.
 */
void LoadGameObject()
{
	float aspectRatio = 1000.0f / 800.0f;

	WorldManager::Get().AddGameObject(
		"FixCamera", 
		std::make_unique<FixCamera>(
		DirectX::XMFLOAT3(+0.0f, +0.0f, -60.0f),
		DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f),
		DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f),
		DirectX::XM_PIDIV4,
		aspectRatio
	));

	int32_t countBlock = 0;
	for (float x = -12.0f; x <= 12.0f; x += 2.0f)
	{
		for (float y = -22.0f; y <= 22.0f; y += 2.0f)
		{
			WorldManager::Get().AddGameObject(
				StringHelper::Format("Block%d", countBlock++),
				std::make_unique<Block>(
					DirectX::XMFLOAT3(x, y, 2.0f),
					Block::EColor::PURPLE
				)
			);
		}
	}

	for (float y = -22.0f; y <= 22.0f; y += 2.0f)
	{
		if (y == -22.0f || y == 22.0f)
		{
			for (float x = -12.0f; x <= 12.0f; x += 2.0f)
			{
				WorldManager::Get().AddGameObject(
					StringHelper::Format("Block%d", countBlock++),
					std::make_unique<Block>(
						DirectX::XMFLOAT3(x, y, 0.0f),
						Block::EColor::GRAY
					)
				);
			}
		}
		else
		{
			WorldManager::Get().AddGameObject(
				StringHelper::Format("Block%d", countBlock++),
				std::make_unique<Block>(
					DirectX::XMFLOAT3(-12.0f, y, 0.0f),
					Block::EColor::GRAY
				)
			);

			WorldManager::Get().AddGameObject(
				StringHelper::Format("Block%d", countBlock++),
				std::make_unique<Block>(
					DirectX::XMFLOAT3(+12.0f, y, 0.0f),
					Block::EColor::GRAY
				)
			);
		}
	}
}


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
	
	std::unique_ptr<Window> window = std::make_unique<Window>(
		"Tetris3D", 
		200, 200, 
		1000, 800, 
		EWindowFlags::SHOWN | EWindowFlags::RESIZABLE
	);

	InputManager::Get().Setup();
	RenderManager::Get().Setup(window.get());
	ContentManager::Get().Setup();
	WorldManager::Get().Setup();

	LoadShader();
	LoadTexture();
	LoadStaticMesh();
	LoadGameObject();

	GameTimer gameTimer;
	gameTimer.Reset();

	RenderManager::Get().SetAlphaBlend(true);
	RenderManager::Get().SetDepthBuffer(true);
	RenderManager::Get().SetFillMode(true);

	bool bIsDone = false;

	InputManager::Get().BindWindowEventAction(EWindowEvent::CLOSE, [&]() { bIsDone = true; });
	InputManager::Get().BindWindowEventAction(EWindowEvent::RESIZED, [&]() {
		RenderManager::Get().Resize();
	
		FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
		fixCamera->SetAspectRatio(window->GetAspectRatio());
	});

	while (!bIsDone)
	{
		InputManager::Get().Tick();
		gameTimer.Tick();

		RenderManager::Get().BeginFrame(1.0f, 1.0f, 1.0f, 1.0f);
		RenderManager::Get().SetWindowViewport();

		WorldManager::Get().Tick(gameTimer.GetDeltaSeconds());

		RenderManager::Get().EndFrame();
	}

	WorldManager::Get().Cleanup();
	ContentManager::Get().Cleanup();
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