#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using String = std::string;

template <typename T>
using Vector = std::vector<T>;

enum TokenType {
    KEY,
    INT,
    BOOL,
    LEFT_PAREN,
    RIGHT_PAREN,
    D_QUOTE,
    COMMA,
    COLON,
    INVALID,
    STRING,
    NULL_T,
    UNDERSCORE
};

struct Tokens {
    TokenType type;
    std::string value;

    friend std::ostream& operator<<(std::ostream& Cout, const Tokens& tokens)
    {
        Cout << "Token type: " << tokens.type << " Value is: " << tokens.value << '\n';
        return Cout;
    }
};

// TODO:Implement lexer class
class Lexer {
public:
    int next_pos;
    String str;

public:
    Lexer(const String& str)
        : str(str)
        , next_pos(0)
    {
    }
    Vector<Tokens> tokenize();
};

Vector<Tokens> Lexer::tokenize()
{
    Vector<Tokens> tokens;
    Tokens tok;
    String buf;
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isspace(str.at(i))) {
            continue;
        }
        if (str.at(i) == '{') {
            tokens.push_back({ tok.type = TokenType::LEFT_PAREN, tok.value = '{' });
        } else if (str.at(i) == '}') {
            tokens.push_back({ tok.type = TokenType::RIGHT_PAREN, tok.value = '}' });
        } else if (str.at(i) == ',') {
            tokens.push_back({ tok.type = TokenType::COMMA, tok.value = ',' });
        } else if (str.at(i) == '_') {
            tokens.push_back({ tok.type = TokenType::UNDERSCORE, tok.value = '_' });
        } else if (str.at(i) == '"') {

            // TODO:Work on string values.
            buf.push_back(str.at(i));
            tokens.push_back({ tok.type = TokenType::D_QUOTE, tok.value = buf });
            buf.clear();
            if (std::isalpha(str.at(i + 1))) {
                while (i + 1 < str.length() && std::isalpha(str.at(i + 1))) {
                    buf.push_back(str.at(i + 1));
                    i++;
                }
                tokens.push_back({ tok.type = TokenType::STRING, tok.value = buf });
                buf.clear();
            }
        } else if (std::isalpha(str.at(i))) {
            buf.push_back(str.at(i));
            while (i + 1 < str.length() && std::isalpha(str.at(i + 1))) {
                buf.push_back(str.at(i + 1));
                i++;
            }
            if (buf == "true" || buf == "false") {
                tokens.push_back({ tok.type = TokenType::BOOL, tok.value = buf });
            } else if (buf == "null") {
                tokens.push_back({ tok.type = TokenType::NULL_T, tok.value = buf });
            } else {
                if (str.at(i + 2) == ':') {
                    tokens.push_back({ tok.type = TokenType::KEY, tok.value = buf });
                }
            }
            buf.clear();
        } else if (str.at(i) == ':') {
            buf.push_back(str.at(i));
            tokens.push_back({ tok.type = TokenType::COLON, tok.value = ':' });
            buf.clear();
        } else if (std::isdigit(str.at(i))) {
            buf.push_back(str.at(i));
            while (i + 1 < str.length() && std::isdigit(str.at(i + 1))) {
                buf.push_back(str.at(i + 1));
                i++;
            }
            tokens.push_back({ tok.type = TokenType::INT, tok.value = buf });

            buf.clear();

        } else {
            buf.push_back(str.at(i));

            tokens.push_back({ tok.type = TokenType::INVALID, tok.value = buf });
            buf.clear();
        }
    }
    return tokens;
}

String file_str(std::ifstream& file)
{
    std::stringstream file_content;
    file_content << file.rdbuf();
    return file_content.str();
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "You must provide a json file!\n";
        return 1;
    }

    String file_name = argv[1];

    std::ifstream file;
    file.open(file_name, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Fail to open file\n";
        return 1;
    }

    String file_content = file_str(file);
    file.close();

    Lexer lex { file_content };
    Vector<Tokens> tokens = lex.tokenize();

    for (auto c : tokens) {
        std::cout << c << '\n';
    }

    return 0;
}
