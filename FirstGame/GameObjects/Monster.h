#pragma once
#include "Character.h"

class Player;
class Monster : public Character
{
public:
	enum class Types
	{
		Normal = 0,
		Faster = 1,
		Heavy  = 2,
	};

	/*
	static const int totalTypes = 3;
	static const std::string textureIds[3];
	static const float speedStats[3];
	static const int hpStats[3];
	static const int damageStats[3];
	static const float attackRateStats[3];
	*/

protected:
	Types monsterType;

	std::string textureId;
	float speed = 0.0f;
	int maxHp = 0;
	int damage = 0;
	float attackRate = 0.0f;

	int hp = 0;
	float attackTimer = 0.0f;

	Player* player;

	//float monsterMoveSpeed = 50.f;

public:
	Monster(const std::string& n = "") : Character(n) {}
	virtual ~Monster() override { Release(); }

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	sf::FloatRect GetGlobalBounds() const;

	void SetType(Types t);
	Types GetType() const;

	void SetPlayer(Player* player);
};