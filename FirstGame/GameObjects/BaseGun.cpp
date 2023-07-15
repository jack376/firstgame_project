#include "stdafx.h"
#include "BaseGun.h"
#include "BaseBullet.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include "Character.h"
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

    sprite.setOrigin(-20.0f, 40.0f);

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


    if (player->GetFlipX())
    {
        if (angle > 90.0f) 
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > 0.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > -90.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > -180.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
        }
    }
    else if (!player->GetFlipX())
    {
        if (angle > 90.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > 0.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > -90.0f)
        {
            sprite.setScale(1.0f, 1.0f);
            sprite.setRotation(angle);
        }
        else if (angle > -180.0f)
        {
            sprite.setScale(1.0f, -1.0f);
            sprite.setRotation(angle);
        }
    }
    
    if (player->GetStatus() == Character::StatusType::Idle)
    {
        sprite.setPosition(position);
    }
    else if (player->GetStatus() == Character::StatusType::Move)
    {
        FireRecoilAnimation(fireRecoilAnimationSpeed, flowTime);
    }

    //std::cout << "TEST : " << position.x << std::endl;


    if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
    {
        BaseBullet* bullet = poolBaseBullets.Get();
        bullet->sortLayer = 2;
        bullet->Fire(GetPosition(), look, 2000.f);

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
    SpriteGo::Draw(window);
}

void BaseGun::FireRecoilAnimation(float amount, float flowTimeBySpeed)
{
    float fireRecoilAnimation = sin((flowTimeBySpeed * amount) * 2.0f * M_PI);
    float fireRecoil = 1.0f + ((fireRecoilAnimation + 1.0f) * 5.0f);

    fireRecoil = player->GetFlipX() ? -abs(fireRecoil) : abs(fireRecoil);

    sprite.setPosition(position.x -= fireRecoil, position.y);

    //std::cout << "TEST : " << fireRecoil << std::endl;
}
