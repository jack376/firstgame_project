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

void BaseBullet::Init()
{
	SpriteGo::Init();
}

void BaseBullet::Release()
{
	SpriteGo::Release();
}

void BaseBullet::Reset()
{
    SpriteGo::Reset();

	bulletSpriteCenter.x = sprite.getTexture()->getSize().x / 2;
	bulletSpriteCenter.y = sprite.getTexture()->getSize().y / 2;
	sprite.setOrigin(bulletSpriteCenter);
    sprite.setRotation(0.0f);
    sprite.setPosition(0.0f, 0.0f);

	bulletRange = 500.0f;
}

void BaseBullet::Update(float dt)
{
    SpriteGo::Update(dt);

	bulletRange -= bulletSpeed * dt;

	if (bulletRange < 0.0f)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		pool->Return(this);
		return;
	}

	//std::cout << "TEST : " << bulletRange << std::endl;

	position += bulletDirection * bulletSpeed * dt;
	sprite.setPosition(position);

	bulletCollider.left = position.x - (sprite.getTexture()->getSize().x / 8);
	bulletCollider.top  = position.y - (sprite.getTexture()->getSize().y / 8);

	bulletColliderDraw.setPosition(bulletCollider.left, bulletCollider.top);
	bulletColliderDraw.setSize(sf::Vector2f(bulletCollider.width, bulletCollider.height));
	bulletColliderDraw.setFillColor(sf::Color::Transparent);

	if (Variables::toggleColliderDraw) // TEST CODE
	{
		bulletColliderDraw.setOutlineColor(sf::Color::Red);
		bulletColliderDraw.setOutlineThickness(2.0f);
	}

	SceneGame* sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
	if (sceneGame != nullptr)
	{
		Monster* nearMonster = sceneGame->GetNearMonsterSearch();
		if (nearMonster != nullptr)
		{
			if (bulletCollider.intersects(nearMonster->GetMonsterCollider()))
			{
				bulletLife -= dt;
				if (bulletLife <= 0.0f)
				{
					//std::cout << "TEST : " << "충돌 성공" << std::endl;
					//monster->OnHitBullet(bulletDamage);
					SetActive(false);
					SCENE_MGR.GetCurrentScene()->RemoveGo(this);
					pool->Return(this);
					bulletLife = 0.05f;
					//break;
				}
			}		
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