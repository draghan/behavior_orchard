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

#pragma once

#include "NodeMetadata.hpp"

#include <string>


class Node2Code
{
public:
    explicit Node2Code(std::string tree_identifier_);

    [[nodiscard]] std::string convert(const NodeMetadata &node) const;

private:
    const std::string tree_identifier;
    [[nodiscard]] std::string convert_condition(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_action(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_selector(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_sequence(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_invert(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_link(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_loop(const NodeMetadata &node) const;
    [[nodiscard]] std::string convert_max_n_tries(const NodeMetadata &node) const;
};
