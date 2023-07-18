#pragma once
#include "GameObject.h"

class GameObject;
class Player;
class Character : public GameObject
{
public:
	enum class StatusType
	{
		Idle,
		Move,
		Attack,
	};

protected:
	StatusType status = StatusType::Idle;

	sf::Sprite body;
	sf::Vector2f bodyAnimation;

	sf::Sprite legL;
	sf::Sprite legR;
	sf::Vector2f legsAnimation;

	sf::Vector2f look;

	//sf::Sprite equipGun;
	//sf::Vector2f gunAnimation;

	sf::Vector2f direction;

	float moveSpeed = 500.f;
	float flowTime = 0.0f;

	float animationSpeed = 2.0f;
	float legsIdleWidth = 15.0f;
	float legsWalkWidth = 2.0f;

	sf::FloatRect wallBounds;


	bool filpX = false;

public:
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;


	Character(const std::string& n = "") : GameObject(n) {}
	virtual ~Character() override { Release(); }

	virtual void Init() override = 0;
	virtual void Reset() override = 0;
	virtual void Update(float dt) override = 0;
	virtual void Draw(sf::RenderWindow& window) override = 0;

	bool GetFlipX() const;
	void SetFlipX(bool filp);

	sf::Vector2f GetOrigin() const;
	StatusType GetStatus() const;

	void BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
	void LegsAnimation(float walkWidth, float flowTimeBySpeed);

	void SetWallBounds(const sf::FloatRect& bounds);

	sf::FloatRect GetWallBounds();
};