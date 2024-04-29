#include "stdafx.h"
#include "EntityEffect.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

EntityEffect::EntityEffect(const float& flowTime, const std::string& textureId, const std::string& n)
	: SpriteEffect(flowTime, textureId, n)
{
}

void EntityEffect::Init()
{
	SpriteEffect::Init();
}

void EntityEffect::Reset()
{
	SpriteEffect::Reset();
}

void EntityEffect::Update(float dt)
{
	flowTime += dt;

	GlitchAnimation(64.0f, flowTime);

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

		if (onEntityEffectComplete)
		{
			onEntityEffectComplete();
		}
	}
}

void EntityEffect::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void EntityEffect::SetDuration(float duration)
{
	this->duration = duration;
}

void EntityEffect::SetPool(ObjectPool<EntityEffect>* pool)
{
	this->pool = pool;
}

void EntityEffect::SetOnEntityEffectComplete(std::function<void()> callBack)
{
	onEntityEffectComplete = callBack;
}

void EntityEffect::GlitchAnimation(float range, float flowTimeBySpeed)
{
	float defaultOpacity = sin(flowTimeBySpeed * 10.0f * _PI);
	int changeOpacity = (defaultOpacity + 1.0f) / 2.0f * (255 - range) + range;

	sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(changeOpacity)));
}
