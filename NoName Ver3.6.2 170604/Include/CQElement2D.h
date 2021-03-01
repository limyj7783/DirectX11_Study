#pragma once

using namespace SimpleMath;

class CQElement2D
{
public:

	//GETTER
	virtual const Vector2& GetPosition() const = 0;
	virtual const Vector2& GetOrigin() const = 0;
	virtual const Vector2& GetScale() const = 0;
	virtual const Color& GetColor() const = 0;
	virtual const float& GetAlpha() const = 0;
	virtual const float& GetRotation() const = 0;

	//SETTER
	virtual void SetPostion(const Vector2& position) = 0;
	virtual void SetOrigin(const Vector2& origin) = 0;
	virtual void SetScale(const Vector2& scale) = 0;
	virtual void SetColor(const Color& color) = 0;
	virtual void SetAlpha(const float alpha) = 0;
	virtual void SetRotation(const float rotation) = 0;

};
