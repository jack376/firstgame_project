#include "stdafx.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

void Player::Init()
{
}

void Player::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_body_default.png"));
	body.setOrigin(body.getTexture()->getSize().x / 2, body.getTexture()->getSize().y / 2);
	body.setPosition(0.0f, 0.0f);

	legL.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_legs_default.png"));
	legL.setOrigin(legL.getTexture()->getSize().x / 2, legL.getTexture()->getSize().y / 2);
	legL.setPosition(0.0f, 0.0f);

	legR.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_legs_default.png"));
	legR.setOrigin(legR.getTexture()->getSize().x / 2, legR.getTexture()->getSize().y / 2);
	legR.setPosition(0.0f, 0.0f);

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

		body.setPosition(position);

		legR.setPosition(position.x + legsIdleWidth, position.y);
		legR.setScale(-1.0f, 1.0f);
		legR.setRotation(0.0f);

		legL.setPosition(position.x - legsIdleWidth, position.y);
		legL.setScale(1.0f, 1.0f);
		legL.setRotation(0.0f);
	}
	BodyAnimation(1.0f, 0.2f, flowTime);

	playerCollider.left = position.x - playerBodyCenter.x;
	playerCollider.top = position.y - playerBodyCenter.y;

	if (hitColorOverlayDuration > 0)
	{
		hitColorOverlayDuration -= dt;
		if (hitColorOverlayDuration <= 0)
		{
			body.setColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(legR);
	window.draw(legL);
	window.draw(body);
}

sf::FloatRect Player::GetPlayerCollider() const
{
	return playerCollider;
}

void Player::OnHitted(int damage)
{
	if (!isAlive)
	{
		return;
	}
	currentHp -= std::min(currentHp, damage);
	hitColorOverlayDuration = 0.05f;
	body.setColor(sf::Color(255, 0, 0, 255));

	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->SetHpUI((float)currentHp / maxHp);

	/*
	if (currentHp <= 30)
	{
		SpriteGo* hitFade = (SpriteGo*)FindGo("title_fade");
	}
	*/

	if (currentHp <= 0)
	{
		OnDie();
	}
}

void Player::OnDie()
{
	isAlive = false;
	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->OnDiePlayer();
}