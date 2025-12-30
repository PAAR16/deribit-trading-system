#include "DeribitWsClient.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "../infra/Logger.hpp"

DeribitWsClient::DeribitWsClient()
    : request_id_(1),
      connected_(false),
      authenticated_(false) {
    setupClient();
}

DeribitWsClient::~DeribitWsClient() {
    disconnect();
}

void DeribitWsClient::connect(bool use_testnet) {
    std::string url = use_testnet ?
        "wss://test.deribit.com/ws/api/v2" :
        "wss://www.deribit.com/ws/api/v2";

    websocketpp::lib::error_code ec;
    connection_ = client_.get_connection(url, ec);

    if (ec) {
        throw std::runtime_error("Connection error: " + ec.message());
    }

    client_.connect(connection_);

    ws_thread_ = std::thread([this]() {
        try {
            client_.run();
        }
        catch (const std::exception& e) {
            Logger::log(Logger::Level::ERROR, "WebSocket error: " + std::string(e.what()));
        }
        });

    waitForConnection();
    Logger::log(Logger::Level::INFO, "Connected to Deribit");
}

void DeribitWsClient::disconnect() {
    if (connection_) {
        connection_->close(websocketpp::close::status::normal, "Closing connection");
    }
    if (ws_thread_.joinable()) {
        ws_thread_.join();
    }
    connected_ = false;
    authenticated_ = false;
}

void DeribitWsClient::authenticate(const std::string& client_id, const std::string& client_secret) {
    json auth_params = {
        {"grant_type", "client_credentials"},
        {"client_id", client_id},
        {"client_secret", client_secret}
    };

    sendRequest("public/auth", auth_params);
    waitForAuthentication();
}

void DeribitWsClient::getTime() {
    sendRequest("public/get_time", json::object());
}

void DeribitWsClient::getInstruments(const std::string& currency, const std::string& kind) {
    json params = {
        {"currency", currency},
        {"kind", kind}
    };
    sendRequest("public/get_instruments", params);
}

void DeribitWsClient::getCurrencies() {
    sendRequest("public/get_currencies", json::object());
}

void DeribitWsClient::getOrderbook(const std::string& instrument_name, int depth) {
    json params = {
        {"instrument_name", instrument_name},
        {"depth", depth}
    };
    sendRequest("public/get_order_book", params);
}

void DeribitWsClient::getTradingviewChartData(const std::string& instrument_name,
                                             const std::string& start_timestamp,
                                             const std::string& end_timestamp,
                                             const std::string& resolution) {
    json params = {
        {"instrument_name", instrument_name},
        {"start_timestamp", start_timestamp},
        {"end_timestamp", end_timestamp},
        {"resolution", resolution}
    };
    sendRequest("public/get_tradingview_chart_data", params);
}

void DeribitWsClient::getAccountSummary(const std::string& currency) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params = {{"currency", currency}};
    sendPrivateRequest("private/get_account_summary", params);
}

void DeribitWsClient::getPositions(const std::string& currency) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params = {{"currency", currency}};
    sendPrivateRequest("private/get_positions", params);
}

void DeribitWsClient::placeOrder(const Order& order) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params = {
        {"instrument_name", order.instrument},
        {"amount", order.amount}
    };

    if (order.type == OrderType::MARKET) {
        params["type"] = "market";
    } else {
        params["type"] = "limit";
        params["price"] = order.price;
        params["time_in_force"] = "good_til_cancelled";
        params["post_only"] = false;
        params["reduce_only"] = false;
    }

    std::string method = (order.side == OrderSide::BUY) ? "private/buy" : "private/sell";
    sendPrivateRequest(method, params);
}

void DeribitWsClient::cancelOrder(const std::string& order_id) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params = {{"order_id", order_id}};
    sendPrivateRequest("private/cancel", params);
}

void DeribitWsClient::cancelAllOrders() {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    sendPrivateRequest("private/cancel_all", json::object());
}

void DeribitWsClient::modifyOrder(const std::string& order_id, double amount, double price) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params = {
        {"order_id", order_id},
        {"amount", amount},
        {"price", price}
    };
    sendPrivateRequest("private/edit", params);
}

void DeribitWsClient::getOpenOrders(const std::string& instrument_name) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params;
    if (!instrument_name.empty()) {
        params["instrument_name"] = instrument_name;
    }
    sendPrivateRequest("private/get_open_orders_by_instrument", params);
}

void DeribitWsClient::getOrderHistory(const std::string& instrument_name) {
    if (!authenticated_) throw std::runtime_error("Not authenticated");
    json params;
    if (!instrument_name.empty()) {
        params["instrument_name"] = instrument_name;
    }
    sendPrivateRequest("private/get_order_history_by_instrument", params);
}

void DeribitWsClient::subscribeToOrderbook(const std::string& instrument_name) {
    json params = {
        {"channels", { "book." + instrument_name + ".100ms" }}
    };
    sendRequest("public/subscribe", params);
    std::lock_guard<std::mutex> lock(subscription_mutex_);
    active_subscriptions_["book." + instrument_name + ".100ms"] = true;
}

void DeribitWsClient::subscribeToTrades(const std::string& instrument_name) {
    std::string channel = "trades." + instrument_name + ".100ms";
    json params = {{"channels", {channel}}};
    sendRequest("public/subscribe", params);
    std::lock_guard<std::mutex> lock(subscription_mutex_);
    active_subscriptions_[channel] = true;
}

void DeribitWsClient::subscribeToTicker(const std::string& instrument_name) {
    std::string channel = "ticker." + instrument_name + ".100ms";
    json params = {{"channels", {channel}}};
    sendRequest("public/subscribe", params);
    std::lock_guard<std::mutex> lock(subscription_mutex_);
    active_subscriptions_[channel] = true;
}

void DeribitWsClient::unsubscribe(const std::string& channel) {
    json params = {{"channels", {channel}}};
    sendRequest("public/unsubscribe", params);
    std::lock_guard<std::mutex> lock(subscription_mutex_);
    active_subscriptions_[channel] = false;
}

void DeribitWsClient::setResponseHandler(std::function<void(const std::string&)> handler) {
    response_handler_ = handler;
}

void DeribitWsClient::setSubscriptionHandler(std::function<void(const std::string& channel, const std::string& data)> handler) {
    subscription_handler_ = handler;
}

void DeribitWsClient::setupClient() {
    client_.clear_access_channels(websocketpp::log::alevel::all);
    client_.set_access_channels(websocketpp::log::alevel::connect);
    client_.set_access_channels(websocketpp::log::alevel::disconnect);

    client_.init_asio();

    client_.set_tls_init_handler([](websocketpp::connection_hdl) {
        return websocketpp::lib::make_shared<boost::asio::ssl::context>(
            boost::asio::ssl::context::tlsv12);
        });

    client_.set_message_handler([this](websocketpp::connection_hdl hdl,
        Client::message_ptr msg) {
            handleMessage(msg->get_payload());
        });

    client_.set_open_handler([this](websocketpp::connection_hdl hdl) {
        std::lock_guard<std::mutex> lock(mutex_);
        connected_ = true;
        cv_.notify_all();
        });
}

void DeribitWsClient::sendRequest(const std::string& method, const json& params) {
    json request = {
        {"jsonrpc", "2.0"},
        {"id", request_id_++},
        {"method", method},
        {"params", params}
    };

    try {
        client_.send(connection_, request.dump(), websocketpp::frame::opcode::text);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Failed to send request: " + std::string(e.what()));
    }
}

void DeribitWsClient::sendPrivateRequest(const std::string& method, const json& params) {
    json request_params = params;
    request_params["access_token"] = access_token_;
    sendRequest(method, request_params);
}

void DeribitWsClient::handleMessage(const std::string& message) {
    try {
        json response = json::parse(message);

        // Handle authentication response
        if (response.contains("result") &&
            response["result"].contains("access_token")) {
            std::lock_guard<std::mutex> lock(mutex_);
            access_token_ = response["result"]["access_token"];
            authenticated_ = true;
            cv_.notify_all();
            Logger::log(Logger::Level::INFO, "Authentication successful");
            return;
        }

        // Handle subscription messages
        if (response.contains("method") && response["method"] == "subscription") {
            std::string channel = response["params"]["channel"];
            if (subscription_handler_) {
                subscription_handler_(channel, response["params"]["data"].dump());
            }
            return;
        }

        // Handle regular responses
        if (response_handler_) {
            response_handler_(message);
        }
    }
    catch (const std::exception& e) {
        Logger::log(Logger::Level::ERROR, "Error parsing message: " + std::string(e.what()));
    }
}

void DeribitWsClient::waitForConnection() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return connected_; });
}

void DeribitWsClient::waitForAuthentication() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return authenticated_; });
}
