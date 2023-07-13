#include "stdafx.h"
#include "Button.h"
#include "InputMgr.h"
#include "SceneMgr.h"

Button::Button(const std::string& textureId, const std::string& n) : SpriteGo(textureId, n)
{
}

Button::~Button()
{
    Release();
}

void Button::Init()
{
    SpriteGo::Init();
}

void Button::Release()
{
    SpriteGo::Release();
}

void Button::Reset()
{
    SpriteGo::Reset();

    isHover = false;
}

void Button::Update(float dt)
{
    SpriteGo::Update(dt);

    sf::Vector2f mousePos   = INPUT_MGR.GetMousePos();
    sf::Vector2f uiMousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);

    bool prevHover = isHover;
    isHover = sprite.getGlobalBounds().contains(uiMousePos);

    if (!prevHover && isHover)
    {
        if (OnEnter != nullptr)
        {
            OnEnter();
        }
    }
    if (prevHover && !isHover)
    {
        if (OnExit != nullptr)
        {
            OnExit();
        }
    }
    if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
    {
        if (OnClick != nullptr)
        {
            OnClick();
        }
    }


}

void Button::Draw(sf::RenderWindow& window)
{
    SpriteGo::Draw(window);
}
