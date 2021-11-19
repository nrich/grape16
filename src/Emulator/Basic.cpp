#include "Emulator/Basic.h"

using namespace Emulator;

#include <fstream>
#include <sstream>
#include <ctime>

#include <stack>
#include <variant>

#define READ_INDEX "DATA"
#define FRAME_INDEX "FRAME"

#define VOICE_INDEX "VOICE"
#define VOICE_ARGS 6

#define LINE_INDEX "LINE"
#define LINE_ARGS 6

static void error(uint32_t linenumber, const std::string &err);

struct UserFunction {
    uint32_t call;
    std::vector<std::string> args;

    UserFunction(uint32_t call, std::vector<std::string> args) {
        this->call = call;
        this->args = args;
    }
};

class Environment {
    private:
        std::map<const std::string, uint32_t> vars;
        std::shared_ptr<Environment> parent;
        const uint32_t offset;
    public:
        Environment(uint32_t offset) : parent(NULL), offset(offset) {
        }

        Environment(std::shared_ptr<Environment> parent) : parent(parent), offset(1) {
        }

        const size_t Offset() const {
            return offset;
        }

        const size_t size() const {
            return vars.size();
        }

        uint32_t get(const std::string &name) const {
            auto found = vars.find(name);

            if (found != vars.end()) {
                return found->second;
            } else {
                if (parent) {
                    return parent->get(name);
                } else {
                    throw std::invalid_argument("Unknown variable `" + name + "'");
                }
            }
        }

        bool isGlobal(const std::string &name) {
            auto found = vars.find(name);

            if (found != vars.end() && parent) {
                return false;
            }

            return true;
        }

        uint32_t create(const std::string &name, size_t count=1) {
            auto existing = vars.find(name);
            if (existing != vars.end()) {
                return existing->second;
            }

            uint32_t next = Offset() + vars.size();
            for (size_t i = 0; i  < count; i++) {
                vars.insert(std::make_pair(name + std::string(i, ' '), next+i));
            }
            return next;
        }

        std::shared_ptr<Environment> Parent() const {
            return parent;
        }
};

std::vector<std::tuple<uint32_t, EventType, uint32_t>> events; 

static std::map<std::string, UserFunction> userfunctions;

static int current = 0;
static std::map<uint32_t, std::string> jumps;
static std::stack<std::pair<uint32_t,uint32_t>> while_loops;
static std::stack<std::tuple<uint32_t,uint32_t,std::string>> for_loops;

static std::shared_ptr<Environment> env;

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isWhitespace(char c) {
    return (c == ' ') || (c == '\t');
}

static std::string str_toupper(std::string s) {
    std::transform(
        s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::toupper(c); }
    );

    return s;
}

std::pair<uint32_t, std::vector<BasicToken>> parseLine(const std::string &line) {
    std::vector<BasicToken> tokens;
    size_t i = 0;
    uint32_t lineno = 0;

    while (i < line.size()) {
        if (isDigit(line[i])) {
            lineno *= 10;
            lineno += (line[i] - '0');
        } else {
            break;
        }

        i++;
    }

    while (i < line.size()) {
        if (isWhitespace(line[i])) {
            i++;
            continue;
        }

        if (isDigit(line[i])) {
            auto tokenType = BasicTokenType::INT; 
            size_t start = i++;

            while (isDigit(line[i]))
                i++;

            if (line[i] == '.' && isDigit(line[i+1])) {
                tokenType = BasicTokenType::FLOAT;
                i++;

                while (isDigit(line[i]))
                    i++;
            }

            tokens.push_back(BasicToken(tokenType, line.substr(start, i-start)));
        } else if (isAlpha(line[i])) {
            auto precedence = Precedence::NONE;
            size_t start = i++;
            auto tokenType = BasicTokenType::IDENTIFIER;

            while((isAlpha(line[i]) || isDigit(line[i])))
                i++;

            auto token = line.substr(start, i-start);
            auto keyword = str_toupper(token);

            switch (keyword[0]) {
                case 'A':
                    if (keyword == "AND") {
                        tokenType = BasicTokenType::AND;
                        precedence = Precedence::AND;
                    }
                    else
                    if (keyword == "ABS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "ATAN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'B':
                    if (keyword == "BEEP") {
                        tokenType = BasicTokenType::BEEP;
                    }
                case 'C':
                    if (keyword == "CHR") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "CLS") {
                        tokenType = BasicTokenType::CLS;
                    }
                    else
                    if (keyword == "CINT") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "COLOR") {
                        tokenType = BasicTokenType::COLOR;
                    }
                    else
                    if (keyword == "COS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "CSNG") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "CVI") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "CVS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'D':
                    if (keyword == "DATA") {
                        tokenType = BasicTokenType::DATA;
                    }
                    else
                    if (keyword == "DEF") {
                        tokenType = BasicTokenType::DEF;
                    }
                    else
                    if (keyword == "DIM") {
                        tokenType = BasicTokenType::DIM;
                    }
                    break;
                case 'E':
                    if (keyword == "ELSE") {
                        tokenType = BasicTokenType::ELSE;
                    }
                    else
                    if (keyword == "END") {
                        tokenType = BasicTokenType::END;
                    }
                    break;
                case 'F':
                    if (keyword == "FOR") {
                        tokenType = BasicTokenType::FOR;
                    }
                    else
                    if (keyword == "FIX") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword.substr(0,2) == "FN") {
                        tokenType = BasicTokenType::USRFUNCTION;
                    }
                    break;
                case 'G':
                    if (keyword == "GOTO") {
                        tokenType = BasicTokenType::GOTO;
                    }
                    else
                    if (keyword == "GOSUB") {
                        tokenType = BasicTokenType::GOSUB;
                    }
                    break;
                case 'I':
                    if (keyword == "IF") {
                        tokenType = BasicTokenType::IF;
                    }
                    else
                    if (keyword == "INKEY") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "INPUT") {
                        tokenType = BasicTokenType::INPUT;
                    }
                    else
                    if (keyword == "INT") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'K':
                    if (keyword == "KEY") {
                        tokenType = BasicTokenType::KEY;
                    }
                    break;
                case 'L':
                    if (keyword == "LEN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "LET") {
                        tokenType = BasicTokenType::LET;
                    }
                    else
                    if (keyword == "LINE") {
                        tokenType = BasicTokenType::LINE;
                    }
                    else
                    if (keyword == "LOCATE") {
                        tokenType = BasicTokenType::LOCATE;
                    }
                    else
                    if (keyword == "LOG") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'M':
                    if (keyword == "MAX") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "MIN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "MKI") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "MKS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'N':
                    if (keyword == "NEXT") {
                        tokenType = BasicTokenType::NEXT;
                    }
                    else
                    if (keyword == "NOT") {
                        tokenType = BasicTokenType::NOT;
                    }
                    break;
                case 'O':
                    if (keyword == "ON") {
                        tokenType = BasicTokenType::ON;
                    }
                    else
                    if (keyword == "OR") {
                        precedence = Precedence::OR;
                        tokenType = BasicTokenType::OR;
                    }
                    break;
                case 'P':
                    if (keyword == "PALETTE") {
                        tokenType = BasicTokenType::PALETTE;
                    }
                    else
                    if (keyword == "PEEK") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "POKE") {
                        tokenType = BasicTokenType::POKE;
                    }
                    else
                    if (keyword == "PRINT") {
                        tokenType = BasicTokenType::PRINT;
                    }
                    else
                    if (keyword == "PSET") {
                        tokenType = BasicTokenType::PSET;
                    }
                    else
                    if (keyword == "PUT") {
                        tokenType = BasicTokenType::PUT;
                    }
                    break;
                case 'R':
                    if (keyword == "RANDOMIZE") {
                        tokenType = BasicTokenType::RANDOMIZE;
                    }
                    else
                    if (keyword == "READ") {
                        tokenType = BasicTokenType::READ;
                    }
                    else
                    if (keyword == "REM") {
                        tokenType = BasicTokenType::REM;
                    }
                    else
                    if (keyword == "RETURN") {
                        tokenType = BasicTokenType::RETURN;
                    }
                    else
                    if (keyword == "RND") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'S':
                    if (keyword == "SGN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "SIN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "SQR") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "SOUND") {
                        tokenType = BasicTokenType::SOUND;
                    }
                    else
                    if (keyword == "STEP") {
                        tokenType = BasicTokenType::STEP;
                    }
                    else
                    if (keyword == "SWAP") {
                        tokenType = BasicTokenType::SWAP;
                    }
                    break;
                case 'T':
                    if (keyword == "TAN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "THEN") {
                        tokenType = BasicTokenType::THEN;
                    }
                    else
                    if (keyword == "TIMER") {
                        tokenType = BasicTokenType::TIMER;
                    }
                    else
                    if (keyword == "TO") {
                        tokenType = BasicTokenType::TO;
                    }
                    break;
                case 'V':
                    if (keyword == "VARPTR") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (keyword == "VOICE") {
                        tokenType = BasicTokenType::VOICE;
                    }
                    break;
                case 'W':
                    if (keyword == "WAIT") {
                        tokenType = BasicTokenType::WAIT;
                    }
                    else
                    if (keyword == "WHILE") {
                        tokenType = BasicTokenType::WHILE;
                    }
                    else
                    if (keyword == "WEND") {
                        tokenType = BasicTokenType::WEND;
                    }
                    break;
                default:
                    break;
            }

            if (tokenType != BasicTokenType::IDENTIFIER)
                token = keyword;

            tokens.push_back(BasicToken(tokenType, token, precedence));
        } else if (line[i] == '"') {
            i++;
            std::string str;
            while (line[i++] != '"') {
                char c = line[i-1];

                if (c == '\\' && line[i] == 'n') {
                    c = '\n';
                    i++;
                } else if (c == '\\' && line[i] == '"') {
                    c = '"';
                    i++;
                } else if (c == '\\' && line[i] == 't') {
                    c = '\t';
                    i++;
                } else if (c == '\\' && line[i] == '\\') {
                    c = '\\';
                    i++;
                }

                str += c;
            }

            tokens.push_back(BasicToken(BasicTokenType::STRING, str));
        } else {
            switch (line[i++]) {
                case '-':
                    tokens.push_back(BasicToken(BasicTokenType::MINUS, "-", Precedence::TERM));
                    break;
                case '+':
                    tokens.push_back(BasicToken(BasicTokenType::PLUS, "+", Precedence::TERM));
                    break;
                case '/':
                    tokens.push_back(BasicToken(BasicTokenType::SLASH, "/", Precedence::FACTOR));
                    break;
                case '*':
                    tokens.push_back(BasicToken(BasicTokenType::STAR, "*", Precedence::FACTOR));
                    break;
                case '^':
                    tokens.push_back(BasicToken(BasicTokenType::CARAT, "^", Precedence::FACTOR));
                    break;
                case '\\':
                    tokens.push_back(BasicToken(BasicTokenType::BACKSLASH, "\\", Precedence::FACTOR));
                    break;
                case '%':
                    tokens.push_back(BasicToken(BasicTokenType::PERCENT, "%", Precedence::FACTOR));
                    break;
                case ';':
                    tokens.push_back(BasicToken(BasicTokenType::SEMICOLON, ";"));
                    break;
                case ':':
                    tokens.push_back(BasicToken(BasicTokenType::COLON, ":"));
                    break;
                case '\'':
                    tokens.push_back(BasicToken(BasicTokenType::QUOTE, "'"));
                    break;
                case ',':
                    tokens.push_back(BasicToken(BasicTokenType::COMMA, ","));
                    break;
                case '=':
                    tokens.push_back(BasicToken(BasicTokenType::EQUAL, "=", Precedence::EQUALITY));
                    break;
                case '>':
                        switch (line[i]) {
                            case '=':
                                tokens.push_back(BasicToken(BasicTokenType::GREATER_EQUAL, ">=", Precedence::COMPARISON));
                                i++;
                                break;
                            default:
                                tokens.push_back(BasicToken(BasicTokenType::GREATER, ">", Precedence::COMPARISON));
                        }
                    break;
                case '<':
                        switch (line[i]) {
                            case '>':
                                tokens.push_back(BasicToken(BasicTokenType::NOT_EQUAL, "<>", Precedence::EQUALITY));
                                i++;
                                break;
                            case '=':
                                tokens.push_back(BasicToken(BasicTokenType::LESS_EQUAL, "<=", Precedence::COMPARISON));
                                i++;
                                break;
                            default:
                                tokens.push_back(BasicToken(BasicTokenType::LESS, "<", Precedence::COMPARISON));
                        }
                    break;
                case '(':
                    tokens.push_back(BasicToken(BasicTokenType::LEFT_PAREN, "(", Precedence::CALL));
                    break;
                case ')':
                    tokens.push_back(BasicToken(BasicTokenType::RIGHT_PAREN, ")"));
                    break;
            }
        }
    }

    tokens.push_back(BasicToken(BasicTokenType::EOL, ""));

    return std::pair<uint32_t, std::vector<BasicToken>>(lineno, tokens);
}

std::map<uint32_t, std::vector<BasicToken>> parseFile(const std::string &filename) {
    std::map<uint32_t, std::vector<BasicToken>> tokens;
    std::string line;
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "Could not open `" << filename << "'" << std::endl;
        exit(-1);
    }

    while (std::getline(infile, line)) {
        auto data = parseLine(line); 

        tokens[data.first] = data.second;
    }

    return tokens;
}

static void error(uint32_t linenumber, const std::string &err) {
    std::ostringstream s;
    s << "Error on line " << linenumber << " at position " << current << ": " << err;
    throw std::domain_error(s.str());
}

static std::string identifier(uint32_t linenumber, const BasicToken &token) {
    if (token.type != BasicTokenType::IDENTIFIER) {
        error(linenumber, "Identifier expected");
    }

    return token.str;
}

static void check(uint32_t linenumber, const BasicToken &token, BasicTokenType type, const std::string &err) {
    if (token.type != type)
        error(linenumber, err);
}

static void expression(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens, int rbp=0);

static void function(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto token = tokens[current];

    check(linenumber, tokens[current+1], BasicTokenType::LEFT_PAREN, "`(' expected");

    current += 2;

    if (token.str == "ABS") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPA);
        program.addValue(OpCode::SETB, ShortAsValue(0));
        program.add(OpCode::CMP);
        program.add(OpCode::MOVCB);
        program.add(OpCode::MUL);
        program.add(OpCode::PUSHC);
    } else if (token.str == "ATAN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::ATAN);
        program.add(OpCode::PUSHC);
    } else if (token.str == "CHR") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.addShort(OpCode::ALLOC, 2);
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.add(OpCode::PUSHIDX);
    } else if (token.str == "CINT" || token.str == "INT") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::INT);
        program.add(OpCode::PUSHC);
    } else if (token.str == "COS") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::COS);
        program.add(OpCode::PUSHC);
    } else if (token.str == "CSNG") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::FLT);
        program.add(OpCode::PUSHC);
    } else if (token.str == "CVI" || token.str == "CVS") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPIDX);
        program.add(OpCode::VSTR);
        program.add(OpCode::PUSHC);
    } else if (token.str == "INKEY") {
        if (tokens[current].type == BasicTokenType::RIGHT_PAREN) {
            program.addSyscall(OpCode::SYSCALL, SysCall::READKEY, RuntimeValue::C);
        } else {
            expression(program, linenumber, {tokens.begin(), tokens.end()});
            check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
            program.addSyscall(OpCode::SYSCALL, SysCall::KEYSET, RuntimeValue::C);
        }

        program.add(OpCode::PUSHC);
    } else if (token.str == "LEN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPIDX);
        program.addValue(OpCode::SETB, ShortAsValue(0));
        program.addValue(OpCode::SETC, ShortAsValue(0));
        auto loop = program.add(OpCode::NOP);
        program.add(OpCode::PUSHC);
        program.add(OpCode::IDXA);
        program.add(OpCode::CMP);
        auto cmp = program.addShort(OpCode::JMPEZ, (int16_t)0);
        program.add(OpCode::POPC);
        program.addValue(OpCode::INCC, ShortAsValue(1));
        program.addValue(OpCode::INCIDX, ShortAsValue(1));
        program.addShort(OpCode::JMP, (int16_t)loop);
        program.add(OpCode::PUSHC);
        auto end = program.add(OpCode::NOP);
        program.add(OpCode::POPC);
        program.add(OpCode::PUSHC);
        program.updateShort(cmp+1, end);
    } else if (token.str == "LOG") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::LOG);
        program.add(OpCode::PUSHC);
    } else if (token.str == "MAX") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::GT);
        auto cmp = program.addShort(OpCode::JMPNZ, (int16_t)0);
        program.add(OpCode::PUSHB);
        program.add(OpCode::POPA);
        auto end = program.add(OpCode::PUSHA);
        program.updateShort(cmp+1, end);
    } else if (token.str == "MIN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::LT);
        auto cmp = program.addShort(OpCode::JMPNZ, (int16_t)0);
        program.add(OpCode::PUSHB);
        program.add(OpCode::POPA);
        auto end = program.add(OpCode::PUSHA);
        program.updateShort(cmp+1, end);

    } else if (token.str == "MKI" || token.str == "MKS") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPIDX);
        program.add(OpCode::VSTR);
        program.add(OpCode::PUSHC);
    } else if (token.str == "PEEK") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::MOVCIDX);
        program.add(OpCode::IDXC);
        program.add(OpCode::PUSHC);
    } else if (token.str == "RND") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::RND);
        program.add(OpCode::PUSHC);
    } else if (token.str == "SGN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPA);
        program.addValue(OpCode::SETB, ShortAsValue(0));
        program.add(OpCode::CMP);
        program.add(OpCode::PUSHC);
    } else if (token.str == "SIN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::SIN);
        program.add(OpCode::PUSHC);
    } else if (token.str == "SQR") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::SQR);
        program.add(OpCode::PUSHC);
    } else if (token.str == "TAN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::TAN);
        program.add(OpCode::PUSHC);
    } else if (token.str == "VARPTR") {
        auto name = identifier(linenumber, tokens[current++]);
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.addValue(OpCode::SETC, PointerAsValue(env->get(name)));
        program.add(OpCode::PUSHC);
    } else {
        error(linenumber, std::string("Unknown function `") + token.str + std::string("'"));
    }
}

static void usrfunction(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto token = tokens[current];

    check(linenumber, tokens[current+1], BasicTokenType::LEFT_PAREN, "`(' expected");

    current += 2;

    auto find = userfunctions.find(token.str);
    if (find == userfunctions.end()) {
        error(linenumber, "Undefined User Function");
    }

    auto userfunction = find->second;

    size_t argcount = 0;

    if (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        argcount++;
    }

    while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        argcount++;
    }
    check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");

    if (argcount != userfunction.args.size()) {
        error(linenumber, std::string("Argument count mismath for `") + token.str + std::string("': ") + std::to_string(argcount) + " vs " + std::to_string(userfunction.args.size()));
    }

    program.addShort(OpCode::CALL, (int16_t)userfunction.call);
}


static void TokenAsValue(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto token = tokens[current];

    if (token.type == BasicTokenType::STRING) {
        program.addShort(OpCode::ALLOC, token.str.size()+1);
        program.addString(OpCode::SDATA, token.str);
        program.add(OpCode::PUSHIDX);
        program.add(OpCode::POPC);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::INT) {
        program.addValue(OpCode::SETC, ShortAsValue((int16_t)std::stoi(token.str)));
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::FLOAT) {
        program.addValue(OpCode::SETC, RealAsValue(std::stof(token.str)));
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::IDENTIFIER) {
        if (env->isGlobal(token.str)) {
            program.addPointer(OpCode::LOADC, env->get(token.str));
            program.add(OpCode::PUSHC);
        } else {
            program.add(OpCode::PUSHIDX);
            program.addPointer(OpCode::LOADIDX, env->get(FRAME_INDEX));
            program.add(OpCode::IDXB);
            program.add(OpCode::PUSHB);
            program.add(OpCode::POPIDX);
            program.addValue(OpCode::INCIDX, ShortAsValue(env->get(token.str)));
            program.add(OpCode::IDXC);
            program.add(OpCode::POPIDX);
            program.add(OpCode::PUSHC);
        }
    } else if (token.type == BasicTokenType::FUNCTION) {
        function(program, linenumber, tokens);
    } else if (token.type == BasicTokenType::USRFUNCTION) {
        usrfunction(program, linenumber, tokens);
    } else {
        error(linenumber, std::string("value expected, got `") + token.str + "'");
    }
}

static void Op(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto token = tokens[current++];

    if (token.type == BasicTokenType::STAR) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::MUL);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::SLASH) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::DIV);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::PLUS) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::ADD);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::MINUS) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::SUB);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::PERCENT) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::MOD);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::BACKSLASH) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::IDIV);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::CARAT) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::EXP);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::EQUAL) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::EQ);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::NOT_EQUAL) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::NE);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::LESS) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::LT);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::GREATER) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::GT);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::LESS_EQUAL) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::LE);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::GREATER_EQUAL) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::GE);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::AND) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::AND);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::OR) {
        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.add(OpCode::OR);
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::LEFT_PAREN) {
        program.add(OpCode::POPIDX);

        expression(program, linenumber, {tokens.begin(), tokens.end()});

        while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

            expression(program, linenumber, {tokens.begin(), tokens.end()});

            program.add(OpCode::IDXA);
            program.add(OpCode::POPB);
            program.add(OpCode::MUL);

            program.addValue(OpCode::INCIDX, ShortAsValue(1));

            program.add(OpCode::MOVCA);
            program.add(OpCode::POPB);
            program.add(OpCode::ADD);

            program.add(OpCode::PUSHC);
        }
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

        program.add(OpCode::POPB);

        program.add(OpCode::PUSHIDX);
        program.add(OpCode::POPA);

        program.add(OpCode::ADD);

        program.add(OpCode::PUSHC);

        program.add(OpCode::POPIDX);
        program.add(OpCode::IDXC);

        program.add(OpCode::PUSHC);
    } else {
        error(linenumber, std::string("op expected, got `") + token.str + "'");
    }
}

static void prefix(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens, int rbp) {
    if (tokens[current].type == BasicTokenType::LEFT_PAREN) {
        current++;
        expression(program, linenumber, {tokens.begin(), tokens.end()}, 0);
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
    } else if (tokens[current].type == BasicTokenType::NOT) {
        current++;
        prefix(program, linenumber, tokens, rbp);
        program.add(OpCode::POPC);
        program.add(OpCode::NOT);
        program.add(OpCode::PUSHC);
    } else if (tokens[current].type == BasicTokenType::PLUS) {
        current++;
        prefix(program, linenumber, tokens, rbp);
    } else if (tokens[current].type == BasicTokenType::MINUS) {
        current++;
        prefix(program, linenumber, tokens, rbp);
        program.add(OpCode::POPB);
        program.addValue(OpCode::SETA, ShortAsValue(0));
        program.add(OpCode::SUB);
        program.add(OpCode::PUSHC);
    } else {
        TokenAsValue(program, linenumber, tokens);
    }
}

static void expression(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens, int rbp) {
    if (tokens.size() == 0) {
        error(linenumber, "Expression expected");
    }

    prefix(program, linenumber, tokens, rbp);
    auto token = tokens[++current];

    while (rbp < token.lbp) {
        Op(program, linenumber, {tokens.begin(), tokens.end()});
        token = tokens[current];
    }
}

static void statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens);

static void if_statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    expression(program, linenumber, tokens);
    program.add(OpCode::POPC); 
    check(linenumber, tokens[current++], BasicTokenType::THEN, "`THEN' expected");
    auto _false = program.addShort(OpCode::JMPEZ, 0); 

    statement(program, linenumber, tokens);

    if (tokens[current].type == BasicTokenType::ELSE) {
        current++;
        auto _true = program.addShort(OpCode::JMP, 0);
        auto _else = program.add(OpCode::NOP);
        statement(program, linenumber, tokens);
        program.updateShort(_false+1, _else);

        auto end = program.add(OpCode::NOP);
        program.updateShort(_true+1, end);
    } else {
        auto end = program.add(OpCode::NOP);
        program.updateShort(_false+1, end);
    }
}

static void while_statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto loop = program.add(OpCode::NOP);
    expression(program, linenumber, tokens);
    program.add(OpCode::POPC); 
    auto _false = program.addShort(OpCode::JMPEZ, 0); 

    while_loops.push(std::pair<uint32_t,uint32_t>(loop, _false));
}

static void wend_statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    if (while_loops.size() == 0)
        error(linenumber, "WEND without WHILE");

    auto loop = while_loops.top();
    while_loops.pop();

    program.addShort(OpCode::JMP, loop.first);
    auto pos = program.add(OpCode::NOP);
    program.updateShort(loop.second+1, pos);
}

static void for_statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto name = identifier(linenumber, tokens[current++]);
    check(linenumber, tokens[current++], BasicTokenType::EQUAL, "`=' expected");
    expression(program, linenumber, tokens);
    program.add(OpCode::POPC);
    program.addPointer(OpCode::STOREC, env->create(name));

    check(linenumber, tokens[current++], BasicTokenType::TO, "`TO' expected");
    expression(program, linenumber, tokens);

    int16_t step = 1;
    if (tokens[current].type == BasicTokenType::STEP) {
        current++;
        expression(program, linenumber, tokens);
    } else {
        program.addValue(OpCode::SETC, ShortAsValue(step));
        program.add(OpCode::PUSHC);
    }

    auto cmp = program.add(OpCode::NOP);
    program.addPointer(OpCode::LOADA, env->get(name));
    program.add(OpCode::POPC); 
    program.add(OpCode::POPB); 
    program.add(OpCode::PUSHB); 
    program.add(OpCode::PUSHC);
    program.add(OpCode::LE);
    auto _false = program.addShort(OpCode::JMPEZ, 0); 
    for_loops.push(std::tuple<uint32_t,uint32_t,std::string>(cmp, _false, name));
}

static void next_statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    if (for_loops.size() == 0)
        error(linenumber, "NEXT without FOR");

    auto loop = for_loops.top();
    for_loops.pop();

    program.addPointer(OpCode::LOADA, env->get(std::get<2>(loop)));
    program.add(OpCode::POPB);
    program.add(OpCode::ADD);
    program.addPointer(OpCode::STOREC, env->get(std::get<2>(loop)));
    program.add(OpCode::PUSHB);

    program.addShort(OpCode::JMP, std::get<0>(loop));
    auto pos = program.add(OpCode::NOP);
    program.add(OpCode::POPA);
    program.add(OpCode::POPB);
    program.updateShort(std::get<1>(loop)+1, pos);
}

static void statement(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    if (tokens[current].type == BasicTokenType::PRINT) {
        current++;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.addSyscall(OpCode::SYSCALL, SysCall::WRITE, RuntimeValue::C);
    } else if (tokens[current].type == BasicTokenType::INPUT) {
        if (tokens[current+1].type == BasicTokenType::STRING) {
            auto prompt = tokens[current+1].str;
            auto name = identifier(linenumber, tokens[current+2]);

            program.addShort(OpCode::ALLOC, prompt.size()+1);
            program.addString(OpCode::SDATA, prompt);
            program.addSyscall(OpCode::SYSCALL, SysCall::WRITE, RuntimeValue::IDX);

            program.addShort(OpCode::ALLOC, 81);
            program.addSyscall(OpCode::SYSCALL, SysCall::READ, RuntimeValue::IDX);
            program.addPointer(OpCode::SAVEIDX, env->create(name));

            current += 3;
        } else if (tokens[current+1].type == BasicTokenType::IDENTIFIER) {
            auto name = identifier(linenumber, tokens[current+1]);

            program.addShort(OpCode::ALLOC, 81);
            program.addSyscall(OpCode::SYSCALL, SysCall::READ, RuntimeValue::IDX);
            program.addPointer(OpCode::SAVEIDX, env->create(name));

            current += 2;
        } else {
            error(linenumber, "expected: INPUT [...] <Identifier> <EOL>");
        }
    } else if (tokens[current].type == BasicTokenType::READ) {
        auto name = identifier(linenumber, tokens[current+1]);

        if (tokens[current+2].type == BasicTokenType::LEFT_PAREN) {
            current += 3;
            program.addPointer(OpCode::LOADIDX, env->get(name));

            expression(program, linenumber, {tokens.begin(), tokens.end()});

            while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
                check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

                expression(program, linenumber, {tokens.begin(), tokens.end()});

                program.add(OpCode::IDXA);
                program.add(OpCode::POPB);
                program.add(OpCode::MUL);

                program.addValue(OpCode::INCIDX, ShortAsValue(1));

                program.add(OpCode::MOVCA);
                program.add(OpCode::POPB);
                program.add(OpCode::ADD);

                program.add(OpCode::PUSHC);
            }
            check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

            program.add(OpCode::POPB);

            program.add(OpCode::PUSHIDX);
            program.add(OpCode::POPA);

            program.add(OpCode::ADD);

            program.add(OpCode::PUSHC);
            program.add(OpCode::POPIDX);

            program.add(OpCode::PUSHIDX);

            program.addValue(OpCode::SETA, PointerAsValue(0));
            program.addPointer(OpCode::LOADB, env->get(READ_INDEX));
            program.add(OpCode::ADD);

            program.addValue(OpCode::INCB, ShortAsValue(1));
            program.addPointer(OpCode::STOREB, env->get(READ_INDEX));

            program.add(OpCode::PUSHC);
            program.add(OpCode::POPIDX);
            program.add(OpCode::IDXC);

            program.add(OpCode::POPIDX);

            program.add(OpCode::WRITECX);

        } else {
            program.addValue(OpCode::SETA, PointerAsValue(0));
            program.addPointer(OpCode::LOADB, env->get(READ_INDEX));
            program.add(OpCode::ADD);

            program.addValue(OpCode::INCB, ShortAsValue(1));
            program.addPointer(OpCode::STOREB, env->get(READ_INDEX));

            program.add(OpCode::PUSHC);
            program.add(OpCode::POPIDX);

            program.add(OpCode::IDXC);
            program.addPointer(OpCode::STOREC, env->get(name));
        }
    } else if (tokens[current].type == BasicTokenType::GOTO) {
        if (tokens[current+1].type != BasicTokenType::INT)
            error(linenumber, "Line number expected");

        uint32_t jmp = program.addShort(OpCode::JMP, 0)+1;
        jumps[jmp] = tokens[current+1].str;

        current += 2;
    } else if (tokens[current].type == BasicTokenType::GOSUB) {
        if (tokens[current+1].type != BasicTokenType::INT)
            error(linenumber, "Line number expected");

        uint32_t jmp = program.addShort(OpCode::CALL, 0)+1;
        jumps[jmp] = tokens[current+1].str;

        current += 2;
    } else if (tokens[current].type == BasicTokenType::POKE) {
        current += 1;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPA);
        program.add(OpCode::POPC);
        program.add(OpCode::MOVCIDX);
        program.add(OpCode::WRITEAX);
    } else if (tokens[current].type == BasicTokenType::RETURN) {
        program.add(OpCode::RETURN);
        current += 1;
    } else if (tokens[current].type == BasicTokenType::WAIT) {
        program.add(OpCode::YIELD);
        current += 1;
    } else if (tokens[current].type == BasicTokenType::PALETTE) {
        current += 1;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.addSyscall(OpCode::SYSCALL, SysCall::PALETTE, RuntimeValue::C);
    } else if (tokens[current].type == BasicTokenType::COLOR) {
        current += 1;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.addSyscall(OpCode::SYSCALL, SysCall::COLOUR, RuntimeValue::NONE);
    } else if (tokens[current].type == BasicTokenType::LOCATE) {
        current += 1;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.addSyscall(OpCode::SYSCALL, SysCall::CURSOR, RuntimeValue::NONE);
    } else if (tokens[current].type == BasicTokenType::BEEP) {
        current += 1;
        program.addValue(OpCode::SETA, ShortAsValue(800));
        program.addValue(OpCode::SETB, ShortAsValue(250));
        program.addValue(OpCode::SETC, ShortAsValue(0));
        program.addSyscall(OpCode::SYSCALL, SysCall::SOUND, RuntimeValue::NONE);
    } else if (tokens[current].type == BasicTokenType::VOICE) {
        current += 1;
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        if (tokens[current].type == BasicTokenType::COMMA) {
            auto voiceptr = env->get(VOICE_INDEX);

            program.add(OpCode::POPC);
            program.addPointer(OpCode::STOREC, voiceptr);

            for (size_t i = 1; i < VOICE_ARGS; i++) {
                check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
                expression(program, linenumber, {tokens.begin(), tokens.end()});
                program.add(OpCode::POPC);
                program.addPointer(OpCode::STOREC, voiceptr+i);
            }
            program.addPointer(OpCode::SETIDX, voiceptr);
            program.add(OpCode::POPC);
        } else {
            program.add(OpCode::POPC);
            program.add(OpCode::PUSHC);
            program.add(OpCode::POPIDX);
            program.add(OpCode::POPC);
        }

        program.addSyscall(OpCode::SYSCALL, SysCall::VOICE, RuntimeValue::C);
    } else if (tokens[current].type == BasicTokenType::SOUND) {
        current += 1;

        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        if (tokens[current].type == BasicTokenType::COMMA) {
            current++;
            expression(program, linenumber, {tokens.begin(), tokens.end()});
            program.add(OpCode::POPC);
        } else {
            program.addValue(OpCode::SETC, ShortAsValue(0));
        }

        program.add(OpCode::POPB);
        program.add(OpCode::POPA);

        program.addSyscall(OpCode::SYSCALL, SysCall::SOUND, RuntimeValue::NONE);
    } else if (tokens[current].type == BasicTokenType::SWAP) {
        current += 1;

        auto left = identifier(linenumber, tokens[current]);
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        auto right = identifier(linenumber, tokens[current]);
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPC);
        program.addPointer(OpCode::STOREC, env->get(left));

        program.add(OpCode::POPC);
        program.addPointer(OpCode::STOREC, env->get(right));
    } else if (tokens[current].type == BasicTokenType::CLS) {
        current++;
        program.addSyscall(OpCode::SYSCALL, SysCall::CLS, RuntimeValue::NONE);
    } else if (tokens[current].type == BasicTokenType::RANDOMIZE) {
        current++;
        if (tokens[current].type == BasicTokenType::TIMER) {
            program.addValue(OpCode::SETC, ShortAsValue((int16_t)time(NULL)));
        } else {
            expression(program, linenumber, {tokens.begin(), tokens.end()});
        }
        program.add(OpCode::SEED);
    } else if (tokens[current].type == BasicTokenType::END) {
        program.add(OpCode::HALT);

        current += 1;
    } else if (tokens[current].type == BasicTokenType::IF) {
        current++;
        if_statement(program, linenumber, {tokens.begin(), tokens.end()});
    } else if (tokens[current].type == BasicTokenType::WHILE) {
        current++;
        while_statement(program, linenumber, {tokens.begin(), tokens.end()});
    } else if (tokens[current].type == BasicTokenType::WEND) {
        current++;
        wend_statement(program, linenumber, {tokens.begin(), tokens.end()});
    } else if (tokens[current].type == BasicTokenType::FOR) {
        current++;
        for_statement(program, linenumber, {tokens.begin(), tokens.end()});
    } else if (tokens[current].type == BasicTokenType::NEXT) {
        current++;
        next_statement(program, linenumber, {tokens.begin(), tokens.end()});
    } else if (tokens[current].type == BasicTokenType::PSET) {
        current++;

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPC);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.addSyscall(OpCode::SYSCALL, SysCall::DRAW, RuntimeValue::IDX);
    } else if (tokens[current].type == BasicTokenType::PUT) {
        current++;

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        auto dst = identifier(linenumber, tokens[current++]);

        program.addPointer(OpCode::LOADIDX, env->get(dst));

        program.add(OpCode::IDXC);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.addSyscall(OpCode::SYSCALL, SysCall::BLIT, RuntimeValue::IDX);
    } else if (tokens[current].type == BasicTokenType::ON) {
        current++;

        check(linenumber, tokens[current++], BasicTokenType::KEY, "`KEY' expected");

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
        check(linenumber, tokens[current++], BasicTokenType::GOSUB, "`GOSUB' expected");

        if (tokens[current+1].type != BasicTokenType::INT)
            error(linenumber, "Line number expected");

        uint32_t jmp = program.addShort(OpCode::CALL, 0)+1;
        jumps[jmp] = tokens[current+1].str;

        current += 2;
    } else if (tokens[current].type == BasicTokenType::LINE) {
        current++;

        program.addPointer(OpCode::SETIDX, env->get(LINE_INDEX));

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

        check(linenumber, tokens[current++], BasicTokenType::MINUS, "`-' expected");

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        program.add(OpCode::WRITECX);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        if (tokens[current].type == BasicTokenType::COMMA) {
            current++;
            auto box = identifier(linenumber, tokens[current++]);

            if (box == "B") {
                program.addValue(OpCode::SETC, ShortAsValue(0));
            } else if (box == "BF") {
                program.addValue(OpCode::SETC, ShortAsValue(1));
            } else {
                error(linenumber, "Expected: B or BF");
            }

            program.add(OpCode::WRITECX);
            program.addPointer(OpCode::SETIDX, env->get(LINE_INDEX));
            program.addSyscall(OpCode::SYSCALL, SysCall::DRAWBOX, RuntimeValue::IDX);
        } else {
            program.addValue(OpCode::SETC, ShortAsValue(0));
            program.add(OpCode::WRITECX);
            program.addPointer(OpCode::SETIDX, env->get(LINE_INDEX));
            program.addSyscall(OpCode::SYSCALL, SysCall::DRAWLINE, RuntimeValue::IDX);
        }
    } else if (tokens[current].type == BasicTokenType::IDENTIFIER && tokens[current+1].type == BasicTokenType::EQUAL) {
        auto name = identifier(linenumber, tokens[current++]);

        check(linenumber, tokens[current++], BasicTokenType::EQUAL, "`=' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPC);
        program.addPointer(OpCode::STOREC, env->create(name));
    } else if (tokens[current].type == BasicTokenType::DEF) {
        current++;
        check(linenumber, tokens[current], BasicTokenType::USRFUNCTION, "`<FN...>' expected");
        auto func = tokens[current++];
        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");

        std::vector<std::string> args;

        if (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            auto arg = identifier(linenumber, tokens[current++]);
            args.push_back(arg);
        }

        while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
            auto arg = identifier(linenumber, tokens[current++]);
            args.push_back(arg);
        }
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

        check(linenumber, tokens[current++], BasicTokenType::EQUAL, "`=' expected");

        auto skip = program.addShort(OpCode::JMP, 0);
        auto call = program.add(OpCode::NOP);

        program.addPointer(OpCode::LOADIDX, env->get(FRAME_INDEX));
        program.addValue(OpCode::INCIDX, ShortAsValue(1));

        env = std::make_shared<Environment>(env);

        auto rargs = args;
        std::reverse(rargs.begin(), rargs.end());

        for (size_t i = 0; i< rargs.size(); i++) {
            program.add(OpCode::POPC);
            program.add(OpCode::WRITECX);
            program.addValue(OpCode::INCIDX, ShortAsValue(1));
            env->create(rargs[i]);
        }

        program.addPointer(OpCode::LOADC, env->get(FRAME_INDEX));
        program.add(OpCode::WRITECX);

        program.addPointer(OpCode::SAVEIDX, env->get(FRAME_INDEX));

        expression(program, linenumber, {tokens.begin(), tokens.end()});

        userfunctions.insert(std::make_pair(func.str, UserFunction(call, args)));
        env = env->Parent();

        program.addPointer(OpCode::LOADIDX, env->get(FRAME_INDEX));
        program.add(OpCode::IDXA);
        program.add(OpCode::PUSHA);
        program.add(OpCode::POPIDX);
        program.addPointer(OpCode::SAVEIDX, env->get(FRAME_INDEX));

        program.add(OpCode::RETURN);
        auto end = program.add(OpCode::NOP);
        program.updateShort(skip+1, end);
    } else if (tokens[current].type == BasicTokenType::REM || tokens[current].type == BasicTokenType::QUOTE) {
        program.add(OpCode::NOP);
        while (tokens[current].type == BasicTokenType::EOL)
            current++;
    } else if (tokens[current].type == BasicTokenType::IDENTIFIER && tokens[current+1].type == BasicTokenType::LEFT_PAREN) {
        auto name = identifier(linenumber, tokens[current++]);

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");

        program.addPointer(OpCode::LOADIDX, env->get(name));

        expression(program, linenumber, {tokens.begin(), tokens.end()});

        while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

            expression(program, linenumber, {tokens.begin(), tokens.end()});

            program.add(OpCode::IDXA);
            program.add(OpCode::POPB);
            program.add(OpCode::MUL);

            program.addValue(OpCode::INCIDX, ShortAsValue(1));

            program.add(OpCode::MOVCA);
            program.add(OpCode::POPB);
            program.add(OpCode::ADD);

            program.add(OpCode::PUSHC);
        }
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

        program.add(OpCode::POPB);

        program.add(OpCode::PUSHIDX);
        program.add(OpCode::POPA);

        program.add(OpCode::ADD);

        program.add(OpCode::PUSHC);
        program.add(OpCode::POPIDX);

        program.add(OpCode::PUSHIDX);
        check(linenumber, tokens[current++], BasicTokenType::EQUAL, "`=' expected");

        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPC);

        program.add(OpCode::POPIDX);

        program.add(OpCode::WRITECX);
    } else {
        expression(program, linenumber, tokens);
    }

    if (tokens[current].type == BasicTokenType::SEMICOLON || tokens[current].type == BasicTokenType::COLON) {
        current++;
        statement(program, linenumber, tokens);
    }
}

static void declaration(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto name = identifier(linenumber, tokens[current++]);

    check(linenumber, tokens[current++], BasicTokenType::EQUAL, "`=' expected");
    expression(program, linenumber, {tokens.begin(), tokens.end()});

    program.add(OpCode::POPC); 
    program.addPointer(OpCode::STOREC, env->create(name));
}

static void dim_declaration(Program &program, uint32_t linenumber, const std::vector<BasicToken> &tokens) {
    auto name = identifier(linenumber, tokens[current++]);
    std::stack<int16_t> data_lengths;

    check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
    expression(program, linenumber, {tokens.begin(), tokens.end()});

    int size = 1;

    while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        size++;
    }
    check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

    program.addValue(OpCode::SETC, ShortAsValue(1));
    program.add(OpCode::PUSHC);

    for (int i = 0; i < size; i++) {
        program.add(OpCode::POPB);

        program.add(OpCode::POPA);
        program.addValue(OpCode::INCA, ShortAsValue(1));

        program.add(OpCode::MUL);

        program.add(OpCode::PUSHC);
    }

    program.add(OpCode::POPC);

    program.addValue(OpCode::INCC, ShortAsValue(size));

    program.add(OpCode::CALLOC);

    program.addValue(OpCode::INCC, ShortAsValue(-size));
    program.add(OpCode::WRITECX);

    program.addValue(OpCode::INCIDX, ShortAsValue(1));
    program.addPointer(OpCode::SAVEIDX, env->create(name));

    for (int i = 1; i < size; i++) {
        program.add(OpCode::WRITEAX);
        program.add(OpCode::MOVCB);
        program.add(OpCode::IDIV);
        program.add(OpCode::MOVCA);
        program.addValue(OpCode::INCIDX, ShortAsValue(1));
    }
}

void compile(const std::map<uint32_t, std::vector<BasicToken>> &lines, Program &program) {
    uint32_t entry = program.add(OpCode::NOP);

    program.setEntryPoint(entry);

    std::map<uint32_t, std::vector<BasicToken>> datalines;
    std::copy_if(std::begin(lines), std::end(lines),
        std::inserter(datalines, std::end(datalines)),
        [](std::pair<uint32_t, std::vector<BasicToken>> const& dataline) {
            return dataline.second[0].type == BasicTokenType::DATA;
        }
    );

    int datacount = 0;
    for (auto dataline : datalines) {
        uint32_t linenumber = dataline.first;
        const std::vector<BasicToken> &tokens = dataline.second;

        bool comma_expected = false;

        for (auto token : tokens) {
            if (token.type == BasicTokenType::DATA)
                continue;

            if (token.type == BasicTokenType::INT) {
                program.addPointer(OpCode::SETIDX, datacount++);
                program.addShort(OpCode::IDATA, (int16_t)std::stol(token.str));
            } else if (token.type == BasicTokenType::FLOAT) {
                program.addPointer(OpCode::SETIDX, datacount++);
                program.addFloat(OpCode::FDATA, (float)std::stof(token.str));
                datacount++;
            } else if (token.type == BasicTokenType::STRING) {
                program.addShort(OpCode::ALLOC, token.str.size()+1);
                program.addString(OpCode::SDATA, token.str);
                program.add(OpCode::PUSHIDX);
                program.add(OpCode::POPC);
                program.addPointer(OpCode::STOREC, datacount++);
            } else if (token.type == BasicTokenType::COMMA) {
                if (comma_expected) {
                    check(linenumber, token, BasicTokenType::COMMA, "`,' expected");
                } else {
                    error(linenumber, "unexpected token `,'");
                }
            } else if (token.type == BasicTokenType::EOL) {
                break;
            } else {
                error(linenumber, "Invalid DATA value `" + token.str + "'");
            }

            comma_expected = !comma_expected;
        }
    }

    env = std::make_shared<Environment>(datacount);

    env->create(READ_INDEX);
    env->create(VOICE_INDEX, VOICE_ARGS);
    env->create(FRAME_INDEX);
    env->create(LINE_INDEX, LINE_ARGS);

    auto frame = program.addValue(OpCode::SETC, PointerAsValue(0));
    program.addPointer(OpCode::STOREC, env->create(FRAME_INDEX));

    for (auto line : lines) {
        current = 0;
        uint32_t linenumber = line.first;
        const std::vector<BasicToken> &tokens = line.second;

        program.addLabel(linenumber);

        if (tokens[current].type == BasicTokenType::LET) {
            current++;
            declaration(program, linenumber, tokens);
        } else if (tokens[current].type == BasicTokenType::DATA) {
            program.add(OpCode::NOP);
        } else if (tokens[current].type == BasicTokenType::DIM) {
            current++;
            dim_declaration(program, linenumber, tokens);
        } else {
            // emit event handler jump
            statement(program, linenumber, tokens);
        }
    }

    for (auto jump : jumps) {
        program.updateShort(jump.first, program.getLabel(jump.second));
    }

    program.updateValue(frame+1, PointerAsValue(env->Offset() + env->size()));
}
