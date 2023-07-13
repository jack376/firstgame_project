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
	/*
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Idle.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Move.csv"));
    animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Jump.csv"));

    animation.SetTarget(&sprite);
	*/
}

void Player::Reset()
{
	//animation.Play("Idle");
	SetOrigin(75.0f, 110.0f);
	SetPosition({ 0, 0 });
	SetFlipX(false);

	SpriteGo::Reset();
}

void Player::Update(float dt)
{
	flowTime += dt;
	
	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 0.0f) // Move
	{
		bodyAnimationSpeed = 4.0f;

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
		bodyAnimationSpeed = 2.0f;
	}

	//float speedChangeRate = 0.01f;
	//bodyAnimationSpeed = Utils::Lerp(bodyAnimationSpeed, targetSpeed, 0.1f);

	BodyAnimation(1.0f, 0.2f, bodyAnimationSpeed, flowTime);

	std::cout << bodyAnimationSpeed << std::endl;
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

bool Player::GetFlipX() const
{
	return filpX;
}

void Player::BodyAnimation(float scale, float range, float speed, float flowTime)
{
	bodyAnimation.x = -sin(flowTime * speed * _PI);
	float scaleX = scale + ((bodyAnimation.x + 1.0f) / 2.0f) * range;

	scaleX = GetFlipX() ? -abs(scaleX) : abs(scaleX);

	bodyAnimation.y = sin(flowTime * speed * _PI);
	float scaleY = scale + ((bodyAnimation.y + 1.0f) / 2.0f) * range;

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

