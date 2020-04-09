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

#include <vector>
#include <memory>


using NodeMetadataPtr = NodeMetadata*;
using NodeMetadataOwningPtr = std::unique_ptr<NodeMetadata>;

class MetadataTree
{
public:
    inline static constexpr NodeMetadata::Id root_id = 1;
    explicit MetadataTree(NodeType root_type);

    NodeMetadata::Id add_node(NodeType type, NodeMetadata::Id parent_id);

    [[nodiscard]] std::vector<NodeMetadataPtr> get_children_of(NodeMetadata::Id id);
    [[nodiscard]] NodeMetadataPtr get_parent_of(NodeMetadata::Id id);
    [[nodiscard]] NodeMetadataPtr get_node(NodeMetadata::Id id);
    [[nodiscard]] NodeMetadataPtr get_left_sibling_of(NodeMetadata::Id id);
    [[nodiscard]] NodeMetadataPtr get_right_sibling_of(NodeMetadata::Id id);

private:
    std::vector<NodeMetadataOwningPtr> nodes;
    NodeMetadata::Id last_id = NodeMetadata::id_null;

    NodeMetadata::Id emplace_node(NodeType type, NodeMetadata::Id parent_id, wxRealPoint position);
};



