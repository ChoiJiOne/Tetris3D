#define _CRTDBG_MAP_ALLOC

#include <SDL2/SDL.h>
#include <windows.h>

#include <crtdbg.h>
#include <memory>

#include "AudioManager.h"
#include "Block.h"
#include "Board.h"
#include "ColorNoEffectShader.h"
#include "CommandLine.h"
#include "ContentManager.h"
#include "CrashHandler.h"
#include "EndScene.h"
#include "FixCamera.h"
#include "GameTimer.h"
#include "GeometryGenerator.h"
#include "InputManager.h"
#include "PlayScene.h"
#include "RenderManager.h"
#include "Sound.h"
#include "SpriteNoEffectShader.h"
#include "StaticMesh.h"
#include "StringHelper.hpp"
#include "StartScene.h"
#include "Texture2D.h"
#include "Tetromino.h"
#include "TextNoEffectShader.h"
#include "TextureNoEffectShader.h"
#include "TTFont.h"
#include "Vertex.h"
#include "Window.h"
#include "WorldManager.h"


/**
 * @brief 3D 테트리스 게임을 초기화하고 실행합니다.
 */
class Tetris3D
{
public:
	/**
	 * @brief 3D 테트리스 게임의 기본 생성자입니다.
	 * 
	 * @note 생성자에서는 아무런 초기화도 수행하지 않습니다.
	 */
	Tetris3D() = default;


	/**
	 * @brief 3D 테트리스 게임의 가상 소멸자입니다.
	 */
	virtual ~Tetris3D() 
	{
		Cleanup();
	}


	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Tetris3D);


	/**
	 * @brief 3D 테트리스 게임을 초기화합니다.
	 * 
	 * @throws 초기화에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void Setup()
	{
		CHECK(SDL_Init(SDL_INIT_EVERYTHING) == 0, "failed to initialize SDL2...");

#if defined(SHIPPING) // Shipping 모드 시 콘솔 창 가리기
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

		window_ = std::make_unique<Window>("Tetris3D", 200, 200, 1000, 800, EWindowFlags::SHOWN | EWindowFlags::RESIZABLE);

		InputManager::Get().Setup();
		RenderManager::Get().Setup(window_.get());
		AudioManager::Get().Setup();
		ContentManager::Get().Setup();
		WorldManager::Get().Setup();

		json config = ContentManager::Get().LoadJsonFromFile(CommandLine::GetValue("Content") + "config.json");

		LoadShader();
		LoadTexture(config);
		LoadTTFont(config);
		LoadSound(config);
		LoadStaticMesh();
		LoadGameObject();

		InputManager::Get().BindWindowEventAction(EWindowEvent::CLOSE, [&]() { bIsDone_ = true; });

		RenderManager::Get().SetAlphaBlend(true);
		RenderManager::Get().SetDepthBuffer(true);
		RenderManager::Get().SetFillMode(true);
	}
	

	/**
	 * @brief 3D 테트리스 게임을 실행합니다.
	 * 
	 * @throws 게임 실행에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void Run()
	{
		gameTimer_.Reset();
				
		while (!bIsDone_)
		{
			InputManager::Get().Tick();
			gameTimer_.Tick();

			RenderManager::Get().BeginFrame(0.5294f, 0.8078f, 0.9216f, 1.0f);
			RenderManager::Get().SetWindowViewport();

			WorldManager::Get().Tick(gameTimer_.GetDeltaSeconds());

			RenderManager::Get().EndFrame();
		}
	}


	/**
	 * @brief 3D 테트리스 게임의 리소스를 정리합니다.
	 */
	void Cleanup()
	{
		WorldManager::Get().Cleanup();
		ContentManager::Get().Cleanup();
		AudioManager::Get().Cleanup();
		RenderManager::Get().Cleanup();
		InputManager::Get().Cleanup();

		window_.reset();

		SDL_Quit();
	}


private:
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
		
		ContentManager::Get().AddEffectShader(
			"TextNoEffectShader",
			std::make_unique<TextNoEffectShader>(
				RenderManager::Get().GetDevice(),
				shaderSourcePath + L"TextNoEffectVS.hlsl",
				shaderSourcePath + L"TextNoEffectPS.hlsl"
			)
		);

		ContentManager::Get().AddEffectShader(
			"SpriteNoEffectShader",
			std::make_unique<SpriteNoEffectShader>(
				RenderManager::Get().GetDevice(),
				shaderSourcePath + L"SpriteNoEffectVS.hlsl",
				shaderSourcePath + L"SpriteNoEffectPS.hlsl"
			)
		);
	}


	/**
	 * @brief 텍스처 파일을 로딩합니다.
	 * 
	 * @param config 컨텐츠의 설정 파일입니다.
	 *
	 * @throws 텍스처 파일 로딩에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void LoadTexture(const json& config)
	{
		std::string format = config["texture"]["format"];
		std::vector<std::string> textures = config["texture"]["resource"];
		std::string contentPath = CommandLine::GetValue("Content");

		for (const auto& texture : textures)
		{
			ContentManager::Get().AddTexture2D(
				texture,
				std::make_unique<Texture2D>(
					RenderManager::Get().GetDevice(),
					StringHelper::Format("%stexture\\%s.%s", contentPath.c_str(), texture.c_str(), format.c_str())
				)
			);
		}
	}


	/**
	 * @brief 트루 타입 폰트를 로딩합니다.
	 * 
	 * @param config 컨텐츠의 설정 파일입니다.
	 * 
	 * @throws 트루 타입 폰트 로딩에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void LoadTTFont(const json& config)
	{
		std::string format = config["font"]["format"];
		std::string fontName = config["font"]["resource"];
		std::vector<int32_t> fontSizes = config["font"]["size"];
		int32_t beginCodePoint = config["font"]["beginCodepoint"];
		int32_t endCodePoint = config["font"]["endCodepoint"];
		std::string fontPath = StringHelper::Format(
			"%sfont\\%s.%s", CommandLine::GetValue("Content").c_str(), fontName.c_str(), format.c_str()
		);

		for (const int32_t& fontSize : fontSizes)
		{
			ContentManager::Get().AddTTFont(
				StringHelper::Format("%s%d", fontName.c_str(), fontSize),
				std::make_unique<TTFont>(
					RenderManager::Get().GetDevice(), 
					fontPath, 
					beginCodePoint, 
					endCodePoint, 
					static_cast<float>(fontSize)
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

		GeometryGenerator::CreateQuadXY(10.0f, 10.0f, vertices, indices);
		ContentManager::Get().AddStaticMesh(
			"QuadXY",
			std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices)
		);

		GeometryGenerator::CreateQuadYZ(10.0f, 10.0f, vertices, indices);
		ContentManager::Get().AddStaticMesh(
			"QuadYZ",
			std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices)
		);

		GeometryGenerator::CreateQuadXZ(10.0f, 10.0f, vertices, indices);
		ContentManager::Get().AddStaticMesh(
			"QuadXZ",
			std::make_unique<StaticMesh>(RenderManager::Get().GetDevice(), vertices, indices)
		);
	}


	/**
	 * @brief 사운드를 로딩합니다.
	 * 
	 * @param config 컨텐츠의 설정 파일입니다.
	 * 
	 * @throws 사운드 로딩에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void LoadSound(const json& config)
	{
		std::string format = config["audio"]["format"];
		std::vector<std::string> sounds = config["audio"]["resource"];
		std::string contentPath = CommandLine::GetValue("Content");

		for (const auto& sound : sounds)
		{
			ContentManager::Get().AddSound(
				sound,
				std::make_unique<Sound>(
					StringHelper::Format("%saudio\\%s.%s", contentPath.c_str(), sound.c_str(), format.c_str())
				)
			);
		}
	}


	/**
	 * @brief 게임 오브젝트를 로딩합니다.
	 *
	 * @throws 게임 오브젝트 로딩에 실패하면 C++ 표준 예외를 던집니다.
	 */
	void LoadGameObject()
	{
		WorldManager::Get().AddGameObject(
			"FixCamera",
			std::make_unique<FixCamera>(
				1, 
				true,
				DirectX::XMFLOAT3(+0.0f, +0.0f, -60.0f),
				DirectX::XMFLOAT3(+0.0f, +0.0f, +0.0f),
				DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f),
				DirectX::XM_PIDIV4,
				window_->GetAspectRatio()
			)
		);

		std::function<void()> quitEvent = [&]() {
			bIsDone_ = true;
		};

		WorldManager::Get().AddGameObject("StartScene", std::make_unique<StartScene>(6, false));
		WorldManager::Get().AddGameObject("PlayScene", std::make_unique<PlayScene>(6, false));
		WorldManager::Get().AddGameObject("EndScene", std::make_unique<EndScene>(6, false));

		StartScene* startScene = reinterpret_cast<StartScene*>(WorldManager::Get().GetGameObject("StartScene"));
		PlayScene* playScene = reinterpret_cast<PlayScene*>(WorldManager::Get().GetGameObject("PlayScene"));
		EndScene* endScene = reinterpret_cast<EndScene*>(WorldManager::Get().GetGameObject("EndScene"));

		startScene->SetQuitEvent(quitEvent);
		endScene->SetQuitEvent(quitEvent);

		startScene->LinkNextScene(playScene);
		playScene->LinkNextScene(endScene);
		endScene->LinkNextScene(startScene);

		startScene->Entry();
	}


private:
	/**
	 * @brief 게임이 종료되었는지 확인합니다.
	 */
	bool bIsDone_ = false;


	/**
	 * @brief 게임 내 시간을 기록할 타이머입니다.
	 */
	GameTimer gameTimer_;


	/**
	 * @brief 윈도우 창입니다.
	 */
	std::unique_ptr<Window> window_ = nullptr;
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

	std::unique_ptr<Tetris3D> game = std::make_unique<Tetris3D>();
	game->Setup();
	game->Run();
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