#include "database.h"
#include <algorithm>

Database::Database()
    :database_{}
{    
}

Database::~Database()
{
}

void Database::CreateTable(std::string table_name)
{
    database_.push_back(std::pair{table_name, SimpleTable{}});
}

ECommandStatus Database::Insert(const InsertArgs& insert_args)
{

    auto table_it = std::find_if(database_.begin()
        , database_.end()
        , [&insert_args](const std::pair<std::string, SimpleTable>& table)
        { return table.first == insert_args.table_name_; }
    );


    if (table_it == database_.end())
    {
        return ECommandStatus::kErrorTableNotFound;
    }

    SimpleTable& table = table_it->second;

    if (table.find(insert_args.id_) != table.end())
    { 
        return ECommandStatus::kErrorInsertIdExist;
    }

    /*Try to emplace*/

    if (table.emplace(insert_args.id_, insert_args.data_).second == true)
    {
        return ECommandStatus::kSucces;
    }

    return ECommandStatus::kError;
}


ECommandStatus Database::Truncate(const std::string& table_name)
{

    auto table_it = std::find_if(database_.begin()
        , database_.end()
        , [&table_name](const std::pair<std::string, SimpleTable>& table)
        { return table.first == table_name; }
    );


    if (table_it == database_.end())
    {
        return ECommandStatus::kErrorTableNotFound;
    }

    database_.erase(table_it);

    return ECommandStatus::kSucces;
}


std::string Database::Intersection() const 
{
    std::string str_result{};

    auto tableA_it = std::find_if(database_.begin(), database_.end(),
        [](const auto& table) { return table.first == "A"; });
    auto tableB_it = std::find_if(database_.begin(), database_.end(),
        [](const auto& table) { return table.first == "B"; });

    if (tableA_it == database_.end() || tableB_it == database_.end()) 
    {
        return str_result;
    }

    const auto& tableA = tableA_it->second;
    const auto& tableB = tableB_it->second;

    std::ostringstream oss_result;

    for (const auto& [id, nameA] : tableA) 
    {
        auto itB = tableB.find(id);
        if (itB != tableB.end()) 
        {
            oss_result << id << "," << nameA << "," << itB->second << "\n";
        }
    }  

    str_result = std::move(oss_result.str());

    return str_result;
}



std::string Database::SymmetricDifference() const
{
    std::string str_result{};

    auto tableA_it = std::find_if(database_.begin(), database_.end(),
        [](const auto& table) { return table.first == "A"; });
    auto tableB_it = std::find_if(database_.begin(), database_.end(),
        [](const auto& table) { return table.first == "B"; });

    if (tableA_it == database_.end() || tableB_it == database_.end())
    {
        return str_result;
    }

    const auto& tableA = tableA_it->second;
    const auto& tableB = tableB_it->second;

    std::ostringstream oss_result;

    for (const auto& [id, nameA] : tableA) 
    {
        if (tableB.find(id) == tableB.end()) 
        {
            oss_result << id << "," << nameA << ",\n";
        }
    }

    for (const auto& [id, nameB] : tableB) 
    {
        if (tableA.find(id) == tableA.end()) 
        {
            oss_result << id << ",," << nameB << "\n";
        }
    }

    str_result = std::move(oss_result.str());

    return str_result;
}