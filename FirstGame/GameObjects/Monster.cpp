#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
#include "Character.h"
#include "InputMgr.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"
#include "MonsterTable.h"
#include "DataTableMgr.h"
#include <functional>

void Monster::Init()
{
}

void Monster::Reset()
{
	body.setTexture(*RESOURCE_MGR.GetTexture(textureId));

	monsterCollider.width  = body.getGlobalBounds().width * 0.75f;
	monsterCollider.height = body.getGlobalBounds().height * 0.75f;

	monsterBodyCenter.x    = body.getTexture()-> getSize().x * 0.5f;
	monsterBodyCenter.y    = body.getTexture()-> getSize().y * 0.5f;

	body.setOrigin(monsterBodyCenter);
	body.setPosition(0.0f, 0.0f);

	SetFlipX(false);

	currentHp = maxHp;
	attackTimer = attackRate;

	hitColorOverlayDuration = 0.1f;
	body.setColor(sf::Color(255, 255, 255, 255));
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

	if (distance > 50.f)
	{
		position += direction * moveSpeed * dt;
		body.setPosition(position);
	}

	monsterCollider.left = position.x - monsterCollider.width * 0.5f;
	monsterCollider.top = position.y - monsterCollider.height * 0.5f;

	attackTimer += dt;
	if (attackTimer > attackRate)
	{
		if (monsterCollider.intersects(player->GetPlayerCollider()))
		{
			attackTimer = 0.0f;
			player->OnHitted(damage);
		}
	}

	// Draw view ///////////////////////////////////////////////////////////////////////////////////
	/*
	monsterColliderDraw.setPosition(monsterCollider.left, monsterCollider.top);
	monsterColliderDraw.setSize(sf::Vector2f(monsterCollider.width, monsterCollider.height));
	monsterColliderDraw.setFillColor(sf::Color::Transparent);
	monsterColliderDraw.setOutlineColor(sf::Color::Red);
	monsterColliderDraw.setOutlineThickness(1.0f);
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////

	float magnitude = Utils::Magnitude(direction);
	if (magnitude > 0.0f)
	{
		state = StateType::Move;
		animationSpeed = 2.0f;
		if (magnitude > 1.f)
		{
			direction /= magnitude;
		}
		body.setPosition(position);
	}
	else
	{
		state = StateType::Idle;
		animationSpeed = 1.0f;
	}

	BodyAnimation(1.0f, 0.2f, flowTime);

	if (hitColorOverlayDuration > 0)
	{
		hitColorOverlayDuration -= dt;
		if (hitColorOverlayDuration <= 0)
		{
			body.setColor(sf::Color(255, 255, 255, 255));
		}
	}
}

void Monster::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	//window.draw(monsterColliderDraw);
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
	moveSpeed   = info.speed;
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
	SetDamageTextEffect(position, bulletDamage);

	hitColorOverlayDuration = 0.1f;
	body.setColor(sf::Color(192, 0, 64, 255));

	if (currentHp <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrentScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);

		float scale = body.getTexture()->getSize().x / static_cast<float>(100);
		SetDieEffect(position, scale);

		if (sceneGame != nullptr)
		{
			sceneGame->OnDieMonster(this);
		}
	}
}

void Monster::SetEntityEffectPool(ObjectPool<EntityEffect>* pool)
{
	entityEffectPool = pool;
}

void Monster::SetEntityEffect(sf::Vector2f position, std::function<void()> onEntityEffectComplete)
{
	float randomValue = Utils::RandomRange(1, 16);

	EntityEffect* entityEffect = entityEffectPool->Get();
	entityEffect->SetActive(true);
	entityEffect->SetOrigin(Origins::MC);
	entityEffect->SetPosition(position);
	entityEffect->sprite.setRotation(randomValue * 22.5f);
	entityEffect->SetOnEntityEffectComplete(onEntityEffectComplete);

	SCENE_MGR.GetCurrentScene()->AddGo(entityEffect);
}

void Monster::SetBulletHitEffectPool(ObjectPool<BulletHitEffect>* pool)
{
	effectPool = pool;
}

void Monster::SetBulletHitEffect(sf::Vector2f position)
{
	BulletHitEffect* bulletHitEffect = effectPool->Get();
	bulletHitEffect->SetActive(true);
	bulletHitEffect->SetOrigin(Origins::MC);
	bulletHitEffect->SetPosition(position);

	SCENE_MGR.GetCurrentScene()->AddGo(bulletHitEffect);
}

void Monster::SetDieEffectPool(ObjectPool<DieEffect>* pool)
{
	dieEffectPool = pool;
}

void Monster::SetDieEffect(sf::Vector2f position, float scale)
{
	DieEffect* dieEffect = dieEffectPool->Get();
	float randomValue = Utils::RandomRange(-6, 7);
	float positionFactor = 25.0f * scale;

	dieEffect->SetActive(true);
	dieEffect->SetOrigin(Origins::MC);
	dieEffect->SetPosition(position.x, position.y + positionFactor);
	dieEffect->SetAnimationScale(scale);
	dieEffect->sprite.setRotation(randomValue * 10);

	SCENE_MGR.GetCurrentScene()->AddGo(dieEffect);
}

void Monster::SetDamageTextEffectPool(ObjectPool<DamageTextEffect>* pool)
{
	damageTextEffectPool = pool;
}

void Monster::SetDamageTextEffect(sf::Vector2f position, int damage)
{
	DamageTextEffect* damageTextEffect = damageTextEffectPool->Get();
	std::string damageString = std::to_string(damage);
	damageTextEffect->SetActive(true);
	damageTextEffect->SetOrigin(Origins::BC);
	damageTextEffect->SetPosition(position.x, position.y);
	damageTextEffect->SetString(damageString);
	damageTextEffect->SetFillColor(sf::Color::White);
	damageTextEffect->SetOutlineColor(sf::Color::Black);
	damageTextEffect->SetOutlineThickness(5.0f);
	damageTextEffect->SetCharacterSize(48);

	SCENE_MGR.GetCurrentScene()->AddGo(damageTextEffect);
}