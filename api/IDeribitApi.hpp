#ifndef IDERIBIT_API_HPP
#define IDERIBIT_API_HPP

#include <string>
#include <functional>

class IDeribitApi {
public:
    virtual ~IDeribitApi() = default;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void subscribe(const std::string& channel) = 0;
    virtual void unsubscribe(const std::string& channel) = 0;
    virtual void sendRequest(const std::string& request, std::function<void(const std::string&)> callback) = 0;
};

#endif // IDERIBIT_API_HPP