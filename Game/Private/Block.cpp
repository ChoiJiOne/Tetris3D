#include "ContentManager.h"
#include "FixCamera.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "MathHelper.hpp"
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
	{ Block::EColor::PINK,    "PinkBlock"   },
	{ Block::EColor::PURPLE,  "PurpleBlock"  },
	{ Block::EColor::RED,     "RedBlock"     },
	{ Block::EColor::YELLOW,  "YellowBlock"  },
};

Block::Block(const DirectX::XMFLOAT3& position, float size, const EColor& color)
	: position_(position)
	, size_(size)
	, color_(color)
{
	boundingBox_ = DirectX::BoundingBox(position_, DirectX::XMFLOAT3(size_ / 2.0f, size_ / 2.0f, size_ / 2.0f));
}

Block::~Block()
{
}

void Block::SetPosition(const DirectX::XMFLOAT3& position)
{
	position_ = position;
	boundingBox_.Center = position_;
}

bool Block::IsCollision(const Block& otherBlock) const
{
	return boundingBox_.Intersects(otherBlock.boundingBox_);
}

bool Block::IsOverlap(const Block& otherBlock) const
{
	float diffX = position_.x - otherBlock.position_.x;
	float diffY = position_.y - otherBlock.position_.y;
	float diffZ = position_.z - otherBlock.position_.z;

	return IsCollision(otherBlock) 
		&& MathHelper::IsZero(diffX) 
		&& MathHelper::IsZero(diffY) 
		&& MathHelper::IsZero(diffZ);
}

const std::string& Block::GetColorTextureSignature(const EColor& color)
{
	return colorSignatureMappings[color];
}
