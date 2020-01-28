// Copyright 2018 Your Name <your_email>

#include <json.hpp>

void Json::check_str(const std::string& s) {
    int f = s.find_first_of("{["), l = s.find_last_of("}]");
    if ((f == -1) || (l == -1)) throw std::exception();
    if (s[f] == '{') {
        if (s[l] != '}') throw std::exception();
    } else if (s[l] != ']') {
        throw std::exception();
    }
}

void Json::check_whitespace(const std::string& s) {
    if (static_cast<int>(s.find_first_not_of(" \n\t")) != -1)
        throw std::exception();
}

int Json::get_int(const std::string& s) { return std::stoi(s); }

double Json::get_double(const std::string& s) { return std::stod(s); }

bool Json::get_bool(const std::string& s, int& b) {
    if (s.substr(0, 4) == "true") {
        b += 3;
        return true;
    } else if (s.substr(0, 5) == "false") {
        b += 4;
        return false;
    } else{
        throw std::exception();
    }
}

int Json::find_end(const std::string& s, char close) {
    int x = 0, count_open = 1, count_close = 0;
    char open;
    if (close == '}')
        open = '{';
    else
        open = '[';
    std::string str;
    str.push_back(open);
    str.push_back(close);
    while (count_open != count_close) {
        x++;
        if (static_cast<int>(s.substr(x).find_first_of(str)) != -1) {
            x = static_cast<int>(s.substr(x).find_first_of(str) + x);
            if (s[x] == open)
                count_open++;
            else
                count_close++;
        } else {
            throw std::exception();
        }
    }
    return x;
}

void Json::parse_object(const std::string& s, int& a, int& b) {
    std::unordered_multimap<std::string, std::any> map;
    while (true) {
        std::string key = "";
        std::any value = nullptr;
        a = b + 1;
        b = static_cast<int>(s.substr(a).find('\"') + a);
        if (static_cast<int>(s.substr(a).find('\"')) == -1) {
            if (map.empty()) {
                b = s.substr(a).find('}') + a;
                check_whitespace(s.substr(a, b - a));
                break;
            } else {
                throw std::exception();
            }
        }
        check_whitespace(s.substr(a, b - a));
        a = b + 1;
        b = static_cast<int>(s.substr(a).find('\"') + a);
        //        if ((int)s.substr(a).find('\"') == -1)
        //            throw std::exception();
        key = s.substr(a, b - a);
        a = b + 1;
        b = static_cast<int>(s.substr(a).find(':') + a);
        if (static_cast<int>(s.substr(a).find(':')) == -1) throw std::exception();
        check_whitespace(s.substr(a, b - a));
        a = b + 1;
        b = static_cast<int>(s.substr(a).find_first_of("{[\"0123456789tfn-") + a);
        //        if ((int)s.substr(a).find_first_of("{[\"0123456789tfn-") == -1)
        //            throw std::exception();
        check_whitespace(s.substr(a, b - a));
        switch (s[b]) {
            case '{': {
                a = b;
                b = find_end(s.substr(a, s.find_last_of('}') - a), '}') + b;
                value = Json(s.substr(a, b - a + 1));
                break;
            }
            case '[': {
                a = b;
                b = find_end(s.substr(a, s.find_last_of('}') - a), ']') + b;
                value = Json(s.substr(a, b - a + 1));
                break;
            }
            case '\"': {
                a = b + 1;
                b = static_cast<int>(s.substr(a).find('\"') + a);
                if (static_cast<int>(s.substr(a).find('\"')) == -1)
                    throw std::exception();
                value = s.substr(a, b - a);
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-': {
                a = b;
                b = static_cast<int>(s.substr(a).find_first_of(" \t\n,}]") + a);
                //                if ((int)s.substr(a).find_first_of(" \t\n,}]") == -1)
                //                    throw std::exception();
                b--;
                if (static_cast<int>(
                            s.substr(a + 1, b - a).find_first_not_of("0123456789")) == -1) {
                    value = get_int(s.substr(a, b - a + 1));
                } else {
                    if (static_cast<int>(s.substr(a + 1, b - a)
                            .find_first_not_of("0123456789.eE+-")) != -1)
                        throw std::exception();
                    if (static_cast<int>(s.substr(a + 1, b - a).find('.')) != -1) {
                        int c = static_cast<int>(s.substr(a + 1, b - a).find('.') + a + 1);
                        //                        if ((int)s.substr(c + 1, b - c).find('.')
                        //                        != -1)
                        //                            throw std::exception();
                        if (static_cast<int>(
                                    s.substr(c - 1, 1).find_first_not_of("0123456789")) != -1 ||
                            static_cast<int>(
                                    s.substr(c + 1, 1).find_first_not_of("0123456789")) != -1)
                            throw std::exception();
                    }
                    if (static_cast<int>(s.substr(a + 1, b - a).find_first_of("+-")) !=
                        -1) {
                        int c = static_cast<int>(
                                s.substr(a + 1, b - a).find_first_of("+-") + a + 1);
                        if (!(s[c - 1] == 'e' || s[c - 1] == 'E')) throw std::exception();
                    }
                    if (static_cast<int>(s.substr(a + 1, b - a).find_first_of("eE")) !=
                        -1) {
                        int c = static_cast<int>(
                                s.substr(a + 1, b - a).find_first_of("eE") + a + 1);
                        if (static_cast<int>(s.substr(c + 1, b - c).find_first_of("eE")) !=
                            -1)
                            throw std::exception();
                    }
                    value = get_double(s.substr(a, b - a + 1));
                }
                break;
            }
            case 't':
            case 'f': {
                a = b;
                value = get_bool(s.substr(a), b);
                break;
            }
            case 'n': {
                a = b;
                if (s.substr(a, 4) == "null") {
                    b += 3;
                    value = nullptr;
                } else {
                    throw std::exception();
                }
            }
        }
        map.insert({key, value});
        a = b + 1;
        b = static_cast<int>(s.substr(a).find_first_of(",}") + a);
        if (static_cast<int>(s.substr(a).find_first_of(",}")) == -1)
            throw std::exception();
        if (s[b] == '}') {
            check_whitespace(s.substr(a, b - a));
            break;
        } else {
            check_whitespace(s.substr(a, b - a));
        }
    }
    if (!map.empty()) json_map = map;
}

void Json::parse_array(const std::string& s, int& a, int& b) {
    std::vector<std::any> vector;
    while (true) {
        std::any value = nullptr;
        a = b + 1;
        b = static_cast<int>(s.substr(a).find_first_of("{[\"0123456789tfn-") + a);
        if (static_cast<int>(s.substr(a).find_first_of("{[\"0123456789tfn-")) ==
            -1) {
            if (vector.empty()) {
                b = s.substr(a).find(']') + a;
                check_whitespace(s.substr(a, b - a));
                break;
            } else {
                throw std::exception();
            }
        }
        check_whitespace(s.substr(a, b - a));
        switch (s[b]) {
            case '{': {
                a = b;
                b = find_end(s.substr(a, s.find_last_of(']') - a), '}') + b;
                value = Json(s.substr(a, b - a + 1));
                break;
            }
            case '[': {
                a = b;
                b = find_end(s.substr(a, s.find_last_of(']') - a), ']') + b;
                value = Json(s.substr(a, b - a + 1));
                break;
            }
            case '\"': {
                a = b + 1;
                b = static_cast<int>(s.substr(a).find('\"') + a);
                if (static_cast<int>(s.substr(a).find('\"')) == -1)
                    throw std::exception();
                value = s.substr(a, b - a);
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-': {
                a = b;
                b = static_cast<int>(s.substr(a).find_first_of(" \t\n,}]") + a);
                //                if ((int)s.substr(a).find_first_of(" \t\n,}]") == -1)
                //                    throw std::exception();
                b--;
                if (static_cast<int>(
                            s.substr(a + 1, b - a).find_first_not_of("0123456789")) == -1) {
                    value = get_int(s.substr(a, b - a + 1));
                } else {
                    if (static_cast<int>(s.substr(a + 1, b - a)
                            .find_first_not_of("0123456789.eE+-")) != -1)
                        throw std::exception();
                    if (static_cast<int>(s.substr(a + 1, b - a).find('.')) != -1) {
                        int c = static_cast<int>(s.substr(a + 1, b - a).find('.') + a + 1);
                        if (static_cast<int>(s.substr(c + 1, b - c).find('.')) != -1)
                            throw std::exception();
                        if (static_cast<int>(
                                    s.substr(c - 1, 1).find_first_not_of("0123456789")) != -1 ||
                            static_cast<int>(
                                    s.substr(c + 1, 1).find_first_not_of("0123456789")) != -1)
                            throw std::exception();
                    }
                    if (static_cast<int>(s.substr(a + 1, b - a).find_first_of("+-")) !=
                        -1) {
                        int c = static_cast<int>(
                                s.substr(a + 1, b - a).find_first_of("+-") + a + 1);
                        if (!(s[c - 1] == 'e' || s[c - 1] == 'E')) throw std::exception();
                    }
                    if (static_cast<int>(s.substr(a + 1, b - a).find_first_of("eE")) !=
                        -1) {
                        int c = static_cast<int>(
                                s.substr(a + 1, b - a).find_first_of("eE") + a + 1);
                        if (static_cast<int>(s.substr(c + 1, b - c).find_first_of("eE")) !=
                            -1)
                            throw std::exception();
                    }
                    value = get_double(s.substr(a, b - a + 1));
                }
                break;
            }
            case 't':
            case 'f': {
                a = b;
                value = get_bool(s.substr(a), b);
                break;
            }
            case 'n': {
                a = b;
                if (s.substr(a, 4) == "null") {
                    b += 3;
                    value = nullptr;
                } else {
                    throw std::exception();
                }
            }
        }
        vector.push_back(value);
        a = b + 1;
        b = static_cast<int>(s.substr(a).find_first_of(",]") + a);
        //        if ((int)s.substr(a).find_first_of(",]") == -1)
        //            throw std::exception();
        if (s[b] == ']') {
            check_whitespace(s.substr(a, b - a));
            break;
        } else {
            check_whitespace(s.substr(a, b - a));
        }
    }
    if (!vector.empty()) json_map = vector;
}

Json::Json(const std::string& s) {
    check_str(s);
    int a = 0, b = 0;
    a = b = s.find_first_of("{[");
    if (s[a] == '{')
        parse_object(s.substr(a), a, b);
    else
        parse_array(s.substr(a), a, b);
}

bool Json::is_array() const {
    if (json_map.type() == typeid(std::vector<std::any>))
        return true;
    else
        return false;
}

bool Json::is_object() const {
    if (json_map.type() == typeid(std::unordered_multimap<std::string, std::any>))
        return true;
    else
        return false;
}

std::any Json::operator[](const std::string& key) {
    if (this->is_object()) {
        auto object =
                     std::any_cast<std::unordered_multimap<std::string, std::any>>(json_map);
        std::any value = object.find(key)->second;
        return value;
    } else {
        throw std::exception();
    }
}

std::any Json::operator[](int index) {
    if (this->is_array()) {
        auto array = std::any_cast<std::vector<std::any>>(json_map);
        return array[index];
    } else {
        throw std::exception();
    }
}

Json Json::parse(const std::string& s) {
    Json A(s);
    return A;
}