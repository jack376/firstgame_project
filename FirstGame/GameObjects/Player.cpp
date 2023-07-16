#include "stdafx.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

void Player::Init()
{
}

void Player::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture("graphics/potato_body_default.png"));
	body.setOrigin(75.0f, 102.0f);
	body.setPosition(0, 0);

	legL.setTexture(*RESOURCE_MGR.GetTexture("graphics/potato_legs_default.png"));
	legL.setOrigin(75.0f, 75.0f);
	legL.setPosition(0, 0);

	legR.setTexture(*RESOURCE_MGR.GetTexture("graphics/potato_legs_default.png"));
	legR.setOrigin(75.0f, 75.0f);
	legR.setPosition(0, 0);

	SetFlipX(false);
}

void Player::Update(float dt)
{
	flowTime += dt * animationSpeed;

	direction.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	direction.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float fixDirectionX = INPUT_MGR.GetAxisRaw(Axis::Horizontal);

	if (fixDirectionX < 0)
	{
		SetFlipX(true);
	}
	else if (fixDirectionX > 0)
	{
		SetFlipX(false);
	}

	if (!wallBounds.contains(position))
	{
		position = Utils::Clamp(position, wallBoundsLT, wallBoundsRB);
	}

	float magnitude = Utils::Magnitude(direction);

	if (magnitude > 0.0f)
	{
		status = StatusType::Move;

		animationSpeed = 3.0f;

		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		position += direction * moveSpeed * dt;
		body.setPosition(position);

		LegsAnimation(legsWalkWidth, flowTime);
	}
	else
	{
		status = StatusType::Idle;

		animationSpeed = 1.0f;

		legR.setPosition(position.x + legsWalkWidth, position.y);
		legR.setScale(-1.0f, 1.0f);

		legL.setPosition(position.x - legsWalkWidth, position.y);
		legL.setScale(1.0f, 1.0f);
	}
	BodyAnimation(1.0f, 0.2f, flowTime);
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(legR);
	window.draw(legL);
	window.draw(body);
}