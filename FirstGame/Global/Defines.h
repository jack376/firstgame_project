#pragma once

#define _USE_MATH_DEFINES
#define _PI 3.141592

enum class Languages
{
	KOR,
	ENG,
	JP,
	COUNT,
};

enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
	AnimationClip,
};

enum class SceneId
{
	None = -1,
	Title,
	Game,
	Count,
};

enum class uiType
{
	Text,
	Thumbnail,
	Box,
};