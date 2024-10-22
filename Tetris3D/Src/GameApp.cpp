#include "Assertion.h"
#include "IGameScene3D.h"
#include "RenderStateManager.h"

#include "GameScene.h"
#include "GameApp.h"

GameApp::GameApp() : IApp("Tetris3D", 200, 200, 800, 600, false, false)
{}

GameApp::~GameApp()
{}

void GameApp::Startup()
{
	RenderStateManager::GetRef().SetAlphaBlendMode(true);

	gameScene_ = std::make_unique<class GameScene>();
	AddSceneByName("GameScene", gameScene_.get());

	SetCurrentScene(gameScene_.get());
}

void GameApp::Shutdown()
{
	gameScene_.reset();
}