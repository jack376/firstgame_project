#include "stdafx.h"
#include "Player.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

Player::Player(const std::string& textureId, const std::string& n) 
    : SpriteGo(textureId, n)
{
}

void Player::Init()
{
	SpriteGo::Init();
}

void Player::Reset()
{
	SetOrigin(75.0f, 110.0f);
	SetPosition({ 0, 0 });
	SetFlipX(false);

	SpriteGo::Reset();
}

void Player::Update(float dt)
{
	flowTime += dt * bodyAnimationSpeed;
	
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);

	if (magnitude > 0.0f) // Move
	{
		bodyAnimationSpeed = 3.0f;

		if (direction.x < 0)
		{
			SetFlipX(true);
		}
		else if (direction.x > 0)
		{
			SetFlipX(false);
		}

		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		position += direction * moveSpeed * dt;
		SetPosition(position);
	}
	else // Idle
	{
		bodyAnimationSpeed = 1.0f;
	}

	BodyAnimation(1.0f, 0.2f, flowTime);

	//std::cout << "TEST : " << bodyAnimationSpeed << std::endl;
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

bool Player::GetFlipX() const
{
	return filpX;
}

void Player::BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	bodyAnimation.x = -sin(flowTimeBySpeed * 2.0f * _PI);
	float scaleX = defaultScale + ((bodyAnimation.x + 1.0f) / 2.0f) * scaleRange;

	scaleX = GetFlipX() ? -abs(scaleX) : abs(scaleX);

	bodyAnimation.y = sin(flowTimeBySpeed * 2.0f * _PI);
	float scaleY = defaultScale + ((bodyAnimation.y + 1.0f) / 2.0f) * scaleRange;

	sprite.setScale(scaleX, scaleY);
}

void Player::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = sprite.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	sprite.setScale(scale);
}

void Player::SetScale(float x, float y)
{
	sprite.setScale(x, y);
}

