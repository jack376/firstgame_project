#include "stdafx.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "SpriteEffect.h"
#include "ResourceMgr.h"

SpriteEffect::SpriteEffect(const float& flowTime, const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n), flowTime(flowTime)
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
}

void SpriteEffect::Update(float dt)
{
	flowTime += dt;
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
}

void SpriteEffect::SetDuration(float duration)
{
	this->duration = duration; 
}

void SpriteEffect::SetPool(ObjectPool<SpriteEffect>* pool)
{
	this->pool = pool;
}