#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class SpriteEffect : public SpriteGo
{
protected:
	float inverseDuration = 0.0f;
	float duration = 0.0f;
	float flowTime = 0.0f;

	sf::Vector2f scaleAnimation;
	ObjectPool<SpriteEffect>* pool = nullptr;

public:
	SpriteEffect(const float& flowTime = 0.0f, const std::string textureId = "", const std::string& n = "");
	virtual ~SpriteEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<SpriteEffect>* pool);

	void ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};

