/*
    This file is distributed under MIT License.

    Copyright (c) 2020 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include "Node2Code.hpp"

Node2Code::Node2Code(std::string tree_identifier_)
        : tree_identifier{std::move(tree_identifier_)}
{
}

std::string Node2Code::convert(const NodeMetadata &node) const
{
    switch (node.get_type())
    {
        case NodeType::condition:
            return Node2Code::convert_condition(node);
        case NodeType::action:
            return Node2Code::convert_action(node);
        case NodeType::selector:
            return Node2Code::convert_selector(node);
        case NodeType::sequence:
            return Node2Code::convert_sequence(node);
        case NodeType::invert:
            return Node2Code::convert_invert(node);
        case NodeType::link:
            return Node2Code::convert_link(node);
        case NodeType::loop:
            return Node2Code::convert_loop(node);
        case NodeType::max_n_tries:
            return Node2Code::convert_max_n_tries(node);
    }
}

std::string Node2Code::convert_condition(const NodeMetadata &node) const
{
    if (!node.get_data())
    {
        throw std::logic_error("There is no data in the conditional NodeMetadata! Expected: string with predicate code.");
    }
    const auto data = node.get_data().value();
    if (!std::holds_alternative<std::string>(data))
    {
        throw std::logic_error("Conditional NodeMetadata contains wrong data type! Expected: string with predicate code.");
    }
    return this->tree_identifier + ".add_condition([&](){" + std::get<std::string>(data) + "});";
}

std::string Node2Code::convert_action(const NodeMetadata &node) const
{
    if (!node.get_data())
    {
        throw std::logic_error("There is no data in the action NodeMetadata! Expected: string with action code.");
    }
    const auto data = node.get_data().value();
    if (!std::holds_alternative<std::string>(data))
    {
        throw std::logic_error("Action NodeMetadata contains wrong data type! Expected: string with action code.");
    }
    return this->tree_identifier + ".add_action([&](){" + std::get<std::string>(data) + "});";
}

std::string Node2Code::convert_selector(const NodeMetadata &node) const
{
    if (node.get_data())
    {
        throw std::logic_error("There is some unexpected data in the selector NodeMetadata!");
    }
    return this->tree_identifier + ".add_selector();";
}

std::string Node2Code::convert_sequence(const NodeMetadata &node) const
{
    if (node.get_data())
    {
        throw std::logic_error("There is some unexpected data in the sequence NodeMetadata!");
    }
    return this->tree_identifier + ".add_sequence();";
}

std::string Node2Code::convert_invert(const NodeMetadata &node) const
{
    if (node.get_data())
    {
        throw std::logic_error("There is some unexpected data in the invert NodeMetadata!");
    }
    return this->tree_identifier + ".add_invert();";
}

std::string Node2Code::convert_link(const NodeMetadata &node) const
{
    if (!node.get_data())
    {
        throw std::logic_error("There is no data in the link NodeMetadata! Expected: string with identifier of pointer to BT.");
    }
    const auto data = node.get_data().value();
    if (!std::holds_alternative<std::string>(data))
    {
        throw std::logic_error("Link NodeMetadata contains wrong data type! Expected: string with identifier of pointer to BT.");
    }
    return this->tree_identifier + ".add_link(" + std::get<std::string>(data) + ");";
}

std::string Node2Code::convert_loop(const NodeMetadata &node) const
{
    if (!node.get_data())
    {
        throw std::logic_error("There is no data in the loop NodeMetadata! Expected: ulong with number of iterations.");
    }
    const auto data = node.get_data().value();
    if (!std::holds_alternative<unsigned long>(data))
    {
        throw std::logic_error("Loop NodeMetadata contains wrong data type! Expected: ulong with number of iterations.");
    }
    return this->tree_identifier + ".add_loop(" + std::to_string(std::get<unsigned long>(data)) + ");";
}

std::string Node2Code::convert_max_n_tries(const NodeMetadata &node) const
{
    if (!node.get_data())
    {
        throw std::logic_error("There is no data in the max-n-tries NodeMetadata! Expected: ulong with max number of attempts.");
    }
    const auto data = node.get_data().value();
    if (!std::holds_alternative<unsigned long>(data))
    {
        throw std::logic_error("Loop NodeMetadata contains wrong data type! Expected: ulong with max number of attempts.");
    }
    return this->tree_identifier + ".add_max_N_tries(" + std::to_string(std::get<unsigned long>(data)) + ");";
}




