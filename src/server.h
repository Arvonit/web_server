#include <asio.hpp>

class user
{
  private:
    asio::ip::tcp::socket socket;

  public:
    user(asio::ip::tcp::socket socket) : socket(std::move(socket))
    {
    }
};
