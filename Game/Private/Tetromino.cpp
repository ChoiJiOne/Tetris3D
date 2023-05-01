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

	blocks_ = {
		Block(DirectX::XMFLOAT3(-3.0f, 0.0f, 0.0f), blockSize, blockColor),
		Block(DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f), blockSize, blockColor),
		Block(DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f), blockSize, blockColor),
		Block(DirectX::XMFLOAT3(+3.0f, 0.0f, 0.0f), blockSize, blockColor),
	};
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
		outBlocks[0];
		outBlocks[1];
		outBlocks[2];
		outBlocks[3];
		break;

	case EShape::O:
		break;

	case EShape::T:
		break;

	case EShape::J:
		break;

	case EShape::L:
		break;

	case EShape::S:
		break;

	case EShape::Z:
		break;

	default:
		ENFORCE_THROW_EXCEPTION("undefined tetromino shape type...");
	}
	

}
