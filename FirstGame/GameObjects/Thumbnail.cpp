#include "stdafx.h"
#include "Thumbnail.h"

Thumbnail::Thumbnail(const std::string& n = "") : BaseUI(n)
{
}

Thumbnail::~Thumbnail()
{
    Release();
}

void Thumbnail::Init()
{
    BaseUI::Init();
}

void Thumbnail::Reset()
{
    BaseUI::Reset();



}

void Thumbnail::Update(float dt)
{
    BaseUI::Update(dt);
}

void Thumbnail::Draw(sf::RenderWindow& window)
{
    BaseUI::Draw(window);
}
