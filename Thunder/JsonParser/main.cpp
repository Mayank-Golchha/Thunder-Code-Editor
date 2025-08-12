// #include <iostream>

// #include "tokens.h"
// #include "parser.h"

// int main(){
//     // Token a = "hello world";
//     // std::unordered_map<std::string,JsonNode> D;
//     // JsonNode json(D);
//     // json["name"] = "Mayank Golchha";
//     // JsonNode n = std::get<std::unordered_map<std::string,JsonNode>>(json.data)["name"];
//     // std::cout << std::get<std::string>(n.data);
//     // std::string code = "{\"data\":\"Hello World\"}";
    
    
//     // std::string code = "{\"data\":12,\"data2\":3}";
//     // JsonNode json = *Json(code).parse();
//     // JsonNode n = std::get<std::unordered_map<std::string,JsonNode>>(json.data)["data2"];
//     // std::cout << std::get<int>(n.data);
//     // std::cout << std::get<int>(json["data"].data);
//     // std::cout << int(json["data"]);

//     // std::string code = "{\"details\":{\"name\":\"Mayank Golchha\",\"age\":true},\"a\":[0,1]}";
//     std::string code = "{\"a\":[0,1]}";
//     JsonNode json = *Json(code).parse();
//     // JsonNode details = std::unordered_map<std::string,JsonNode>(json)["details"];
//     // JsonNode name = std::unordered_map<std::string,JsonNode>(details)["name"];
//     // JsonNode age = std::unordered_map<std::string,JsonNode>(details)["age"];
//     // std::cout << std::string(name);
//     // std::cout << bool(age);
//     // std::cout << std::string(json["details"]["name"]);
//     std::cout << int(json["a"][0]);
// }






#include <string>
#include <fstream>

#include "parser.h"
#include <iostream>

// int main() {
//     std::string json_text = R"({
//         "name": "John",
//         "age": 25,
//         "skills": ["C++", "Python", "JSON"],
//         "isStudent": true
//     })";

//     JsonNode result = *Json(json_text).parse();

//     std::cout << "Name: " << std::string(result["name"]) << "\n";
//     std::cout << "Age: " << int(result["age"]) << "\n";
//     std::cout << "skill 1: " << std::string(result["skills"][0]) << "\n";
//     std::cout << "Is student: " << (bool(result["isStudent"]) ? "Yes" : "No") << "\n";

//     return 0;
// }




// #include "JsonParser/parser.h"

#include "dumper.h"

void loadTheme(){
    std::string code,line;
    std::ifstream theme("../.thunder/ui/ui_colors.json");
    while (std::getline (theme, line)) {
        // Output the text from the file
        code += line + "\n";
    }

    Json json(code);
    JsonNode node =  *json.parse();
    std::cout << std::string(node["mainWindow"]);
    node["mainWindow"] = "#123";
    std::cout << std::string(node["mainWindow"]);
    Dumper d(&node);
    d.dump("temp.txt");
    json.freeMemory();
    std::cout << "done";
}


int main(){
    loadTheme();
}
