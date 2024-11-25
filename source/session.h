#pragma once

#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/awaitable.hpp> 
#include <memory> 
#include <list> 

#include "os_interface.h"
#include "parser.h" // !!!
#include "database.h"

using BstTcpAcceptor = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::acceptor>;
using BstTcpSocket = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::socket>;

class ClientSession : public std::enable_shared_from_this<ClientSession>, IMessageCreater
{
public:
    ClientSession(BstTcpSocket&&, Parser&, Database&, IMessageSender* net_observer);
    ~ClientSession() = default;

    void NewMessageNotify(ECommandStatus, std::string&& string = "") override;

    BstTcpSocket& GetSocket();
    bool IsActive() const;
    void Deactivate();
    boost::asio::awaitable<void> HandleRequest(std::string&& );

private:

    
    BstTcpSocket socket_;    
    Parser& common_parser_;
    Database& common_database_;
    IMessageSender* const message_sender_;
    bool active_;
};

