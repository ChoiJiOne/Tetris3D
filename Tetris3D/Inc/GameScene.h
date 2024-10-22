#pragma once

#include "IGameScene3D.h"
#include "DebugDrawManager3D.h"

class GameScene : public IGameScene3D
{
public:
	GameScene();
	virtual ~GameScene();

	DISALLOW_COPY_AND_ASSIGN(GameScene);

	virtual void Render() override;

private:
	DebugDrawManager3D* debugDrawMgr_ = nullptr;

	float minX_ = -50.0f;
	float maxX_ = +50.0f;
	float minY_ = -50.0f;
	float maxY_ = +50.0f;
	float minZ_ = -50.0f;
	float maxZ_ = +50.0f;
	float stride_ = 1.0f;
};