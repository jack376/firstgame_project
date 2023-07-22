#include "stdafx.h"
#include "TextGo.h"
#include "Button.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

Button::Button(const std::string& textureId, const std::string& n) 
    : SpriteGo(textureId, n)
{
}

Button::~Button()
{
    Release();
}

void Button::Init()
{
    SpriteGo::Init();
    text.setFont(*RESOURCE_MGR.GetFont("fonts/NanumGothic.ttf"));
    text.setCharacterSize(18);
    //text.setOrigin(Origins::ML);
    text.setPosition(body.getPosition());
    text.setFillColor(sf::Color::White);
}

void Button::Release()
{
    SpriteGo::Release();
}

void Button::Reset()
{
    SpriteGo::Reset();
    isHover = false;

    corner.setTexture(*RESOURCE_MGR.GetTexture("graphics/corner.png"));
    corner.setOrigin(0.0f, 0.0f);
    corner.setColor(bodyColor);

    sf::Vector2f cornerSize(corner.getTexture()->getSize().x, corner.getTexture()->getSize().y);
    sf::Vector2f spriteTextureSize(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
    sf::Vector2f spritePosition(sprite.getPosition().x, sprite.getPosition().y);
    sf::Vector2f bodyRectSize(sprite.getTexture()->getSize().x - cornerSize.x * 2, sprite.getTexture()->getSize().y - cornerSize.y * 2);
    sf::Vector2f sideSizeWidth(sprite.getTexture()->getSize().x - cornerSize.x * 2, corner.getTexture()->getSize().y);
    sf::Vector2f sideSizeHeight(corner.getTexture()->getSize().x, sprite.getTexture()->getSize().y - cornerSize.y * 2);

    corners.resize(4, corner);

    corners[0].setPosition(spritePosition); // Top-Left
    corners[0].rotate(0);

    corners[1].setPosition(spritePosition.x + spriteTextureSize.x, spritePosition.y); // Top-Right
    corners[1].rotate(90);

    corners[2].setPosition(spritePosition.x, spritePosition.y + spriteTextureSize.y); // Bottom-Left
    corners[2].rotate(270);

    corners[3].setPosition(spritePosition + spriteTextureSize); // Bottom-Right
    corners[3].rotate(180);

    body.setSize(bodyRectSize);
    body.setPosition(spritePosition.x + cornerSize.x, spritePosition.y + cornerSize.y);
    body.setFillColor(bodyColor);
    body.setOrigin(0.0f, 0.0f);

    sides.resize(4, side);

    sides[0].setPosition(spritePosition.x + cornerSize.x, spritePosition.y); // Top
    sides[0].setSize(sideSizeWidth);
    sides[0].setFillColor(bodyColor);

    sides[1].setPosition(spritePosition.x, spritePosition.y + cornerSize.y); // Left
    sides[1].setSize(sideSizeHeight);
    sides[1].setFillColor(bodyColor);

    sides[2].setPosition(spritePosition.x + cornerSize.x, spritePosition.y + spriteTextureSize.y - cornerSize.y); // Bottom
    sides[2].setSize(sideSizeWidth);
    sides[2].setFillColor(bodyColor);

    sides[3].setPosition(spritePosition.x + spriteTextureSize.x - cornerSize.x, spritePosition.y + cornerSize.y); // Right
    sides[3].setSize(sideSizeHeight);
    sides[3].setFillColor(bodyColor);

    text.setFont(*RESOURCE_MGR.GetFont("fonts/NanumGothic.ttf"));
    text.setCharacterSize(36);
    text.setOrigin(0, 0);
    text.setPosition(spritePosition.x + cornerSize.x, bodyRectSize.y);
    text.setFillColor(sf::Color::White);
}

void Button::Update(float dt)
{
    SpriteGo::Update(dt);

    sf::Vector2f mousePos   = INPUT_MGR.GetMousePos();
    sf::Vector2f uiMousePos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

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

    window.draw(body);
    for (sf::Sprite& corner : corners)
    {
        window.draw(corner);
    }
    for (sf::RectangleShape& side : sides)
    {
        window.draw(side);
    }
    window.draw(text);
}

void Button::SetColor(int red, int green, int blue, int alpha)
{
    bodyColor = sf::Color::Color(red, green, blue, alpha);

    body.setFillColor(bodyColor);
    if (corners.size() >= 4)
    {
        for (int i = 0; i < 4; i++)
        {
            corners[i].setColor(bodyColor);
        }
    }
    if (sides.size() >= 4)
    {
        for (int i = 0; i < 4; i++)
        {
            sides[i].setFillColor(bodyColor);
        }
    }
}

void Button::SetString(const std::string& string)
{   
    text.setString(string);
}
