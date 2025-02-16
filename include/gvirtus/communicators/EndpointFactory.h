#pragma once

#include <gvirtus/common/JSON.h>
#include <memory>
#include <nlohmann/json.hpp>
#include "Endpoint.h"
#include "Endpoint_Rdma.h"
#include "Endpoint_Tcp.h"

/**
 * 代码全在h文件中，EndpointFactory.cpp 文件中只有
 * #include "gvirtus/communicators/EndpointFactory.h"
 * int gvirtus::communicators::EndpointFactory::ind_endpoint = 0;
 *  */ 
//#define DEBUG

namespace gvirtus::communicators {
class EndpointFactory {
 public:
/**
 * @brief Factory method to create an Endpoint object based on the configuration provided in a JSON file.
 *
 * This method reads a JSON configuration file to determine the type of endpoint to create.
 * It supports creating TCP/IP and Infiniband RDMA endpoints.
 *
 * @param json_path The file path to the JSON configuration file.
 * @return A shared pointer to the created Endpoint object.
 * @throws std::runtime_error if the endpoint suite specified in the JSON file is not supported.
 */
  static std::shared_ptr<Endpoint> get_endpoint(const fs::path &json_path) {
#ifdef DEBUG
      std::cout << "EndpointFactory::get_endpoint() called" << std::endl;
#endif

    std::shared_ptr<Endpoint> ptr;
    std::ifstream ifs(json_path);
    nlohmann::json j;
    ifs >> j;

    // FIXME: This if-else smells...
    // tcp/ip
    if ("tcp/ip" == j["communicator"][ind_endpoint]["endpoint"].at("suite")) {
#ifdef DEBUG
        std::cout << "EndpointFactory::get_endpoint() found tcp/ip endpoint" << std::endl;
#endif
        auto end = common::JSON<Endpoint_Tcp>(json_path).parser();
        ptr = std::make_shared<Endpoint_Tcp>(end);
    }
    // infiniband
    else if ("infiniband-rdma" == j["communicator"][ind_endpoint]["endpoint"].at("suite")) {
#ifdef DEBUG
        std::cout << "EndpointFactory::get_endpoint() found infiniband endpoint" << std::endl;
#endif
        auto end = common::JSON<Endpoint_Rdma>(json_path).parser();
        ptr = std::make_shared<Endpoint_Rdma>(end);
    }
    else {
        throw "EndpointFactory::get_endpoint(): Your suite is not compatible!";
    }

    ind_endpoint++;

    j.clear();
    ifs.close();

#ifdef DEBUG
    std::cout << "EndpointFactoru::get_endpoint(): end is: " << ptr->to_string() << std::endl;
    std::cout << "EndpointFactory::get_endpoint() ended" << std::endl;
#endif

    return ptr;
  }

  static int index() { return ind_endpoint; }

 private:
  static int ind_endpoint;
};
}  // namespace gvirtus::communicators
