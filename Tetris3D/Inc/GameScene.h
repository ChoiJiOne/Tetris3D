#pragma once

#include "IGameScene3D.h"

class GameScene : public IGameScene3D
{
public:
	GameScene();
	virtual ~GameScene();

	DISALLOW_COPY_AND_ASSIGN(GameScene);

	virtual void Render() override;

private:

};