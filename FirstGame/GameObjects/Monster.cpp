#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "Collision.h"

sf::FloatRect Monster::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Monster::Init()
{
}

void Monster::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture("graphics/monster.png"));

	sf::Vector2f spriteCenter = sf::Vector2f(body.getTexture()->getSize().x / 2, body.getTexture()->getSize().y / 2);
	body.setOrigin(spriteCenter);

	body.setPosition(0, 0);

	SetFlipX(false);
}

void Monster::Update(float dt)
{
	flowTime += dt * animationSpeed;

	float distance = Utils::Distance(player->GetPosition(), position);

	direction = Utils::Normalize(player->GetPosition() - position);

	if (direction.x < 0)
	{
		SetFlipX(true);
	}
	else if (direction.x > 0)
	{
		SetFlipX(false);
	}
	
	body.setRotation(Utils::Angle(look));

	if (distance > 25.f)
	{
		position += direction * monsterMoveSpeed * dt;
		body.setPosition(position);
	}

	float magnitude = Utils::Magnitude(direction);

	if (magnitude > 0.0f)
	{
		status = StatusType::Move;

		animationSpeed = 2.0f;

		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		body.setPosition(position);
	}
	else
	{
		status = StatusType::Idle;

		animationSpeed = 1.0f;
	}
	BodyAnimation(1.0f, 0.2f, flowTime);
}

void Monster::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}