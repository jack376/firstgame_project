#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class BaseBullet;
class Player;
class BaseGun : public SpriteGo
{
protected:
	float flowTime = 0.0f;

	Player* player = nullptr;
	BaseBullet* bullet = nullptr;
	
	sf::Vector2f look;
	sf::Vector2f direction;

	bool isFire = false;
	sf::Sprite fireRecoilEffect;
	bool isFireRecoilEffect = false;
	float fireRecoilEffectDuration = 0.0f;
	float fireRecoilAnimationSpeed = 12.0f;

	ObjectPool<BaseBullet> poolBaseBullets;

public:
	BaseGun(Player* player, const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n), player(player) {}
	virtual ~BaseGun() override {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void FireRecoilAnimation(float amount, float flowTimeBySpeed);
	void UpdateFlipAndRotation(bool flip, float angle);

	void SetFire(bool test);
};