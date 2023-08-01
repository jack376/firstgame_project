#include "stdafx.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "DataTableMgr.h"
#include "UpgradeTable.h"

void Player::Init()
{
}

void Player::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_body_default.png"));

	legL.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_legs_default.png"));
	legL.setOrigin(legL.getTexture()->getSize().x / 2, legL.getTexture()->getSize().y / 2);
	legL.setPosition(0.0f, 0.0f);

	legR.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/potato_legs_default.png"));
	legR.setOrigin(legR.getTexture()->getSize().x / 2, legR.getTexture()->getSize().y / 2);
	legR.setPosition(0.0f, 0.0f);

	SetFlipX(false);

	playerCollider.width  = body.getGlobalBounds().width * 0.25f;
	playerCollider.height = body.getGlobalBounds().height * 0.25f;

	playerBodyCenter.x = body.getTexture()->getSize().x * 0.5f;
	playerBodyCenter.y = body.getTexture()->getSize().y * 0.5f;

	body.setOrigin(playerBodyCenter);
	body.setPosition(0.0f, 0.0f);

	level = 1;

	currentExp  = 0;
	maxExp      = 16;

	currentHp   = 20;
	maxHp       = 20;

	hpRegen     = 0.0f;
	damage      = 0.0f;
	attackSpeed = 1.0f;
	critical    = 0.03f;
	armor       = 0.0f;
	dodge       = 0.0f;
	moveSpeed   = 500.0f;

	isAlive = true;

	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->SetExpUI((float)currentExp / maxExp);
	sceneGame->SetHpUI(currentHp, maxHp);
	sceneGame->SetLevelUpUI(level);
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
		state = StateType::Move;
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
		state = StateType::Idle;
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

	playerCollider.left = position.x - playerCollider.width * 0.5f;
	playerCollider.top = position.y - playerCollider.height * 0.5f;

	// Test
	/*
	playerColliderDraw.setPosition(playerCollider.left, playerCollider.top);
	playerColliderDraw.setSize(sf::Vector2f(playerCollider.width, playerCollider.height));
	playerColliderDraw.setFillColor(sf::Color::Transparent);
	playerColliderDraw.setOutlineColor(sf::Color::Red);
	playerColliderDraw.setOutlineThickness(1.0f);
	*/
	//

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
	//window.draw(playerColliderDraw);
}

sf::FloatRect Player::GetPlayerCollider() const
{
	return playerCollider;
}

void Player::OnHitted(float damage)
{
	if (!isAlive)
	{
		return;
	}
	currentHp -= std::min(currentHp, damage);
	hitColorOverlayDuration = 0.1f;
	body.setColor(sf::Color(255, 0, 0, 255));

	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->SetHpUI(currentHp, maxHp);

	if (currentHp <= 0)
	{
		OnDie();
	}
}

void Player::OnKilled()
{
	if (!isAlive)
	{
		return;
	}

	currentExp++;

	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->SetExpUI((float)currentExp / maxExp);

	if (currentExp >= maxExp)
	{
		level++;
		sceneGame->SetLevelUpUI(level);
		
		currentHp = maxHp;
		sceneGame->SetHpUI(currentHp, maxHp);

		currentExp = 0;
		maxExp = maxExp + maxExp / 8;
	}
}

void Player::OnDie()
{
	isAlive = false;
	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
	sceneGame->OnDiePlayer();
}

void Player::UpgradeStat(const std::string& upgradeName, const std::string& statName, float increaseAmount)
{
	Scene* scene = SCENE_MGR.GetCurrentScene();
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);

	if (stats.find(statName) != stats.end())
	{
		*stats[statName] += increaseAmount;
		sceneGame->SetHpUI(currentHp, maxHp);
	}
	else
	{
		std::cerr << "Unknown stat: " << statName << std::endl;
	}
}

float Player::GetDamage() const
{
	return damage;
}
