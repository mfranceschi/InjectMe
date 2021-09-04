#include <cstdint>
#include <ostream>

#include "../IntegrationTestHelpers.hpp"
#include "InjectMe.hpp"

using namespace mf::InjectMe;

struct Client {
  std::uint16_t id;
};

std::ostream& operator<<(std::ostream& oStream, const Client& client) {
  return oStream << "Client{id=" << client.id << "}";
}

class ClientDAO {
 public:
  virtual Client getById(std::uint16_t clientId) = 0;

  ClientDAO() = default;
  NO_COPY(ClientDAO);
  virtual ~ClientDAO() = default;
};

class ClientDAOImpl : public ClientDAO {
 public:
  ClientDAOImpl() = default;
  Client getById(std::uint16_t clientId) override {
    return Client{clientId};
  }
};

class Controller {
 public:
  Client getClientById(std::uint16_t clientId) const {
    return clientDao->getById(clientId);
  }

  Controller() = default;
  NO_COPY(Controller);
  virtual ~Controller() = default;

 private:
  Injected<ClientDAO> clientDao{};
};

class Api {
 public:
  std::string getClient(std::uint16_t clientId, std::uint32_t tokenId) {
    if (tokenId != acceptedToken) {
      return "";
    }

    std::ostringstream oss;
    oss << controller->getClientById(clientId) << std::endl;
    return oss.str();
  }

  Api() = delete;
  Api(std::uint32_t acceptedToken) : acceptedToken(acceptedToken) {
  }
  NO_COPY(Api);
  virtual ~Api() = default;

 private:
  Injected<Controller> controller{};
  const std::uint32_t acceptedToken;
};

void configureInjectors() {
  configure<ClientDAO>()
      .setProvider([]() {
        return new ClientDAOImpl;
      })
      .done();
  configure<Controller>().done();
  configure<Api>()
      .setProvider([]() {
        return new Api(1);
      })
      .done();
}

int main() {
  configureInjectors();
  return 0;
}
