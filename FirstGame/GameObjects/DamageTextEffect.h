#pragma once
#include "ObjectPool.h"
#include "TextGo.h"

class DamageTextEffect : public TextGo
{
protected:	
	float duration = 0.0f;
	float flowTime = 0.0f;

	ObjectPool<DamageTextEffect>* pool = nullptr;
	sf::Vector2f scaleAnimation;

	float animationScale = 1.0f;

public:
	DamageTextEffect(const float& flowTime = 0.0f, const std::string& fontId = "", const std::string& n = "");
	virtual ~DamageTextEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<DamageTextEffect>* pool);

	void ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);

	void SetOutlineColor(const sf::Color& color);
	void SetOutlineThickness(float amount);
};