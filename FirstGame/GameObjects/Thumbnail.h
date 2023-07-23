#pragma once
#include "BaseUI.h"

class Thumbnail : public BaseUI
{
public:
	Thumbnail(const std::string& n);
	virtual ~Thumbnail() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
};

