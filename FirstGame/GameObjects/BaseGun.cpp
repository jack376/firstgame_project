#include "stdafx.h"
#include "BaseGun.h"
#include "BaseBullet.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include "Player.h"
#include "Monster.h"
#include "inputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "SceneGame.h"

void BaseGun::Init()
{
    SpriteGo::Init();

    poolBaseBullets.OnCreate = [this](BaseBullet* bullet) 
    {
        bullet->textureId = "graphics/game/bullet.png";
        bullet->pool = &poolBaseBullets;
    };

    poolBaseBullets.Init(256);
}

void BaseGun::Release()
{
    SpriteGo::Release();
    poolBaseBullets.Release();
}

void BaseGun::Reset()
{
    SpriteGo::Reset();

    fireRecoilEffect.setTexture(*RESOURCE_MGR.GetTexture("graphics/game/bullet_muzzle_fire.png"));
    SetOrigin(sprite.getTexture()->getSize().x / 2 + gunOrigin, sprite.getTexture()->getSize().y / 2);
    fireRecoilEffect.setOrigin(0.0f, sprite.getTexture()->getSize().y / 2);

    for (auto bullet : poolBaseBullets.GetUseList())
    {
        SCENE_MGR.GetCurrentScene()->RemoveGo(bullet);
    }
    poolBaseBullets.Clear();
}

void BaseGun::Update(float dt)
{
    SpriteGo::Update(dt);

    flowTime += dt;
    position = player->GetPosition();
    gunMuzzlePosition = position + look * gunLength;

    auto sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
    if (sceneGame != nullptr)
    {
        Monster* nearMonster = sceneGame->GetNearMonsterSearch();
        if (nearMonster != nullptr && nearMonster->GetActive())
        {
            sf::Vector2f monsterPosition = nearMonster->GetPosition();

            look = Utils::Normalize(monsterPosition - position);
            float angle = Utils::Angle(look);
            UpdateFlipAndRotation(player->GetFlipX(), angle);
        }
        else
        {
            sf::Vector2f mousePosition   = INPUT_MGR.GetMousePos();
            sf::Vector2f mouseWorldPos   = SCENE_MGR.GetCurrentScene()-> ScreenToWorldPos(mousePosition);
            sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrentScene()-> WorldPosToScreen(position);

            look = Utils::Normalize(mousePosition - playerScreenPos);
            float angle = Utils::Angle(look);
            UpdateFlipAndRotation(player->GetFlipX(), angle);
        }
    }

    Monster* nearMonster = sceneGame->GetNearMonsterSearch();
    if (nearMonster != nullptr)
    {
        sf::Vector2f monsterPosition = nearMonster->GetPosition();
        float distance = std::sqrt(std::pow(monsterPosition.x - position.x, 2) + std::pow(monsterPosition.y - position.y, 2));

        if (nearMonster->GetActive() && distance <= 700.0f)
        {
            FireRecoilAnimation(look, fireRecoilAnimationSpeed, flowTime);

            isFireRecoilEffect = true;

            fireRecoilEffect.setColor(sf::Color(255, 255, 255, 255));

            bulletTotalCooldown -= dt;
            if (bulletTotalCooldown <= 0.0f)
            {
                bullet = poolBaseBullets.Get();
                bullet->Init();
                bullet->Reset();
                bullet->Fire(gunMuzzlePosition, look, bulletSpeed);
                bullet->IncreaseBulletDamage(player->GetDamage());
                bullet->sortLayer = 3;

                Scene* scene = SCENE_MGR.GetCurrentScene();
                SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
                if (sceneGame != nullptr)
                {
                    bullet->SetMonsterList(sceneGame->GetMonsterList());
                }

                sceneGame->AddGo(bullet);
                bulletTotalCooldown = bulletCurrentCooldown;
            }
        }
        else
        {
            sprite.setPosition(position);
            isFireRecoilEffect = false;

            fireRecoilEffect.setColor(sf::Color::Transparent);
        }
    }
    else
    {
        sprite.setPosition(position);
        isFireRecoilEffect = false;

        fireRecoilEffect.setColor(sf::Color::Transparent);
    }
}

void BaseGun::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(fireRecoilEffect, sf::BlendAdd);
}

void BaseGun::FireRecoilAnimation(const sf::Vector2f direction, float playSpeed, float flowTime)
{
    float fireRecoilAnimation = sin((flowTime * playSpeed) * 2.0f * _PI);
    float fireRecoil = 1.0f + ((fireRecoilAnimation + 1.0f) * 5.0f);

    sf::Vector2f dir = Utils::Normalize(direction) * fireRecoil;

    sprite.setPosition(position.x -= dir.x, position.y -= dir.y);

    fireRecoilEffect.setPosition(gunMuzzlePosition.x -= dir.x, gunMuzzlePosition.y -= dir.y);
    fireRecoilEffect.setScale(fireRecoil * 0.1f, fireRecoil * 0.1f);
}

void BaseGun::UpdateFlipAndRotation(bool flip, float angle)
{
    float scaleValue = (angle > 90.0f || angle < -90.0f) ? -1.0f : 1.0f;

    sprite.setScale(1.0f, scaleValue);
    sprite.setRotation(angle);
    
    if (isFireRecoilEffect)
    {
        fireRecoilEffect.setPosition(position);
        fireRecoilEffect.setRotation(angle);
    }
}

void BaseGun::SetGunCooldown(float cooldown)
{
    bulletCurrentCooldown = cooldown;
}

sf::Vector2f BaseGun::GetMuzzlePosition()
{
    return gunMuzzlePosition;
}
