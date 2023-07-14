#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
protected:
	AnimationController animation;

	sf::Vector2f direction;
	sf::Vector2f bodyAnimation;

	float bodyAnimationSpeed = 2.0f;
	float moveSpeed = 500.f;
	float flowTime = 0.0f;

	bool filpX = false;

public:
	Player(const std::string& textureId = "", const std::string& n = "");
	virtual ~Player() override { Release(); }

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);

	void SetScale(float x, float y);

	void BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};