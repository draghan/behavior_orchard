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

    SECTION("Add node - negative")
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
        NodeMetadata::Id nonexisting_id = 15;
        REQUIRE_THROWS_AS(new_node_id = tree.add_node(new_node_type, nonexisting_id), std::out_of_range);
    }

    SECTION("Add multiple nodes")
    {
        /*
            Create nodes:
            .
            └── 1-sequence
                ├── 2-invert
                │   └── 3-condition
                ├── 4-action1
                └── 5-action2
         */
        auto tree = MetadataTree(NodeType::sequence);
        auto root_id = MetadataTree::root_id;
        auto invert_id = NodeMetadata::id_null;
        auto condition_id = NodeMetadata::id_null;
        auto action1_id = NodeMetadata::id_null;
        auto action2_id = NodeMetadata::id_null;
        REQUIRE_NOTHROW(invert_id = tree.add_node(NodeType::invert, root_id));
        REQUIRE_NOTHROW(condition_id = tree.add_node(NodeType::condition, invert_id));
        REQUIRE_NOTHROW(action1_id = tree.add_node(NodeType::action, root_id));
        REQUIRE_NOTHROW(action2_id = tree.add_node(NodeType::action, root_id));

        REQUIRE(invert_id > root_id);
        REQUIRE(condition_id > invert_id);
        REQUIRE(action1_id > condition_id);
        REQUIRE(action2_id > action1_id);

        NodeMetadataPtr root = nullptr;
        NodeMetadataPtr invert = nullptr;
        NodeMetadataPtr condition = nullptr;
        NodeMetadataPtr action1 = nullptr;
        NodeMetadataPtr action2 = nullptr;

        REQUIRE_NOTHROW(root = tree.get_node(root_id));
        REQUIRE_NOTHROW(invert = tree.get_node(invert_id));
        REQUIRE_NOTHROW(condition = tree.get_node(condition_id));
        REQUIRE_NOTHROW(action1 = tree.get_node(action1_id));
        REQUIRE_NOTHROW(action2 = tree.get_node(action2_id));

        REQUIRE(root->get_id() == root_id);
        REQUIRE(invert->get_id() == invert_id);
        REQUIRE(condition->get_id() == condition_id);
        REQUIRE(action1->get_id() == action1_id);
        REQUIRE(action2->get_id() == action2_id);

        REQUIRE(root->get_parent_id() == NodeMetadata::id_null);
        REQUIRE(invert->get_parent_id() == root_id);
        REQUIRE(condition->get_parent_id() == invert_id);
        REQUIRE(action1->get_parent_id() == root_id);
        REQUIRE(action2->get_parent_id() == root_id);

        REQUIRE(root->get_type() == NodeType::sequence);
        REQUIRE(invert->get_type() == NodeType::invert);
        REQUIRE(condition->get_type() == NodeType::condition);
        REQUIRE(action1->get_type() == NodeType::action);
        REQUIRE(action2->get_type() == NodeType::action);
    }

    SECTION("Get children of")
    {
        /*
            .
            └── 01-sequence
                ├── 02-invert
                │   └── 03-condition1
                ├── 04-selector
                │   ├── 05-loop
                │   │   └── 6-action1
                │   ├── 07-action2
                │   ├── 08-action3
                │   └── 09-condition2
                └── 10-action4_id
         */
        auto tree = MetadataTree(NodeType::sequence);
        auto sequence_id = MetadataTree::root_id;
        auto invert_id = tree.add_node(NodeType::invert, sequence_id);
        auto condition1_id = tree.add_node(NodeType::condition, invert_id);
        auto selector_id = tree.add_node(NodeType::selector, sequence_id);
        auto loop_id = tree.add_node(NodeType::loop, selector_id);
        auto action1_id = tree.add_node(NodeType::action, loop_id);
        auto action2_id = tree.add_node(NodeType::action, selector_id);
        auto action3_id = tree.add_node(NodeType::action, selector_id);
        auto condition2_id = tree.add_node(NodeType::condition, selector_id);
        auto action4_id = tree.add_node(NodeType::condition, sequence_id);

        std::vector<NodeMetadataPtr> children;
        REQUIRE_NOTHROW(children = tree.get_children_of(sequence_id));
        REQUIRE(children.size() == 3);
        REQUIRE(children[0] == tree.get_node(invert_id));
        REQUIRE(children[1] == tree.get_node(selector_id));
        REQUIRE(children[2] == tree.get_node(action4_id));

        REQUIRE_NOTHROW(children = tree.get_children_of(action4_id));
        REQUIRE(children.empty());

        REQUIRE_NOTHROW(children = tree.get_children_of(invert_id));
        REQUIRE(children.size() == 1);
        REQUIRE(children[0] == tree.get_node(condition1_id));

        REQUIRE_NOTHROW(children = tree.get_children_of(selector_id));
        REQUIRE(children.size() == 4);
        REQUIRE(children[0] == tree.get_node(loop_id));
        REQUIRE(children[1] == tree.get_node(action2_id));
        REQUIRE(children[2] == tree.get_node(action3_id));
        REQUIRE(children[3] == tree.get_node(condition2_id));

        REQUIRE_THROWS_AS(tree.get_children_of(300), std::out_of_range);
    }

    SECTION("Get parent of")
    {
        /*
            .
            └── 01-sequence
                ├── 02-invert
                │   └── 03-condition1
                ├── 04-selector
                │   ├── 05-loop
                │   │   └── 6-action1
                │   ├── 07-action2
                │   ├── 08-action3
                │   └── 09-condition2
                └── 10-action4
         */
        auto tree = MetadataTree(NodeType::sequence);
        auto sequence_id = MetadataTree::root_id;
        auto invert_id = tree.add_node(NodeType::invert, sequence_id);
        auto condition1_id = tree.add_node(NodeType::condition, invert_id);
        auto selector_id = tree.add_node(NodeType::selector, sequence_id);
        auto loop_id = tree.add_node(NodeType::loop, selector_id);
        auto action1_id = tree.add_node(NodeType::action, loop_id);
        auto action2_id = tree.add_node(NodeType::action, selector_id);
        auto action3_id = tree.add_node(NodeType::action, selector_id);
        auto condition2_id = tree.add_node(NodeType::condition, selector_id);
        auto action4_id = tree.add_node(NodeType::condition, sequence_id);

        NodeMetadataPtr parent = nullptr;

        REQUIRE_NOTHROW(parent = tree.get_parent_of(sequence_id));
        REQUIRE(parent == nullptr);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(invert_id));
        REQUIRE(parent->get_id() == sequence_id);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(condition1_id));
        REQUIRE(parent->get_id() == invert_id);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(action1_id));
        REQUIRE(parent->get_id() == loop_id);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(condition2_id));
        REQUIRE(parent->get_id() == selector_id);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(action3_id));
        REQUIRE(parent->get_id() == selector_id);

        REQUIRE_NOTHROW(parent = tree.get_parent_of(action4_id));
        REQUIRE(parent->get_id() == sequence_id);

        REQUIRE_THROWS_AS(tree.get_parent_of(300), std::out_of_range);
    }

    SECTION("Get left sibling of")
    {
        /*
            .
            └── 01-sequence
                ├── 02-invert
                │   └── 03-condition1
                ├── 04-selector
                │   ├── 05-loop
                │   │   └── 6-action1
                │   ├── 07-action2
                │   ├── 08-action3
                │   └── 09-condition2
                └── 10-action4
         */
        auto tree = MetadataTree(NodeType::sequence);
        auto sequence_id = MetadataTree::root_id;
        auto invert_id = tree.add_node(NodeType::invert, sequence_id);
        auto condition1_id = tree.add_node(NodeType::condition, invert_id);
        auto selector_id = tree.add_node(NodeType::selector, sequence_id);
        auto loop_id = tree.add_node(NodeType::loop, selector_id);
        auto action1_id = tree.add_node(NodeType::action, loop_id);
        auto action2_id = tree.add_node(NodeType::action, selector_id);
        auto action3_id = tree.add_node(NodeType::action, selector_id);
        auto condition2_id = tree.add_node(NodeType::condition, selector_id);
        auto action4_id = tree.add_node(NodeType::condition, sequence_id);

        NodeMetadataPtr left_sibling = nullptr;

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(sequence_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(invert_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(condition1_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(selector_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == invert_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(loop_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(action1_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(action2_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == loop_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(action3_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == action2_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(condition2_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == action3_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_left_sibling_of(action4_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == selector_id);

        REQUIRE_THROWS_AS(tree.get_left_sibling_of(300), std::out_of_range);
    }

    SECTION("Get right sibling of")
    {
        /*
            .
            └── 01-sequence
                ├── 02-invert
                │   └── 03-condition1
                ├── 04-selector
                │   ├── 05-loop
                │   │   └── 6-action1
                │   ├── 07-action2
                │   ├── 08-action3
                │   └── 09-condition2
                └── 10-action4
         */
        auto tree = MetadataTree(NodeType::sequence);
        auto sequence_id = MetadataTree::root_id;
        auto invert_id = tree.add_node(NodeType::invert, sequence_id);
        auto condition1_id = tree.add_node(NodeType::condition, invert_id);
        auto selector_id = tree.add_node(NodeType::selector, sequence_id);
        auto loop_id = tree.add_node(NodeType::loop, selector_id);
        auto action1_id = tree.add_node(NodeType::action, loop_id);
        auto action2_id = tree.add_node(NodeType::action, selector_id);
        auto action3_id = tree.add_node(NodeType::action, selector_id);
        auto condition2_id = tree.add_node(NodeType::condition, selector_id);
        auto action4_id = tree.add_node(NodeType::condition, sequence_id);

        NodeMetadataPtr left_sibling = nullptr;

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(sequence_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(invert_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == selector_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(condition1_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(selector_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == action4_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(loop_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == action2_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(action1_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(action2_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == action3_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(action3_id));
        REQUIRE(left_sibling != nullptr);
        REQUIRE(left_sibling->get_id() == condition2_id);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(condition2_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_NOTHROW(left_sibling = tree.get_right_sibling_of(action4_id));
        REQUIRE(left_sibling == nullptr);

        REQUIRE_THROWS_AS(tree.get_right_sibling_of(300), std::out_of_range);
    }
}