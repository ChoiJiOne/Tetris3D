#include "ContentManager.h"
#include "FixCamera.h"
#include "RenderManager.h"
#include "TTFont.h"
#include "TextNoEffectShader.h"
#include "Texture2D.h"
#include "SpriteNoEffectShader.h"
#include "WorldManager.h"
#include "Widget.h"

Widget::Widget(ConstructorParam&& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		constructorParam.width,
		constructorParam.height,
		constructorParam.backgroundSignature,
		constructorParam.backgroundTransparency,
		constructorParam.text,
		constructorParam.fontSignature,
		constructorParam.textColor
	)
{}

Widget::Widget(const ConstructorParam& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		constructorParam.width,
		constructorParam.height,
		constructorParam.backgroundSignature,
		constructorParam.backgroundTransparency,
		constructorParam.text,
		constructorParam.fontSignature,
		constructorParam.textColor
	)
{}

Widget::Widget(ConstructorParamWithoutBackground&& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		0.0f,
		0.0f,
		"",
		0.0,
		constructorParam.text,
		constructorParam.fontSignature,
		constructorParam.textColor
	)
{}

Widget::Widget(const ConstructorParamWithoutBackground& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		0.0f,
		0.0f,
		"",
		0.0,
		constructorParam.text,
		constructorParam.fontSignature,
		constructorParam.textColor
	)
{}

Widget::Widget(ConstructorParamWithoutText&& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		constructorParam.width,
		constructorParam.height,
		constructorParam.backgroundSignature,
		constructorParam.backgroundTransparency,
		L"",
		"",
		DirectX::XMFLOAT4()
	)
{}

Widget::Widget(const ConstructorParamWithoutText& constructorParam)
	: Widget(
		constructorParam.updateOrder,
		constructorParam.bIsActive,
		constructorParam.center,
		constructorParam.width,
		constructorParam.height,
		constructorParam.backgroundSignature,
		constructorParam.backgroundTransparency,
		L"",
		"",
		DirectX::XMFLOAT4()
	)
{}

Widget::Widget(
	int32_t updateOrder, 
	bool bIsActive, 
	const DirectX::XMFLOAT2& center, 
	float width, 
	float height, 
	const std::string& backgroundSignature, 
	float backgroundTransparency, 
	const std::wstring& text, 
	const std::string& fontSignature,
	const DirectX::XMFLOAT4& textColor
) : GameObject(updateOrder, bIsActive)
, center_(center)
, width_(width)
, height_(height)
, backgroundTransparency_(backgroundTransparency)
, text_(text)
, textColor_(textColor)
{
	background_ = ContentManager::Get().GetTexture2D(backgroundSignature);
	font_ = ContentManager::Get().GetTTFont(fontSignature);
}

Widget::~Widget()
{
}

void Widget::Tick(float deltaSeconds)
{
	RenderManager::Get().SetDepthBuffer(false);
	FixCamera* fixCamera = reinterpret_cast<FixCamera*>(WorldManager::Get().GetGameObject("FixCamera"));

	if (fixCamera)
	{
		DrawBackgroundUI(fixCamera);
		DrawTextUI(fixCamera);
	}
	
	RenderManager::Get().SetDepthBuffer(true);
}

void Widget::DrawBackgroundUI(FixCamera* fixCamera)
{
	if (background_ == nullptr) return;

	DirectX::XMMATRIX orthoMatrix = fixCamera->GetOrthoMatrix();

	SpriteNoEffectShader* spriteNoEffectShader = reinterpret_cast<SpriteNoEffectShader*>(ContentManager::Get().GetEffectShader("SpriteNoEffectShader"));
	spriteNoEffectShader->SetProjectionMatrix(orthoMatrix);
	spriteNoEffectShader->DrawSprite2D(RenderManager::Get().GetContext(), background_, center_, width_, height_, backgroundTransparency_);
}

void Widget::DrawTextUI(FixCamera* fixCamera)
{
	if (font_ == nullptr) return;

	DirectX::XMMATRIX orthoMatrix = fixCamera->GetOrthoMatrix();

	TextNoEffectShader* textNoEffectShader = reinterpret_cast<TextNoEffectShader*>(ContentManager::Get().GetEffectShader("TextNoEffectShader"));
	textNoEffectShader->SetProjectionMatrix(orthoMatrix);
	textNoEffectShader->DrawText2D(RenderManager::Get().GetContext(), font_, text_, center_, textColor_);
}