#pragma once
#include "Character.h"

class Player;
class Monster : public Character
{
protected:
	Player* player;
	float monsterMoveSpeed = 50.f;

public:
	Monster(Player* player, const std::string& n = "") : Character(n), player(player) {}
	virtual ~Monster() override { Release(); }

	sf::FloatRect GetGlobalBounds() const;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};