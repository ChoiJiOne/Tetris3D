#include "Board.h"
#include "ContentManager.h"
#include "FixCamera.h"
#include "MathHelper.hpp"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "WorldManager.h"
#include "Tetromino.h"

std::unordered_map<EVirtualKey, Tetromino::EMovement> Tetromino::virtualKeyToMovements_ = {
	{ EVirtualKey::CODE_LEFT,  Tetromino::EMovement::LEFT  },
	{ EVirtualKey::CODE_RIGHT, Tetromino::EMovement::RIGHT },
	{ EVirtualKey::CODE_UP,    Tetromino::EMovement::CW    },
	{ EVirtualKey::CODE_DOWN,  Tetromino::EMovement::DOWN  },
	{ EVirtualKey::CODE_SPACE, Tetromino::EMovement::JUMP  },
};

Tetromino::Tetromino(ConstructorParam&& constructorParam)
	: Tetromino(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.basePosition,
		constructorParam.shape,
		constructorParam.blockSize,
		constructorParam.blockColor,
		constructorParam.maxAccumulatedTime
	) {}

Tetromino::Tetromino(const ConstructorParam& constructorParam)
	: Tetromino(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.basePosition,
		constructorParam.shape,
		constructorParam.blockSize,
		constructorParam.blockColor,
		constructorParam.maxAccumulatedTime
	) {}

Tetromino::Tetromino(
	int32_t updateOrder,
	bool bIsActive,
	const DirectX::XMFLOAT3& basePosition,
	const EShape& shape,
	float blockSize,
	const Block::EColor& blockColor,
	float maxAccumulatedTime
) : GameObject(updateOrder, bIsActive)
, shape_(shape)
, basePosition_(basePosition)
, maxAccumulatedTime_(maxAccumulatedTime)
{
	blockStaticMesh_ = ContentManager::Get().GetStaticMesh("Block");
	blockTexture_ = ContentManager::Get().GetTexture2D(Block::GetColorTextureSignature(blockColor));

	GenerateShapeBlocks(shape_, basePosition_, blockSize, blockColor, rotatePosition_, blocks_);
}

Tetromino::~Tetromino()
{
}

void Tetromino::Tick(float deltaSeconds)
{
	if (state_ == EState::RUNNING)
	{
		accumulatedTime_ += deltaSeconds;
		Update();

		DrawBlocks(shadowBlocks_);
		DrawBlocks(blocks_);
	}
	else
	{
		DrawBlocks(blocks_);
	}
}

void Tetromino::Teleport(const DirectX::XMFLOAT3& basePosition)
{
	rotatePosition_.x -= basePosition_.x;
	rotatePosition_.y -= basePosition_.y;
	rotatePosition_.z -= basePosition_.z;

	rotatePosition_.x += basePosition.x;
	rotatePosition_.y += basePosition.y;
	rotatePosition_.z += basePosition.z;

	for (Block& block : blocks_)
	{
		DirectX::XMFLOAT3 blockPosition = block.GetPosition();

		blockPosition.x -= basePosition_.x;
		blockPosition.y -= basePosition_.y;
		blockPosition.z -= basePosition_.z;

		blockPosition.x += basePosition.x;
		blockPosition.y += basePosition.y;
		blockPosition.z += basePosition.z;

		block.SetPosition(blockPosition);
	}

	basePosition_ = basePosition;
}

bool Tetromino::CanTeleport(const DirectX::XMFLOAT3& basePosition)
{
	Board* board = reinterpret_cast<Board*>(WorldManager::Get().GetGameObject("Board"));

	Teleport(basePosition);
	bool bCanTeleport = !IsCollision(blocks_, board);
	Teleport(basePosition_);

	return bCanTeleport;
}

Tetromino::EShape Tetromino::GetRandomShape()
{
	static std::array<Tetromino::EShape, 7> tetrominoShapes = {
		Tetromino::EShape::I,
		Tetromino::EShape::O,
		Tetromino::EShape::T,
		Tetromino::EShape::J,
		Tetromino::EShape::L,
		Tetromino::EShape::S,
		Tetromino::EShape::Z,
	};

	return tetrominoShapes[MathHelper::GenerateRandomInt(0, static_cast<int32_t>(tetrominoShapes.size()) - 1)];
}

void Tetromino::GenerateShapeBlocks(
	const EShape& shape, 
	const DirectX::XMFLOAT3& basePosition, 
	float blockSize, 
	const Block::EColor& blockColor, 
	DirectX::XMFLOAT3& outRotatePosition, 
	std::vector<Block>& outBlocks
)
{
	outBlocks.clear();
	outBlocks.resize(4);

	switch (shape)
	{
	case EShape::I:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 3.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.5f * blockSize, basePosition.y - 1.5f * blockSize, basePosition.z);
		break;

	case EShape::O:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 0.5f * blockSize, basePosition.y - 0.5f * blockSize, basePosition.z);
		break;

	case EShape::T:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z);
		break;

	case EShape::J:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 2.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z);
		break;

	case EShape::L:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 2.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z);
		break;

	case EShape::S:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z);
		break;

	case EShape::Z:
		outBlocks[0] = Block(DirectX::XMFLOAT3(basePosition.x + 0.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[1] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 0.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[2] = Block(DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);
		outBlocks[3] = Block(DirectX::XMFLOAT3(basePosition.x + 2.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z), blockSize, blockColor);

		outRotatePosition = DirectX::XMFLOAT3(basePosition.x + 1.0f * blockSize, basePosition.y - 1.0f * blockSize, basePosition.z);
		break;

	default:
		ENFORCE_THROW_EXCEPTION("undefined tetromino shape type...");
	}
}

void Tetromino::Update()
{
	Board* board = reinterpret_cast<Board*>(WorldManager::Get().GetGameObject("Board"));

	UpdateInputState(board);
	UpdateAccumulatedTime(board);

	shadowBasePosition_ = basePosition_;
	shadowRotatePosition_ = rotatePosition_;
	shadowBlocks_ = blocks_;
	JumpBottom(shadowBlocks_, shadowBasePosition_, shadowRotatePosition_, board);
}

void Tetromino::UpdateInputState(const Board* board)
{
	for (const auto& virtualKeyToMovement : virtualKeyToMovements_)
	{
		EVirtualKey virtualKey = virtualKeyToMovement.first;
		EMovement movement = virtualKeyToMovement.second;

		if (InputManager::Get().GetKeyPressState(virtualKey) == EPressState::PRESSED)
		{
			if (virtualKey == EVirtualKey::CODE_SPACE)
			{
				JumpBottom(blocks_, basePosition_, rotatePosition_, board);
			}
			else
			{
				if (CanMove(blocks_, basePosition_, rotatePosition_, board, movement))
				{
					Move(blocks_, basePosition_, rotatePosition_, movement);
				}
			}
		}
	}
}

void Tetromino::UpdateAccumulatedTime(Board* board)
{
	if (accumulatedTime_ > maxAccumulatedTime_)
	{
		accumulatedTime_ = 0.0f;

		EMovement moveDown = EMovement::DOWN;
		if (!CanMove(blocks_, basePosition_, rotatePosition_, board, moveDown))
		{
			state_ = EState::END;
			board->AddBlocks(blocks_);
		}
		else
		{
			Move(blocks_, basePosition_, rotatePosition_, moveDown);
		}
	}
}

void Tetromino::DrawBlocks(const std::vector<Block>& blocks)
{
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));

	for (const Block& block : blocks)
	{
		DirectX::XMFLOAT3 position = block.GetPosition();

		effectShader->SetWorldMatrix(DirectX::XMMatrixTranslation(position.x, position.y, position.z));
		effectShader->SetViewMatrix(fixCamera->GetViewMatrix());
		effectShader->SetProjectionMatrix(fixCamera->GetProjectionMatrix());

		effectShader->SetTexture(blockTexture_);
		effectShader->Bind(RenderManager::Get().GetContext());

		blockStaticMesh_->Draw(RenderManager::Get().GetContext());
	}
}

void Tetromino::Move(std::vector<Block>& blocks, DirectX::XMFLOAT3& basePosition, DirectX::XMFLOAT3& rotatePosition, const EMovement& movement)
{
	if (movement == EMovement::NONE) return;

	if (movement == EMovement::CW || movement == EMovement::CCW)
	{
		float angle = DirectX::XM_PIDIV2;
		angle *= (movement == EMovement::CW) ? -1.0f : 1.0f;

		DirectX::XMMATRIX rotateZMatrix = DirectX::XMMatrixRotationZ(angle);
		DirectX::XMVECTOR position;

		for (Block& block : blocks)
		{
			DirectX::XMFLOAT3 blockPosition = block.GetPosition();

			blockPosition.x -= rotatePosition.x;
			blockPosition.y -= rotatePosition.y;
			blockPosition.z -= rotatePosition.z;

			position = DirectX::XMLoadFloat3(&blockPosition);
			position = DirectX::XMVector3Transform(position, rotateZMatrix);
			DirectX::XMStoreFloat3(&blockPosition, position);

			blockPosition.x += rotatePosition.x;
			blockPosition.y += rotatePosition.y;
			blockPosition.z += rotatePosition.z;

			blockPosition = MathHelper::Round(blockPosition);
			block.SetPosition(blockPosition);
		}
	}
	else
	{
		float biasX = (movement == EMovement::LEFT) ? -1.0f : (movement == EMovement::RIGHT ? 1.0f : 0.0f);
		float biasY = (movement == EMovement::UP) ? 1.0f : (movement == EMovement::DOWN ? -1.0f : 0.0f);
		float blockSize = blocks.front().GetSize();

		for (Block& block : blocks)
		{
			DirectX::XMFLOAT3 position = block.GetPosition();

			position.x += (biasX * blockSize);
			position.y += (biasY * blockSize);

			position = MathHelper::Round(position);
			block.SetPosition(position);
		}

		basePosition.x += (biasX * blockSize);
		basePosition.y += (biasY * blockSize);
		basePosition = MathHelper::Round(basePosition);

		rotatePosition.x += (biasX * blockSize);
		rotatePosition.y += (biasY * blockSize);
		rotatePosition = MathHelper::Round(rotatePosition);
	}
}

bool Tetromino::CanMove(
	std::vector<Block>& blocks, 
	DirectX::XMFLOAT3& basePosition, 
	DirectX::XMFLOAT3& rotatePosition,
	const Board* board, 
	const EMovement& movement
)
{
	Move(blocks, basePosition, rotatePosition, movement);
	bool bCanMove = !IsCollision(blocks, board);
	Move(blocks, basePosition, rotatePosition, GetCountMovement(movement));

	return bCanMove;
}

void Tetromino::JumpBottom(
	std::vector<Block>& blocks, 
	DirectX::XMFLOAT3& basePosition, 
	DirectX::XMFLOAT3& rotatePosition,
	const Board* board
)
{
	while (CanMove(blocks, basePosition, rotatePosition, board, EMovement::DOWN))
	{
		Move(blocks, basePosition, rotatePosition, EMovement::DOWN);
	}
}

bool Tetromino::IsCollision(const std::vector<Block>& blocks, const Board* board) const
{
	return IsCollisionBlocks(blocks, board->GetOutlineBlocks()) || IsCollisionBlocks(blocks, board->GetInnerBlocks());
}

Tetromino::EMovement Tetromino::GetCountMovement(const EMovement& movement) const
{
	EMovement countMovement = EMovement::NONE;

	switch (movement)
	{
	case EMovement::LEFT:
		countMovement = EMovement::RIGHT;
		break;

	case EMovement::RIGHT:
		countMovement = EMovement::LEFT;
		break;

	case EMovement::UP:
		countMovement = EMovement::DOWN;
		break;

	case EMovement::DOWN:
		countMovement = EMovement::UP;
		break;

	case EMovement::CW:
		countMovement = EMovement::CCW;
		break;

	case EMovement::CCW:
		countMovement = EMovement::CW;
		break;

	default:
		countMovement = EMovement::NONE;
		break;
	}

	return countMovement;
}

bool Tetromino::IsCollisionBlocks(const std::vector<Block> blocks, const std::vector<Block>& checkBlocks) const
{
	for (const Block& checkBlock : checkBlocks)
	{
		for (const Block& block : blocks)
		{
			if (checkBlock.IsOverlap(block))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool Tetromino::IsCollisionBlocks(const std::vector<Block> blocks, const std::list<Block>& checkBlocks) const
{
	for (const Block& checkBlock : checkBlocks)
	{
		for (const Block& block : blocks)
		{
			if (checkBlock.IsOverlap(block))
			{
				return true;
			}
		}
	}

	return false;
}
