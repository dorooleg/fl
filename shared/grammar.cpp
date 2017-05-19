#include "grammar.h"

grammar::grammar()
    : counter_(0)
{

}

void grammar::push_back(const non_terminal& lvalue, const std::vector<expression>& expressions)
{
    expressions_.push_back(std::make_pair(lvalue, expressions));
}

void grammar::load_file(const std::string& path_to_file)
{
    std::ifstream file(path_to_file);
    if (!file)
    {
        std::cerr << "Can't open file" << std::endl;
        exit(EXIT_FAILURE);
    }
    typedef std::istreambuf_iterator<char> buf_iter;
    std::string token;
    std::string lvalue;
    expression expression;
    std::vector<class expression> expressions;
    size_t line_number = 0;
    for (buf_iter it(file), end; it != end; ++it)
    {
        if (*it == '\n')
        {
            line_number++;
            if (line_number == 1)
            {
                begin_ = non_terminal(token);
                token.clear();
                continue;
            }

            if (token.size() == 0)
            {
                throw std::invalid_argument("token size error");
            }

            if (token[0] == '\'')
            {
                expression.push_back(std::make_shared<terminal>(token));

            }
            else
            {
                expression.push_back(std::make_shared<non_terminal>(token));
            }

            expressions.push_back(expression);
            push_back(lvalue, expressions);

            token.clear();
            expressions.clear();
            lvalue.clear();
            expression.clear();
            continue;
        }

        if (*it == '|' && std::distance(expression.begin(), expression.end()) > 0)
        {
            expressions.push_back(expression);
            expression.clear();
            continue;
        }

        if (*it != ' ' && *it != '	' && *it != '=' && *it != '|')
        {
            token.push_back(*it);
        }
        else
        {
            if (token.empty())
            {
                continue;
            }

            if (lvalue.empty())
            {
                lvalue = token;
                token.clear();
                continue;
            }

            if (token[0] == '\'')
            {
                expression.push_back(std::make_shared<terminal>(token));

            }
            else
            {
                expression.push_back(std::make_shared<non_terminal>(token));
            }

            token.clear();
        }
    }

    if (token.size() > 0)
    {
        if (token[0] == '\'')
        {
            expression.push_back(std::make_shared<terminal>(token));

        }
        else
        {
            expression.push_back(std::make_shared<non_terminal>(token));
        }
    }

    if (std::distance(expression.begin(), expression.end()) > 0)
    {
        expressions.push_back(expression);
        push_back(lvalue, expressions);
    }

    if (line_number == 0)
    {
        begin_ = non_terminal(token);
    }
}

void grammar::remove_long_rule()
{
    size_t isize = expressions_.size();
    for (size_t index = 0; index < isize; index++)
    {
        size_t osize = expressions_[index].second.size();
        for (size_t expi = 0; expi < osize; expi++)
        {
            auto& exp = expressions_[index].second[expi];
            if (exp.only_non_terminals() && exp.size() > 2)
            {
                size_t size = exp.size();
                size_t tmp_counter = counter_;
                for (size_t i = 1; i < size - 2; i++)
                {
                    
                    auto& ex = expressions_[index].second[expi];
                    expression t;
                    t.push_back(ex[i]);
                    t.push_back(std::make_shared<non_terminal>(std::string("B") + std::to_string(counter_ + 1)));
                    push_back(std::string("B") + std::to_string(counter_), { t });
                    counter_++;
                }
                
                auto& ex1 = expressions_[index].second[expi];
                expression t;
                t.push_back(ex1[ex1.size() - 2]);
                t.push_back(ex1[ex1.size() - 1]);
                push_back(std::string("B") + std::to_string(counter_), { t });
                auto& ex = expressions_[index].second[expi];
                counter_++;
                ex.resize(1);
                ex.push_back(std::make_shared<non_terminal>(std::string("B") + std::to_string(tmp_counter)));
            }
        }
    }
}

void grammar::remove_multi_terminals()
{
    size_t isize = expressions_.size();
    for (size_t index = 0; index < isize; index++)
    {
        size_t osize = expressions_[index].second.size();
        for (size_t expi = 0; expi < osize; expi++)
        {
            auto& exp = expressions_[index].second[expi];
            if (exp.count_terminals() >= 1 && exp.size() > 1)
            {
                size_t size = exp.size();
                for (size_t i = 0; i < size; i++)
                {
                    auto& ex = expressions_[index].second[expi];
                    if (!ex[i]->is_terminal())
                    {
                        continue;
                    }
                    expression t;
                    t.push_back(ex[i]);
                    ex[i] = std::make_shared<non_terminal>(std::string("B") + std::to_string(counter_));
                    push_back(std::string("B") + std::to_string(counter_), { t });
                    counter_++;
                }
            }
        }
    }
    remove_long_rule();
}

void grammar::remove_eps()
{
    auto eps = get_eps_deducible();
    for (auto & p : expressions_)
    {
        size_t sz = p.second.size();
        for (size_t t = 0; t < sz; t++)
        {
                auto& exp = p.second[t];
                std::vector<std::shared_ptr<token>> cut;
                expression new_expr;
                for (auto& e : exp)
                {
                    if (std::find_if(eps.begin(), eps.end(), [&](const auto& x) { return x->get_value() == e->get_value(); }) != eps.end())
                        cut.push_back(e);
                    else
                        new_expr.push_back(e);
                }

                if (cut.size() > 0)
                {
                    for (size_t i = 0; i < std::pow(2, cut.size()) - 1; i++)
                    {
                        expression ins_expr = new_expr;
                        for (int o = 0; o < (int)cut.size(); o++)
                            if ((1 << o) & i)
                            {
                                ins_expr.push_back(cut[o]);
                            }
                        if (ins_expr.size() > 0)
                        {
                            expression tmp = p.second[t];
                            
                            tmp.erase(std::remove_if(tmp.begin(), tmp.end(), [&](auto& x) 
                            {
                                return std::find_if(ins_expr.begin(), ins_expr.end(), [&](const auto& h) { return &(*h) == &(*x); }) == ins_expr.end();
                            }), tmp.end());

                            p.second.push_back(tmp);
                        }
                    }
                }
            }
    }

    for (auto & p : expressions_)
        p.second.erase(std::remove_if(p.second.begin(), p.second.end(), [](auto& x) { return x.size() > 0 && x[0]->get_value() == "'eps'"; }), p.second.end());
    if (std::find_if(eps.begin(), eps.end(), [&](auto& x) { return x->get_value() == begin_.get_value(); }) != eps.end()) 
    {
        for (auto & p : expressions_)
        {
            if (p.first.get_value() == begin_.get_value())
            {
                expression expression;
                expression.push_back(std::make_shared<terminal>("'eps'"));
                p.second.push_back(expression);
            }
        }
    }
}

std::vector<std::shared_ptr<non_terminal>> grammar::get_eps_deducible()
{
    std::vector<std::shared_ptr<non_terminal>> eps;
    for (const auto& p : expressions_)
    {
        for (const auto& exp : p.second)
        {
            if (exp.only_terminals() && exp.count_terminals() == 1 && exp[0]->get_value() == "\'eps\'")
            {
                if (std::find_if(eps.begin(), eps.end(), [&](const auto& x) { return x->get_value() == exp[0]->get_value(); }) == eps.end())
                    eps.push_back(std::make_shared<non_terminal>(p.first));
            }
        }
    }

    while (true)
    {
        size_t old_size = eps.size();
        for (const auto& p : expressions_)
        {
            for (const auto& exp : p.second)
            {
                if (exp.is_epsilon(eps))
                {
                    if (std::find_if(eps.begin(), eps.end(), [&](const auto& x) { return x->get_value() == p.first.get_value(); }) == eps.end())
                        eps.push_back(std::make_shared<non_terminal>(p.first));
                }
            }
        }

        if (old_size == eps.size())
            break;
    }

    return eps;
}

void grammar::remove_unborn()
{
    std::vector<std::shared_ptr<token>> borns;
    while (true)
    {
        size_t old_size = borns.size();
        for (auto& p : expressions_)
            for (auto& exp : p.second)
            {
                if (exp.only_terminals(borns))
                {
                    
                    if (std::find_if(borns.begin(), borns.end(), [&p](const std::shared_ptr<token>& x) { return x->get_value() == p.first.get_value(); }) == borns.end())
                    {
                        borns.push_back(std::make_shared<non_terminal>(p.first));
                        
                    }
                    continue;
                }
            }
        if (old_size == borns.size())
            break;
    }

    expressions_.erase(std::remove_if(expressions_.begin(), expressions_.end(), [&borns](std::pair<non_terminal, std::vector<expression>>& x) { return std::find_if(borns.begin(), borns.end(), [&](std::shared_ptr<token> v) { return v->get_value() == x.first.get_value(); }) == borns.end(); }), expressions_.end());
}

void grammar::remove_unattainable()
{
    std::vector<std::shared_ptr<token>> un;
    un.push_back(std::make_shared<non_terminal>(begin_));
    while (true)
    {
        size_t old_size = un.size();
        for (auto& p : expressions_)
            for (auto& exp : p.second)
                for (auto& t : exp)
                    if (std::find_if(un.begin(), un.end(), [&](auto& x) { return p.first.get_value() == x->get_value(); }) != un.end() && !t->is_terminal())
                    {
                        if (std::find_if(un.begin(), un.end(), [&](auto& x) { return t->get_value() == x->get_value(); }) == un.end())
                        {
                            un.push_back(t);
                        }
                    }
        if (old_size == un.size())
            break;
    }
    expressions_.erase(std::remove_if(expressions_.begin(), expressions_.end(), [&un](std::pair<non_terminal, std::vector<expression>>& x) { return std::find_if(un.begin(), un.end(), [&](std::shared_ptr<token> v) { return v->get_value() == x.first.get_value(); }) == un.end(); }), expressions_.end());

}

void grammar::remove_useless()
{
    remove_unborn();
    remove_unattainable();
}

void grammar::remove_chains_rules()
{
    std::vector<std::pair<std::shared_ptr<token>, std::shared_ptr<token>>> vec;
    for (const auto& p : expressions_)
    {
        vec.push_back(std::make_pair(std::make_shared<non_terminal>(p.first), std::make_shared<non_terminal>(p.first)));
    }

    bool change = true;
    while (change)
    {
        change = false;
        for (auto & p : expressions_)
            for (auto & exp : p.second)
                if (exp.only_non_terminals() && exp.size() == 1)
                {
                    for (size_t i = 0; i < vec.size(); i++)
                    {
                        if (vec[i].second->get_value() != p.first.get_value())
                            continue;
                        for (size_t j = 0; j < vec.size(); j++)
                        {
                            if (vec[j].first->get_value() != exp[0]->get_value())
                                continue;

                            
                            
                            std::pair<std::shared_ptr<token>, std::shared_ptr<token>> pair = std::make_pair(vec[i].first, vec[j].second);
                            
                            if (std::find_if(vec.begin(), vec.end(), [&pair](const std::pair<std::shared_ptr<token>, std::shared_ptr<token>>& x) { return x.first->get_value() == pair.first->get_value() && x.second->get_value() == pair.second->get_value(); }) == vec.end())
                            {
                                vec.push_back(pair);
                                change = true;
                            }
                        }
                    }

                }
    }


    for (auto & p : expressions_)
        p.second.erase(std::remove_if(p.second.begin(), p.second.end(), [](auto& x) { return x.only_non_terminals() && x.size() == 1; }), p.second.end());

    for (auto& v : vec)
    {
        if (v.first->get_value() != v.second->get_value())
        {
            for (auto & p : expressions_)
            {
                if (p.first.get_value() == v.first->get_value())
                {
                    for (auto & t : expressions_)
                    {
                        if (t.first.get_value() == v.second->get_value())
                        {
                            for (auto & tmp : t.second)
                                p.second.push_back(tmp);
                            break;
                        }
                    }
                    break;
                }
            }
             
        }
    }
}

void grammar::to_cnf()
{
    remove_long_rule();
    remove_eps();
    remove_chains_rules();
    remove_useless();
    remove_multi_terminals();
}

grammar::matrix_t grammar::cyk(const std::string& w)
{
    matrix_t map;

    //base
    for (const auto& p : expressions_)
    {
        map[p.first.get_value()].resize(w.size());
        for (size_t i = 0; i < w.size(); i++)
            map[p.first.get_value()][i].resize(w.size());

        for (size_t i = 0; i < w.size(); i++)
            map[p.first.get_value()][i][i] = satisfy_terminal(p.second, w[i]);
    }

    for (size_t m = 0; m < w.size(); m++)
        for (const auto& p : expressions_)
            for (size_t j = 0; j < w.size(); j++)
                for (size_t i = 0; i < w.size(); i++)
                if (j - i == m)
                    for (size_t k = i; k < j; k++)
                        for (const auto& exp : p.second)
                        {
                            if (exp.only_terminals()) continue;
                            map[p.first.get_value()][i][j] = map[p.first.get_value()][i][j] || (map[exp[0]->get_value()][i][k] && map[exp[1]->get_value()][k + 1][j]);
                        }
    return map;
}

grammar::dot_matrix_t grammar::dot_cyk(const std::string& w)
{
    dot_matrix_t map;

    //base
    for (const auto& p : expressions_)
    {
        map[p.first.get_value()].resize(w.size());
        for (size_t i = 0; i < w.size(); i++)
            map[p.first.get_value()][i].resize(w.size());

        for (size_t i = 0; i < w.size(); i++)
        {
            if (satisfy_terminal(p.second, w[i]))
            {
                map[p.first.get_value()][i][i].second = true;
                map[p.first.get_value()][i][i].first.push_back({ std::string(1, w[i]), i, i});
            }
        }
    }

    for (size_t m = 0; m < w.size(); m++)
        for (const auto& p : expressions_)
            for (size_t j = 0; j < w.size(); j++)
                for (size_t i = 0; i < w.size(); i++)
                if (j - i == m)
                    for (size_t k = i; k < j; k++)
                        for (const auto& exp : p.second)
                        {
                            if (exp.only_terminals()) continue;
                            if (map[exp[0]->get_value()][i][k].second
                                && map[exp[1]->get_value()][k + 1][j].second)
                            {
                                map[p.first.get_value()][i][j].second = true; 
                                map[p.first.get_value()][i][j].first.push_back({ exp[0]->get_value(), i, k});
                                map[p.first.get_value()][i][j].first.push_back({ exp[1]->get_value(), k + 1, j });
                            }
                        }
    return map;
}


void grammar::grammar_init_hash()
{
    for (const auto& p : expressions_)
        for (const auto& exp : p.second)
        {
            if (exp.only_terminals()) continue;
            hash[exp[0]->get_value() + "#" + exp[1]->get_value()].push_back(p.first);
        }
}


std::vector<non_terminal>& grammar::find_no_terminal(const std::string& a, const std::string& b)
{
    return hash[a + "#" + b];
}

std::vector<non_terminal> grammar::find_terminal(const std::string& t)
{
    std::vector<non_terminal> non_terminals;
    for (const auto& p : expressions_)
    {
        if (satisfy_terminal(p.second, t[0]))
        {
            non_terminals.push_back(p.first);
        }
    }
    return non_terminals;
}

void grammar::cyk_dump_dot(const std::string& w, const std::string& path_to_file)
{
    std::ofstream out(path_to_file);
    auto m = dot_cyk(w);
    std::string s = begin_.get_value();
    out << "graph cyk {" << std::endl;
    size_t offset = 0;
    cyk_dump_dot({ s, 0, w.size() - 1 }, m, offset, out);
    out << "}" << std::endl;
    out.close();
}

grammar::vector_tuples grammar::analyze_graph(graph& graph)
{
    grammar_init_hash();
    std::vector<std::vector<std::unordered_set<std::string>>> matrix(graph.size());
    vector_tuples result;
    for (size_t i = 0; i < graph.size(); i++)
        matrix[i].resize(graph.size());
    auto vg = graph.get_graph();
    for (size_t i = 0; i < graph.size(); i++)
        for (const auto& v : vg[i]) 
        {
            std::string rev;
            rev += v.second;
            auto t = find_terminal(rev);
            for (auto & aba : t)
            {
                matrix[i][v.first].insert(aba.get_value());
            }
        }
    
     
    bool visited = true;
    while (visited)
    {
        visited = false;
        for (size_t i = 0; i < graph.size(); i++)
        {
            std::cout << "i = " << i << std::endl; 
            for (size_t j = 0; j < graph.size(); j++)
                for (size_t k = 0; k < graph.size(); k++)
                {
                    if (k == i || k == j || i == j)
                        continue;
                    auto& v1 = matrix[i][k];
                    auto& v2 = matrix[k][j];
                    for (const auto& t1 : v1)
                        for (const auto& t2 : v2)
                        {
                            auto values = find_no_terminal(t1, t2);
                            for (auto & aba : values)
                            {
                                if (!matrix[i][j].count(aba.get_value()))
                                {
                                    visited = true;
                                    matrix[i][j].insert(aba.get_value());
                                }
                            }
                        }
                }
        }
    }

    for (size_t i = 0; i < graph.size(); i++)
        for (size_t j = 0; j < graph.size(); j++)
            for (const auto & v : matrix[i][j])
                if (i < j) 
                    result.push_back(std::make_tuple(std::to_string(i), std::to_string(j), v));
    return result;
        
}

void grammar::cyk_dump_dot(node nt, dot_matrix_t& m, size_t& offset, std::ofstream& out)
{
    if (m.find(nt.name) == m.end())
        return;
    if (!m[nt.name][nt.i][nt.j].second || m[nt.name][nt.i][nt.j].first.empty())
        return;
    size_t tmp_offset = offset;
    for (const auto& node : m[nt.name][nt.i][nt.j].first)
    {
        offset++;
        size_t c_offset = offset;
        cyk_dump_dot(node, m, offset, out); 
        out << "\"#" << std::to_string(tmp_offset) << " "  << nt.name << "\"" << " -- " << "\"#" << std::to_string(c_offset) << " " << node.name << "\";" << std::endl;
    }
}

bool grammar::cyk_result(const std::string& w)
{
    auto m = cyk(w);
    return m[begin_.get_value()][0][w.size() - 1];
}

void grammar::cyk_dump_csv(const std::string& w, const std::string& path_to_file)
{
    std::ofstream out(path_to_file);
    auto m = cyk(w);
    for (size_t i = 0; i < w.size(); i++)
    {
        for (size_t j = 0; j < w.size(); j++)
        {
            for (auto & a : m)
            {
                if (a.second[i][j])
                    out << a.first << " ";
            }
            out << ", ";
        }
        out << std::endl;
    }
            
    out.close();
}


bool grammar::satisfy_terminal(const std::vector<expression>& expressions, char c)
{
    for (const auto& t : expressions)
    {
        if (t.only_terminals())
        {
            if (t[0]->get_value() == std::string("'") + c + "'")
            {
                return true;
            }
        }
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, const grammar& gram)
{
    os << gram.begin_ << std::endl;
    for (const auto& e : gram.expressions_)
    {
        os << e.first << " =";
        bool first = true;
        for (const auto& p : e.second)
        {
            if (first) os << p;
            else os << " |" << p;
            first = false;
        }
        os << std::endl;
            
    }
    return os;
}

void grammar::optimize_grammar()
{
    auto data = get_terminal_expressions();
    std::map<std::string, std::vector<non_terminal>> bi;
    for (const auto& v : data)
    {
        bi[v.second->get_value()].push_back(v.first);
    }
    for (auto& v : bi)
    {
        for (size_t i = 1; i < v.second.size(); i++)
        {
            std::shared_ptr<token> s = std::shared_ptr<non_terminal>(new non_terminal(v.second[0].get_value())); 
            replace_non_terminals(v.second[i], s);
            remove_lvalue(v.second[i]);
        }
    }
}

bool grammar::remove_lvalue(non_terminal& from)
{
    for (size_t i = 0; i < expressions_.size(); i++)
    {
        auto& line = expressions_[i];
        if (line.first.get_value() == from.get_value())
        {
            expressions_.erase(expressions_.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<std::pair<non_terminal, std::shared_ptr<token>>> grammar::get_terminal_expressions()
{
    std::vector<std::pair<non_terminal, std::shared_ptr<token>>> elements;
    for (auto & line : expressions_)
    {
        for (auto & expr : line.second)
        {
            for (auto & token : expr)
            {
                if (token->is_terminal())
                {
                    elements.push_back(std::make_pair(line.first, token));
                }
            }
        }
    }
    return elements;
}

void grammar::replace_non_terminals(non_terminal& from, std::shared_ptr<token>& to)
{
    for (auto & line : expressions_)
    {
        for (auto & expr : line.second)
        {
            for (auto & token : expr)
            {
                if (!token->is_terminal() && token->get_value() == from.get_value())
                {
                    token = to;
                }
            }
        }
    }
}
