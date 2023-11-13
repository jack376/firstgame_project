#include "stdafx.h"
#include "rapidcsv.h"
#include "ResourceMgr.h"

ResourceMgr::~ResourceMgr()
{
	for (auto pair : mapTexture)
	{
		delete std::get<0>(pair.second);
	}
	mapTexture.clear();

	for (auto pair : mapFont)
	{
		delete std::get<0>(pair.second);
	}
	mapFont.clear();

	for (auto pair : mapSoundBuffer)
	{
		delete std::get<0>(pair.second);
	}
	mapSoundBuffer.clear();
}

void ResourceMgr::Init()
{
	LoadFromCSV("scripts/DefaultResourceList.csv", true);
}

void ResourceMgr::UnloadAll()
{
	{
		auto it =  mapTexture.begin();
		while (it != mapTexture.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapTexture.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	{
		auto it = mapFont.begin();
		while (it != mapFont.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapFont.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	{
		auto it = mapSoundBuffer.begin();
		while (it != mapSoundBuffer.end())
		{
			if (!std::get<1>(it->second))
			{
				delete std::get<0>(it->second);
				it = mapSoundBuffer.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void ResourceMgr::LoadFromCSV(const std::string path, bool isDefault = false)
{
	rapidcsv::Document doc(path);

	auto types = doc.GetColumn<int>(0);
	auto paths = doc.GetColumn<std::string>(1);

	for (int i = 0; i < types.size(); i++)
	{
		Load((ResourceTypes)types[i], paths[i], isDefault);
	}
}

void ResourceMgr::Load(ResourceTypes t, const std::string path, bool isDefault = false)
{
	switch (t)
	{
		case ResourceTypes::Texture:
		{
			auto it = mapTexture.find(path);
			if (mapTexture.end() == it)
			{
				auto texture = new sf::Texture();
				texture->loadFromFile(path);
				texture->setSmooth(true);
				mapTexture.insert({ path, { texture, true } });
			}
		}
		break;
		case ResourceTypes::Font:
		{
			auto it = mapFont.find(path);
			if (mapFont.end() == it)
			{
				auto font = new sf::Font();
				font->loadFromFile(path);
				mapFont.insert({ path, { font, true } });
			}
		}
		break;
		case ResourceTypes::SoundBuffer:
		{
			auto it = mapSoundBuffer.find(path);
			if (mapSoundBuffer.end() == it)
			{
				auto sb = new sf::SoundBuffer();
				sb->loadFromFile(path);
				mapSoundBuffer.insert({ path, { sb, true } });
			}
		}
		break;
	}
}

void ResourceMgr::Unload(ResourceTypes t, const std::string id)
{
	switch (t)
	{
		case ResourceTypes::Texture:
		{
			auto it = mapTexture.find(id);
			if (it != mapTexture.end())
			{
				if (std::get<1>(it->second))
				{
					delete std::get<0>(it->second);
					mapTexture.erase(it);
				}
				else
				{
					std::cout << "ERROR: DEFAULT RESOURCE" << std::endl;
				}
			}
		}
		break;
		case ResourceTypes::Font:
		{
			auto it = mapFont.find(id);
			if (it != mapFont.end())
			{
				if (std::get<1>(it->second))
				{
					delete std::get<0>(it->second);
					mapFont.erase(it);
				}
				else
				{
					std::cout << "ERROR: DEFAULT RESOURCE" << std::endl;
				}
			}
		}
		break;
		case ResourceTypes::SoundBuffer:
		{
			auto it = mapSoundBuffer.find(id);
			if (it != mapSoundBuffer.end())
			{
				if (std::get<1>(it->second))
				{
					delete std::get<0>(it->second);
					mapSoundBuffer.erase(it);
				}
				else
				{
					std::cout << "ERROR: DEFAULT RESOURCE" << std::endl;
				}
			}
		}
		break;
	}
}

sf::Texture* ResourceMgr::GetTexture(const std::string& id)
{
	auto it = mapTexture.find(id);
	if (it != mapTexture.end())
	{
		return std::get<0>(it->second);
	}

	return nullptr;
}

sf::Font* ResourceMgr::GetFont(const std::string& id)
{
	auto it = mapFont.find(id);
	if (it != mapFont.end())
	{
		return std::get<0>(it->second);
	}

	return nullptr;
}

sf::SoundBuffer* ResourceMgr::GetSoundBuffer(const std::string& id)
{
	auto it = mapSoundBuffer.find(id);
	if (it != mapSoundBuffer.end())
	{
		return std::get<0>(it->second);
	}

	return nullptr;
}