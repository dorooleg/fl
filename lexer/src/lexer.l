%{
    #include <ctype.h>
    #include <iostream>
    #include <stdexcept>
    #include <vector>
    #include <sstream>
    #include <cstdlib>
    #include <memory>
    #include <stack>

    using namespace std;

    void multiline_comment();

    #define INVALID_ARGUMENT(s) \
    { \
        stringstream ss; \
        ss << s; \
        throw invalid_argument(ss.str()); \
    }

    int pos = 0;
    int lineno = 0;
    bool comment_filter = false;
    class token;
    bool satisfy_comment_filter(const shared_ptr<token>& token); 

    class printer
    {
    public:
        virtual void print(ostream& os) const = 0;

        friend ostream& operator<<(ostream& os, const printer& p)
        {
            p.print(os);
            return os;
        }

        virtual ~printer()
        {
        }
    };

    class token : public printer
    {
    public:
        token(size_t line, size_t begin, size_t end)
            : line_(line)
            , begin_(begin)
            , end_(end)
        {
        }

        virtual size_t line() const
        {
            return line_;
        }

        virtual size_t begin() const
        {
            return begin_;
        }

        virtual size_t end() const
        {
            return end_;
        }

        virtual ~token()
        {
        }

    private:
        size_t line_;
        size_t begin_;
        size_t end_;
    };

    class key_word_token : public token
    {
    public:
        key_word_token(const std::string& name, size_t line, size_t begin, size_t end)
            : type_(to_KW(name))
            , token(line, begin, end)
        {
        }

        enum class KW
        {
            KW_IF,
            KW_THEN,
            KW_ELSE,
            KW_WHILE,
            KW_DO,
            KW_READ,
            KW_WRITE,
            KW_BEGIN,
            KW_END
        };

        KW type() const
        {
            return type_;
        }

        virtual void print(ostream& os) const override
        {
            os << to_string(type()) << "(" << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string to_string(KW kw) const
        {
            switch (kw)
            {
                case KW::KW_IF: return "KW_If";
                case KW::KW_THEN: return "KW_Then";
                case KW::KW_ELSE: return "KW_Else";
                case KW::KW_WHILE: return "KW_While";
                case KW::KW_DO: return "KW_Do";
                case KW::KW_READ: return "KW_Read";
                case KW::KW_WRITE: return "KW_Write";
                case KW::KW_BEGIN: return "KW_Begin";
                case KW::KW_END: return "KW_End";
            }
        }

        KW to_KW(const std::string& name) const
        {
            if (name == "if") return KW::KW_IF;
            else if (name == "then") return KW::KW_THEN;
            else if (name == "else") return KW::KW_ELSE;
            else if (name == "while") return KW::KW_WHILE;
            else if (name == "do") return KW::KW_DO;
            else if (name == "read") return KW::KW_READ;
            else if (name == "write") return KW::KW_WRITE;
            else if (name == "begin") return KW::KW_BEGIN;
            else if (name == "end") return KW::KW_END;
            INVALID_ARGUMENT("to_KW invalid argument = " << name);
        }

    private:
        KW type_;
    };

    class operator_token : public token
    {
    public:
        operator_token(const std::string& name, size_t line, size_t begin, size_t end)
            : type_(to_OP(name))
            , token(line, begin, end)
        {
        }

        enum class OP
        {
            OP_PLUS,
            OP_MINUS,
            OP_MUL,
            OP_DIV,
            OP_MOD,
            OP_EQ,
            OP_NEQ,
            OP_GT,
            OP_GTE,
            OP_LT,
            OP_LTE,
            OP_AND,
            OP_OR,
            OP_POW
        };

        OP type() const
        {
            return type_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Op(" << to_string(type()) << ", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string to_string(OP op) const
        {
            switch (op)
            {
                case OP::OP_PLUS: return "PLUS";
                case OP::OP_MINUS: return "MINUS";
                case OP::OP_MUL: return "MUL";
                case OP::OP_DIV: return "DIV";
                case OP::OP_MOD: return "MOD";
                case OP::OP_EQ: return "EQ";
                case OP::OP_NEQ: return "NEQ";
                case OP::OP_GT: return "GT";
                case OP::OP_GTE: return "GTE";
                case OP::OP_LT: return "LT";
                case OP::OP_LTE: return "LTE";
                case OP::OP_AND: return "AND";
                case OP::OP_OR: return "OR";
                case OP::OP_POW: return "POW";
            }
        }

        OP to_OP(const std::string& name) const
        {
            if (name == "+") return OP::OP_PLUS;
            else if (name == "-") return OP::OP_MINUS;
            else if (name == "*") return OP::OP_MUL;
            else if (name == "/") return OP::OP_DIV;
            else if (name == "%") return OP::OP_MOD;
            else if (name == "==") return OP::OP_EQ;
            else if (name == "!=") return OP::OP_NEQ;
            else if (name == ">") return OP::OP_GT;
            else if (name == ">=") return OP::OP_GTE;
            else if (name == "<") return OP::OP_LT;
            else if (name == "<=") return OP::OP_LTE;
            else if (name == "&&") return OP::OP_AND;
            else if (name == "||") return OP::OP_OR;
            else if (name == "**") return OP::OP_POW;
            INVALID_ARGUMENT("to_OP invalid argument = " << name);
        }

    private:
        OP type_;
    };

    class number_token : public token
    {
    public:
        number_token(const std::string& number, size_t line, size_t begin, size_t end)
            : number_(strtod(number.c_str(), nullptr))
            , token(line, begin, end)
        {
        }

        double number() const
        {
            return number_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Num(" << number() << ", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        double number_;
    };

    class identifier_token : public token
    {
    public:
        identifier_token(const std::string& identifier, size_t line, size_t begin, size_t end)
            : identifier_(identifier)
            , token(line, begin, end)
        {
        }

        string identifier() const
        {
            return identifier_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Ident(\"" << identifier() << "\", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string identifier_;
    };

    class delimiter_token : public token
    {
    public:
        delimiter_token(const string& delimiter, size_t line, size_t begin, size_t end)
            : delimiter_(to_DELIM(delimiter))
            , token(line, begin, end)
        {
        }

        enum class DELIM
        {
            DELIM_SEMICOLON,
            DELIM_LEFT_BRACKET,
            DELIM_RIGHT_BRACKET
        };

        DELIM delimiter()
        {
            return delimiter_;
        }

        virtual void print(ostream& os) const override
        {
            os << to_string(delimiter_) << "(" << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string to_string(DELIM delimiter) const
        {
            switch (delimiter)
            {
                case DELIM::DELIM_SEMICOLON: return "Semicolon";
                case DELIM::DELIM_LEFT_BRACKET: return "LeftBracket";
                case DELIM::DELIM_RIGHT_BRACKET: return "RightBracket";
            }
        }

        DELIM to_DELIM(const std::string& name) const
        {
            if (name == ";") return DELIM::DELIM_SEMICOLON;
            else if (name == "(") return DELIM::DELIM_LEFT_BRACKET;
            else if (name == ")") return DELIM::DELIM_RIGHT_BRACKET;
            INVALID_ARGUMENT("to_DELIM invalid argument = " << name);
        }

    private:
        DELIM delimiter_;
    };

    class comment_token : public token
    {
    public:
        comment_token(const std::string& comment, size_t line, size_t begin, size_t end)
            : comment_(comment)
            , token(line, begin, end)
        {
        }

        string comment() const
        {
            return comment_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Comment(" << comment() << ", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string comment_;
    };

    class multiline_comment_token : public token
    {
    public:
        multiline_comment_token(const std::string& comment, size_t line_begin, size_t line_end, size_t begin, size_t end)
            : comment_(comment)
            , line_end_(line_end)
            , token(line_begin, begin, end)
        {
        }

        string comment() const
        {
            return comment_;
        }

        virtual void print(ostream& os) const override
        {
            os << "MultilineComment(" << comment() << ", " << line() << ", " << begin() << ", " << line_end_ << ", " << end() << "); ";
        }

    private:
        string comment_;
        size_t line_end_;
    };

    class error_token : public token
    {
    public:
        error_token(const std::string& error, size_t line_begin, size_t line_end, size_t begin, size_t end)
            : error_(error)
            , line_end_(line_end)
            , token(line_begin, begin, end)
        {
        }

        string error() const
        {
            return error_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Error(" << error() << ", " << line() << ", " << begin() << ", " << line_end_ << ", " << end() << "); ";
        }

    private:
        string error_;
        size_t line_end_;
    };

    class bool_token : public token
    {
    public:
        bool_token(const std::string& name, size_t line, size_t begin, size_t end)
            : type_(to_BOOL(name))
            , token(line, begin, end)
        {
        }

        enum class BOOL
        {
            BOOL_TRUE,
            BOOL_FALSE
        };

        BOOL type() const
        {
            return type_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Bool(" << to_string(type()) << ", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string to_string(BOOL type) const
        {
            switch (type)
            {
                case BOOL::BOOL_TRUE: return "TRUE";
                case BOOL::BOOL_FALSE: return "FALSE";
            }
        }

        BOOL to_BOOL(const std::string& name) const
        {
            if (name == "true") return BOOL::BOOL_TRUE;
            else if (name == "false") return BOOL::BOOL_FALSE;
            INVALID_ARGUMENT("to_BOOL invalid argument = " << name);
        }

    private:
        BOOL type_;
    };

    class assignment_token : public token
    {
    public:
        assignment_token(const std::string& name, size_t line, size_t begin, size_t end)
            : name_(name)
            , token(line, begin, end)
        {
        }

        virtual void print(ostream& os) const override
        {
            os << "Assignment(" << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string name_;
    };

    vector<shared_ptr<token>> _tokens;

    void print_tokens()
    {
        for (const auto& token : _tokens)
        {
            if (!satisfy_comment_filter(token))
            {
                cout << *token;
            }
        }
    }
%}

KW          "if"|"then"|"else"|"while"|"do"|"read"|"write"|"begin"|"end"
OP          \+|-|\*|\/|%|==|!=|>|>=|<|<=|&&|\|\||\*\*
BOOL        "true"|"false"
IDENT       [\_a-zA-Z][\_a-zA-Z0-9]*
D           [0-9]
DB          [1-9]
R           (e[\+-]{D}+|0)?
NUMBER      \-?(({DB}{D}*\.{D}+{R}|0\.{D}+{R}|\.{D}+{R})|({DB}{D}*{R})|0)
SPACE       [ ]
NEW_LINE    \n
TAB         \t
DELIMITER   [\(|\)|;]
COMMENT     \/\/.*$
ASSIGNMENT  :=
MULTILINE_COMMENT \(\*
ERROR       .

%%
{KW} {
    _tokens.push_back(make_shared<key_word_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{NUMBER} {
    _tokens.push_back(make_shared<number_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{OP} {
    _tokens.push_back(make_shared<operator_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{BOOL} {
    _tokens.push_back(make_shared<bool_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{IDENT} {
    _tokens.push_back(make_shared<identifier_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{SPACE} {
    pos += yyleng;
}

{NEW_LINE} {
    lineno++;
    pos = 0;
}

{TAB} {
    pos += yyleng;
}

{DELIMITER} {
    _tokens.push_back(make_shared<delimiter_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{COMMENT} {
    _tokens.push_back(make_shared<comment_token>(yytext + 2, lineno, pos, pos + yyleng - 1));
    lineno++;
    pos = 0;
}

{MULTILINE_COMMENT} {
    multiline_comment();
}

{ASSIGNMENT} {
    _tokens.push_back(make_shared<assignment_token>(yytext, lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{ERROR} {
    _tokens.push_back(make_shared<error_token>(yytext, lineno, lineno, pos, pos + yyleng - 1));
    print_tokens();
    exit(EXIT_FAILURE);
}

%%

static bool is_open_comment(char c1, char c2)
{
    return c1 == '(' && c2 == '*'; 
}

static bool is_close_comment(char c1, char c2)
{
    return c1 == '*' && c2 == ')'; 
}

void multiline_comment()
{
    int begin_line = lineno;
    int begin_pos = pos;
    pos += 2;
    string comment("(*");
    int balance = 1;
    int c1 = 0;
    int c2 = 0;
    while (balance > 0)
    {
        c1 = c2;
        c2 = yyinput();
        if (c2 <= 0)
        {
            _tokens.push_back(make_shared<error_token>(comment, begin_line, lineno, begin_pos, pos));
            print_tokens();
            exit(EXIT_FAILURE);
        }
        comment.push_back(c2);
        pos++;
        if (c2 == '\n')
        {
            pos = 0;
            lineno++;
            continue;
        }
        balance += is_open_comment(c1, c2) - is_close_comment(c1, c2);
    }
    _tokens.push_back(make_shared<multiline_comment_token>(comment, begin_line, lineno, begin_pos, pos));
}

bool satisfy_comment_filter(const shared_ptr<token>& token)
{
    return comment_filter
            && (dynamic_pointer_cast<comment_token>(token)
            || dynamic_pointer_cast<multiline_comment_token>(token));
}

int main(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp("-filter", argv[i]))
        {
            comment_filter = true;
        }
        else
        {
            freopen(argv[i], "r", stdin);
        }
    }

    if ((argc == 3 && !comment_filter) || (argc == 2 && comment_filter))
    {
        std::cerr << "Invalid arguments" << std::endl;
        return EXIT_FAILURE;
    }

    yylex();
    print_tokens();
    return EXIT_FAILURE;
}
