#include "network_handler.h"
#include "settings.h"

NetworkHandler::NetworkHandler()
{
}

boost::asio::awaitable<void> NetworkHandler::ListenConnect(const uint16_t port, Parser& parser, Database& d)
{
    auto executor = co_await boost::asio::this_coro::executor;

    BstTcpAcceptor acceptor(executor, {boost::asio::ip::tcp::v4(), port});

    while (true)
    {
        BstTcpSocket socket = co_await acceptor.async_accept();

        try
        {
            auto session = std::make_shared<ClientSession>(std::move(socket), parser, d, this);
            boost::asio::co_spawn(executor, GetClientResponse(session), boost::asio::detached);

            std::cout << "Connect" << std::endl;
        }
        catch (const std::exception& e)
        {            
            logger::ErrorException(e.what(), __func__);
        }
    }
}/* ListenConnect */

boost::asio::awaitable<void> NetworkHandler::SendMessageToClient(BstTcpSocket& socket
    , ECommandStatus status, std::string&& string)
{
    try
    {
        std::string message{};

        switch (status)
        {
        case ECommandStatus::kError:
            message = "Unknown Error";
            break;

        case ECommandStatus::kErrorArgMissed:
            message = "Argument is missed";
            break;

        case ECommandStatus::kSucces:
            message = "OK";
            break;

        case ECommandStatus::kErrorInsertIdExist:
            message = "ERR duplicate id";
            break;

        case ECommandStatus::kSuccesIntersection:
        case ECommandStatus::kSuccesSymmetricDifference:
            message = std::move(string);
            message += "OK";
            break;

        default:
            break;
        }

        co_await boost::asio::async_write(socket, boost::asio::buffer(message));

    }
    catch (const std::exception& e)
    {
        logger::ErrorException(e.what(), __func__);
    }

}/* SendMessageToClient */



boost::asio::awaitable<void> NetworkHandler::GetClientResponse(std::shared_ptr<ClientSession> session)
{
    auto executor = co_await boost::asio::this_coro::executor;
    
    BstTcpSocket& socket = session->GetSocket();

    while (session->IsActive())
    {
        try
        {
            char data[settings::kBufferSize];

            size_t length = co_await socket.async_read_some(boost::asio::buffer(data));
            std::string received_data(data, length);

            boost::asio::co_spawn(executor, session->HandleRequest(std::move(received_data)), boost::asio::detached);            

        }    
        catch (const boost::system::system_error& e)
        {
            if (e.code() == boost::asio::error::eof 
                || e.code() == boost::asio::error::connection_reset)
            { 
                session->Deactivate();
                logger::ErrorException(e.what(), __func__);
                break;
            }       
        }
        catch (const std::exception& e)
        {
            logger::ErrorException(e.what(), __func__);
        }
    }

}/* GetClientResponse */






