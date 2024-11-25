#include "session.h"
#include "settings.h"

ClientSession::ClientSession(BstTcpSocket&& s, Parser& p, Database& d, IMessageSender* net_observer)
    :socket_{std::move(s)}, common_parser_{p}, common_database_{d}, message_sender_{net_observer}, active_{true}
{
}

inline void ClientSession::NewMessageNotify(ECommandStatus status, std::string&& string)
{
    boost::asio::co_spawn(socket_.get_executor()
        , message_sender_->SendMessageToClient(socket_, status, std::move(string))
        , boost::asio::detached);
}


BstTcpSocket& ClientSession::GetSocket()
{
    return socket_;
}

bool ClientSession::IsActive() const
{
    return active_;
}

void ClientSession::Deactivate()
{
    active_ = false;
}

boost::asio::awaitable<void> ClientSession::HandleRequest(std::string&& user_request)
{    

    CommandVariant variant = std::move(common_parser_.ParseString(user_request));
    ECommandStatus message{ECommandStatus::kError};

    if ((std::holds_alternative<ECommand>(variant)))
    {
        if (std::get<ECommand>(variant) == ECommand::kNull)
        {
            /* Error */
        }
        else if (std::get<ECommand>(variant) == ECommand::kIntersection)
        {
            std::string response = common_database_.Intersection();
            if (!response.empty())
            {
                message = ECommandStatus::kSuccesIntersection;
                NewMessageNotify(message, std::move(response));
                co_return;
            }
            
        }
        else if (std::get<ECommand>(variant) == ECommand::kSymmetricDifference)
        {
            std::string response = common_database_.SymmetricDifference();
            if (!response.empty())
            {
                message = ECommandStatus::kSuccesSymmetricDifference;
                NewMessageNotify(message, std::move(response));
                co_return;
            }
        }

      
    }
    else if (std::holds_alternative<InsertVariant>(variant))
    {
        const auto& insert_args = std::get<InsertVariant>(variant)->second;
        message = common_database_.Insert(insert_args);
    }
    else if (std::holds_alternative<OnlyTableNameVar>(variant))
    {
        /* Handle ECommand::kTruncate */

        if (std::get<OnlyTableNameVar>(variant)->first == ECommand::kTruncate)
        {
            const auto& table_name = std::get<OnlyTableNameVar>(variant)->second;
            message = common_database_.Truncate(table_name);
        }
    }


    NewMessageNotify(message);

    co_return;
}
