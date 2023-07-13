#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player : public SpriteGo
{
public:
	struct ClipInfo
	{
		std::string idle;
		std::string move;
		bool flipX = false;
		sf::Vector2f point;
	};

protected:
	AnimationController animation;
	sf::Vector2f direction;
	float speed = 300.f;

	bool filpX = false;
	bool isGround = true;

	std::vector<ClipInfo> clipInfos;
	ClipInfo currentClipInfo;

public:
	Player(const std::string& textureId, const std::string& n);
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;

	bool GetFlipX() const;
	void SetFlipX(bool filp);
};

