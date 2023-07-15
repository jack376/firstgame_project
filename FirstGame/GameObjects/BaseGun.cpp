#include "stdafx.h"
#include "BaseGun.h"
#include "ResourceMgr.h"
#include "Utils.h"
#include "Character.h"
#include "inputMgr.h"


BaseGun::BaseGun(Character* player, const std::string& textureId = "", const std::string& n = "")
    : SpriteGo(textureId, n), player(player)
{
    RESOURCE_MGR.LoadFromCSV("scripts/GunResourceList.csv", false);
}

BaseGun::~BaseGun()
{
}

void BaseGun::Init()
{
}

void BaseGun::Release()
{
}

void BaseGun::Reset()
{
    sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/gun.png"));
    sprite.setOrigin(-20.0f, 60.0f);
    sprite.setPosition(0, 0);
    
    SpriteGo::Reset();
}

void BaseGun::Update(float dt)
{
    flowTime += dt;
    position = player->GetPosition();

    if (player->GetFlipX())
    {
        sprite.setScale(-1.0f, 1.0f);
    }
    else if (!player->GetFlipX())
    {
        sprite.setScale(1.0f, 1.0f);
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
}

void BaseGun::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void BaseGun::FireRecoilAnimation(float amount, float flowTimeBySpeed)
{
    fireRecoilAnimation = sin((flowTimeBySpeed * amount) * 2.0f * M_PI);
    float fireRecoil = 1.0f + ((fireRecoilAnimation + 1.0f) * 10.0f);

    fireRecoil = player->GetFlipX() ? -abs(fireRecoil) : abs(fireRecoil);

    sprite.setPosition(position.x -= fireRecoil, position.y);

    //std::cout << "TEST : " << fireRecoil << std::endl;
}
