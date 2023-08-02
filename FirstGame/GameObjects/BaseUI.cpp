#include "stdafx.h"
#include "BaseUI.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"

BaseUI::BaseUI(const std::string& n, uiType uiStatus) : GameObject(n), uiStatus(uiStatus)
{
    switch (uiStatus)
    {
    case uiType::Text:
        bodyColor = sf::Color(0, 0, 0, 192);
        break;
    case uiType::Thumbnail:
        bodyColor = sf::Color(50, 50, 50, 255);
        break;
    case uiType::Box:
        bodyColor = sf::Color::Black;
        break;
    }
    textColor = sf::Color::White;
    strokeColor = sf::Color::Transparent;
}

BaseUI::~BaseUI()
{
    Release();
}

void BaseUI::Init()
{
}

void BaseUI::Reset()
{
    isHover = false;

    corner.setTexture(*RESOURCE_MGR.GetTexture("graphics/corner.png"));
    corner.setOrigin(0.0f, 0.0f);
    corner.setColor(bodyColor);

    cornerStroke.setTexture(*RESOURCE_MGR.GetTexture("graphics/stroke_corner.png"));
    cornerStroke.setOrigin(0.0f, 0.0f);
    cornerStroke.setColor(strokeColor);

    sideStroke.setTexture(*RESOURCE_MGR.GetTexture("graphics/stroke_side.png"));
    sideStroke.setOrigin(0.0f, 0.0f);
    sideStroke.setColor(strokeColor);

    thumbnail.setTexture(*RESOURCE_MGR.GetTexture("graphics/thumbnail.png"));
    thumbnail.setOrigin(thumbnail.getTexture()->getSize().x * 0.5f, thumbnail.getTexture()->getSize().y * 0.5f);

    text.setFont(*RESOURCE_MGR.GetFont("fonts/Chewy-Regular.ttf"));
    text.setCharacterSize(fontSize);
    text.setLetterSpacing(1.0f);

    sf::Vector2f cornerTextureSize(corner.getTexture()->getSize());

    if (uiStatus == uiType::Text)
    {
        sf::Vector2f textOriginTuning(-6.0f, 0.0f);
        sf::FloatRect textRect = text.getLocalBounds();
        textHeight = (fontSize * 1.8) + buttonModifySize.y;

        buttonSizeX = textRect.top + textRect.width + cornerTextureSize.x * 2.0f;
        buttonSizeY = textHeight;

        text.setOrigin(textOriginTuning.x, textOriginTuning.y + (textRect.top + textRect.height * 0.5f));
        text.setPosition(buttonModifyPosition.x + cornerTextureSize.x, buttonModifyPosition.y + textHeight * 0.5f);
        text.setFillColor(textColor);
    }
    else if (uiStatus == uiType::Thumbnail)
    {
        sf::FloatRect thumRect = thumbnail.getLocalBounds();
        float blankSize = 0.0f;

        buttonSizeX = thumRect.width + (cornerTextureSize.x * 2.0f) * blankSize;
        buttonSizeY = thumRect.height + (cornerTextureSize.y * 2.0f) * blankSize;
    }
    else if (uiStatus == uiType::Box)
    {
        sf::FloatRect boxRect(buttonModifyPosition, buttonModifySize);

        buttonSizeX = buttonModifySize.x;
        buttonSizeY = buttonModifySize.y;
    }

    sf::Vector2f buttonSize(buttonSizeX, buttonSizeY);
    sf::Vector2f buttonPosition(buttonModifyPosition);
    sf::Vector2f bodyRectSize(buttonSize.x - cornerTextureSize.x * 2, buttonSize.y - cornerTextureSize.y * 2);
    sf::Vector2f sideSizeWidth(buttonSize.x - cornerTextureSize.x * 2, cornerTextureSize.y);
    sf::Vector2f sideSizeHeight(cornerTextureSize.x, buttonSize.y - cornerTextureSize.y * 2);

    thumbnail.setPosition(buttonPosition.x + cornerTextureSize.x + bodyRectSize.x * 0.5f, buttonPosition.y + cornerTextureSize.y + bodyRectSize.y * 0.5f);

    buttonCollider.setSize(buttonSize);
    buttonCollider.setPosition(buttonPosition);
    buttonCollider.setFillColor(sf::Color::Transparent);
    //buttonCollider.setOutlineColor(sf::Color::Green); // Collider View Test
    //buttonCollider.setOutlineThickness(2.0f); // Collider View Test

    // Body
    body.setSize(bodyRectSize);
    body.setPosition(buttonPosition.x + cornerTextureSize.x, buttonPosition.y + cornerTextureSize.y);
    body.setFillColor(bodyColor);
    body.setOrigin(0.0f, 0.0f);

    // Corner
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

    // Side
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

    // CornerStroke
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

    // SideStroke
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

void BaseUI::Update(float dt)
{
    sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
    sf::Vector2f uiMousePos = SCENE_MGR.GetCurrentScene()->ScreenToUiPos(mousePos);

    bool prevHover = isHover;
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

void BaseUI::Draw(sf::RenderWindow& window)
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

    switch (uiStatus)
    {
    case uiType::Text:
        window.draw(text);
        break;
    case uiType::Thumbnail:
        window.draw(thumbnail);
        break;
    case uiType::Box:
        break;
    }
    window.draw(buttonCollider);
}

void BaseUI::SetColor(int red, int green, int blue, int alpha)
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

void BaseUI::SetTextColor(int red, int green, int blue, int alpha)
{
    textColor = sf::Color(red, green, blue, alpha);
    text.setFillColor(textColor);
}

void BaseUI::SetStrokeColor(int red, int green, int blue, int alpha)
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

void BaseUI::SetThumbnailColor(int red, int green, int blue, int alpha)
{
    thumbnail.setColor(sf::Color(red, green, blue, alpha));
}

void BaseUI::SetString(const std::string& string)
{
    std::string utf8String = string;
    sf::String unicodeString = sf::String::fromUtf8(utf8String.begin(), utf8String.end());
    text.setString(unicodeString);
}

void BaseUI::SetCharacterSize(unsigned int fontSize)
{
    this->fontSize = fontSize;
}

void BaseUI::SetSizeAdd(float x, float y)
{
    buttonModifySize = sf::Vector2f(x, y);
    buttonCollider.setSize(sf::Vector2f(x, y));
}

void BaseUI::SetPosition(float x, float y)
{
    buttonModifyPosition = sf::Vector2f(x, y);
    buttonCollider.setPosition(x, y);
}

