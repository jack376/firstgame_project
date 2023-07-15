#pragma once
#include "GameObject.h"

class GameObject;
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

	float animationSpeed = 2.0f;

	sf::Sprite body;
	sf::Vector2f bodyAnimation;

	sf::Sprite legL;
	sf::Sprite legR;
	sf::Vector2f legsAnimation;
	float legsWalkWidth = 15.0f;

	//sf::Sprite equipGun;
	//sf::Vector2f gunAnimation;

	sf::Vector2f direction;

	float moveSpeed = 500.f;
	float flowTime = 0.0f;

	bool filpX = false;

public:
	Character(const std::string& n);
	virtual ~Character() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);

	sf::Vector2f GetOrigin() const;
	StatusType GetStatus() const;

	void BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
	void LegsAnimation(float walkWidth, float flowTimeBySpeed);
};