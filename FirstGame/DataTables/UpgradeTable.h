#pragma once
#include "DataTable.h"

struct UpgradeInfo
{
    std::string textureId;
    std::string name;
    std::string category;
    std::string highlight;
    std::string title;
    int value;
    int amount;
    int tier;
};

class UpgradeTable : public DataTable
{
protected:
    std::unordered_map<std::string, UpgradeInfo> table;

public:
    UpgradeTable() : DataTable(DataTable::Ids::Upgrade) {};
    virtual ~UpgradeTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    const UpgradeInfo& Get(const std::string& name);
};