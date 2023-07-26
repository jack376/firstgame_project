#pragma once
#include "Character.h"

class Character;
class Player : public Character
{
protected:
	bool isAlive = false;

	int level      = 1;
	int maxExp     = 8;
	int currentExp = 0;
	int maxHp      = 20;
	int currentHp  = 20;

	float hpRegen     = 0.0f;
	float damage      = 0.0f;
	float attackSpeed = 1.0f;
	float critical    = 0.03f;
	float armor       = 0.0f;
	float dodge       = 0.0f;
	float moveSpeed   = 300.0f;

	sf::Vector2f playerBodyCenter;
	sf::FloatRect playerCollider = sf::FloatRect(0.0f, 0.0f, 50.0f, 50.0f);
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