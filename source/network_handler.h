#pragma once

#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <memory>

#include "session.h"
#include "os_interface.h"


using BstTcpAcceptor = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::acceptor>;
using BstTcpSocket = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::socket>;


class NetworkHandler : public IMessageSender
{
public:
    NetworkHandler();
    ~NetworkHandler() = default;
   
    boost::asio::awaitable<void> GetClientResponse(std::shared_ptr<ClientSession> session);
    boost::asio::awaitable<void> ListenConnect(const uint16_t port, Parser&, Database&);
    boost::asio::awaitable<void> SendMessageToClient(BstTcpSocket&, ECommandStatus, std::string&& s = "") override;
    

private:
};



