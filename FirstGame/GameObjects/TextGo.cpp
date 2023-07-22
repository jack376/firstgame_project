#include "stdafx.h"
#include "TextGo.h"
#include "ResourceMgr.h"
#include "Utils.h"

TextGo::TextGo(const std::string& fontId = "", const std::string& n = "")
	: GameObject(n), fontId(fontId)
{
}

TextGo::~TextGo()
{
}

void TextGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	text.setPosition(p);
}

void TextGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	text.setPosition(position);
}

void TextGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(text, origin);
	}
}

void TextGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	text.setOrigin(x, y);
}

void TextGo::SetString(const std::string& str)
{
	std::string utf8String = str;
	sf::String unicodeString = sf::String::fromUtf8(utf8String.begin(), utf8String.end());
	text.setString(unicodeString);
}

void TextGo::SetFillColor(const sf::Color& color)
{
	text.setFillColor(color);
}

void TextGo::SetFont(sf::Font& font)
{
	text.setFont(font);
}

void TextGo::SetCharacterSize(const int& fontSize)
{
	text.setCharacterSize(fontSize);
}

void TextGo::Init()
{
}

void TextGo::Release()
{
}

void TextGo::Reset()
{
	sf::Font* font = RESOURCE_MGR.GetFont(fontId);
	if (font != nullptr)
	{
		text.setFont(*font);
	}
	SetOrigin(origin);
}

void TextGo::Update(float dt)
{
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

