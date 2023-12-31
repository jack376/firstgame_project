#include "stdafx.h"
#include "BaseBullet.h"
#include "BaseGun.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "Character.h"
#include "inputMgr.h"
#include "Monster.h"
#include "SceneGame.h"
#include "SpriteEffect.h"
#include "Player.h"

void BaseBullet::Init()
{
	SpriteGo::Init();
}

void BaseBullet::Reset()
{
    SpriteGo::Reset();

	sprite.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);
    sprite.setRotation(0.0f);
    sprite.setPosition(0.0f, 0.0f);

	bulletRange  = 700.0f;
	bulletSpeed  = 2500.0f;
	bulletDamage = 50.0f;
}

void BaseBullet::Update(float dt)
{
    SpriteGo::Update(dt);

	flowTime += dt;
	bulletRange -= bulletSpeed * dt;

	if (bulletRange < 0.0f)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		pool->Return(this);

		return;
	}

	position += bulletDirection * bulletSpeed * dt;
	sprite.setPosition(position);

	bulletCollider.left = position.x - (sprite.getTexture()->getSize().x / 8);
	bulletCollider.top  = position.y - (sprite.getTexture()->getSize().y / 8);

	bulletColliderDraw.setPosition(bulletCollider.left, bulletCollider.top);
	bulletColliderDraw.setSize(sf::Vector2f(bulletCollider.width, bulletCollider.height));
	bulletColliderDraw.setFillColor(sf::Color::Transparent);

	if (isBulletColliderDrawView) // TEST CODE
	{
		bulletColliderDraw.setOutlineColor(sf::Color::Red);
		bulletColliderDraw.setOutlineThickness(1.0f);
	}
	
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	Monster* targetMonster = sceneGame->GetNearMonsterSearch();

	if (targetMonster != nullptr && monsters != nullptr)
	{
		for (auto monster : *monsters)
		{
			if (!monster->GetActive())
			{
				continue;
			}
			if (bulletCollider.intersects(monster->GetMonsterCollider()))
			{
				monster->SetBulletHitEffect(position);
				monster->OnHitBullet(bulletDamage);
				SetActive(false);
				SCENE_MGR.GetCurrentScene()->RemoveGo(this);
				pool->Return(this);
				break;
			}
		}
	}

	sf::FloatRect bulletReturnArea(-960.0f, -960.0f, 1920.0f, 1920.0f);
	if (!bulletReturnArea.contains(position.x, position.y))
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		pool->Return(this);
	}
}

void BaseBullet::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
	window.draw(bulletColliderDraw);
}

void BaseBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed)
{
	sprite.setRotation(Utils::Angle(dir));
	SetPosition(pos);
	bulletDirection = dir;
	bulletSpeed = speed;
}

void BaseBullet::SetMonsterList(const std::list<Monster*>* list)
{
	monsters = list;
}

void BaseBullet::SetBulletSpec(float damage, float criticalDamage, float range)
{
	bulletDamage   = damage;
	bulletCritical = criticalDamage;
	bulletRange    = range;
}

void BaseBullet::IncreaseBulletDamage(float increaseDamage)
{
	bulletDamage = bulletDamage * (increaseDamage + 1.0f);
	//std::cout << "increaseDamage : " << increaseDamage << std::endl;
}
