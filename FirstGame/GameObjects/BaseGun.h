#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class BaseBullet;
class Character;
class BaseGun : public SpriteGo
{
protected:
	float flowTime = 0.0f;

	Character* player = nullptr;
	BaseBullet* bullet = nullptr;
	
	sf::Vector2f look;
	sf::Vector2f direction;

	float fireRecoilAnimationSpeed = 15.0f;

	sf::Sprite fireRecoilEffect;
	bool isFireRecoilEffect = false;
	float fireRecoilEffectDuration = 0.0f;

	ObjectPool<BaseBullet> poolBaseBullets;

public:
	BaseGun(Character* player, const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n), player(player) {}
	virtual ~BaseGun() override {}

	virtual void Init() override;
	virtual void Release() override;

	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void FireRecoilAnimation(float amount, float flowTimeBySpeed);
	void UpdateFlipAndRotation(bool flip, float angle);
};