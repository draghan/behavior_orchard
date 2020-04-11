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

#include "MetadataTree.hpp"
#include <execution>


std::vector<NodeMetadataPtr> MetadataTree::get_children_of(NodeMetadata::Id id) const
{
    std::vector<NodeMetadataPtr> children;
    std::for_each(std::execution::seq, this->nodes.cbegin(), this->nodes.cend(), [&](const auto &node)
    {
        if (node->get_parent_id() == id)
        {
            children.push_back(node.get());
        }
    });

    if(children.empty())
    {
        const auto found = std::any_of(std::execution::par, this->nodes.cbegin(), this->nodes.cend(), [&](const auto &node)
        {
            return node->get_id() == id;
        });
        if (!found)
        {
            throw std::out_of_range{"There is no requested node in the pool!"};
        }
    }

    return children;
}

NodeMetadataPtr MetadataTree::get_node(NodeMetadata::Id id) const
{
    const auto found_node = std::find_if(std::execution::par, this->nodes.cbegin(), this->nodes.cend(), [&](const auto &node)
    {
        return node->get_id() == id;
    });

    if (found_node == this->nodes.cend())
    {
        return nullptr;
    }

    return found_node->get();
}

MetadataTree::MetadataTree(NodeType root_type)
{
    this->emplace_node(root_type, NodeMetadata::id_null, {0, 0});
}

NodeMetadata::Id MetadataTree::emplace_node(NodeType type, NodeMetadata::Id parent_id, wxRealPoint position)
{
    // precondition check if given parent exists - but do not check null parent
    if (parent_id != NodeMetadata::id_null)
    {
        const auto parent_found = std::any_of(std::execution::par, this->nodes.cbegin(), this->nodes
                                                                                       .cend(), [&](const auto &node)
                                        {
                                            return node->get_id() == parent_id;
                                        });

        if (!parent_found)
        {
            throw std::out_of_range{"There is no requested parent in the pool!"};
        }
    }

    ++this->last_id;
    this->nodes.emplace_back(std::make_unique<NodeMetadata>(type, parent_id, this->last_id, position));
    return this->last_id;
}

NodeMetadata::Id MetadataTree::add_node(NodeType type, NodeMetadata::Id parent_id)
{
    const auto parent = this->get_node(parent_id);
    if (parent == nullptr)
    {
        throw std::out_of_range{"There is no requested parent in the pool!"};
    }
    const auto siblings_number = static_cast<double>(this->get_number_of_children(parent_id));
    const wxRealPoint node_position{parent->position.x + siblings_number * 1.5 * NodeMetadata::node_item_size.GetX(), parent->position.y + 1.5 * NodeMetadata::node_item_size.GetY()};
    return this->emplace_node(type, parent_id, node_position);
}

NodeMetadataPtr MetadataTree::get_parent_of(NodeMetadata::Id id) const
{
    const auto wanted_node = this->get_node(id);
    if (wanted_node == nullptr)
    {
        throw std::out_of_range{"There is no requested node in the pool!"};
    }
    return this->get_node(wanted_node->get_parent_id());
}



NodeMetadataPtr MetadataTree::get_right_sibling_of(NodeMetadata::Id id) const
{
    const auto found_node = std::find_if(std::execution::par, this->nodes.cbegin(), this->nodes.cend(), [&](const auto &node)
    {
        return node->get_id() == id;
    });
    if (found_node == this->nodes.cend())
    {
        throw std::out_of_range{"There is no requested node in the pool!"};
    }
    const auto parent_id = (*found_node)->get_parent_id();
    const auto first_child_of_parent = std::find_if(std::execution::seq, found_node + 1, this->nodes.cend(), [&](const auto &current_node)
                                              {
                                                  return current_node->get_parent_id() == parent_id;
                                              });

    if (first_child_of_parent == this->nodes.cend())
    {
        return nullptr;
    }
    return first_child_of_parent->get();
}

NodeMetadataPtr MetadataTree::get_left_sibling_of(NodeMetadata::Id id) const
{
    const auto found_node = std::find_if(std::execution::par, this->nodes.rbegin(), this->nodes.rend(), [&](const auto &node)
    {
        return node->get_id() == id;
    });
    if (found_node == this->nodes.rend())
    {
        throw std::out_of_range{"There is no requested node in the pool!"};
    }
    const auto parent_id = (*found_node)->get_parent_id();
    auto first_child_of_parent = std::find_if(std::execution::seq, found_node + 1, this->nodes.rend(), [&](const auto &current_node)
    {
        return current_node->get_parent_id() == parent_id;
    });

    if (first_child_of_parent == this->nodes.rend())
    {
        return nullptr;
    }
    return first_child_of_parent->get();
}

size_t MetadataTree::get_number_of_children(NodeMetadata::Id id) const
{
    const auto children_number = std::count_if(std::execution::par, this->nodes.cbegin(), this->nodes.cend(), [&](const auto& node)
    {
        return node->get_parent_id() == id;
    });

    if(children_number == 0)
    {
        const auto found = std::any_of(std::execution::par, this->nodes.cbegin(), this->nodes.cend(), [&](const auto &node)
        {
            return node->get_id() == id;
        });
        if (!found)
        {
            throw std::out_of_range{"There is no requested node in the pool!"};
        }
    }
    return static_cast<size_t>(children_number);
}
