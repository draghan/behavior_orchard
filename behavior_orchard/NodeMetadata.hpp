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

#include <wx/gdicmn.h>


enum class NodeType
{
    condition,
    action,
    selector,
    sequence,
    invert,
    link,
    loop,
    max_n_tries
};

class NodeMetadata
{
public:
    using Id = unsigned long long;
    inline static const Id id_null{0};
    inline static const wxSize node_item_size{80, 40};

    NodeMetadata(NodeType type_, Id parent_id_, Id id_, wxRealPoint position_);

    [[nodiscard]] Id get_parent_id() const;
    [[nodiscard]] Id get_id() const;
    [[nodiscard]] NodeType get_type() const;

    wxRealPoint position;
private:
    const Id id;
    const Id parent_id;
    const NodeType type;
};