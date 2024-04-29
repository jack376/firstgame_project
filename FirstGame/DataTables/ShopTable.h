#pragma once
#include "DataTable.h"

struct ShopItemInfo
{
    std::string textureId;
    std::string name;
    std::string category;
    std::string title;
    std::string info;
    int amount;
    int tier;
    int damage;
    float critical;
    float cooldown;
    float range;
};

class ShopTable : public DataTable
{
protected:
    std::unordered_map<std::string, ShopItemInfo> table;

public:
    ShopTable() : DataTable(DataTable::Ids::Shop) {};
    virtual ~ShopTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const ShopItemInfo& Get(const std::string& name);
    std::string replaceAll(std::string str, const std::string& from, const std::string& to);
};