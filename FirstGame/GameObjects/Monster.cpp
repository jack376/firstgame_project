#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "Collision.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include <functional>
#include "MonsterTable.h"
#include "DataTableMgr.h"

/*
const std::string Monster::textureIds[3] = 
{
	"graphics/monster1.png",
	"graphics/monster2.png",
	"graphics/monster3.png",
};
const float Monster::speedStats[3]      = { 40.f, 70.f, 20.f };
const int   Monster::hpStats[3]         = { 100, 75, 50 };
const int   Monster::damageStats[3]     = { 10, 5, 7 };
const float Monster::attackRateStats[3] = { 2.f, 0.5, 1.f };
*/

void Monster::Init()
{
}

void Monster::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture(textureId));

	sf::Vector2f spriteCenter = sf::Vector2f(body.getTexture()->getSize().x / 2, body.getTexture()->getSize().y / 2);
	body.setOrigin(spriteCenter);
	body.setPosition(0, 0);

	SetFlipX(false);

	hp = maxHp;
	attackTimer = attackRate;
}

void Monster::Update(float dt)
{
	flowTime += dt * animationSpeed;

	if (player == nullptr)
	{
		return;
	}

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
		position += direction * speed * dt;
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

sf::FloatRect Monster::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Monster::SetType(Types t)
{
	monsterType = t;
	const MonsterInfo& info = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get(t);
	
	int index = (int)monsterType;

	monsterType = (Monster::Types)info.monsterType;
	textureId   = info.textureId;
	speed       = info.speed;
	maxHp       = info.maxHp;
	damage      = info.damage;
	attackRate  = info.attackRate;

	/*
	textureId  = textureIds[index];
	speed      = speedStats[index];
	maxHp      = hpStats[index];
	damage     = damageStats[index];
	attackRate = attackRateStats[index];
	*/
}

Monster::Types Monster::GetType() const
{
	return monsterType;
}

void Monster::SetPlayer(Player* player)
{
	this->player = player;
}
