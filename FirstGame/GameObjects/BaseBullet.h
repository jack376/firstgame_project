#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Monster;
class BaseGun;
class BaseBullet : public SpriteGo
{
protected:
	BaseGun* baseGun = nullptr;
	Monster* targetMonster = nullptr;

	sf::Vector2f bulletDirection;
	
	float bulletSpeed    = 500.0f;
	float bulletRange    = 500.0f;
	float bulletCooldown = 0.5f;
	int   bulletDamage   = 5;

public:
	ObjectPool<BaseBullet>* bulletPool = nullptr;

	BaseBullet(const std::string& textureId = "", const std::string& n = "") 
		: SpriteGo(textureId, n) {}
	virtual ~BaseBullet() override {}

	void Fire(const sf::Vector2f& position, const sf::Vector2f& direction, float speed);

	void SetTargetMonster(Monster* monster);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};