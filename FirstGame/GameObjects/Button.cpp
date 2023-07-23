#include "stdafx.h"
#include "Button.h"

Button::Button(const std::string& n = "") : BaseUI(n)
{
}

Button::~Button()
{
    Release();
}

void Button::Init()
{
    BaseUI::Init();
}

void Button::Reset()
{
    BaseUI::Reset();
}

void Button::Update(float dt)
{
    BaseUI::Update(dt);
}

void Button::Draw(sf::RenderWindow& window)
{
    BaseUI::Draw(window);
}
