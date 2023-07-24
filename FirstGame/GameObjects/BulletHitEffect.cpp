#include "stdafx.h"
#include "BulletHitEffect.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

BulletHitEffect::BulletHitEffect(const float& flowTime, const std::string& textureId, const std::string& n)
    : SpriteEffect(flowTime, textureId, n)
{
}

void BulletHitEffect::Init()
{
    SpriteEffect::Init();
}

void BulletHitEffect::Reset()
{
    SpriteEffect::Reset();
}

void BulletHitEffect::Update(float dt)
{
	flowTime += dt;

	ScaleAnimation(0.1f, 0.9f, flowTime);

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
	}
}

void BulletHitEffect::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
}

void BulletHitEffect::SetDuration(float duration)
{
	this->duration = duration;
}

void BulletHitEffect::SetPool(ObjectPool<BulletHitEffect>* pool)
{
	this->pool = pool;
}

void BulletHitEffect::ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	scaleAnimation.x = sin(flowTimeBySpeed * 15.0f * _PI);
	float scaleX = defaultScale + ((scaleAnimation.x + 1.0f) / 2.0f) * scaleRange;

	scaleAnimation.y = sin(flowTimeBySpeed * 15.0f * _PI);
	float scaleY = defaultScale + ((scaleAnimation.y + 1.0f) / 2.0f) * scaleRange;

	sprite.setScale(scaleX, scaleY);
}
