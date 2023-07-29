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

	ObjectPool<BaseBullet> poolBaseBullets;

	sf::Vector2f look;
	sf::Vector2f direction;

	bool isFire = false;
	sf::Sprite fireRecoilEffect;
	bool isFireRecoilEffect = false;
	float fireRecoilEffectDuration = 0.0f;
	float fireRecoilAnimationSpeed = 12.0f;

	float bulletTotalCooldown = bulletCurrentCooldown;
	float bulletCurrentCooldown = 0.1f;

	float gunOrigin = -60.0f;
	float gunLength = 90.0f;
	sf::Vector2f gunMuzzlePosition;


public:
	BaseGun(Player* player, const std::string& textureId = "", const std::string& n = "")
		: SpriteGo(textureId, n), player(player) {}
	virtual ~BaseGun() override {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void FireRecoilAnimation(const sf::Vector2f direction, float playSpeed, float flowTime);
	void UpdateFlipAndRotation(bool flip, float angle);

	void SetFire(bool test);

	sf::Vector2f GetMuzzlePosition();
};