#include "Button.h"
#include "Label.h"
#include "WorldManager.h"
#include "StartScene.h"

void StartScene::Tick(float deltaSeconds)
{
}

void StartScene::Entry()
{
	SetActive(true);

	uiUpdateOrder_ = 4;

	Label::ConstructorParam titleParam {
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(0.0f, 0.4f),
		"SeoulNamsanEB128",
		L"TETRIS 3D",
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
	};

	WorldManager::Get().AddGameObject("TitleLabel", std::make_unique<Label>(titleParam));

	Button::ConstructorParam startButtonParam {
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(0.0f, 0.0f),
		200.0f, 
		200.0f,
		"Start",
		1.0f,
		0.5f,
		0.95f,
		[&]() {
		}
	};

	WorldManager::Get().AddGameObject("StartButton", std::make_unique<Button>(startButtonParam));

	Button::ConstructorParam quitButtonParam{
		uiUpdateOrder_,
		true,
		DirectX::XMFLOAT2(0.0f, -0.6f),
		200.0f,
		200.0f,
		"Quit",
		1.0f,
		0.5f,
		0.95f,
		[&]() {
		}
	};

	WorldManager::Get().AddGameObject("QuitButton", std::make_unique<Button>(quitButtonParam));
}

void StartScene::Leave()
{
	SetActive(false);
}
