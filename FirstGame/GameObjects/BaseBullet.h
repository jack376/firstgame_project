#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Monster;
class BaseGun;
class BaseBullet : public SpriteGo
{
protected:
	BaseGun* baseGun = nullptr;

	const std::list<Monster*>* monsters;

	sf::Vector2f bulletDirection;
	float bulletSpeed    = 500.0f;
	float bulletRange    = 1000.0f;
	float bulletCooldown = 0.5f;
	int   bulletDamage   = 5;

	sf::Vector2f bulletSpriteCenter;
	sf::FloatRect bulletCollider = sf::FloatRect(0.0f, 0.0f, 30.0f, 30.0f);
	sf::RectangleShape bulletColliderDraw;

	bool toggleColliderDraw = false;

public:
	ObjectPool<BaseBullet>* bulletPool = nullptr;

	BaseBullet(const std::string& textureId = "", const std::string& n = "") 
		: SpriteGo(textureId, n) {}
	virtual ~BaseBullet() override {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;


	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);
	void SetMonsterList(const std::list<Monster*>* list);
};