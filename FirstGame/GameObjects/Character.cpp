#include "stdafx.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"

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

void Character::SetWallBounds(const sf::FloatRect& bounds)
{
	wallBounds = bounds;
	wallBoundsLT = { bounds.left, bounds.top };
	wallBoundsRB = { bounds.left + bounds.width, bounds.top + bounds.height };
}

sf::FloatRect Character::GetWallBounds()
{
	return wallBounds;
}
