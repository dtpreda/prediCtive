#include <crow.h>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/grammar")
    .methods("POST"_method)
    ([](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json)
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        if (!json.has("grammar")) {
            crow::json::wvalue response({{"error", "Missing \'grammar\' field."}});
            return crow::response(crow::status::BAD_REQUEST, response);
        }
        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/parse-content")
    .methods("POST"_method)
    ([](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json)
            return crow::response(crow::status::BAD_REQUEST); // same as crow::response(400)
        if (!json.has("content")) {
            crow::json::wvalue response({{"error", "Missing \'content\' field."}});
            return crow::response(crow::status::BAD_REQUEST, response);
        }
        return crow::response(crow::status::OK);
    });

    app.port(18080).multithreaded().run();
}
