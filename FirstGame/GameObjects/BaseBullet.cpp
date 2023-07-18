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

void BaseBullet::Fire(const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
{
    sprite.setRotation(Utils::Angle(direction));
    sprite.setPosition(position);

	bulletDirection = direction;
    bulletSpeed = speed;
}

void BaseBullet::SetTargetMonster(Monster* monster)
{
	this->targetMonster = monster;
}

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

	sf::Vector2f spriteCenter = sf::Vector2f(sprite.getTexture()->getSize().x / 2 - 50.0f, sprite.getTexture()->getSize().y / 2);
	sprite.setOrigin(spriteCenter);
    sprite.setRotation(0.0f);
    sprite.setPosition(0.0f, 0.0f);

	sortLayer = 2;

	bulletDirection = { 0.0f, 0.0f };
}

void BaseBullet::Update(float dt)
{
    SpriteGo::Update(dt);

	bulletRange -= bulletSpeed * dt;

	if (bulletPool != nullptr && bulletRange < 0.0f)
	{
		SetActive(false);
		SCENE_MGR.GetCurrentScene()->RemoveGo(this);
		bulletPool->Return(this);
		//std::cout << "TEST : " << bulletPool->GetUseList().size() << std::endl;
		//std::cout << "TEST : " << bulletRange << std::endl;
		return;
	}

	position += bulletDirection * bulletSpeed * dt;
	sprite.setPosition(position);

	/*
	if (targetMonster != nullptr)
	{
		if (sprite.getGlobalBounds().intersects(targetMonster->GetGlobalBounds()))
		{
			std::cout << "TEST : " << "충돌 성공" << std::endl;

			//monster->OnHitBullet(bulletDamage);
			//SetActive(false);
			SCENE_MGR.GetCurrentScene()->RemoveGo(this);
			bulletPool->Return(this);
			//break;
		}	
	}
	*/
}

void BaseBullet::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
    //SpriteGo::Draw(window);
}
