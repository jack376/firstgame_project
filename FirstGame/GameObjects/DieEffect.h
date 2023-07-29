#pragma once
#include "SpriteEffect.h"
#include "ObjectPool.h"

class DieEffect : public SpriteEffect
{
protected:
	ObjectPool<DieEffect>* pool = nullptr;

	float animationScale = 1.0f;

public:
	std::function<void()> onSpawnEffectComplete;

	DieEffect(const float& flowTime = 0.0f, const std::string& textureId = "", const std::string& n = "");
	virtual ~DieEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<DieEffect>* pool);
	void SetOnSpawnEffectComplete(std::function<void()> callBack);
	void SetAnimationScale(float scale);
};

