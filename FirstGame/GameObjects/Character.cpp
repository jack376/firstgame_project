#include "stdafx.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

Character::Character(const std::string& n = "")
	: GameObject(n)
{
	RESOURCE_MGR.LoadFromCSV("scripts/CharacterResourceList.csv", false);
}

void Character::Init()
{
}

void Character::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture("graphics/potato_body_default.png"));
	body.setOrigin(75.0f, 110.0f);
	body.setPosition(0, 0);

	legs.setTexture(*RESOURCE_MGR.GetTexture("graphics/potato_left_legs_default.png"));
	legs.setOrigin(75.0f, 110.0f);
	legs.setPosition(0, 0);

	SetFlipX(false);
}

void Character::Update(float dt)
{
	flowTime += dt * bodyAnimationSpeed;

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
		bodyAnimationSpeed = 3.0f;

		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		position += direction * moveSpeed * dt;
		
		body.setPosition(position);
		legs.setPosition(position);
		//SetPosition(position);
	}
	else // Idle
	{
		bodyAnimationSpeed = 1.0f;
	}

	BodyAnimation(1.0f, 0.2f, flowTime);
	//LegsAnimation();

	//std::cout << "TEST : " << fixDirectionX << std::endl;
}

void Character::Draw(sf::RenderWindow& window)
{
	window.draw(legs);
	window.draw(body);
}

bool Character::GetFlipX() const
{
	return filpX;
}

void Character::BodyAnimation(float defaultScale, float scaleRange, float flowTimeBySpeed)
{
	bodyAnimation.x = -sin(flowTimeBySpeed * 2.0f * _PI);
	float scaleX = defaultScale + ((bodyAnimation.x + 1.0f) / 2.0f) * scaleRange;

	scaleX = GetFlipX() ? -abs(scaleX) : abs(scaleX);

	bodyAnimation.y = sin(flowTimeBySpeed * 2.0f * _PI);
	float scaleY = defaultScale + ((bodyAnimation.y + 1.0f) / 2.0f) * scaleRange;

	body.setScale(scaleX, scaleY);
}

void Character::SetFlipX(bool filp)
{
	filpX = filp;

	sf::Vector2f scale = body.getScale();
	scale.x = !filpX ? abs(scale.x) : -abs(scale.x);
	body.setScale(scale);
}
