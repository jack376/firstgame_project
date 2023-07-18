#include "stdafx.h"
#include "BaseGun.h"
#include "BaseBullet.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include "Player.h"
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
        bullet->bulletPool = &poolBaseBullets;
    };
    poolBaseBullets.Init();
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

    sprite.setOrigin(-20.0f, 40.0f);
    fireRecoilEffect.setOrigin(-45.0f, 40.0f);

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
    
    sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
    sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorldPos(mousePos);
    sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrentScene()->WorldPosToScreen(position);

    look = Utils::Normalize(mousePos - playerScreenPos);
    float angle = Utils::Angle(look);

    /*
    if (player->GetFlipX())
    {
        if (angle > 90.0f) 
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > 0.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > -90.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > -180.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
    }
    else if (!player->GetFlipX())
    {
        if (angle > 90.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > 0.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > -90.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
        else if (angle > -180.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);

            if (isFireRecoilEffect)
            {
                fireRecoilEffect.setPosition(position);
                fireRecoilEffect.setRotation(angle);
            }
        }
    }
    */

    if (player->GetFlipX())
    {
        if (angle > 90.0f || angle > 0.0f || angle > -90.0f || angle > -180.0f)
        {
            UpdateFlipAndRotation(true, angle);
        }
    }
    else if (!player->GetFlipX())
    {
        if (angle > 90.0f || angle > 0.0f || angle > -90.0f || angle > -180.0f)
        {
            UpdateFlipAndRotation(false, angle);
        }
    }

    if (player->GetStatus() == Character::StatusType::Idle)
    {
        sprite.setPosition(position);
        isFireRecoilEffect = false;
        fireRecoilEffect.setColor(sf::Color::Transparent);
    }
    else if (player->GetStatus() == Character::StatusType::Move)
    {
        FireRecoilAnimation(fireRecoilAnimationSpeed, flowTime);
        isFireRecoilEffect = true;
        fireRecoilEffect.setColor(sf::Color(255, 255, 255, 200));
    }

    //if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left)) // 이거 수정 800 범위 안에 들어올 시
    if (isFire)
    {
        BaseBullet* bullet = poolBaseBullets.Get();
        bullet->Fire(GetPosition(), look, 2000.f);
        bullet->sortLayer = 3;

        Scene* scene = SCENE_MGR.GetCurrentScene();
        SceneGame* inGame = dynamic_cast<SceneGame*>(scene);
        if (inGame != nullptr)
        {
            //bullet->SetZombieList(SceneGame->GetZombieList());
            inGame->AddGo(bullet);
        }
    }
}

void BaseGun::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(fireRecoilEffect, sf::BlendAdd);
}

void BaseGun::FireRecoilAnimation(float amount, float flowTimeBySpeed)
{
    float fireRecoilAnimation = sin((flowTimeBySpeed * amount) * 2.0f * M_PI);
    float fireRecoil = 1.0f + ((fireRecoilAnimation + 1.0f) * 5.0f);

    fireRecoil = player->GetFlipX() ? -abs(fireRecoil) : abs(fireRecoil);

    sprite.setPosition(position.x -= fireRecoil, position.y);
    fireRecoilEffect.setPosition(position.x -= fireRecoil, position.y);
}

void BaseGun::UpdateFlipAndRotation(bool flip, float angle)
{
    float scaleValue = (angle > 90.0f || angle < -90.0f) ? -1.0f : 1.0f;

    sprite.setScale(1.0f, scaleValue);
    sprite.setRotation(angle);
    
    if (isFireRecoilEffect) // 아직 없어도 되지만 추후에 사용할 bool 변수
    {
        fireRecoilEffect.setPosition(position);
        fireRecoilEffect.setRotation(angle);
    }
}

void BaseGun::SetFire(bool test)
{
    isFire = test;
}
