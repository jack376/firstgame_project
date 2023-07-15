#include "stdafx.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

Character::Character(const std::string& n = "")
	: GameObject(n)
{
}

void Character::Init()
{
}

void Character::Reset()
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

void Character::Update(float dt)
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

	float magnitude = Utils::Magnitude(direction);

	if (magnitude > 0.0f) // Move
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
	else // Idle
	{
		status = StatusType::Idle;

		animationSpeed = 1.0f;

		legR.setPosition(position.x + legsWalkWidth, position.y);
		legR.setScale(-1.0f, 1.0f);

		legL.setPosition(position.x - legsWalkWidth, position.y);
		legL.setScale(1.0f, 1.0f);
	}
	BodyAnimation(1.0f, 0.2f, flowTime);
	//GunAnimation(flowTime);

	//std::cout << "TEST : " << fixDirectionX << std::endl;
}

void Character::Draw(sf::RenderWindow& window)
{
	window.draw(legR);
	window.draw(legL);
	window.draw(body);
}

bool Character::GetFlipX() const
{
	return filpX;
}

void Character::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = body.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	body.setScale(scale);
}

sf::Vector2f Character::GetOrigin() const
{
	return body.getOrigin();
}

Character::StatusType Character::GetStatus() const
{
	return status;
}

void Character::BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	bodyAnimation.x = -sin(flowTimeBySpeed * 2.0f * M_PI); // -1 ~ 1
	float scaleX = defaultScale + ((bodyAnimation.x + 1.0f) / 2.0f) * scaleRange; 
	//                 100%       +       0 ~ 2      /       1    *    0.2f)

	//std::cout << "TEST : " << scaleX << std::endl;

	scaleX = GetFlipX() ? -abs(scaleX) : abs(scaleX);

	bodyAnimation.y = sin(flowTimeBySpeed * 2.0f * M_PI);
	float scaleY = defaultScale + ((bodyAnimation.y + 1.0f) / 2.0f) * scaleRange;

	body.setScale(scaleX, scaleY);
}

void Character::LegsAnimation(float walkWidth, float flowTimeBySpeed)
{
	legsAnimation.x = sin(flowTimeBySpeed * 1.0f * M_PI);
	float PositionX = (legsAnimation.x + 1.0f) * walkWidth;

	if (GetFlipX()) // ¿ÞÂÊ
	{
		legR.setPosition(PositionX + position.x - walkWidth, position.y);
		legR.setScale(1.0f, 1.0f);

		legL.setPosition(-PositionX + position.x + walkWidth, position.y);
		legL.setScale(-1.0f, 1.0f);
	}
	else if (!GetFlipX()) // ¿À¸¥ÂÊ
	{
		legR.setPosition(PositionX + position.x - walkWidth, position.y);
		legR.setScale(-1.0f, 1.0f);

		legL.setPosition(-PositionX + position.x + walkWidth, position.y);
		legL.setScale(1.0f, 1.0f);
	}
}