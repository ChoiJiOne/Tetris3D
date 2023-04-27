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
}

Block::~Block()
{
}

void Block::Tick(float deltaSeconds)
{
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));

	effectShader->SetWorldMatrix(DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z));
	effectShader->SetViewMatrix(fixCamera->GetViewMatrix());
	effectShader->SetProjectionMatrix(fixCamera->GetProjectionMatrix());

	effectShader->SetTexture(texture_);
	effectShader->Bind(RenderManager::Get().GetContext());

	staticMesh_->Draw(RenderManager::Get().GetContext());
}
