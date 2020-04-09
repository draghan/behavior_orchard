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

#include "catch.hpp"
#include "../NodeMetadata.hpp"
#include "../MetadataTree.hpp"


TEST_CASE("NodeMetadata basics", "[NodeMetadata]")
{
    SECTION("Basic initialization")
    {
        NodeMetadata::Id parent_id = 1;
        NodeMetadata::Id current_id = 2;
        wxRealPoint position{15, 15};
        NodeType type = NodeType::action;

        auto node = NodeMetadata(type, parent_id, current_id, position);
        REQUIRE(node.get_id() == current_id);
        REQUIRE(node.get_parent_id() == parent_id);
        REQUIRE(node.get_type() == type);
        REQUIRE(node.position == position);
    }
}

TEST_CASE("MetadataTree basics", "[MetadataTree]")
{
    SECTION("Ensure initialization is not throwing")
    {
        REQUIRE_NOTHROW(MetadataTree(NodeType::max_n_tries));
    }

    SECTION("Basic initialization")
    {
        NodeType root_type = NodeType::sequence;
        auto tree = MetadataTree(root_type);
        auto root_node = tree.get_node(1);
        REQUIRE(root_node->get_id() == MetadataTree::root_id);
        REQUIRE(root_node->get_parent_id() == NodeMetadata::id_null);
        REQUIRE(root_node->get_type() == root_type);
        REQUIRE(root_node->position == wxRealPoint{0, 0});
    }

    SECTION("Add node - positive")
    {
        NodeType root_type = NodeType::sequence;
        auto tree = MetadataTree(root_type);
        auto root_node = tree.get_node(1);
        REQUIRE(root_node->get_id() == MetadataTree::root_id);
        REQUIRE(root_node->get_parent_id() == NodeMetadata::id_null);
        REQUIRE(root_node->get_type() == root_type);
        REQUIRE(root_node->position == wxRealPoint{0, 0});

        auto new_node_id = NodeMetadata::id_null;
        NodeType new_node_type = NodeType::action;
        REQUIRE_NOTHROW(new_node_id = tree.add_node(new_node_type, root_node->get_id()));

        NodeMetadataPtr new_node = nullptr;
        REQUIRE_NOTHROW(new_node = tree.get_node(new_node_id));
        REQUIRE(new_node->get_id() == new_node_id);
        REQUIRE(new_node->get_parent_id() == root_node->get_id());
        REQUIRE(new_node->get_type() == new_node_type);
        REQUIRE(new_node->position == wxRealPoint{0, 60});
    }
}