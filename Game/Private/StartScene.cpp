#include <array>

#include "Button.h"
#include "ContentManager.h"
#include "FixCamera.h"
#include "Sound.h"
#include "InputManager.h"
#include "Label.h"
#include "RenderManager.h"
#include "WorldManager.h"
#include "StartScene.h"

void StartScene::Tick(float deltaSeconds)
{
	accumulateTime_ += deltaSeconds;
	UpdateTitleTextColor();
}

void StartScene::Entry()
{
	SetActive(true);

	accumulateTime_ = 0.0f;

	InputManager::Get().BindWindowEventAction(
		EWindowEvent::RESIZED, 
		[&]() {
			RenderManager::Get().Resize();

			float bufferWidth = 0.0f;
			float bufferHeight = 0.0f;
			RenderManager::Get().GetBackbufferSize(bufferWidth, bufferHeight);

			FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
			fixCamera->SetAspectRatio(bufferWidth / bufferHeight);
		}
	);

	uiUpdateOrder_ = 4;

	Label* startTitle = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("StartTitle"));
	if (!startTitle)
	{
		Label::ConstructorParam startTitleParam {
			uiUpdateOrder_,
			true,
			DirectX::XMFLOAT2(0.0f, 0.6f),
			"SeoulNamsanEB128",
			L"TETRIS 3D",
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		};

		WorldManager::Get().AddGameObject("StartTitle", std::make_unique<Label>(startTitleParam));
	}
	else
	{
		startTitle->SetActive(true);
	}

	Button* startButton = reinterpret_cast<Button*>(WorldManager::Get().GetGameObject("StartButton"));
	if (!startButton)
	{
		Button::ConstructorParam startButtonParam{
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
				Sound* clickSound = ContentManager::Get().GetSound("Click");
				clickSound->Reset();
				clickSound->Play();

				Leave();
			}
		};

		WorldManager::Get().AddGameObject("StartButton", std::make_unique<Button>(startButtonParam));
	}
	else
	{
		startButton->SetActive(true);
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
				Sound* clickSound = ContentManager::Get().GetSound("Click");
				clickSound->Reset();
				clickSound->Play();

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

	Sound* titleSound = ContentManager::Get().GetSound("Title");
	titleSound->Reset();
	titleSound->SetLooping(true);
	titleSound->Play();
}

void StartScene::Leave()
{
	std::array<std::string, 4> signatures = {
		"StartTitle",
		"StartButton",
		"QuitButton",
		"StartScene",
	};

	for (const auto& signature : signatures)
	{
		GameObject* object = WorldManager::Get().GetGameObject(signature);
		object->SetActive(false);
	}

	SetActive(false);
	nextScene_->Entry();
}

void StartScene::UpdateTitleTextColor()
{
	DirectX::XMFLOAT4 color;
	float bias = 0.5f;
	float speed = 5.0f;

	color.x = std::abs(std::sin(speed * accumulateTime_));
	color.y = std::abs(std::cos(speed * accumulateTime_ + bias));
	color.z = std::abs(std::sin(speed * accumulateTime_ + 2.0f * bias));
	color.w = 1.0f;

	Label* startTitle = reinterpret_cast<Label*>(WorldManager::Get().GetGameObject("StartTitle"));
	startTitle->SetColor(color);
}