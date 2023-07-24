#pragma once
#include "SpriteEffect.h"
#include "ObjectPool.h"

class BulletHitEffect : public SpriteEffect
{
protected:
	ObjectPool<BulletHitEffect>* pool = nullptr;
	sf::Vector2f scaleAnimation;

public:
	BulletHitEffect(const float& flowTime = 0.0f, const std::string& textureId = "", const std::string& n = "");
	virtual ~BulletHitEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<BulletHitEffect>* pool);

	void ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};