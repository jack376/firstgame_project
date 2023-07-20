#include "stdafx.h"
#include "BaseBullet.h"
#include "BaseGun.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "Character.h"
#include "inputMgr.h"
#include "Collision.h"
#include "Monster.h"
#include "SceneGame.h"
#include "SpriteEffect.h"

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

	bulletRange = 700.0f;
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
	
	SceneGame* sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	targetMonster = sceneGame->GetNearMonsterSearch();
	if (targetMonster != nullptr && bulletCollider.intersects(targetMonster->GetMonsterCollider()))
	{
		bulletLife -= dt;
		if (bulletLife < 0.0f)
		{
			targetMonster->SetBulletHitEffect(position);
			targetMonster->OnHitBullet(bulletDamage);
			SetActive(false);
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
			pool->Return(this);
			bulletLife = 0.02f;
		}
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