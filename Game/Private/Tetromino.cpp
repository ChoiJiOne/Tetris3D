#include "ContentManager.h"
#include "FixCamera.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "Texture2D.h"
#include "TextureNoEffectShader.h"
#include "WorldManager.h"
#include "Tetromino.h"

Tetromino::Tetromino(int32_t updateOrder, bool bIsActive, float blockSize, const Block::EColor& blockColor)
	: GameObject(updateOrder, bIsActive)
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
