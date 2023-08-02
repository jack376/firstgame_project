#include "stdafx.h"
#include "ShopTable.h"
#include "rapidcsv.h"

bool ShopTable::Load()
{
    rapidcsv::Document doc("tables/ShopTable.csv");

    std::vector<std::string> textureId = doc.GetColumn<std::string>(0);
    std::vector<std::string> name      = doc.GetColumn<std::string>(1);
    std::vector<std::string> category  = doc.GetColumn<std::string>(2);
    std::vector<std::string> title     = doc.GetColumn<std::string>(3);
    std::vector<std::string> info      = doc.GetColumn<std::string>(4);
    std::vector<int>         amount    = doc.GetColumn<int>(5);
    std::vector<int>         tier      = doc.GetColumn<int>(6);
    std::vector<int>         damage    = doc.GetColumn<int>(7);
    std::vector<float>       critical  = doc.GetColumn<float>(8);
    std::vector<float>       cooldown  = doc.GetColumn<float>(9);
    std::vector<float>       range     = doc.GetColumn<float>(10);

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