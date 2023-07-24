#pragma once
#include "SpriteEffect.h"
#include "ObjectPool.h"

class DieEffect : public SpriteEffect
{
protected:
	ObjectPool<DieEffect>* pool = nullptr;

public:
	DieEffect(const float& flowTime = 0.0f, const std::string& textureId = "", const std::string& n = "");
	virtual ~DieEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<DieEffect>* pool);
};

