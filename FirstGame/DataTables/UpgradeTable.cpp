#include "stdafx.h"
#include "UpgradeTable.h"
#include "rapidcsv.h"

bool UpgradeTable::Load()
{
    rapidcsv::Document doc("tables/UpgradeTable.csv");

    std::vector<std::string> textureId = doc.GetColumn<std::string>(0);
    std::vector<std::string> name      = doc.GetColumn<std::string>(1);
    std::vector<std::string> category  = doc.GetColumn<std::string>(2);
    std::vector<std::string> highlight = doc.GetColumn<std::string>(3);
    std::vector<std::string> title     = doc.GetColumn<std::string>(4);
    std::vector<float>       value     = doc.GetColumn<float>(5);
    std::vector<int>         amount    = doc.GetColumn<int>(6);
    std::vector<int>         tier      = doc.GetColumn<int>(7);

    for (int i = 0; i < textureId.size(); ++i)
    {
        UpgradeInfo upgradeInfo;

        upgradeInfo.textureId = textureId[i];
        upgradeInfo.name      = name[i];
        upgradeInfo.category  = category[i];
        upgradeInfo.highlight = highlight[i];
        upgradeInfo.title     = title[i];
        upgradeInfo.value     = value[i];
        upgradeInfo.amount    = amount[i];
        upgradeInfo.tier      = tier[i];

        table[name[i]] = upgradeInfo;
    }
    return true;
}

void UpgradeTable::Release()
{
    table.clear();
}

const UpgradeInfo& UpgradeTable::Get(const std::string& name)
{
    auto find = table.find(name);
    if (find == table.end())
    {
        throw std::runtime_error("ERROR : Undefined Item Name");
    }
    return find->second;
}