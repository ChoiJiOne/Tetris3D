#include "ContentManager.h"
#include "FixCamera.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "RenderManager.h"
#include "WorldManager.h"
#include "Block.h"

std::unordered_map<Block::EColor, std::string> colorSignatureMappings = {
	{ Block::EColor::BLUE,    "BlueBlock"    },
	{ Block::EColor::CYAN,    "CyanBlock"    },
	{ Block::EColor::GRAY,    "GrayBlock"    },
	{ Block::EColor::GREEN,   "GreenBlock"   },
	{ Block::EColor::MAGENTA, "MagentaBlock" },
	{ Block::EColor::ORANGE,  "OrangeBlock"  },
	{ Block::EColor::PURPLE,  "PurpleBlock"  },
	{ Block::EColor::RED,     "RedBlock"     },
	{ Block::EColor::YELLOW,  "YellowBlock"  },
};

Block::Block(const DirectX::XMFLOAT3 position, const EColor& color)
	: GameObject(2, true)
	, position_(position)
	, color_(color)
{
	staticMesh_ = ContentManager::Get().GetStaticMesh("Block");
	texture_ = ContentManager::Get().GetTexture2D(colorSignatureMappings[color_]);

	boundingBox_ = DirectX::BoundingBox(position_, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
}

Block::~Block()
{
}

void Block::Tick(float deltaSeconds)
{
	if (this == WorldManager::Get().GetGameObject("LEFT"))
	{
		const DirectX::BoundingBox& rightBoundingBox = reinterpret_cast<Block*>(WorldManager::Get().GetGameObject("RIGHT"))->boundingBox_;

		if (!boundingBox_.Intersects(rightBoundingBox))
		{
			position_.x += (deltaSeconds * 10.0f);
			boundingBox_.Center = position_;
		}
	}

	if (this == WorldManager::Get().GetGameObject("RIGHT"))
	{
		const DirectX::BoundingBox& leftBoundingBox = reinterpret_cast<Block*>(WorldManager::Get().GetGameObject("LEFT"))->boundingBox_;

		if (!boundingBox_.Intersects(leftBoundingBox))
		{
			position_.x -= (deltaSeconds * 10.0f);
			boundingBox_.Center = position_;
		}
	}

	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));

	effectShader->SetWorldMatrix(DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z));
	effectShader->SetViewMatrix(fixCamera->GetViewMatrix());
	effectShader->SetProjectionMatrix(fixCamera->GetProjectionMatrix());

	effectShader->SetTexture(texture_);
	effectShader->Bind(RenderManager::Get().GetContext());

	staticMesh_->Draw(RenderManager::Get().GetContext());
}
