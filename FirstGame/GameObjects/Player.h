#pragma once
#include "Character.h"

class BaseBullet;
class Character;
class Player : public Character
{
protected:
	bool isAlive = true;

	int level = 1;

	int currentExp = 0;
	float currentHp  = 20;

	int maxExp        = 16;
	float maxHp       = 20;
	float hpRegen     = 0.0f;
	float damage      = 0.0f;
	float attackSpeed = 1.0f;
	float critical    = 0.03f;
	float armor       = 0.0f;
	float dodge       = 0.0f;
	float moveSpeed   = 500.0f;

	std::map<std::string, float*> stats = 
	{
		{"maxHp", &maxHp},
		{"hpRegen", &hpRegen},
		{"damage", &damage},
		{"attackSpeed", &attackSpeed},
		{"critical", &critical},
		{"armor", &armor},
		{"dodge", &dodge},
		{"moveSpeed", &moveSpeed}
	};

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

	void OnHitted(float damage);
	void OnKilled();
	void OnDie();

	void UpgradeStat(const std::string& statName, float increaseAmount);
	float GetDamage() const;
};