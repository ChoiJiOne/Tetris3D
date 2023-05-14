#include "ContentManager.h"
#include "FixCamera.h"
#include "RenderManager.h"
#include "TTFont.h"
#include "TextNoEffectShader.h"
#include "WorldManager.h"
#include "Label.h"

Label::Label(ConstructorParam&& constructorParam)
	: Label(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.relativeCenter,
		constructorParam.fontSignature,
		constructorParam.text,
		constructorParam.color
	) {}

Label::Label(const ConstructorParam& constructorParam)
	: Label(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.relativeCenter,
		constructorParam.fontSignature,
		constructorParam.text,
		constructorParam.color
	) {}

Label::Label(
	int32_t updateOrder,
	bool bIsActive,
	const DirectX::XMFLOAT2& relativeCenter,
	const std::string& fontSignature,
	const std::wstring& text,
	const DirectX::XMFLOAT4& color
) : GameObject(updateOrder, bIsActive)
, relativeCenter_(relativeCenter)
, text_(text)
, color_(color)
{
	font_ = ContentManager::Get().GetTTFont(fontSignature);
}

Label::~Label()
{
}

void Label::Tick(float deltaSeconds)
{
	float screenWidth = 0.0f;
	float screenHeight = 0.0f;
	RenderManager::Get().GetBackbufferSize(screenWidth, screenHeight);

	centerInScreen_ = ConvertRelativeToScreenPosition(relativeCenter_, screenWidth, screenHeight);

	RenderManager::Get().SetDepthBuffer(false);

	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	TextNoEffectShader* textNoEffectShader = reinterpret_cast<TextNoEffectShader*>(ContentManager::Get().GetEffectShader("TextNoEffectShader"));

	DirectX::XMMATRIX orthoMatrix = fixCamera->GetOrthoMatrix();

	textNoEffectShader->SetProjectionMatrix(orthoMatrix);
	textNoEffectShader->DrawText2D(RenderManager::Get().GetContext(), font_, text_, centerInScreen_, color_);

	RenderManager::Get().SetDepthBuffer(true);
}

DirectX::XMFLOAT2 Label::ConvertRelativeToScreenPosition(const DirectX::XMFLOAT2& relativePosition, float screenWidth, float screenHeight) const
{
	return DirectX::XMFLOAT2(
		(relativePosition.x * screenWidth) / 2.0f,
		(relativePosition.y * screenHeight) / 2.0f
	);
}
