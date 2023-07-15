#include "stdafx.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "SpriteEffect.h"


SpriteEffect::SpriteEffect(const std::string& textureId = "", const std::string& n = "")
	: SpriteGo(textureId, n)
{
}

void SpriteEffect::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void SpriteEffect::Reset()
{
	SpriteGo::Reset();
	timer = 0.f;
	sprite.setColor({ 255, 255, 255, 255 });
}

void SpriteEffect::Update(float dt)
{
	timer += dt;
	//UINT8 a = Utils::Lerp(255, 0, (timer / duration));
	//sprite.setColor({ 255, 255, 255, a });

	if (timer > duration)
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
