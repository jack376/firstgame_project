#pragma once
#include "SpriteGo.h"

class Character;
class BaseGun : public SpriteGo
{
protected:
	Character* player = nullptr;

	sf::Vector2f position;
	sf::Vector2f direction;
	float fireRecoilAnimation;
	float fireRecoilAnimationSpeed = 10.0f;

	float range = 3.0f;
	float cooldown = 1.0f;
	float flowTime;

public:
	sf::Sprite sprite;
	std::string textureId;

	BaseGun(Character* player, const std::string& textureId, const std::string& n);
	virtual ~BaseGun() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void FireRecoilAnimation(float amount, float flowTimeBySpeed);
};