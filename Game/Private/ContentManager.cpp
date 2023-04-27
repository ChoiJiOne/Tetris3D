#include "EffectShader.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "ContentManager.h"

void ContentManager::Setup()
{
	if (bIsSetup_) return;

	bIsSetup_ = true;
}

void ContentManager::Cleanup()
{
	if (!bIsSetup_) return;

	for (auto& texture : textures_)
	{
		texture.second.reset();
	}

	for (auto& staticMesh : staticMeshs_)
	{
		staticMesh.second.reset();
	}

	for (auto& effectShader : effectShaders_)
	{
		effectShader.second.reset();
	}

	textures_.clear();
	staticMeshs_.clear();
	effectShaders_.clear();
	
	bIsSetup_ = false;
}

EffectShader* ContentManager::AddEffectShader(const std::string& signature, std::unique_ptr<EffectShader> effectShader)
{
	CHECK((effectShaders_.find(signature) == effectShaders_.end()), "collision effect shader signature key...");

	effectShaders_[signature] = std::move(effectShader);
	return effectShaders_[signature].get();
}

EffectShader* ContentManager::GetEffectShader(const std::string& signature)
{
	EffectShader* effectShader = nullptr;

	if (effectShaders_.find(signature) != effectShaders_.end())
	{
		effectShader = effectShaders_[signature].get();
	}

	return effectShader;
}

void ContentManager::RemoveEffectShader(const std::string& signature)
{
	if (effectShaders_.find(signature) != effectShaders_.end())
	{
		effectShaders_.erase(signature);
	}
}

ContentManager::~ContentManager()
{
	Cleanup();
}