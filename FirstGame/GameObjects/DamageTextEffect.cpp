#include "stdafx.h"
#include "DamageTextEffect.h"
#include "Utils.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

DamageTextEffect::DamageTextEffect(const float& flowTime, const std::string& fontId, const std::string& n)
	: TextGo(n, fontId), flowTime(flowTime)
{
}

void DamageTextEffect::Init()
{
	TextGo::Init();
}

void DamageTextEffect::Reset()
{
	TextGo::Reset();

	text.setFillColor({ 255, 200, 0, 255 });
	text.setOutlineColor({ 0, 0, 0, 255 });

	flowTime = 0.0f;
}

void DamageTextEffect::Update(float dt)
{
	flowTime += dt;

	UINT8 colorFx = Utils::Lerp(255, 0, (flowTime / duration));
	text.setFillColor({ 255, 200, 0, colorFx });
	text.setOutlineColor({ 0, 0, 0, colorFx });
	//ScaleAnimation(10.0f, 0.9f, flowTime);

	if (flowTime > duration)
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
			pool->Return(this);

			//std::cout << "Return Count : " << pool->GetUseList().size() << std::endl;
		}
		else
		{
			SetActive(false);
		}
	}
}

void DamageTextEffect::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void DamageTextEffect::SetDuration(float duration)
{
	this->duration = duration;
}

void DamageTextEffect::SetPool(ObjectPool<DamageTextEffect>* pool)
{
	this->pool = pool;
}

void DamageTextEffect::ScaleAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	scaleAnimation.x = sin(flowTimeBySpeed * 3.0f * _PI);
	float scaleX = defaultScale + ((scaleAnimation.x + 1.0f) / 2.0f) * scaleRange;

	scaleAnimation.y = sin(flowTimeBySpeed * 3.0f * _PI);
	float scaleY = defaultScale + ((scaleAnimation.y + 1.0f) / 2.0f) * scaleRange;

	text.setScale(scaleX, scaleY);
}

void DamageTextEffect::SetOutlineColor(const sf::Color& color)
{
	text.setOutlineColor(color);
}

void DamageTextEffect::SetOutlineThickness(float amount)
{
	text.setOutlineThickness(amount);
}
