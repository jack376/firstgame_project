#pragma once
#include "SpriteEffect.h"
#include "ObjectPool.h"

class EntityEffect : public SpriteEffect
{
protected:
	ObjectPool<EntityEffect>* pool = nullptr;

public:
	std::function<void()> onEntityEffectComplete;

	EntityEffect(const float& flowTime = 0.0f, const std::string& textureId = "", const std::string& n = "");
	virtual ~EntityEffect() override {};

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDuration(float duration);
	void SetPool(ObjectPool<EntityEffect>* pool);

	void SetOnEntityEffectComplete(std::function<void()> callBack);

	void GlitchAnimation(float range, float flowTimeBySpeed);
};

