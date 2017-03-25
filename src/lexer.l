%{
    #include <ctype.h>
    #include <iostream>
    #include <stdexcept>
    #include <vector>
    #include <sstream>
    #include <memory>

    using namespace std;

    #define INVALID_ARGUMENT(s) \
    { \
        stringstream ss; \
        ss << s; \
        throw invalid_argument(ss.str()); \
    }

    int pos = 0;
    int lineno = 0;

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
            OP_OR
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
            INVALID_ARGUMENT("to_OP invalid argument = " << name);
        }

    private:
        OP type_;
    };

    class number_token : public token
    {
    public:
        number_token(const std::string& number, size_t line, size_t begin, size_t end)
            : number_(stod(number))
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

    class colon_token : public token
    {
    public:
        colon_token(size_t line, size_t begin, size_t end)
            : token(line, begin, end)
        {
        }

        virtual void print(ostream& os) const override
        {
            os << "Colon(" << line() << ", " << begin() << ", " << end() << "); ";
        }
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

    class error_token : public token
    {
    public:
        error_token(const std::string& error, size_t line, size_t begin, size_t end)
            : error_(error)
            , token(line, begin, end)
        {
        }

        string error() const
        {
            return error_;
        }

        virtual void print(ostream& os) const override
        {
            os << "Error(" << error() << ", " << line() << ", " << begin() << ", " << end() << "); ";
        }

    private:
        string error_;
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

    vector<shared_ptr<token>> _tokens;

    void print_tokens()
    {
        for (const auto& token : _tokens)
        {
            cout << *token;
        }
    }
%}

KW          "if"|"then"|"else"|"while"|"do"|"read"|"write"|"begin"|"end"
OP          \+|-|\*|\/|%|==|!=|>|>=|<|<=|&&|\|\|
BOOL        "true"|"false"
IDENT       [\_a-zA-Z][\_a-zA-Z0-9]*
D           [0-9]
DB          [1-9]
R           (e[\+-]{D}+|0)?
NUMBER      \-?(({DB}{D}*\.{D}+{R}|0\.{D}+{R}|\.{D}+{R})|({DB}{D}*{R})|0)
SPACE       [ ]
NEW_LINE    \n
TAB         \t
COLON       [\(|\)|;]
COMMENT     \/\/.*$
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

{COLON} {
    _tokens.push_back(make_shared<colon_token>(lineno, pos, pos + yyleng - 1));
    pos += yyleng;
}

{COMMENT} {
    _tokens.push_back(make_shared<comment_token>(yytext + 2, lineno, pos, pos + yyleng - 1));
    lineno++;
    pos = 0;
}

{ERROR} {
    _tokens.push_back(make_shared<error_token>(yytext, lineno, pos, pos + yyleng - 1));
    print_tokens();
    exit(EXIT_FAILURE);
}

%%

int main()
{
    yylex();
    print_tokens();
}
