#include "ContentManager.h"
#include "FixCamera.h"
#include "RenderManager.h"
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
	blockStaticMesh_ =  ContentManager::Get().GetStaticMesh("Block");
	blockTexture_ = ContentManager::Get().GetTexture2D(Block::GetColorTextureSignature(blockColor));
	
	GenerateShapeBlocks(shape_, basePosition_, blockSize, blockColor, rotatePosition_, blocks_);
}

Tetromino::~Tetromino()
{
}

void Tetromino::Tick(float deltaSeconds)
{
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextureNoEffectShader* effectShader = reinterpret_cast<TextureNoEffectShader*>(ContentManager::Get().GetEffectShader("TextureNoEffectShader"));
	
	for (Block& block : blocks_)
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
