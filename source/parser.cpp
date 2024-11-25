#include "parser.h"

using namespace text_p;

Parser::Parser()
    :desc_{kOptDescription}, pos_desc_{}
{
    desc_.add_options()
        (kCommandKey, BstProgOpt::value<std::string>(), kCommandKeyDescription)
        (kArgsKey, BstProgOpt::value<std::vector<std::string>>()->multitoken(), kArgsKeyDescription);

    int first_position = 1;
    int second_pos_unlimited = -1;

    pos_desc_.add(kCommandKey, first_position);
    pos_desc_.add(kArgsKey, second_pos_unlimited);

}

Parser::~Parser()
{
}


CommandVariant Parser::ParseString(const std::string& user_input)
{
    try
    {
        std::vector<std::string> args_vector{};
        std::string arg{};
        std::istringstream iss(user_input);
        BstProgOpt::variables_map vm{};

        while (iss >> arg)
        {
            args_vector.push_back(arg);
        }
 
        
        BstProgOpt::store(BstProgOpt::command_line_parser(args_vector).options(desc_).positional(pos_desc_).run(), vm);
        BstProgOpt::notify(vm);

        if (!vm.count(kCommandKey))
        {
            return ECommand::kNull;
        }

        std::string command = vm[kCommandKey].as<std::string>();


        if (command == command_str::kInsert) { return std::move(ParseInsert(vm)); }
        else if (command == command_str::kTruncate) { return std::move(ParseTruncate(vm)); }
        else if (command == command_str::kIntersection) { return ECommand::kIntersection; }
        else if (command == command_str::kSymmetricDifference) { return ECommand::kSymmetricDifference; }
        else
        {
            return ECommand::kNull;
        }

    }
    catch (std::exception& e)
    {
        logger::ErrorException(e.what(), __func__);
    }

    return ECommand::kNull;
}/* ParseString */

CommandVariant Parser::ParseInsert(BstProgOpt::variables_map& vm)
{
    using pair = std::pair<ECommand, InsertArgs>;

    try
    {
        if (!vm.count(kArgsKey))
        {
            return ECommand::kNull;
        }

        auto& args = vm[kArgsKey].as<std::vector<std::string>>();

        if (args.size() != command_str::kInsertArgsSize)
        {
            return ECommandStatus::kErrorArgMissed;
        }
    
        InsertArgs command_args{};
        command_args.table_name_ = args[0];
        command_args.id_ = static_cast<uint32_t>(std::stoi(args[1]));
        command_args.data_ = args[2];

        return std::make_unique<pair>(ECommand::kInsert, std::move(command_args));

    } 
    catch (std::exception& e)
    {
        logger::ErrorException(e.what(), __func__);
    }

    return ECommand::kNull;
}/* ParseInsert */

CommandVariant Parser::ParseTruncate(BstProgOpt::variables_map& vm)
{
    using pair = std::pair<ECommand, std::string>;

    try
    {
        if (!vm.count(kArgsKey))
        {
            return ECommand::kNull;
        }

        auto& args = vm[kArgsKey].as<std::vector<std::string>>();

        if (args.size() != command_str::kTruncateArgsSize)
        {
            return ECommandStatus::kErrorArgMissed;
        }

        OnlyTableNameVar t = std::make_unique<pair>(ECommand::kTruncate, std::move(args[0]));

        return t;
    }
    catch (std::exception& e)
    {
        logger::ErrorException(e.what(), __func__);
    }

    return ECommand::kNull;

}/* ParseTruncate */


//CommandVariant Parser::ParseTruncate(BstProgOpt::variables_map& vm)
//{
//    using pair = std::pair<ECommand, InsertArgs>;
//    try
//    {
//
//    }
//    catch (std::exception& e)
//    {
//        logger::ErrorException(e.what(), __func__);
//    }
//
//    return ECommand::kNull;
//}/* ParseTruncate */
//
//CommandVariant Parser::ParseTruncate(BstProgOpt::variables_map& vm)
//{
//    using pair = std::pair<ECommand, InsertArgs>;
//    try
//    {
//
//    }
//    catch (std::exception& e)
//    {
//        logger::ErrorException(e.what(), __func__);
//    }
//
//    return ECommand::kNull;
//}/* ParseTruncate */