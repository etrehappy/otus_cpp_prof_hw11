#pragma once
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp> 
#include <boost/asio/experimental/awaitable_operators.hpp>

#include "settings.h"

using BstTcpAcceptor = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::acceptor>;
using BstTcpSocket = boost::asio::use_awaitable_t<>::as_default_on_t<boost::asio::ip::tcp::socket>;


/*!
* @brief: Observer
*/
class IMessageSender
{
public:
    virtual ~IMessageSender() {};
    virtual boost::asio::awaitable<void> SendMessageToClient(BstTcpSocket&, ECommandStatus, std::string&& s = "") = 0;
};

/*!
* @brief: Subject
*/
class IMessageCreater
{
public:
    virtual ~IMessageCreater() {};
    virtual void NewMessageNotify(ECommandStatus, std::string&& s = "") = 0;

    //virtual void Attach(IMessageSender* observer) = 0;
    //virtual void Detach(IMessageSender* observer) = 0;
};

