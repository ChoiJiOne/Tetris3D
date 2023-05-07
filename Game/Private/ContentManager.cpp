#include "EffectShader.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TTFont.h"
#include "ContentManager.h"

void ContentManager::Setup()
{
	if (bIsSetup_) return;

	bIsSetup_ = true;
}

void ContentManager::Cleanup()
{
	if (!bIsSetup_) return;

	for (auto& font : fonts_)
	{
		font.second.reset();
	}

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

	fonts_.clear();
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

Texture2D* ContentManager::AddTexture2D(const std::string& signature, std::unique_ptr<Texture2D> texture)
{
	CHECK((textures_.find(signature) == textures_.end()), "collision texture signature key...");

	textures_[signature] = std::move(texture);
	return textures_[signature].get();
}

Texture2D* ContentManager::GetTexture2D(const std::string& signature)
{
	Texture2D* texture = nullptr;

	if (textures_.find(signature) != textures_.end())
	{
		texture = textures_[signature].get();
	}

	return texture;
}

void ContentManager::RemoveTexture2D(const std::string& signature)
{
	if (textures_.find(signature) != textures_.end())
	{
		textures_.erase(signature);
	}
}

StaticMesh* ContentManager::AddStaticMesh(const std::string& signature, std::unique_ptr<StaticMesh> staticMesh)
{
	CHECK((staticMeshs_.find(signature) == staticMeshs_.end()), "collision static mesh signature key...");

	staticMeshs_[signature] = std::move(staticMesh);
	return staticMeshs_[signature].get();
}

StaticMesh* ContentManager::GetStaticMesh(const std::string& signature)
{
	StaticMesh* staticMesh = nullptr;

	if (staticMeshs_.find(signature) != staticMeshs_.end())
	{
		staticMesh = staticMeshs_[signature].get();
	}

	return staticMesh;
}

void ContentManager::RemoveStaticMesh(const std::string& signature)
{
	if (staticMeshs_.find(signature) != staticMeshs_.end())
	{
		staticMeshs_.erase(signature);
	}
}

TTFont* ContentManager::AddTTFont(const std::string& signature, std::unique_ptr<TTFont> font)
{
	CHECK((fonts_.find(signature) == fonts_.end()), "collision true type font signature key...");

	fonts_[signature] = std::move(font);
	return fonts_[signature].get();
}

TTFont* ContentManager::GetTTFont(const std::string& signature)
{
	TTFont* font = nullptr;

	if (fonts_.find(signature) != fonts_.end())
	{
		font = fonts_[signature].get();
	}

	return font;
}

void ContentManager::RemoveTTFont(const std::string& signature)
{
	if (fonts_.find(signature) != fonts_.end())
	{
		fonts_.erase(signature);
	}
}

ContentManager::~ContentManager()
{
	Cleanup();
}