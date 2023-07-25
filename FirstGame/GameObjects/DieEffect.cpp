#include "stdafx.h"
#include "DieEffect.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

DieEffect::DieEffect(const float& flowTime, const std::string& textureId, const std::string& n)
    : SpriteEffect(flowTime, textureId, n)
{
}

void DieEffect::Init()
{
    SpriteEffect::Init();
}

void DieEffect::Reset()
{
    SpriteEffect::Reset();
    sprite.setColor({ 255, 255, 255, 255 });
}

void DieEffect::Update(float dt)
{
	flowTime += dt;

	sf::Vector2f scaleFx = Utils::Lerp(sf::Vector2f(0.05f, 0.05f), sf::Vector2f(1.35f, 1.35f), (flowTime / (duration / 25)));
	sprite.setScale(scaleFx);

	UINT8 colorFx = Utils::Lerp(255, 0, (flowTime / duration));
	sprite.setColor({ 255, 255, 255, colorFx });

	if (flowTime > duration)
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
			pool->Return(this);
			
		}
		else
		{
			SetActive(false);
		}

		if (onSpawnEffectComplete)
		{
			onSpawnEffectComplete();
		}
	}
}

void DieEffect::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void DieEffect::SetDuration(float duration)
{
	this->duration = duration;
}

void DieEffect::SetPool(ObjectPool<DieEffect>* pool)
{
	this->pool = pool;
}

void DieEffect::SetOnSpawnEffectComplete(std::function<void()> callBack)
{
	onSpawnEffectComplete = callBack;
}