#include "ContentManager.h"
#include "FixCamera.h"
#include "RenderManager.h"
#include "InputManager.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "WorldManager.h"
#include "Tetromino.h"

Tetromino::Tetromino(
	int32_t updateOrder,
	bool bIsActive,
	const DirectX::XMFLOAT3& basePosition,
	const EShape& shape,
	float blockSize,
	const Block::EColor& blockColor
) : GameObject(updateOrder, bIsActive)
, shape_(shape)
, basePosition_(basePosition)
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
	if (InputManager::Get().GetKeyPressState(EVirtualKey::CODE_LEFT) == EPressState::PRESSED)
	{
		Move(EMovement::LEFT);
	}

	if (InputManager::Get().GetKeyPressState(EVirtualKey::CODE_RIGHT) == EPressState::PRESSED)
	{
		Move(EMovement::RIGHT);
	}

	if (InputManager::Get().GetKeyPressState(EVirtualKey::CODE_UP) == EPressState::PRESSED)
	{
		Move(EMovement::UP);
	}

	if (InputManager::Get().GetKeyPressState(EVirtualKey::CODE_DOWN) == EPressState::PRESSED)
	{
		Move(EMovement::DOWN);
	}

	if (InputManager::Get().GetKeyPressState(EVirtualKey::CODE_SPACE) == EPressState::PRESSED)
	{
		Move(EMovement::CW);
	}

	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));
	
	for (const Block& block : blocks_)
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

void Tetromino::Move(const EMovement& movement)
{
	if (movement == EMovement::NONE) return;

	if (movement == EMovement::CW || movement == EMovement::CCW)
	{
		float angle = DirectX::XM_PIDIV2;
		angle *= (movement == EMovement::CW) ? -1.0f : 1.0f;

		DirectX::XMMATRIX rotateZMatrix = DirectX::XMMatrixRotationZ(angle);
		DirectX::XMVECTOR position;

		for (Block& block : blocks_)
		{
			DirectX::XMFLOAT3 blockPosition = block.GetPosition();

			blockPosition.x -= rotatePosition_.x;
			blockPosition.y -= rotatePosition_.y;
			blockPosition.z -= rotatePosition_.z;

			position = DirectX::XMLoadFloat3(&blockPosition);
			position = DirectX::XMVector3Transform(position, rotateZMatrix);
			DirectX::XMStoreFloat3(&blockPosition, position);

			blockPosition.x += rotatePosition_.x;
			blockPosition.y += rotatePosition_.y;
			blockPosition.z += rotatePosition_.z;

			block.SetPosition(blockPosition);
		}
	}
	else
	{
		float biasX = (movement == EMovement::LEFT) ? -1.0f : (movement == EMovement::RIGHT ? 1.0f : 0.0f);
		float biasY = (movement == EMovement::UP) ? 1.0f : (movement == EMovement::DOWN ? -1.0f : 0.0f);
		float blockSize = blocks_.front().GetSize();

		for (Block& block : blocks_)
		{
			DirectX::XMFLOAT3 position = block.GetPosition();

			position.x += (biasX * blockSize);
			position.y += (biasY * blockSize);

			block.SetPosition(position);
		}

		basePosition_.x += (biasX * blockSize);
		basePosition_.y += (biasY * blockSize);

		rotatePosition_.x += (biasX * blockSize);
		rotatePosition_.y += (biasY * blockSize);
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