#include "stdafx.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "SpriteEffect.h"
#include "ResourceMgr.h"

SpriteEffect::SpriteEffect(const float& flowTime, const std::string textureId, const std::string& n)
	:SpriteGo(textureId, n), duration(duration), flowTime(flowTime), inverseDuration(1.0f / duration)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	flowTime = 0.0f;
	//sprite.setColor({ 255, 255, 255, 255 });
}

void SpriteEffect::Update(float dt)
{
	flowTime += dt;
	//UINT8 a = Utils::Lerp(255, 0, (flowTime / duration));
	//sprite.setColor({ 255, 255, 255, a });

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

void SpriteEffect::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
}

void SpriteEffect::SetDuration(float duration)
{
	this->duration = duration; 
}

void SpriteEffect::SetPool(ObjectPool<SpriteEffect>* pool)
{
	this->pool = pool;
}

void SpriteEffect::ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	scaleAnimation.x = sin(flowTimeBySpeed * 15.0f * M_PI);
	float scaleX = defaultScale + ((scaleAnimation.x + 1.0f) / 2.0f) * scaleRange;

	scaleAnimation.y = sin(flowTimeBySpeed * 15.0f * M_PI);
	float scaleY = defaultScale + ((scaleAnimation.y + 1.0f) / 2.0f) * scaleRange;

	sprite.setScale(scaleX, scaleY);
}