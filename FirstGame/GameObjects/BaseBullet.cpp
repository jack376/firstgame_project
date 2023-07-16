#include "stdafx.h"
#include "BaseBullet.h"
#include "BaseGun.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "Utils.h"
#include "Character.h"
#include "inputMgr.h"

void BaseBullet::Fire(const sf::Vector2f& pos, const sf::Vector2f& dir, float speed)
{
    sprite.setRotation(Utils::Angle(dir));
    SetPosition(pos);

    direction = dir;
    bulletSpeed = speed;
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

	sortLayer = 2;

	sf::Vector2f spriteCenter = sf::Vector2f(sprite.getTexture()->getSize().x / 2 - 50.0f, sprite.getTexture()->getSize().y / 2);
	sprite.setOrigin(spriteCenter);

    sprite.setRotation(0.0f);
    SetPosition(0.0f, 0.0f);
    direction = { 0.0f, 0.0f };
    bulletSpeed = 500.0f;
    bulletRange = 1000.0f;
    bulletDamage = 25;
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

	position += direction * bulletSpeed * dt;
	sprite.setPosition(position);

	/*
	if (zombies != nullptr)
	{
		for (Zombie* zombie : *zombies)
		{
			if (sprite.getGlobalBounds().intersects(zombie->sprite.getGlobalBounds()))
			{
				zombie->OnHitBullet(bulletDamage);
				SetActive(false);
				SCENE_MGR.GetCurrentScene()->RemoveGo(this);
				baseBulletPool->Return(this);
				break;
			}
		}
	}
	*/
}

void BaseBullet::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
    //SpriteGo::Draw(window);
}
