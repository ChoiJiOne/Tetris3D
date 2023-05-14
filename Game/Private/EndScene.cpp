#include <array>

#include "Button.h"
#include "Label.h"
#include "WorldManager.h"
#include "EndScene.h"

void EndScene::Tick(float deltaSeconds)
{
}

void EndScene::Entry()
{
	SetActive(true);

	uiUpdateOrder_ = 4;

	Label* endTitle = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("EndTitle"));
	if (!endTitle)
	{
		Label::ConstructorParam endTitleParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.4f),
			"SeoulNamsanEB128",
			L"TETRIS 3D",
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		WorldManager::Get().AddGameObject("EndTitle", std::make_unique<Label>(endTitleParam));
	}
	else
	{
		endTitle->SetActive(true);
	}

	Button* okButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("OkButton"));
	if (!okButton)
	{
		Button::ConstructorParam okButtonParam{
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.0f),
			200.0f,
			200.0f,
			"Ok",
			1.0f,
			0.5f,
			0.95f,
			[&]() {
				Leave();
			}
		};

		WorldManager::Get().AddGameObject("OkButton", std::make_unique<Button>(okButtonParam));
	}
	else
	{
		okButton->SetActive(true);
	}

	Button* quitButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("QuitButton"));
	if (!quitButton)
	{
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
				if (quitEvent_)
				{
					quitEvent_();
				}
			}
		};

		WorldManager::Get().AddGameObject("QuitButton", std::make_unique<Button>(quitButtonParam));
	}
	else
	{
		quitButton->SetActive(true);
	}
}

void EndScene::Leave()
{
	std::array<std::string, 4> signatures = {
		"EndTitle",
		"OkButton",
		"QuitButton",
		"EndScene",
	};

	for (const auto& signature : signatures)
	{
		GameObject* object = WorldManager::Get().GetGameObject(signature);
		object->SetActive(false);
	}

	SetActive(false);
	nextScene_->Entry();
}