#include "Assertion.h"
#include "DebugDrawManager3D.h"
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
	debugDrawMgr_ = DebugDrawManager3D::GetPtr();
	mainCamera_ = EntityManager::GetRef().Create<FreeEulerCamera3D>(Vec3f(3.0f, 3.0f, 3.0f), -GameMath::ToRadian(135.0f), -PI / 6.0f, PI_DIV_4, 0.01f, 100.0f);

	updateEntities_.insert({ "MainCamera", mainCamera_,});
}

GameScene::~GameScene()
{
}

void GameScene::Render()
{
	renderStateMgr_->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

	debugDrawMgr_->Begin(mainCamera_);
	{
		Vec4f color;
		for (float x = minX_; x <= maxX_; x += stride_)
		{
			color = (x == 0.0f) ? Vec4f(0.0f, 0.0f, 1.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			debugDrawMgr_->DrawLine(Vec3f(x, 0.0f, minZ_), Vec3f(x, 0.0f, maxZ_), color);
		}

		for (float z = minZ_; z <= maxZ_; z += stride_)
		{
			color = (z == 0.0f) ? Vec4f(1.0f, 0.0f, 0.0f, 1.0f) : Vec4f(0.5f, 0.5f, 0.5f, 0.5f);
			debugDrawMgr_->DrawLine(Vec3f(minX_, 0.0f, z), Vec3f(maxX_, 0.0f, z), color);
		}

		debugDrawMgr_->DrawLine(Vec3f(0.0f, minY_, 0.0f), Vec3f(0.0f, maxY_, 0.0f), Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
	}
	debugDrawMgr_->End();

	renderStateMgr_->EndFrame();
}