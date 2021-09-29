#include "Emulator/Basic.h"

using namespace Emulator;

#include <fstream>

#include <stack>
#include <variant>

#define READ_INDEX "DATA"
#define FRAME_INDEX "FRAME"

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
        Environment(uint32_t offset) : parent(NULL), offset(offset), Get(OpCode::LOADC), Set(OpCode::STOREC) {
        }

        Environment(std::shared_ptr<Environment> parent) : parent(parent), offset(1), Get(OpCode::IDXC), Set(OpCode::WRITECX) {
        }

        const OpCode Get;
        const OpCode Set;

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

        uint32_t create(const std::string &name) {
            auto existing = vars.find(name);
            if (existing != vars.end()) {
                return existing->second;
            }

            uint32_t next = Offset() + vars.size();
            vars.insert(std::make_pair(name, next));
            return next;
        }

        std::shared_ptr<Environment> Parent() const {
            return parent;
        }
};

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

            switch (token[0]) {
                case 'A':
                    if (token == "AND") {
                        tokenType = BasicTokenType::AND;
                        precedence = Precedence::AND;
                    }
                    else
                    if (token == "ABS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (line.substr(i, 3) == "TAN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'B':
                    if (token == "BEEP") {
                        tokenType = BasicTokenType::BEEP;
                    }
                case 'C':
                    if (token == "CLS") {
                        tokenType = BasicTokenType::CLS;
                    }
                    else
                    if (token == "CINT") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "COS") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "CSNG") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'D':
                    if (token == "DATA") {
                        tokenType = BasicTokenType::DATA;
                    }
                    else
                    if (token == "DEF") {
                        tokenType = BasicTokenType::DEF;
                    }
                    else
                    if (token == "DIM") {
                        tokenType = BasicTokenType::DIM;
                    }
                    break;
                case 'E':
                    if (token == "ELSE") {
                        tokenType = BasicTokenType::ELSE;
                    }
                    else
                    if (token == "END") {
                        tokenType = BasicTokenType::END;
                    }
                    break;
                case 'F':
                    if (token == "FOR") {
                        tokenType = BasicTokenType::FOR;
                    }
                    else
                    if (token == "FIX") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token.substr(0,2) == "FN") {
                        tokenType = BasicTokenType::USRFUNCTION;
                    }
                    break;
                case 'G':
                    if (token == "GOTO") {
                        tokenType = BasicTokenType::GOTO;
                    }
                    else
                    if (token == "GOSUB") {
                        tokenType = BasicTokenType::GOSUB;
                    }
                    break;
                case 'I':
                    if (token == "IF") {
                        tokenType = BasicTokenType::IF;
                    }
                    else
                    if (token == "INKEY") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "INPUT") {
                        tokenType = BasicTokenType::INPUT;
                    }
                    else
                    if (token == "INT") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'L':
                    if (token == "LET") {
                        tokenType = BasicTokenType::LET;
                    }
                    else
                    if (token == "LINE") {
                        tokenType = BasicTokenType::LINE;
                    }
                    else
                    if (token == "LOG") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'N':
                    if (token == "NEXT") {
                        tokenType = BasicTokenType::NEXT;
                    }
                    else
                    if (token == "NOT") {
                        tokenType = BasicTokenType::NOT;
                    }
                    break;
                case 'O':
                    if (token == "OR") {
                        precedence = Precedence::OR;
                        tokenType = BasicTokenType::OR;
                    }
                    break;
                case 'P':
                    if (token == "PEEK") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "POKE") {
                        tokenType = BasicTokenType::POKE;
                    }
                    else
                    if (token == "PRINT") {
                        tokenType = BasicTokenType::PRINT;
                    }
                    else
                    if (token == "PSET") {
                        tokenType = BasicTokenType::PSET;
                    }
                    else
                    if (token == "PUT") {
                        tokenType = BasicTokenType::PUT;
                    }
                    break;
                case 'R':
                    if (token == "READ") {
                        tokenType = BasicTokenType::READ;
                    }
                    else
                    if (token == "REM") {
                        tokenType = BasicTokenType::REM;
                    }
                    else
                    if (token == "RETURN") {
                        tokenType = BasicTokenType::RETURN;
                    }
                    break;
                case 'S':
                    if (token == "SGN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "SIN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "SQR") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "SOUND") {
                        tokenType = BasicTokenType::SOUND;
                    }
                    else
                    if (token == "STEP") {
                        tokenType = BasicTokenType::STEP;
                    }
                    else
                    if (token == "SWAP") {
                        tokenType = BasicTokenType::SWAP;
                    }
                    break;
                case 'T':
                    if (token == "TAN") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    else
                    if (token == "THEN") {
                        tokenType = BasicTokenType::THEN;
                    }
                    else
                    if (token == "TO") {
                        tokenType = BasicTokenType::TO;
                    }
                    break;
                case 'V':
                    if (token == "VARPTR") {
                        tokenType = BasicTokenType::FUNCTION;
                    }
                    break;
                case 'W':
                    if (token == "WAIT") {
                        tokenType = BasicTokenType::WAIT;
                    }
                    else
                    if (token == "WHILE") {
                        tokenType = BasicTokenType::WHILE;
                    }
                    else
                    if (token == "WEND") {
                        tokenType = BasicTokenType::WEND;
                    }
                    break;
                default:
                    break;
            }

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
    std::cerr << "Error on line " << linenumber << " at position " << current << ": " << err <<std::endl;
    exit(-1);
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
        program.addValue(OpCode::SETB, IntAsValue(0));
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
    } else if (token.str == "INKEY") {
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.addSyscall(OpCode::SYSCALL, SysCall::READKEY, RuntimeValue::C);
        program.add(OpCode::PUSHC);
    } else if (token.str == "LOG") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::LOG);
        program.add(OpCode::PUSHC);
    } else if (token.str == "PEEK") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPC);
        program.add(OpCode::MOVCIDX);
        program.add(OpCode::IDXC);
        program.add(OpCode::PUSHC);
    } else if (token.str == "SGN") {
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current], BasicTokenType::RIGHT_PAREN, "`)' expected");
        program.add(OpCode::POPA);
        program.addValue(OpCode::SETB, IntAsValue(0));
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
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
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
        program.addValue(OpCode::SETC, IntAsValue((int16_t)std::stoi(token.str)));
        program.add(OpCode::PUSHC);
    } else if (token.type == BasicTokenType::FLOAT) {
        program.addValue(OpCode::SETC, FloatAsValue(std::stof(token.str)));
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
            program.addValue(OpCode::INCIDX, IntAsValue(env->get(token.str)));
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

        expression(program, linenumber, {tokens.begin(), tokens.end()}, token.lbp);

        program.addValue(OpCode::INCIDX, IntAsValue(1));

        while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

            expression(program, linenumber, {tokens.begin(), tokens.end()});

            program.add(OpCode::IDXA);
            program.add(OpCode::POPB);
            program.add(OpCode::MUL);

            program.addValue(OpCode::INCIDX, IntAsValue(1));

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
        program.addValue(OpCode::SETA, IntAsValue(0));
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
        program.addValue(OpCode::SETC, IntAsValue(step));
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

            program.addValue(OpCode::INCIDX, IntAsValue(1));

            while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
                check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

                expression(program, linenumber, {tokens.begin(), tokens.end()});

                program.add(OpCode::IDXA);
                program.add(OpCode::POPB);
                program.add(OpCode::MUL);

                program.addValue(OpCode::INCIDX, IntAsValue(1));

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

            program.addValue(OpCode::INCB, IntAsValue(1));
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

            program.addValue(OpCode::INCB, IntAsValue(1));
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
    } else if (tokens[current].type == BasicTokenType::BEEP) {
        current += 1;
        program.addValue(OpCode::SETA, IntAsValue(800));
        program.addValue(OpCode::SETB, IntAsValue(250));
        program.addSyscall(OpCode::SYSCALL, SysCall::SOUND, RuntimeValue::C);
    } else if (tokens[current].type == BasicTokenType::SOUND) {
        current += 1;

        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});

        program.add(OpCode::POPB);
        program.add(OpCode::POPA);

        program.addSyscall(OpCode::SYSCALL, SysCall::SOUND, RuntimeValue::C);
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
        program.addSyscall(OpCode::SYSCALL, SysCall::CLS, RuntimeValue::C);
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
        program.addValue(OpCode::INCIDX, IntAsValue(1));

        program.add(OpCode::POPB);
        program.add(OpCode::POPA);
        program.addSyscall(OpCode::SYSCALL, SysCall::BLIT, RuntimeValue::IDX);
    } else if (tokens[current].type == BasicTokenType::LINE) {
        current++;

        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");

        program.add(OpCode::POPA);
        program.addValue(OpCode::SETB, IntAsValue(320));
        program.add(OpCode::MUL);
        program.add(OpCode::MOVCA);
        program.add(OpCode::POPB);
        program.add(OpCode::ADD);

        program.add(OpCode::PUSHC);

        check(linenumber, tokens[current++], BasicTokenType::MINUS, "`-' expected");
        check(linenumber, tokens[current++], BasicTokenType::LEFT_PAREN, "`(' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");
        expression(program, linenumber, {tokens.begin(), tokens.end()});
        check(linenumber, tokens[current++], BasicTokenType::RIGHT_PAREN, "`)' expected");
        check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

        program.add(OpCode::POPA);
        program.addValue(OpCode::SETB, IntAsValue(320));
        program.add(OpCode::MUL);
        program.add(OpCode::MOVCA);
        program.add(OpCode::POPB);
        program.add(OpCode::ADD);

        program.add(OpCode::MOVCA);
        program.add(OpCode::POPB);


        program.add(OpCode::PUSHB);
        program.add(OpCode::PUSHA);

        expression(program, linenumber, {tokens.begin(), tokens.end()});
        program.add(OpCode::POPC);
        //check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

        //program.addShort(OpCode::TRACE, 1);
        program.add(OpCode::POPB);
        program.add(OpCode::POPA);

        program.addSyscall(OpCode::SYSCALL, SysCall::DRAWLINE, RuntimeValue::IDX);
        //program.addShort(OpCode::TRACE, 0);
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
        program.addValue(OpCode::INCIDX, IntAsValue(1));

        env = std::make_shared<Environment>(env);

        for (size_t i = 0; i< args.size(); i++) {
            program.add(OpCode::POPC);
            program.add(OpCode::WRITECX);
            program.addValue(OpCode::INCIDX, IntAsValue(1));
            env->create(args[i]);
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

        program.addValue(OpCode::INCIDX, IntAsValue(1));

        while (tokens[current].type != BasicTokenType::RIGHT_PAREN) {
            check(linenumber, tokens[current++], BasicTokenType::COMMA, "`,' expected");

            expression(program, linenumber, {tokens.begin(), tokens.end()});

            program.add(OpCode::IDXA);
            program.add(OpCode::POPB);
            program.add(OpCode::MUL);

            program.addValue(OpCode::INCIDX, IntAsValue(1));

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

    program.addValue(OpCode::SETC, IntAsValue(1));
    program.add(OpCode::PUSHC);

    for (int i = 0; i < size; i++) {
        program.add(OpCode::POPB);

        program.add(OpCode::POPA);
        program.addValue(OpCode::INCA, IntAsValue(1));

        program.add(OpCode::MUL);

        program.add(OpCode::PUSHC);
    }

    program.add(OpCode::POPC);

    program.addValue(OpCode::INCC, IntAsValue(size));

    program.add(OpCode::CALLOC);
    program.addPointer(OpCode::SAVEIDX, env->create(name));

    program.add(OpCode::WRITECX);
    program.addValue(OpCode::INCC, IntAsValue(-size));

    for (int i = 1; i < size; i++) {
        program.addValue(OpCode::INCIDX, IntAsValue(1));
        program.add(OpCode::WRITEBX);
        program.add(OpCode::MOVCA);
        program.add(OpCode::IDIV);
        program.add(OpCode::MOVCB);
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
    env->create(FRAME_INDEX);

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
            statement(program, linenumber, tokens);
        }
    }

    for (auto jump : jumps) {
        program.updateShort(jump.first, program.getLabel(jump.second));
    }

    program.updateValue(frame+1, PointerAsValue(env->Offset() + env->size()));
}
