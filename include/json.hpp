// Copyright 2018 Your Name <your_email>

// Copyright 2019  Dimontich

#ifndef INCLUDE_JSON_HPP
#define INCLUDE_JSON_HPP

#include <iostream>
#include <fstream>
#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <exception>

class Json
{
private:
    std::any json_map;
    static void check_str(const std::string& s);
    static void check_whitespace(const std::string& s);
    static int get_int(const std::string& s);
    static double get_double(const std::string& s);
    static bool get_bool(const std::string& s, int& b);
    static int find_end(const std::string& s, char close);
    void parse_object(const std::string& s, int& a, int& b);
    void parse_array(const std::string& s, int& a, int& b);
public:
    Json(const std::string& s);
    bool is_array() const;
    bool is_object() const;
    std::any operator[](const std::string& key);
    std::any operator[](int index);
    static Json parse(const std::string& s);
    static Json parseFile(const std::string& path_to_file);
};


#endif //INCLUDE_JSON_HPP