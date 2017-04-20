#pragma once
#include "expression.h"
#include "non_terminal.h"
#include "terminal.h"
#include <fstream>
#include <tuple>
#include <algorithm>
#include <map>

class grammar
{
public:
	typedef std::vector<std::pair<non_terminal, std::vector<expression>>> container;

	grammar()
		: counter_(0)
	{

	}

	void push_back(const non_terminal& lvalue, const std::vector<expression>& expressions)
	{
		expressions_.push_back(std::make_pair(lvalue, expressions));
	}

	void load_file(const std::string& path_to_file)
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

	void remove_long_rule()
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

	void remove_multi_terminals()
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

	void remove_eps()
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

	std::vector<std::shared_ptr<non_terminal>> get_eps_deducible()
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

	void remove_unborn()
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

	void remove_unattainable()
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

	void remove_useless()
	{
		remove_unborn();
		remove_unattainable();
	}

	void remove_chains_rules()
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

	void to_cnf()
	{
		remove_long_rule();
		remove_eps();
		remove_chains_rules();
		remove_useless();
		remove_multi_terminals();
	}

	typedef std::map<std::string, std::vector<std::vector<bool>>> matrix_t;
	matrix_t cyk(const std::string& w)
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

    struct node
    {
        std::string name;
        size_t i = 0;
        size_t j = 0;
    };

	typedef std::map<std::string, std::vector<std::vector<std::pair<std::vector<node>, bool>>>> dot_matrix_t;
	dot_matrix_t dot_cyk(const std::string& w)
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

    void cyk_dump_dot(const std::string& w, const std::string& path_to_file)
    {
        std::ofstream out(path_to_file);
        auto m = dot_cyk(w);
        std::string s = begin_.get_value();
        out << "graph cyk {" << std::endl;
        cyk_dump_dot({ s, 0, w.size() - 1 }, m, 0, out);
        out << "}" << std::endl;
        out.close();
    }

    void cyk_dump_dot(node nt, dot_matrix_t& m, size_t offset, std::ofstream& out)
    {
        if (m.find(nt.name) == m.end())
            return;
        if (!m[nt.name][nt.i][nt.j].second || m[nt.name][nt.i][nt.j].first.empty())
            return;
        for (const auto& node : m[nt.name][nt.i][nt.j].first)
        {
            out << "\"" << std::to_string(offset) << " "  << nt.name << "\"" << " -- " << "\"" << std::to_string(offset + 1) << " " << node.name << "\";" << std::endl;
        }
        for (const auto& node : m[nt.name][nt.i][nt.j].first)
        {
            cyk_dump_dot(node, m, offset + 1, out); 
        }
    }

    bool cyk_result(const std::string& w)
    {
        auto m = cyk(w);
        return m[begin_.get_value()][0][w.size() - 1];
    }

    void cyk_dump_csv(const std::string& w, const std::string& path_to_file)
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


	bool satisfy_terminal(const std::vector<expression>& expressions, char c)
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

	friend std::ostream& operator<<(std::ostream& os, const grammar& gram)
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

private:
	size_t       counter_;
	non_terminal begin_;
	container	 expressions_;
};

