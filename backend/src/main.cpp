#include <crow.h>
#include "parser/prediCtiveParser.h"
#include "parser/Parser.h"

int main() {
    crow::SimpleApp app;

    Parser prediCtiveParser = buildPrediCtiveParser();
    std::optional<Parser> newGrammarParser = std::nullopt;

    CROW_ROUTE(app, "/")([](){
        return "Hello world";
    });

    CROW_ROUTE(app, "/grammar")
    .methods("POST"_method)
    ([prediCtiveParser, &newGrammarParser](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json) {
            crow::json::wvalue error({{"error", "Unable to load JSON body."}});
            return crow::response(crow::status::BAD_REQUEST, error);
        }
        if (!json.has("grammar")) {
            crow::json::wvalue error({{"error", "Missing \'grammar\' field."}});
            return crow::response(crow::status::BAD_REQUEST, error);
        }

        try {
            std::string grammar = json["grammar"].s();
            newGrammarParser = buildParser(prediCtiveParser, grammar);
            return crow::response(crow::status::OK);
        } catch (const std::exception& e) {
            crow::json::wvalue error({{"error", e.what()}});
            return crow::response(crow::status::INTERNAL_SERVER_ERROR, error);
        }
    });

    CROW_ROUTE(app, "/parse-content")
    .methods("POST"_method)
    ([&newGrammarParser](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json) {
            crow::json::wvalue error({{"error", "Unable to load JSON body."}});
            return crow::response(crow::status::BAD_REQUEST, error);
        }
        if (!json.has("content")) {
            crow::json::wvalue error({{"error", "Missing \'content\' field."}});
            return crow::response(crow::status::BAD_REQUEST, error);
        }

        if (!newGrammarParser.has_value()) {
            crow::json::wvalue error({{"error", "No grammar defined. Please define a grammar."}});
            return crow::response(crow::status::BAD_REQUEST, error);
        } else {
            try {
                std::string content = json["content"].s();
                std::string result = parseResult(newGrammarParser.value(), content);
                crow::json::wvalue response({{"parseTree", result}});
                return crow::response(crow::status::OK, response);
            } catch (const std::exception& e) {
                crow::json::wvalue error({{"error", e.what()}});
                return crow::response(crow::status::INTERNAL_SERVER_ERROR, error);
            }
        }
    });

    app.port(18080).multithreaded().run();
}
