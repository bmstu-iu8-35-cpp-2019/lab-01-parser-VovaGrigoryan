// Copyright 2018 Your Name <your_email>

#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    std::string str =
            R"([["Si-9.15","RTS-9.15","GAZP-9.15"],[100024, 100027, 100050],["Futures contract for USD/RUB", "Futures contract for index RTS", "Futures contract for Gazprom shares"]])";
    auto input_json = json::parse(str);
    json result_json;
    std::cout << input_json.dump(3) << std::endl;

    if (!(input_json.is_array())) {
        std::cout << "Wrong syntax: input_json is not json-array" << std::endl;
        return 1;
    }
    if (input_json.size() != 3) {
        std::cout << "Wrong syntax: wrong number of arrays" << std::endl;
        return 1;
    }
    if (input_json[0].size() != input_json[1].size() ||
        input_json[0].size() != input_json[2].size())  // Size-check
    {
        std::cout << "Wrong syntax: "
                     "inequality of the number of elements in arrays"
                  << std::endl;
        return 1;
    }

    json temp_json;
    for (size_t i = 0; i < input_json[0].size(); i++) {
        temp_json["ticker"] = input_json[0][i];
        temp_json["id"] = input_json[1][i];
        temp_json["description"] = input_json[2][i];
        result_json.push_back(temp_json);
    }

    std::cout << result_json.dump(1) << std::endl;
}
