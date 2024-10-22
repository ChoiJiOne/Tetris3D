#include "Assertion.h"
#include "FreeEulerCamera3D.h"
#include "EntityManager.h"
#include "GameUtils.h"
#include "ResourceManager.h"
#include "RenderManager2D.h"
#include "RenderManager3D.h"
#include "RenderStateManager.h"
#include "UIManager.h"

#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
}

void GameScene::Render()
{
	renderStateMgr_->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);
	renderStateMgr_->EndFrame();
}