#pragma once
#include "Character.h"

class Character;
class Player : public Character
{


public:
	Player(const std::string& n = "") : Character(n) {}
	virtual ~Player() override { Release(); }

	void Init();
	void Reset();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);
};