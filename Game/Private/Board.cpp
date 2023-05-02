#include "ContentManager.h"
#include "FixCamera.h"
#include "MathHelper.hpp"
#include "RenderManager.h"
#include "InputManager.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "WorldManager.h"
#include "Board.h"

Board::Board(
	int32_t updateOrder, 
	bool bIsActive, 
	const DirectX::XMFLOAT3& basePosition, 
	float blockSize, 
	const Block::EColor& blockColor,
	int32_t countRowBlock,
	int32_t countColBlock
) : GameObject(updateOrder, bIsActive)
, basePosition_(basePosition)
, countRowBlock_(countRowBlock)
, countColBlock_(countColBlock)
{
	blockStaticMesh_ = ContentManager::Get().GetStaticMesh("Block");
	mappingBlockColorToTextures_ = {
		{ Block::EColor::BLUE,    nullptr },
		{ Block::EColor::CYAN,    nullptr },
		{ Block::EColor::GRAY,    nullptr },
		{ Block::EColor::GREEN,   nullptr },
		{ Block::EColor::MAGENTA, nullptr },
		{ Block::EColor::ORANGE,  nullptr },
		{ Block::EColor::PINK,    nullptr },
		{ Block::EColor::PURPLE,  nullptr },
		{ Block::EColor::RED,     nullptr },
		{ Block::EColor::YELLOW,  nullptr },
	};

	for (auto& mappingBlockColorToTexture : mappingBlockColorToTextures_)
	{
		Block::EColor color = mappingBlockColorToTexture.first;
		mappingBlockColorToTexture.second = ContentManager::Get().GetTexture2D(Block::GetColorTextureSignature(color));
	}

	outlineBlocks_ = GenerateOutlineBlocks(basePosition_, blockSize, blockColor, countRowBlock_, countColBlock_);
	backgroundBlocks_ = GenerateBackgroundBlocks(basePosition_, blockSize, blockColor, countRowBlock_, countColBlock_);
}

Board::~Board()
{
}

void Board::Tick(float deltaSeconds)
{
	DrawBlocks(backgroundBlocks_);
	DrawBlocks(outlineBlocks_);
	DrawInnerBlocks(innerBlocks_);
}

void Board::AddBlocks(const std::vector<Block>& addBlocks)
{
	if (!IsAddBlocks(addBlocks)) return;
	
	for (const Block& addBlock : addBlocks)
	{
		innerBlocks_.push_back(addBlock);
	}

	auto comparePosition = [](const Block& lhs, const Block& rhs) -> bool
	{
		DirectX::XMFLOAT3 lhsPosition = lhs.GetPosition();
		DirectX::XMFLOAT3 rhsPosition = rhs.GetPosition();

		if (lhsPosition.x < rhsPosition.x)
		{
			return true;
		}
		else if (MathHelper::IsZero(lhsPosition.x - rhsPosition.x))
		{
			if (lhsPosition.y > rhsPosition.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	};

	innerBlocks_.sort(comparePosition);
}

bool Board::IsAddBlocks(const std::vector<Block>& addBlocks)
{
	for (const Block& innerBlock : innerBlocks_)
	{
		for (const Block& addBlock : addBlocks)
		{
			if (innerBlock.IsOverlap(addBlock))
			{
				return false;
			}
		}
	}

	return true;
}

std::vector<Block> Board::GenerateOutlineBlocks(
	const DirectX::XMFLOAT3& basePosition, 
	float blockSize, 
	const Block::EColor& blockColor,
	int32_t countRowBlock,
	int32_t countColBlock
)
{
	std::vector<Block> blocks;

	for (int32_t row = 0; row < countRowBlock; ++row)
	{
		for (int32_t col = 0; col < countColBlock; ++col)
		{
			if ((row == 0 || row == countRowBlock - 1) || (col == 0 || col == countColBlock - 1))
			{
				float x = basePosition.x + static_cast<float>(col) * blockSize;
				float y = basePosition.y - static_cast<float>(row) * blockSize;
				float z = basePosition.z;

				blocks.push_back(Block(DirectX::XMFLOAT3(x, y, z), blockSize, blockColor));
			}
		}
	}

	return blocks;
}

std::vector<Block> Board::GenerateBackgroundBlocks(
	const DirectX::XMFLOAT3& basePosition, 
	float blockSize,
	const Block::EColor& blockColor,
	int32_t countRowBlock,
	int32_t countColBlock
)
{
	std::vector<Block> blocks(countColBlock * countRowBlock);

	for (int32_t row = 0; row < countRowBlock; ++row)
	{
		for (int32_t col = 0; col < countColBlock; ++col)
		{
			float x = basePosition.x + static_cast<float>(col) * blockSize;
			float y = basePosition.y - static_cast<float>(row) * blockSize;
			float z = basePosition.z + blockSize;

			int32_t offset = col + row * (countColBlock);
			blocks[offset] = Block(DirectX::XMFLOAT3(x, y, z), blockSize, blockColor);
		}
	}

	return blocks;
}

void Board::DrawBlocks(const std::vector<Block>& blocks)
{
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));

	for (const Block& block : blocks)
	{
		DirectX::XMFLOAT3 position = block.GetPosition();

		effectShader->SetWorldMatrix(DirectX::XMMatrixTranslation(position.x, position.y, position.z));
		effectShader->SetViewMatrix(fixCamera->GetViewMatrix());
		effectShader->SetProjectionMatrix(fixCamera->GetProjectionMatrix());

		effectShader->SetTexture(mappingBlockColorToTextures_[block.GetColor()]);
		effectShader->Bind(RenderManager::Get().GetContext());

		blockStaticMesh_->Draw(RenderManager::Get().GetContext());
	}
}

void Board::DrawInnerBlocks(const std::list<Block>& blocks)
{
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));

	for (const Block& block : blocks)
	{
		DirectX::XMFLOAT3 position = block.GetPosition();

		effectShader->SetWorldMatrix(DirectX::XMMatrixTranslation(position.x, position.y, position.z));
		effectShader->SetViewMatrix(fixCamera->GetViewMatrix());
		effectShader->SetProjectionMatrix(fixCamera->GetProjectionMatrix());

		effectShader->SetTexture(mappingBlockColorToTextures_[block.GetColor()]);
		effectShader->Bind(RenderManager::Get().GetContext());

		blockStaticMesh_->Draw(RenderManager::Get().GetContext());
	}
}
