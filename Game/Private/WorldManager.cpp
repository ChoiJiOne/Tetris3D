#include "GameObject.h"
#include "WorldManager.h"

void WorldManager::Setup()
{
	if (bIsSetup_) return;

	bIsSetup_ = true;
}

void WorldManager::Cleanup()
{
	if (!bIsSetup_) return;

	for (auto& gameObject : gameObjects_)
	{
		gameObject.second.reset();
	}

	gameObjects_.clear();

	bIsSetup_ = false;
}

void WorldManager::AddGameObject(const std::string& signature, std::unique_ptr<GameObject> gameObject)
{
	CHECK((gameObjects_.find(signature) == gameObjects_.end()), "collision game object signature key...");

	gameObjects_[signature] = std::move(gameObject);
}

GameObject* WorldManager::GetGameObject(const std::string& signature)
{
	GameObject* gameObject = nullptr;

	if (gameObjects_.find(signature) != gameObjects_.end())
	{
		gameObject = gameObjects_[signature].get();
	}

	return gameObject;
}

void WorldManager::RemoveGameObject(const std::string& signature)
{
	if (gameObjects_.find(signature) != gameObjects_.end())
	{
		gameObjects_.erase(signature);
	}
}

void WorldManager::Tick(float deltaSeconds)
{
	for (auto& gameObject : gameObjects_)
	{
		GameObject* currentGameObject = gameObject.second.get();

		if (currentGameObject->IsActive())
		{
			currentGameObject->Tick(deltaSeconds);
		}
	}
}

WorldManager::~WorldManager()
{
	Cleanup();
}