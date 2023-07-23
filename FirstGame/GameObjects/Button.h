#pragma once
#include "BaseUI.h"

class Button : public BaseUI
{
public:
	Button(const std::string& n);
	virtual ~Button() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

