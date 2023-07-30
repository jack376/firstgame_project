#pragma once
#include "Character.h"

class Character;
class Player : public Character
{
protected:
	bool isAlive = true;
	int level      = 1;
	int currentExp = 0;
	int currentHp = 20;

	int   maxExp      = 16;
	int   maxHp       = 20;
	float hpRegen     = 0.0f;
	float damage      = 0.0f;
	float attackSpeed = 1.0f;
	float critical    = 0.03f;
	float armor       = 0.0f;
	float dodge       = 0.0f;
	float moveSpeed   = 500.0f;

	sf::Vector2f playerBodyCenter;
	sf::FloatRect playerCollider;
	sf::RectangleShape playerColliderDraw;

public:
	Player(const std::string& n = "") : Character(n) {}
	virtual ~Player() override { Release(); }

	void Init();
	void Reset();

	void Update(float dt);
	void Draw(sf::RenderWindow& window);

	sf::FloatRect GetPlayerCollider() const;

	void OnHitted(int damage);
	void OnKilled();
	void OnDie();
};