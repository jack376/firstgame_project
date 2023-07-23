#pragma once
#include "DataTable.h"

struct ShopItemInfo
{
    std::string textureId;
    std::string name;
    std::string category;
    std::string title;
    std::string info;
    std::string amount;
    int tier;
    int damage;
    float critical;
    float cooldown;
    float range;
};

class UIShopTable : public DataTable
{
protected:
    std::unordered_map<std::string, ShopItemInfo> table;

public:
    UIShopTable() : DataTable(DataTable::Ids::Shop) {};
    virtual ~UIShopTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const ShopItemInfo& Get(const std::string& name);
    std::string replaceAll(std::string str, const std::string& from, const std::string& to);
};