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
        bullet->textureId = "graphics/bullet.png";
        bullet->pool = &poolBaseBullets;
    };
    poolBaseBullets.Init(1000);
}

void BaseGun::Release()
{
    SpriteGo::Release();
    poolBaseBullets.Release();
}

void BaseGun::Reset()
{
    SpriteGo::Reset();

    fireRecoilEffect.setTexture(*RESOURCE_MGR.GetTexture("graphics/fire.png"));


    SetOrigin(sprite.getTexture()->getSize().x / 2 + gunOrigin, sprite.getTexture()->getSize().y / 2);
    fireRecoilEffect.setOrigin(sprite.getTexture()->getSize().x / 2, sprite.getTexture()->getSize().y / 2);

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
    gunMuzzle = position + look * gunLength;

    SceneGame* sceneGame = (SceneGame*)SCENE_MGR.GetCurrentScene();
    if (sceneGame != nullptr)
    {
        Monster* nearMonster = sceneGame->GetNearMonsterSearch();

        if (nearMonster != nullptr)
        {
            sf::Vector2f monsterPosition = nearMonster->GetPosition();

            look = Utils::Normalize(monsterPosition - position);
            float angle = Utils::Angle(look);
            UpdateFlipAndRotation(player->GetFlipX(), angle);
        }
        else
        {
            sf::Vector2f mousePos        = INPUT_MGR.GetMousePos();
            sf::Vector2f mouseWorldPos   = SCENE_MGR.GetCurrentScene()-> ScreenToWorldPos(mousePos);
            sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrentScene()-> WorldPosToScreen(position);

            look = Utils::Normalize(mousePos - playerScreenPos);
            float angle = Utils::Angle(look);
            UpdateFlipAndRotation(player->GetFlipX(), angle);
        }
    }

    if (player->GetStatus() == Character::StatusType::Idle)
    {
        sprite.setPosition(position);
        isFireRecoilEffect = false;
        isFire = false;
        fireRecoilEffect.setColor(sf::Color::Transparent);
    }
    else if (player->GetStatus() == Character::StatusType::Move)
    {
        FireRecoilAnimation(look, fireRecoilAnimationSpeed, flowTime);
        isFireRecoilEffect = true;
        isFire = true;
        fireRecoilEffect.setColor(sf::Color(255, 255, 255, 255));
    }

    //if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))

    if (isFire) // 수정 예정 800 범위 안에 들어올 시
    {
        bulletTotalCooldown -= dt;
        if (bulletTotalCooldown <= 0.0f)
        {
            bullet = poolBaseBullets.Get();
            bullet->Init();
            bullet->Reset();
            bullet->Fire(gunMuzzle, look, 1500.f);
            bullet->sortLayer = 3;

            Scene* scene = SCENE_MGR.GetCurrentScene();
            SceneGame* inGame = dynamic_cast<SceneGame*>(scene);
            if (inGame != nullptr)
            {
                //bullet->SetZombieList(SceneGame->GetZombieList());
                inGame->AddGo(bullet);
            }
            bulletTotalCooldown = bulletCurrentCooldown;
        }
    }
}

void BaseGun::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(fireRecoilEffect, sf::BlendAdd);
}

void BaseGun::FireRecoilAnimation(const sf::Vector2f direction, float playSpeed, float flowTime)
{
    float fireRecoilAnimation = sin((flowTime * playSpeed) * 2.0f * M_PI);
    float fireRecoil = 1.0f + ((fireRecoilAnimation + 1.0f) * 5.0f);

    sf::Vector2f dir = Utils::Normalize(direction) * fireRecoil;

    sprite.setPosition(position.x -= dir.x, position.y -= dir.y);
    fireRecoilEffect.setPosition(gunMuzzle.x -= dir.x, gunMuzzle.y -= dir.y);
}

void BaseGun::UpdateFlipAndRotation(bool flip, float angle)
{
    float scaleValue = (angle > 90.0f || angle < -90.0f) ? -1.0f : 1.0f;

    sprite.setScale(1.0f, scaleValue);
    sprite.setRotation(angle);
    
    if (isFireRecoilEffect) // 추후에 사용할 bool 변수
    {
        fireRecoilEffect.setPosition(position);
        fireRecoilEffect.setRotation(angle);
    }
}

void BaseGun::SetFire(bool test)
{
    isFire = test;
}
