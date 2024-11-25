#include "main.h"
#include "settings.h"

uint16_t SimpleFuncToGetPort(int argc, char* argv[])
{
    uint32_t port{};

    if (argc < 2)
    {
        return 0;
    }

    port = static_cast<uint32_t>(std::stoi(argv[1]));

    if (port < 1024 || port > 65535) 
    {
        throw std::out_of_range("Invalid port number. Port number out of range (1024-65535).");
    }

    return static_cast<uint16_t>(port);
    
}


int main(int argc, char* argv[])
{
#ifdef _WIN32    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::locale::global(std::locale("ru_RU.UTF-8"));
#else
    setlocale(LC_ALL, "ru_RU.UTF-8");
#endif


    try 
    {

        const uint16_t port = SimpleFuncToGetPort(argc, argv);

        boost::asio::io_context io_context{};

        Database database{};
        database.CreateTable("A");
        database.CreateTable("B");

        Parser parser{};

        NetworkHandler net_handler{};

        boost::asio::co_spawn(io_context, net_handler.ListenConnect(port, parser, database), boost::asio::detached);

        io_context.run();

    } catch (const std::exception& e)
    {
        logger::ErrorException(e.what(), __func__);
        return 1;
    }

    return 0;
}