#ifndef LISP_PARSER_H
#define LISP_PARSER_H

#include <memory>
#include <string>
#include <stack>
#include <list>
#include <string>
#include <vector>

#include "object.h"

namespace lisp
{

class Parser
{
public:
    Parser(std::string const &input);
    std::shared_ptr<Object> parse();
private:
    enum class Mode
    {
        Default,
        String,
        EscapeChar,
    };
    void step_default(std::string::const_iterator);
    void step_string(std::string::const_iterator);
    void step_escape_char(std::string::const_iterator);
    std::shared_ptr<Object> parse_tokens();
    std::shared_ptr<Object> parse_atom(std::string const &token) const;
private:
    Mode m_mode;
    std::string m_input;
    std::string m_cur_token;
    std::list<std::string> m_tokens;
};

}

#endif // LISP_PARSER_H
