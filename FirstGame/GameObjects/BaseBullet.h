#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class BaseGun;
class BaseBullet : public SpriteGo
{
protected:
	BaseGun* baseGun;

	sf::Vector2f direction;

	float bulletSpeed  = 500.0f;
	float bulletRange  = 1000.0f;
	int   bulletDamage = 25;

public:
	ObjectPool<BaseBullet>* bulletPool = nullptr;

	BaseBullet(const std::string& textureId = "", const std::string& n = "") 
		: SpriteGo(textureId, n) {}
	virtual ~BaseBullet() override {}

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};