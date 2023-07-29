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
	sf::Vector2f direction;

	float flowTime = 0.0f;
	float deltaTime = 0.0f;

	float animationSpeed = 2.0f;
	float legsIdleWidth  = 15.0f;
	float legsWalkWidth  = 2.0f;

	sf::FloatRect wallBounds;
	sf::Vector2f wallBoundsLT;
	sf::Vector2f wallBoundsRB;

	bool filpX = false;

	float hitColorOverlayDuration = 0.1f;

public:
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