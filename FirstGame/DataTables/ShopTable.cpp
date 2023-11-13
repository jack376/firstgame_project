#include "stdafx.h"
#include "ShopTable.h"
#include "rapidcsv.h"

bool ShopTable::Load()
{
    rapidcsv::Document doc("tables/ShopTable.csv");

    auto textureId = doc.GetColumn<std::string>(0);
    auto name      = doc.GetColumn<std::string>(1);
    auto category  = doc.GetColumn<std::string>(2);
    auto title     = doc.GetColumn<std::string>(3);
    auto info      = doc.GetColumn<std::string>(4);
    auto amount    = doc.GetColumn<int>(5);
    auto tier      = doc.GetColumn<int>(6);
    auto damage    = doc.GetColumn<int>(7);
    auto critical  = doc.GetColumn<float>(8);
    auto cooldown  = doc.GetColumn<float>(9);
    auto range     = doc.GetColumn<float>(10);

    for (int i = 0; i < textureId.size(); ++i)
    {
        ShopItemInfo itemInfo;

        itemInfo.textureId = textureId[i];
        itemInfo.name      = name[i];
        itemInfo.category  = category[i];
        itemInfo.title     = replaceAll(title[i], "\\n", "\n");
        itemInfo.info      = replaceAll(info[i], "\\n", "\n");
        itemInfo.amount    = amount[i];
        itemInfo.tier      = tier[i];
        itemInfo.damage    = damage[i];
        itemInfo.critical  = critical[i];
        itemInfo.cooldown  = cooldown[i];
        itemInfo.range     = range[i];

        table[name[i]] = itemInfo;
    }
    return true;
}

void ShopTable::Release()
{
    table.clear();
}

const ShopItemInfo& ShopTable::Get(const std::string& name)
{
    auto find = table.find(name);
    if (find == table.end())
    {
        throw std::runtime_error("ERROR : Undefined Item Name");
    }

    return find->second;
}

std::string ShopTable::replaceAll(std::string str, const std::string& from, const std::string& to)
{
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos)
    {
        str.replace(startPos, from.length(), to);
        startPos += to.length();
    }

    return str;
}