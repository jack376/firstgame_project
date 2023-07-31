#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

class Player;
class SpriteEffect;
class BulletHitEffect;
class DieEffect;
class Monster;
class BaseGun;

class BaseBullet : public SpriteGo
{
protected:
	float flowTime = 0.0f;

	Player* myPlayer = nullptr;
	const std::list<Monster*>* monsters;

	sf::Vector2f bulletDirection;
	float bulletSpeed  = 1500.0f;
	float bulletRange  = 700.0f; // 니어몬스터서치랑 동기화 예정
	float bulletCooldown;
	float bulletDamage = 50.0f;
	//float bulletLife   = 0.05f;

	sf::FloatRect bulletCollider = sf::FloatRect(0.0f, 0.0f, 30.0f, 30.0f);
	sf::RectangleShape bulletColliderDraw;
	
	bool isBulletColliderDrawView = false;
	
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

	void SetBulletDamage(Player* player);
};