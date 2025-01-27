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
    INVALID
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

class Lexer {
public:
    Tokens tok;
    int next_pos;
    std::string content;
};

std::vector<Tokens> tokenize(const std::string& str)
{
    std::vector<Tokens> tokens;
    Tokens tok;
    std::string buf;
    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isspace(str.at(i))) {
            continue;
        }
        if (str.at(i) == '{') {
            buf.push_back(c);
            tokens.push_back({ tok.type = TokenType::LEFT_PAREN, tok.value = '{' });
            buf.clear();
        } else if (str.at(i) == '}') {
            buf.push_back(c);
            tokens.push_back({ tok.type = TokenType::RIGHT_PAREN, tok.value = '}' });
            buf.clear();
        } else if (str.at(i) == '"') {
            buf.push_back(c);
            tokens.push_back({ tok.type = TokenType::D_QUOTE, tok.value = '"' });
            buf.clear();
        } else if (std::isalpha(str.at(i))) {
            buf.push_back(str.at(i));
            while (i + 1 < str.length() && std::isalpha(str.at(i + 1))) {
                buf.push_back(str.at(i + 1));
                i++;
            }
            if (buf == "true" || buf == "false") {
                tokens.push_back({ tok.type = TokenType::BOOL, tok.value = buf });
            } else {
                tokens.push_back({ tok.type = TokenType::KEY, tok.value = buf });
            }
            buf.clear();
        } else if (str.at(i) == ':') {
            buf.push_back(str.at(i));
            tokens.push_back({ tok.type = TokenType::COLON, tok.value = ':' });
            buf.clear();
        } else {
            // while ((!std::isalpha(str.at(i)) | !std::isalnum(str.at(i)))) {
            int next_pos = 1;
            buf.push_back(str.at(i));

            tokens.push_back({ tok.type = TokenType::INVALID, tok.value = buf });
            std::cout << "Invalid tokens: " << buf << '\n';
            buf.clear();
            //}
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

    std::string file_name = argv[1];

    std::ifstream file;
    file.open(file_name, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Fail to open file\n";
        return 1;
    }

    String file_content = file_str(file);
    file.close();

    std::vector<Tokens> tokens = tokenize(file_content);

    for (auto c : tokens) {
        std::cout << c << '\n';
    }

    return 0;
}
