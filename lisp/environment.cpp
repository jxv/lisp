#include "environment.h"
#include "error.h"
#include "nil.h"

namespace lisp
{

Environment::Environment()
    : m_parent(nullptr)
{
}

Environment::Environment(std::shared_ptr<Environment> parent)
    : m_parent(parent)
{
}

std::shared_ptr<Object> &Environment::operator[] (const std::string &key)
{
    return m_map[key];
}

std::shared_ptr<Object> Environment::get(const std::string &key) const
{
    try
    {
        return m_map.at(key);
    }
    catch (const std::out_of_range&)
    {
    }
    std::shared_ptr<Object> obj = Nil::get();
    for (auto env = m_parent; env != nullptr; env = env->m_parent)
    {
        try
        {
            return env->m_map.at(key);
        }
        catch (const std::out_of_range&)
        {
        }
    }
    return obj;
}

std::shared_ptr<Object> Environment::set(const std::string &key, std::shared_ptr<Object> obj)
{
    if (m_map.count(key) == 0)
    {
        throw Error(key + " must already be bounded");
    }
    m_map.erase(key);
    m_map.insert(std::pair<std::string, std::shared_ptr<Object>>(key, obj));
    return obj;
}

std::shared_ptr<Object> Environment::define(const std::string &key, std::shared_ptr<Object> obj)
{
    // If the key exists, overwrite the definition
    if (m_map.count(key) > 0)
    {
        m_map.erase(key);
    }
    m_map.insert(std::pair<std::string, std::shared_ptr<Object>>(key, obj));
    return obj;
}

}
