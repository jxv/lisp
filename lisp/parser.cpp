#include <memory>
#include <string>
#include <stack>
#include <list>
#include <sstream>

#include "parser.h"

#include "error.h"
#include "list.h"
#include "string.h"
#include "symbol.h"
#include "boolean.h"
#include "number.h"

namespace lisp
{

Parser::Parser(std::string const &input)
    : m_input(input)
    , m_mode(Mode::Default)
{
}
    
void Parser::step_default(std::string::const_iterator it)
{
    const char ch = *it;
    if (ch == '"')
    {
        m_mode = Mode::String;
        return;
    }
    else if (ch == '(' || ch == ')')
    {
        if (!m_cur_token.empty())
        {
            m_tokens.push_back(m_cur_token);
            m_cur_token.clear();
        }
        m_tokens.push_back(std::string(1, ch));
    }
    else if (ch == '\r' || ch == '\n' || ch == '\t' || ch == ' ')
    {
        if (!m_cur_token.empty())
        {
            m_tokens.push_back(m_cur_token);
            m_cur_token.clear();
        }
    }
    else
    {
        m_cur_token += ch;
    }
}

void Parser::step_string(std::string::const_iterator it)
{
    const char ch = *it;
    if (m_cur_token.empty())
    {
            m_cur_token += '"';
    }
    if (ch == '"')
    {
        m_cur_token += ch;
        m_tokens.push_back(m_cur_token);
        m_cur_token.clear();
        m_mode = Mode::Default;
    }
    else if (ch == '\\')
    {
        m_mode = Mode::EscapeChar;
    }
    else
    {
        m_cur_token += ch;
    }
}

void Parser::step_escape_char(std::string::const_iterator it)
{
    switch (*it)
    {
    case 'a':
    {
        m_cur_token.push_back('\a');
        break;
    }
    case 'b':
    {
        m_cur_token.push_back('\b');
        break;
    }
    case 'f':
    {
        m_cur_token.push_back('\f');
        break;
    }
    case 'n':
    {
        m_cur_token.push_back('\n');
        break;
    }
    case 'r':
    {
        m_cur_token.push_back('\r');
        break;
    }
    case 't':
    {
        m_cur_token.push_back('\t');
        break;
    }
    case 'v':
    {
        m_cur_token.push_back('\v');
        break;
    }
    case '\\':
    {
        m_cur_token.push_back('\\');
        break;
    }
    case '\'':
    {
        m_cur_token.push_back('\'');
        break;
    }
    case '\"':
    {
        m_cur_token.push_back('\"');
        break;
    }
    case '\?':
    {
        m_cur_token.push_back('\?');
        break;
    }
    default:
    {
        std::ostringstream msg;
        msg << "invalid escape char: '\\" << static_cast<int>(*it) << "'";
        throw lisp::Error(msg.str());
    }
    }
    m_mode = Mode::String;
}

std::shared_ptr<Object> Parser::parse_atom(std::string const &token) const
{
    // string token
    if (token.length() >= 2 && token.front() == '"' && token.back() == '"')
    {
        return std::shared_ptr<Object>(new lisp::String(token.substr(1, token.length() - 2)));
    }

    // boolean token
    if (token.length() == 2 && token.front() == '#')
    {
        switch (token.back())
        {
        case 't':
        {
            return std::shared_ptr<Object>(new lisp::Boolean(true));
        }
        case 'f':
        {
            return std::shared_ptr<Object>(new lisp::Boolean(false));
        }
        default:
        {
            break;
        }
        }
    }
    if (token.front() == '#')
    {
            throw lisp::Error("invalid token");
    }

    // f32 token
    if (token.rfind('.') != std::string::npos)
    {
        try
        {
            float f = std::stof(token);
            return std::shared_ptr<Object>(new lisp::F32(f));
        }
        catch (std::exception)
        {
        }
    }

    // i32 token
    try
    {
        int i = std::stoi(token);
        return std::shared_ptr<Object>(new lisp::I32(i));
    }
    catch (std::exception)
    {
    }

    // fallback on symbol
    return std::shared_ptr<Object>(new lisp::Symbol(token));
}

std::shared_ptr<Object> Parser::parse_tokens()
{
    if (m_tokens.empty())
    {
        throw lisp::Error("no matching ')'");
    }
    std::string token = m_tokens.front();
    m_tokens.pop_front();
    if (token == "(")
    {
        std::list<std::shared_ptr<Object>> list;
        while (m_tokens.front() != ")")
        {
            std::shared_ptr<Object> obj = parse_tokens();
            list.push_back(obj);
        }
        m_tokens.pop_front();
        return std::shared_ptr<Object>(new lisp::LinkedList(list));
    }
    if (token == ")")
    {
        throw lisp::Error("non-matching ')'");
    }
    return parse_atom(token);
}

std::shared_ptr<Object> Parser::parse()
{
    // add space to parens for easier parsing
    {
        for (auto pos = m_input.find("("); pos < m_input.length(); pos = m_input.find("(", pos + 2))
        {
            m_input.replace(pos, 1, "( ");
        }
        for (auto pos = m_input.find(")"); pos < m_input.length(); pos = m_input.find(")", pos + 2))
        {
            m_input.replace(pos, 1, ") ");
        }
    }

    // parse for tokens
    {
        for (std::string::iterator it = m_input.begin(); it != m_input.end(); ++it)
        {
            switch (m_mode)
            {
            case Mode::Default:
            {
                step_default(it);
                break;
            }
            case Mode::String:
            {
                step_string(it);
                break;
            }
            case Mode::EscapeChar:
            {
                step_escape_char(it);
                break;
            }
            }
        }
        if (!m_cur_token.empty())
        {
            m_tokens.push_back(m_cur_token);
            m_cur_token.clear();
        }
    }

    return parse_tokens();
}

}
