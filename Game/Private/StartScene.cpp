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
}

void StartScene::Leave()
{
	SetActive(false);
}
