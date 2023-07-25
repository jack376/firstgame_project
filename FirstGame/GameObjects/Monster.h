#pragma once
#include "Character.h"
#include "ObjectPool.h"
#include "BulletHitEffect.h"
#include "DieEffect.h"
#include "EntityEffect.h"

class Player;
class Monster : public Character
{
public:
	enum class Types
	{
		Normal   = 0,
		Faster   = 1,
		Heavy    = 2,
		//Range    = 3,
		//Tentacle = 4,
	};

protected:
	Player* player = nullptr;

	Types       monsterType;
	std::string textureId;
	float       moveSpeed  = 0.0f;
	int         maxHp      = 0;
	int         damage     = 0;
	float       attackRate = 0.0f;

	int   currentHp   = 0;
	float attackTimer = 0.0f;

	sf::Vector2f monsterBodyCenter;
	sf::FloatRect monsterCollider = sf::FloatRect(0.0f, 0.0f, 100.0f, 100.0f);
	sf::RectangleShape monsterColliderDraw;

	ObjectPool<BulletHitEffect>* effectPool;
	ObjectPool<DieEffect>* dieEffectPool;
	ObjectPool<EntityEffect>* entityEffectPool;



public:
	Monster(const std::string& n = "") : Character(n) {}
	virtual ~Monster() override { Release(); }

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetMonsterCollider() const;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* player);
	void OnHitBullet(int bulletDamage);

	void SetEntityEffectPool(ObjectPool<EntityEffect>* pool);
	void SetEntityEffect(sf::Vector2f position, std::function<void()> onEntityEffectComplete);

	void SetBulletHitEffectPool(ObjectPool<BulletHitEffect>* pool);
	void SetBulletHitEffect(sf::Vector2f position);

	void SetDieEffectPool(ObjectPool<DieEffect>* pool);
	void SetDieEffect(sf::Vector2f position);
};