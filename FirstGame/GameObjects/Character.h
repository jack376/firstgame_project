#pragma once
#include "GameObject.h"

class Character : public GameObject
{
protected:
	sf::Sprite body;
	sf::Texture bodyTexture;
	sf::Vector2f bodyAnimation;
	float bodyAnimationSpeed = 2.0f;

	sf::Sprite legs;
	sf::Texture legsTexture;
	sf::Vector2f legsAnimation;
	float legsAnimationSpeed = 2.0f;

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

	void BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed);
};