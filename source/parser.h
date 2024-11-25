#pragma once

#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <memory>

#include "settings.h"

namespace text_p
{
	static const char* kOptDescription{"Allowed options"};
	static const char* kCommandKey{"command"};
	static const char* kCommandKeyDescription{"..."};
	static const char* kArgsKey{"args"};
	static const char* kArgsKeyDescription{"Command's arguments"};

} /* text_p */

namespace command_str
{
	static const char* kInsert{"INSERT"};
	static const uint16_t kInsertArgsSize{3};

	static const char* kTruncate{"TRUNCATE"};
	static const uint16_t kTruncateArgsSize{1};

	static const char* kIntersection{"INTERSECTION"};
	static const uint16_t kIntersectionArgsSize{0};

	static const char* kSymmetricDifference{"SYMMETRIC_DIFFERENCE"};
	static const uint16_t kSymmetricDifferenceArgsSize{0};


} /* command_str */


struct InsertArgs
{
	std::string table_name_{};
	uint32_t id_{};
	std::string data_{};
};


namespace BstProgOpt = boost::program_options;
using InsertVariant = std::unique_ptr <std::pair<ECommand, InsertArgs>>;
using OnlyTableNameVar = std::unique_ptr <std::pair<ECommand, std::string>>;

using CommandVariant = std::variant<ECommandStatus
	, ECommand
	, InsertVariant
	, OnlyTableNameVar>;

class Parser
{
public:
	Parser();
	~Parser();

	CommandVariant ParseString(const std::string& input);
	CommandVariant ParseInsert(BstProgOpt::variables_map& vm);
	CommandVariant ParseTruncate(BstProgOpt::variables_map& vm);
private:
	BstProgOpt::options_description desc_;
	BstProgOpt::positional_options_description pos_desc_;

};


