#include "ContentManager.h"
#include "FixCamera.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "SpriteNoEffectShader.h"
#include "WorldManager.h"
#include "Button.h"

Button::Button(ConstructorParam&& constructorParam)
	: Button(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.relativeCenter,
		constructorParam.width,
		constructorParam.height,
		constructorParam.textureSignature,
		constructorParam.mouseOverTransparency,
		constructorParam.mouseNotOverTransparency,
		constructorParam.pressReduceRatio,
		constructorParam.eventAction
	) {}

Button::Button(const ConstructorParam& constructorParam)
	: Button(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.relativeCenter,
		constructorParam.width,
		constructorParam.height,
		constructorParam.textureSignature,
		constructorParam.mouseOverTransparency,
		constructorParam.mouseNotOverTransparency,
		constructorParam.pressReduceRatio,
		constructorParam.eventAction
	) {}

Button::Button(
	int32_t updateOrder, 
	bool bIsActive, 
	const DirectX::XMFLOAT2& relativeCenter, 
	float width, 
	float height, 
	const std::string& textureSignature, 
	float mouseOverTransparency,
	float mouseNotOverTransparency, 
	float pressReduceRatio, 
	const std::function<void()>& eventAction
) : GameObject(updateOrder, bIsActive)
, relativeCenter_(relativeCenter)
, width_(width)
, height_(height)
, mouseNotOverTransparency_(mouseNotOverTransparency)
, mouseOverTransparency_(mouseOverTransparency)
, pressReduceRatio_(pressReduceRatio)
, eventAction_(eventAction)
{
	texture_ = ContentManager::Get().GetTexture2D(textureSignature);
}

Button::~Button()
{
}

void Button::Tick(float deltaSeconds)
{
	float screenWidth = 0.0f;
	float screenHeight = 0.0f;
	RenderManager::Get().GetBackbufferSize(screenWidth, screenHeight);

	centerInScreen_ = ConvertRelativeToScreenPosition(relativeCenter_, screenWidth, screenHeight);
	centerInWindow_ = ConvertScreenToWindowPosition(centerInScreen_, screenWidth, screenHeight);
	bIsMouseOverButton_ = IsMouseOverButton();

	float width = width_;
	float height = height_;
	float transparency = bIsMouseOverButton_ ? mouseOverTransparency_ : mouseNotOverTransparency_;

	EPressState pressState = InputManager::Get().GetMousePressState(EMouseButton::LEFT);
	if (bIsMouseOverButton_ && (pressState == EPressState::HELD || pressState == EPressState::PRESSED))
	{
		width *= pressReduceRatio_;
		height *= pressReduceRatio_;
	}

	if (bIsMouseOverButton_ && pressState == EPressState::RELEASED)
	{
		eventAction_();
	}
	
	if (texture_)
	{
		DrawUITexture(texture_, centerInScreen_, width, height, transparency);
	}
}

DirectX::XMFLOAT2 Button::ConvertRelativeToScreenPosition(const DirectX::XMFLOAT2& relativePosition, float screenWidth, float screenHeight) const
{
	return DirectX::XMFLOAT2(
		(relativePosition.x * screenWidth) / 2.0f,
		(relativePosition.y * screenHeight) / 2.0f
	);
}

DirectX::XMFLOAT2 Button::ConvertScreenToWindowPosition(const DirectX::XMFLOAT2& screenPosition, float windowWidth, float windowHeight) const
{
	return DirectX::XMFLOAT2(
		windowWidth / 2.0f + screenPosition.x,
		windowHeight / 2.0f - screenPosition.y
	);
}

bool Button::IsMouseOverButton() const
{
	DirectX::XMINT2 mousePosition = InputManager::Get().GetCurrMousePosition();

	float minX = centerInWindow_.x - width_ / 2.0f;
	float maxX = centerInWindow_.x + width_ / 2.0f;
	float minY = centerInWindow_.y - height_ / 2.0f;
	float maxY = centerInWindow_.y + height_ / 2.0f;
	float x = static_cast<float>(mousePosition.x);
	float y = static_cast<float>(mousePosition.y);
	
	return (minX <= x && x <= maxX) && (minY <= y && y <= maxY);
}

void Button::DrawUITexture(Texture2D* texture, const DirectX::XMFLOAT2& center, float width, float height, float transparency)
{
	RenderManager::Get().SetDepthBuffer(false);

	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));
	SpriteNoEffectShader* spriteNoEffectShader = reinterpret_cast<SpriteNoEffectShader*>(ContentManager::Get().GetEffectShader("SpriteNoEffectShader"));

	DirectX::XMMATRIX orthoMatrix = fixCamera->GetOrthoMatrix();

	spriteNoEffectShader->SetProjectionMatrix(orthoMatrix);
	spriteNoEffectShader->DrawSprite2D(RenderManager::Get().GetContext(), texture, center, width, height, transparency);

	RenderManager::Get().SetDepthBuffer(true);
}
