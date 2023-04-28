#include <algorithm>

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

	gameObjectUpdates_.clear();
	gameObjects_.clear();

	bIsSetup_ = false;
}

void WorldManager::AddGameObject(const std::string& signature, std::unique_ptr<GameObject> gameObject)
{
	CHECK((gameObjects_.find(signature) == gameObjects_.end()), "collision game object signature key...");

	gameObjects_.insert({ signature , std::move(gameObject) });
	gameObjectUpdates_.push_back(gameObjects_[signature].get());

	gameObjectUpdates_.sort(
		[](const GameObject* lhs, const GameObject* rhs) -> bool
		{
			return lhs->GetUpdateOrder() < rhs->GetUpdateOrder();
		}
	);
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
		GameObject* removeGameObject = gameObjects_[signature].get();

		for (GameObject* gameObjectUpdate : gameObjectUpdates_)
		{
			if (gameObjectUpdate == removeGameObject)
			{
				gameObjectUpdates_.remove(gameObjectUpdate);
				break;
			}
		}

		gameObjects_.erase(signature);
	}
}

void WorldManager::Tick(float deltaSeconds)
{
	for (auto& gameObject : gameObjectUpdates_)
	{
		if (gameObject->IsActive())
		{
			gameObject->Tick(deltaSeconds);
		}
	}
}

WorldManager::~WorldManager()
{
	Cleanup();
}