#include "stdafx.h"
#include "Button.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

Button::Button(const std::string& n) 
    : GameObject(n)
{
}

Button::~Button()
{
    Release();
}

void Button::Init()
{
}

void Button::Reset()
{
    isHover = false;

    bodyColor   = sf::Color(0, 0, 0, 192);
    textColor   = sf::Color(255, 255, 255, 255);
    strokeColor = sf::Color(0, 0, 0, 0);

    corner.setTexture(*RESOURCE_MGR.GetTexture("graphics/corner.png"));
    corner.setOrigin(0.0f, 0.0f);
    corner.setColor(bodyColor);

    cornerStroke.setTexture(*RESOURCE_MGR.GetTexture("graphics/stroke_corner.png"));
    cornerStroke.setOrigin(0.0f, 0.0f);
    cornerStroke.setColor(strokeColor);

    sideStroke.setTexture(*RESOURCE_MGR.GetTexture("graphics/stroke_side.png"));
    sideStroke.setOrigin(0.0f, 0.0f);
    sideStroke.setColor(strokeColor);

    text.setFont(*RESOURCE_MGR.GetFont("fonts/Chewy-Regular.ttf"));
    text.setCharacterSize(fontSize);
    sf::FloatRect textRect = text.getLocalBounds();

    sf::Vector2f textOriginTuning(-6.0f, 0.0f);
    float buttonHeight = (fontSize * 1.8) + buttonModifySize.y;
    text.setLetterSpacing(1.0f);

    sf::Vector2f cornerTextureSize(corner.getTexture()->getSize());
    sf::Vector2f buttonPosition(buttonModifyPosition);
    sf::Vector2f buttonSize(sf::Vector2f(buttonModifySize.x + textRect.top + textRect.width + (cornerTextureSize.x * 2.0f), buttonHeight));
    sf::Vector2f bodyRectSize(buttonSize.x - cornerTextureSize.x * 2, buttonSize.y - cornerTextureSize.y * 2);
    sf::Vector2f sideSizeWidth(buttonSize.x - cornerTextureSize.x * 2, cornerTextureSize.y);
    sf::Vector2f sideSizeHeight(cornerTextureSize.x, buttonSize.y - cornerTextureSize.y * 2);

    text.setOrigin(textOriginTuning.x, textOriginTuning.y + (textRect.top + textRect.height * 0.5f));
    text.setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y + buttonHeight * 0.5f);
    text.setFillColor(textColor);

    buttonCollider.setSize(buttonSize);
    buttonCollider.setPosition(buttonPosition);
    buttonCollider.setFillColor(sf::Color::Transparent);
    //buttonCollider.setOutlineColor(sf::Color::Blue); // Collider View Test
    //buttonCollider.setOutlineThickness(2.0f); // Collider View Test

    body.setSize(bodyRectSize);
    body.setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y + cornerTextureSize.y);
    body.setFillColor(bodyColor);
    body.setOrigin(0.0f, 0.0f);

    corners.resize(4, corner);

    corners[0].setPosition(buttonPosition); // Top-Left
    corners[0].setRotation(0);
    corners[0].setColor(bodyColor);

    corners[1].setPosition(buttonPosition.x + buttonSize.x, buttonPosition.y); // Top-Right
    corners[1].setRotation(90);
    corners[1].setColor(bodyColor);

    corners[2].setPosition(buttonPosition.x, buttonPosition.y + buttonSize.y); // Bottom-Left
    corners[2].setRotation(270);
    corners[2].setColor(bodyColor);

    corners[3].setPosition(buttonPosition + buttonSize); // Bottom-Right
    corners[3].setRotation(180);
    corners[3].setColor(bodyColor);

    cornerStrokes.resize(4, cornerStroke);

    cornerStrokes[0].setPosition(buttonPosition); // Top-Left
    cornerStrokes[0].setRotation(0);
    cornerStrokes[0].setColor(strokeColor);

    cornerStrokes[1].setPosition(buttonPosition.x + buttonSize.x, buttonPosition.y); // Top-Right
    cornerStrokes[1].setRotation(90);
    cornerStrokes[1].setColor(strokeColor);

    cornerStrokes[2].setPosition(buttonPosition.x, buttonPosition.y + buttonSize.y); // Bottom-Left
    cornerStrokes[2].setRotation(270);
    cornerStrokes[2].setColor(strokeColor);

    cornerStrokes[3].setPosition(buttonPosition + buttonSize); // Bottom-Right
    cornerStrokes[3].setRotation(180);
    cornerStrokes[3].setColor(strokeColor);

    sides.resize(4, side);

    sides[0].setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y); // Top
    sides[0].setSize(sideSizeWidth);
    sides[0].setFillColor(bodyColor);

    sides[1].setPosition(buttonPosition.x, buttonPosition.y + cornerTextureSize.y); // Left
    sides[1].setSize(sideSizeHeight);
    sides[1].setFillColor(bodyColor);

    sides[2].setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y + buttonSize.y - cornerTextureSize.y); // Bottom
    sides[2].setSize(sideSizeWidth);
    sides[2].setFillColor(bodyColor);

    sides[3].setPosition(buttonPosition.x + buttonSize.x - cornerTextureSize.x, buttonPosition.y + cornerTextureSize.y); // Right
    sides[3].setSize(sideSizeHeight);
    sides[3].setFillColor(bodyColor);

    sideStrokes.resize(4, sideStroke);

    sideStrokes[0].setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y); // Top
    sideStrokes[0].setScale(1.0f * sideSizeWidth.x, 1.0f);
    sideStrokes[0].setRotation(0);
    sideStrokes[0].setColor(strokeColor);

    sideStrokes[1].setPosition(buttonPosition.x, buttonPosition.y + cornerTextureSize.y); // Left
    sideStrokes[1].setScale(-1.0f * sideSizeHeight.y, 1.0f);
    sideStrokes[1].setRotation(270);
    sideStrokes[1].setColor(strokeColor);

    sideStrokes[2].setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y + buttonSize.y); // Bottom
    sideStrokes[2].setScale(1.0f * sideSizeWidth.x, -1.0f);
    sideStrokes[2].setRotation(0);
    sideStrokes[2].setColor(strokeColor);

    sideStrokes[3].setPosition(buttonPosition.x + buttonSize.x, buttonPosition.y + cornerTextureSize.y); // Right
    sideStrokes[3].setScale(1.0f * sideSizeHeight.y, 1.0f);
    sideStrokes[3].setRotation(90);
    sideStrokes[3].setColor(strokeColor);
}

void Button::Update(float dt)
{
    sf::Vector2f mousePos   = INPUT_MGR.GetMousePos();
    sf::Vector2f uiMousePos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

    bool prevHover = isHover;
    //isHover = sprite.getGlobalBounds().contains(uiMousePos);
    isHover = buttonCollider.getGlobalBounds().contains(uiMousePos);

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
    window.draw(body);

    for (auto& corner : corners)
    {
        window.draw(corner);
    }
    for (auto& side : sides)
    {
        window.draw(side);
    }
    for (auto& cornerStroke : cornerStrokes)
    {
        window.draw(cornerStroke);
    }
    for (auto& sideStroke : sideStrokes)
    {
        window.draw(sideStroke);
    }

    window.draw(text);
    window.draw(buttonCollider);
}

void Button::SetColor(int red, int green, int blue, int alpha)
{
    bodyColor = sf::Color(red, green, blue, alpha);
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

void Button::SetTextColor(int red, int green, int blue, int alpha)
{
    textColor = sf::Color(red, green, blue, alpha);
    text.setFillColor(textColor);
}

void Button::SetStrokeColor(int red, int green, int blue, int alpha)
{
    strokeColor = sf::Color(red, green, blue, alpha);
    if (sideStrokes.size() >= 4)
    {
        for (int i = 0; i < 4; i++)
        {
            sideStrokes[i].setColor(strokeColor);
        }
    }
}

void Button::SetString(const std::string& string)
{   
    std::string utf8String = string;
    sf::String unicodeString = sf::String::fromUtf8(utf8String.begin(), utf8String.end());
    text.setString(unicodeString);
}

void Button::SetSize(float x, float y)
{
    buttonModifySize = sf::Vector2f(x, y);
    buttonCollider.setSize(sf::Vector2f(x, y));
}

void Button::SetPosition(float x, float y)
{
    buttonModifyPosition = sf::Vector2f(x, y);
    buttonCollider.setPosition(x, y);
}

