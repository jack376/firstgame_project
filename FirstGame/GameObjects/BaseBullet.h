#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class SpriteEffect;
class Monster;
class BaseGun;

class BaseBullet : public SpriteGo
{
protected:
	float flowTime = 0.0f;

	BaseGun* baseGun = nullptr;
	const std::list<Monster*>* monsters;

	sf::Vector2f bulletDirection;
	float bulletSpeed  = 1500.0f;
	float bulletRange  = 1000.0f;
	float bulletCooldown;
	int   bulletDamage = 25;
	float bulletLife   = 0.05f;

	sf::FloatRect bulletCollider = sf::FloatRect(0.0f, 0.0f, 30.0f, 30.0f);
	sf::RectangleShape bulletColliderDraw;

	Monster* targetMonster;

public:
	ObjectPool<BaseBullet>* pool;

	BaseBullet(const std::string& textureId = "", const std::string& n = "") 
		: SpriteGo(textureId, n) {}
	virtual ~BaseBullet() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed);
	void SetMonsterList(const std::list<Monster*>* list);
	void SetTargetMonster(Monster* nearMonster);

	void BulletHitAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};