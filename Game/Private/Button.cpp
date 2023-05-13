#include "ContentManager.h"
#include "RenderManager.h"
#include "Texture2D.h"
#include "SpriteNoEffectShader.h"
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
	centerInScreen_ = GetScreenPositionFromRelative(relativeCenter_);
	texture_ = ContentManager::Get().GetTexture2D(textureSignature);
}

Button::~Button()
{
}

void Button::Tick(float deltaSeconds)
{
	centerInScreen_ = GetScreenPositionFromRelative(relativeCenter_);
}

DirectX::XMFLOAT2 Button::GetScreenPositionFromRelative(const DirectX::XMFLOAT2& relativePosition)
{
	float screenWidth = 0.0f;
	float screenHeight = 0.0f;
	RenderManager::Get().GetBackbufferSize(screenWidth, screenHeight);

	DirectX::XMFLOAT2 screenPosition = relativePosition;
	screenPosition.x *= screenWidth / 2.0f;
	screenPosition.y *= screenHeight / 2.0f;

	screenPosition.x = screenWidth / 2.0f + screenPosition.x;
	screenPosition.y = screenHeight / 2.0f - screenPosition.y;

	return screenPosition;
}