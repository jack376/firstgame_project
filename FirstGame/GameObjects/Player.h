#pragma once
#include "Character.h"

class Character;
class Player : public Character
{
protected:
	bool  isAlive     = true;

	int   level       = 1;
	int   maxHp       = 100;
	int   currentHp   = 100;
	int   maxExp      = 200;
	int   currentExp  = 0;

	float moveSpeed   = 500.0f;
	float attackSpeed = 1.0f;
	float armorPoint  = 0.0f;
	float dodgePoint  = 0.0f;
	float critical    = 0.03f;

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
	void OnDie();
};