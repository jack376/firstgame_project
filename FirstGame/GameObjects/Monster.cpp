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

void Monster::Init()
{
}

void Monster::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture(textureId));

	monsterSpriteCenter.x = body.getTexture()->getSize().x / 2;
	monsterSpriteCenter.y = body.getTexture()->getSize().y / 2;
	body.setOrigin(monsterSpriteCenter);
	body.setPosition(0, 0);

	SetFlipX(false);

	currentHp = maxHp;
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

	monsterCollider.left = position.x - (body.getTexture()->getSize().x / 2);
	monsterCollider.top  = position.y - (body.getTexture()->getSize().y / 2);

	monsterColliderDraw.setPosition(monsterCollider.left, monsterCollider.top);
	monsterColliderDraw.setSize(sf::Vector2f(monsterCollider.width, monsterCollider.height));
	monsterColliderDraw.setFillColor(sf::Color::Transparent);

	if (Variables::toggleColliderDraw)  // TEST CODE
	{
		monsterColliderDraw.setOutlineColor(sf::Color::Red);
		monsterColliderDraw.setOutlineThickness(2.0f);
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
	window.draw(monsterColliderDraw);
}

sf::FloatRect Monster::GetMonsterCollider() const
{
	return monsterCollider;
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
}

Monster::Types Monster::GetType() const
{
	return monsterType;
}

void Monster::SetPlayer(Player* player)
{
	this->player = player;
}

void Monster::OnHitBullet(int bulletDamage)
{
	currentHp -= bulletDamage;
	if (currentHp <= 0)
	{
		//SetBulletHitEffect(position);
		Scene* scene = SCENE_MGR.GetCurrentScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
		if (sceneGame != nullptr)
		{
			sceneGame->OnDieMonster(this);
		}
	}
}

void Monster::SetBulletHitEffectPool(ObjectPool<SpriteEffect>* pool)
{
	effectPool = pool;
}

void Monster::SetBulletHitEffect(sf::Vector2f position)
{
	SpriteEffect* bulletHitEffect = effectPool->Get();
	bulletHitEffect->SetActive(true);
	bulletHitEffect->SetOrigin(Origins::MC);
	bulletHitEffect->SetPosition(position);

	SCENE_MGR.GetCurrentScene()->AddGo(bulletHitEffect);
}