#ifndef B2DYNAMICTREE_DAYPZRBBUQHC_H
#define B2DYNAMICTREE_DAYPZRBBUQHC_H

#include <SFML/Graphics/Rect.hpp>

#include <float.h>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <stack>

namespace fck::b2
{

const float LENGHT_UNITS_PER_METR = 1.0f;
const float AABB_EXTENSION = 1.0f * LENGHT_UNITS_PER_METR;
const float AABB_MULTIPLIER = 4.0f;

struct RayCastInput
{
    sf::Vector2f p1;
    sf::Vector2f p2;
    float maxFraction;
};

struct RayCastOutput
{
    sf::Vector2f normal;
    float fraction;
};

struct AABB
{
    AABB();
    AABB(const sf::FloatRect &rect);

    sf::Vector2f center() const;
    sf::Vector2f extents() const;
    float perimeter() const;

    void combine(const AABB &aabb);
    void combine(const AABB &aabb1, const AABB &aabb2);

    bool contains(const AABB &aabb) const;

    sf::Vector2f lower_bound;
    sf::Vector2f upper_bound;
};

inline bool b2TestOverlap(const AABB &a, const AABB &b)
{
    sf::Vector2f d1, d2;
    d1 = b.lower_bound - a.upper_bound;
    d2 = a.lower_bound - b.upper_bound;

    if (d1.x > 0.0f || d1.y > 0.0f)
        return false;

    if (d2.x > 0.0f || d2.y > 0.0f)
        return false;

    return true;
}

const int32_t NULL_TREE_NODE = -1;

/// A node in the dynamic tree. The client does not interact with this directly.
template<typename T>
struct TreeNode
{
    bool isLeaf() const
    {
        return child1 == NULL_TREE_NODE;
    }

    AABB aabb;

    T user_data;

    union
    {
        int32_t parent;
        int32_t next;
    };

    int32_t child1 = NULL_TREE_NODE;
    int32_t child2 = NULL_TREE_NODE;

    // leaf = 0, free node = -1
    int32_t height = 0;

    bool moved = false;
};

/// A dynamic AABB tree broad-phase, inspired by Nathanael Presson's btDbvt.
/// A dynamic tree arranges data in a binary tree to accelerate
/// queries such as volume queries and ray casts. Leafs are proxies
/// with an AABB. In the tree we expand the proxy AABB by b2_fatAABBFactor
/// so that the proxy AABB is bigger than the client object. This allows the client
/// object to move by small amounts without triggering a tree update.
///
/// Nodes are pooled and relocatable, so we use node indices rather than pointers.
template<typename T>
class DynamicTree
{
public:
    /// Constructing the tree initializes the node pool.
    DynamicTree();

    /// Destroy the tree, freeing the node pool.
    ~DynamicTree();

    /// Create a proxy. Provide a tight fitting AABB and a userData pointer.
    int32_t createProxy(const AABB &aabb, T user_data);

    /// Destroy a proxy. This asserts if the id is invalid.
    void destroyProxy(int32_t proxy_id);

    /// Move a proxy with a swepted AABB. If the proxy has moved outside of its fattened AABB,
    /// then the proxy is removed from the tree and re-inserted. Otherwise
    /// the function returns immediately.
    /// @return true if the proxy was re-inserted.
    bool moveProxy(int32_t proxy_id, const AABB &aabb1, const sf::Vector2f &displacement);

    /// Get proxy user data.
    /// @return the proxy user data or 0 if the id is invalid.
    T getUserData(int32_t proxy_id) const;

    bool wasMoved(int32_t proxy_id) const;
    void clearMoved(int32_t proxy_id);

    /// Get the fat AABB for a proxy.
    const AABB &getFatAABB(int32_t proxy_id) const;

    /// Query an AABB for overlapping proxies. The callback class
    /// is called for each proxy that overlaps the supplied AABB.
    void querry(const AABB &aabb, const std::function<bool(int32_t)> &callback);

    /// Ray-cast against the proxies in the tree. This relies on the callback
    /// to perform a exact ray-cast in the case were the proxy contains a shape.
    /// The callback also performs the any collision filtering. This has performance
    /// roughly equal to k * log(n), where k is the number of collisions and n is the
    /// number of proxies in the tree.
    /// @param input the ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
    /// @param callback a callback class that is called for each proxy that is hit by the ray.
    //template <typename T>
    //void RayCast(T* callback, const b2RayCastInput& input) const;

    /// Compute the height of the binary tree in O(N) time. Should not be
    /// called often.
    int32_t getHeight() const;

    /// Get the maximum balance of an node in the tree. The balance is the difference
    /// in height of the two children of a node.
    int32_t getMaxBalance() const;

    /// Get the ratio of the sum of the node areas to the root area.
    float getAreaRatio() const;

    /// Shift the world origin. Useful for large worlds.
    /// The shift formula is: position -= newOrigin
    /// @param newOrigin the new origin with respect to the old origin
    void shiftOrigin(const sf::Vector2f &new_origin);

private:
    int32_t allocateNode();
    void freeNode(int32_t node_id);

    void insertLeaf(int32_t leaf_id);
    void removeLeaf(int32_t leaf_id);

    int32_t balance(int32_t i_a);

    int32_t computeHeight() const;
    int32_t computeHeight(int32_t node_id) const;

private:
    int32_t m_root;

    TreeNode<T> *m_nodes;
    int32_t m_node_count;
    int32_t m_node_capacity;

    int32_t m_free_list;

    int32_t m_insertion_count;
};

template<typename T>
DynamicTree<T>::DynamicTree()
{
    m_root = NULL_TREE_NODE;

    m_node_capacity = 16;
    m_node_count = 0;
    m_nodes = (TreeNode<T> *)malloc(m_node_capacity * sizeof(TreeNode<T>));
    memset(m_nodes, 0, m_node_capacity * sizeof(TreeNode<T>));

    // Build a linked list for the free list.
    for (int32_t i = 0; i < m_node_capacity - 1; ++i)
    {
        m_nodes[i].next = i + 1;
        m_nodes[i].height = -1;
    }
    m_nodes[m_node_capacity - 1].next = NULL_TREE_NODE;
    m_nodes[m_node_capacity - 1].height = -1;
    m_free_list = 0;

    m_insertion_count = 0;
}

template<typename T>
DynamicTree<T>::~DynamicTree()
{
    // This frees the entire tree in one shot.
    free(m_nodes);
}

// Create a proxy in the tree as a leaf node. We return the index
// of the node instead of a pointer so that we can grow
// the node pool.
template<typename T>
int32_t DynamicTree<T>::createProxy(const AABB &aabb, T user_data)
{
    int32_t proxy_id = allocateNode();

    // Fatten the aabb.
    sf::Vector2f r(AABB_EXTENSION, AABB_EXTENSION);
    m_nodes[proxy_id].aabb.lower_bound = aabb.lower_bound - r;
    m_nodes[proxy_id].aabb.upper_bound = aabb.upper_bound + r;
    m_nodes[proxy_id].user_data = user_data;
    m_nodes[proxy_id].height = 0;
    m_nodes[proxy_id].moved = true;

    insertLeaf(proxy_id);

    return proxy_id;
}

template<typename T>
void DynamicTree<T>::destroyProxy(int32_t proxy_id)
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);
    assert(m_nodes[proxy_id].isLeaf());

    removeLeaf(proxy_id);
    freeNode(proxy_id);
}

template<typename T>
bool DynamicTree<T>::moveProxy(int32_t proxy_id, const AABB &aabb, const sf::Vector2f &displacement)
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);

    assert(m_nodes[proxy_id].isLeaf());

    // Extend AABB
    AABB fat_aabb;
    sf::Vector2f r(AABB_EXTENSION, AABB_EXTENSION);
    fat_aabb.lower_bound = aabb.lower_bound - r;
    fat_aabb.upper_bound = aabb.upper_bound + r;

    // Predict AABB movement
    sf::Vector2f d = AABB_MULTIPLIER * displacement;

    if (d.x < 0.0f)
    {
        fat_aabb.lower_bound.x += d.x;
    }
    else
    {
        fat_aabb.upper_bound.x += d.x;
    }

    if (d.y < 0.0f)
    {
        fat_aabb.lower_bound.y += d.y;
    }
    else
    {
        fat_aabb.upper_bound.y += d.y;
    }

    const AABB &tree_aabb = m_nodes[proxy_id].aabb;
    if (tree_aabb.contains(aabb))
    {
        // The tree AABB still contains the object, but it might be too large.
        // Perhaps the object was moving fast but has since gone to sleep.
        // The huge AABB is larger than the new fat AABB.
        AABB huge_aabb;
        huge_aabb.lower_bound = fat_aabb.lower_bound - 4.0f * r;
        huge_aabb.upper_bound = fat_aabb.upper_bound + 4.0f * r;

        if (huge_aabb.contains(tree_aabb))
        {
            // The tree AABB contains the object AABB and the tree AABB is
            // not too large. No tree update needed.
            return false;
        }

        // Otherwise the tree AABB is huge and needs to be shrunk
    }

    removeLeaf(proxy_id);

    m_nodes[proxy_id].aabb = fat_aabb;

    insertLeaf(proxy_id);

    m_nodes[proxy_id].moved = true;

    return true;
}

template<typename T>
T DynamicTree<T>::getUserData(int32_t proxy_id) const
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);
    return m_nodes[proxy_id].user_data;
}

template<typename T>
bool DynamicTree<T>::wasMoved(int32_t proxy_id) const
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);
    return m_nodes[proxy_id].moved;
}

template<typename T>
void DynamicTree<T>::clearMoved(int32_t proxy_id)
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);
    m_nodes[proxy_id].moved = false;
}

template<typename T>
const AABB &DynamicTree<T>::getFatAABB(int32_t proxy_id) const
{
    assert(0 <= proxy_id && proxy_id < m_node_capacity);
    return m_nodes[proxy_id].aabb;
}

template<typename T>
void DynamicTree<T>::querry(const AABB &aabb, const std::function<bool(int32_t)> &callback)
{
    //b2GrowableStack<int32, 256> stack;
    std::stack<int32_t> stack;

    stack.push(m_root);

    while (stack.size() > 0)
    {
        int32_t node_id = stack.top();
        stack.pop();

        if (node_id == NULL_TREE_NODE)
        {
            continue;
        }

        const TreeNode<T> *node = m_nodes + node_id;

        if (b2TestOverlap(node->aabb, aabb))
        {
            if (node->isLeaf())
            {
                bool proceed = callback(node_id);
                if (proceed == false)
                {
                    return;
                }
            }
            else
            {
                stack.push(node->child1);
                stack.push(node->child2);
            }
        }
    }
}

template<typename T>
int32_t DynamicTree<T>::getHeight() const
{
    if (m_root == NULL_TREE_NODE)
    {
        return 0;
    }

    return m_nodes[m_root].height;
}

template<typename T>
int32_t DynamicTree<T>::getMaxBalance() const
{
    int32_t max_balance = 0;
    for (int32_t i = 0; i < m_node_capacity; ++i)
    {
        const TreeNode<T> *node = m_nodes + i;
        if (node->height <= 1)
        {
            continue;
        }

        assert(node->isLeaf() == false);

        int32_t child1 = node->child1;
        int32_t child2 = node->child2;
        int32_t balance = std::abs(m_nodes[child2].height - m_nodes[child1].height);
        max_balance = std::max(max_balance, balance);
    }

    return max_balance;
}

template<typename T>
float DynamicTree<T>::getAreaRatio() const
{
    if (m_root == NULL_TREE_NODE)
    {
        return 0.0f;
    }

    const TreeNode<T> *root = m_nodes + m_root;
    float root_area = root->aabb.perimeter();

    float total_area = 0.0f;
    for (int32_t i = 0; i < m_node_capacity; ++i)
    {
        const TreeNode<T> *node = m_nodes + i;
        if (node->height < 0)
        {
            // Free node in pool
            continue;
        }

        total_area += node->aabb.perimeter();
    }

    return total_area / root_area;
}

template<typename T>
void DynamicTree<T>::shiftOrigin(const sf::Vector2f &new_origin)
{
    // Build array of leaves. Free the rest.
    for (int32_t i = 0; i < m_node_capacity; ++i)
    {
        m_nodes[i].aabb.lower_bound -= new_origin;
        m_nodes[i].aabb.upper_bound -= new_origin;
    }
}

// Allocate a node from the pool. Grow the pool if necessary.
template<typename T>
int32_t DynamicTree<T>::allocateNode()
{
    // Expand the node pool as needed.
    if (m_free_list == NULL_TREE_NODE)
    {
        assert(m_node_count == m_node_capacity);

        // The free list is empty. Rebuild a bigger pool.
        TreeNode<T> *old_nodes = m_nodes;
        m_node_capacity *= 2;
        m_nodes = (TreeNode<T> *)malloc(m_node_capacity * sizeof(TreeNode<T>));
        memcpy(m_nodes, old_nodes, m_node_count * sizeof(TreeNode<T>));
        free(old_nodes);

        // Build a linked list for the free list. The parent
        // pointer becomes the "next" pointer.
        for (int32_t i = m_node_count; i < m_node_capacity - 1; ++i)
        {
            m_nodes[i].next = i + 1;
            m_nodes[i].height = -1;
        }
        m_nodes[m_node_capacity - 1].next = NULL_TREE_NODE;
        m_nodes[m_node_capacity - 1].height = -1;
        m_free_list = m_node_count;
    }

    // Peel a node off the free list.
    int32_t node_id = m_free_list;
    m_free_list = m_nodes[node_id].next;
    m_nodes[node_id].parent = NULL_TREE_NODE;
    m_nodes[node_id].child1 = NULL_TREE_NODE;
    m_nodes[node_id].child2 = NULL_TREE_NODE;
    m_nodes[node_id].height = 0;
    m_nodes[node_id].user_data = T();
    m_nodes[node_id].moved = false;
    ++m_node_count;
    return node_id;
}

// Return a node to the pool.
template<typename T>
void DynamicTree<T>::freeNode(int32_t node_id)
{
    assert(0 <= node_id && node_id < m_node_capacity);
    assert(0 < m_node_count);
    m_nodes[node_id].next = m_free_list;
    m_nodes[node_id].height = -1;
    m_free_list = node_id;
    --m_node_count;
}

template<typename T>
void DynamicTree<T>::insertLeaf(int32_t leaf_id)
{
    ++m_insertion_count;

    if (m_root == NULL_TREE_NODE)
    {
        m_root = leaf_id;
        m_nodes[m_root].parent = NULL_TREE_NODE;
        return;
    }

    // Find the best sibling for this node
    AABB leaf_aabb = m_nodes[leaf_id].aabb;
    int32_t index = m_root;
    while (m_nodes[index].isLeaf() == false)
    {
        int32_t child1 = m_nodes[index].child1;
        int32_t child2 = m_nodes[index].child2;

        float area = m_nodes[index].aabb.perimeter();

        AABB combined_aabb;
        combined_aabb.combine(m_nodes[index].aabb, leaf_aabb);
        float combined_area = combined_aabb.perimeter();

        // Cost of creating a new parent for this node and the new leaf
        float cost = 2.0f * combined_area;

        // Minimum cost of pushing the leaf further down the tree
        float inheritance_cost = 2.0f * (combined_area - area);

        // Cost of descending into child1
        float cost1;
        if (m_nodes[child1].isLeaf())
        {
            AABB aabb;
            aabb.combine(leaf_aabb, m_nodes[child1].aabb);
            cost1 = aabb.perimeter() + inheritance_cost;
        }
        else
        {
            AABB aabb;
            aabb.combine(leaf_aabb, m_nodes[child1].aabb);
            float oldArea = m_nodes[child1].aabb.perimeter();
            float newArea = aabb.perimeter();
            cost1 = (newArea - oldArea) + inheritance_cost;
        }

        // Cost of descending into child2
        float cost2;
        if (m_nodes[child2].isLeaf())
        {
            AABB aabb;
            aabb.combine(leaf_aabb, m_nodes[child2].aabb);
            cost2 = aabb.perimeter() + inheritance_cost;
        }
        else
        {
            AABB aabb;
            aabb.combine(leaf_aabb, m_nodes[child2].aabb);
            float oldArea = m_nodes[child2].aabb.perimeter();
            float newArea = aabb.perimeter();
            cost2 = newArea - oldArea + inheritance_cost;
        }

        // Descend according to the minimum cost.
        if (cost < cost1 && cost < cost2)
        {
            break;
        }

        // Descend
        if (cost1 < cost2)
        {
            index = child1;
        }
        else
        {
            index = child2;
        }
    }

    int32_t sibling = index;

    // Create a new parent.
    int32_t old_parent = m_nodes[sibling].parent;
    int32_t new_parent = allocateNode();
    m_nodes[new_parent].parent = old_parent;
    m_nodes[new_parent].user_data = T();
    m_nodes[new_parent].aabb.combine(leaf_aabb, m_nodes[sibling].aabb);
    m_nodes[new_parent].height = m_nodes[sibling].height + 1;

    if (old_parent != NULL_TREE_NODE)
    {
        // The sibling was not the root.
        if (m_nodes[old_parent].child1 == sibling)
        {
            m_nodes[old_parent].child1 = new_parent;
        }
        else
        {
            m_nodes[old_parent].child2 = new_parent;
        }

        m_nodes[new_parent].child1 = sibling;
        m_nodes[new_parent].child2 = leaf_id;
        m_nodes[sibling].parent = new_parent;
        m_nodes[leaf_id].parent = new_parent;
    }
    else
    {
        // The sibling was the root.
        m_nodes[new_parent].child1 = sibling;
        m_nodes[new_parent].child2 = leaf_id;
        m_nodes[sibling].parent = new_parent;
        m_nodes[leaf_id].parent = new_parent;
        m_root = new_parent;
    }

    // Walk back up the tree fixing heights and AABBs
    index = m_nodes[leaf_id].parent;
    while (index != NULL_TREE_NODE)
    {
        index = balance(index);

        int32_t child1 = m_nodes[index].child1;
        int32_t child2 = m_nodes[index].child2;

        assert(child1 != NULL_TREE_NODE);
        assert(child2 != NULL_TREE_NODE);

        m_nodes[index].height = 1 + std::max(m_nodes[child1].height, m_nodes[child2].height);
        m_nodes[index].aabb.combine(m_nodes[child1].aabb, m_nodes[child2].aabb);

        index = m_nodes[index].parent;
    }
}

template<typename T>
void DynamicTree<T>::removeLeaf(int32_t leaf_id)
{
    if (leaf_id == m_root)
    {
        m_root = NULL_TREE_NODE;
        return;
    }

    int32_t parent = m_nodes[leaf_id].parent;
    int32_t grand_parent = m_nodes[parent].parent;
    int32_t sibling;
    if (m_nodes[parent].child1 == leaf_id)
    {
        sibling = m_nodes[parent].child2;
    }
    else
    {
        sibling = m_nodes[parent].child1;
    }

    if (grand_parent != NULL_TREE_NODE)
    {
        // Destroy parent and connect sibling to grandParent.
        if (m_nodes[grand_parent].child1 == parent)
        {
            m_nodes[grand_parent].child1 = sibling;
        }
        else
        {
            m_nodes[grand_parent].child2 = sibling;
        }
        m_nodes[sibling].parent = grand_parent;
        freeNode(parent);

        // Adjust ancestor bounds.
        int32_t index = grand_parent;
        while (index != NULL_TREE_NODE)
        {
            index = balance(index);

            int32_t child1 = m_nodes[index].child1;
            int32_t child2 = m_nodes[index].child2;

            m_nodes[index].aabb.combine(m_nodes[child1].aabb, m_nodes[child2].aabb);
            m_nodes[index].height = 1 + std::max(m_nodes[child1].height, m_nodes[child2].height);

            index = m_nodes[index].parent;
        }
    }
    else
    {
        m_root = sibling;
        m_nodes[sibling].parent = NULL_TREE_NODE;
        freeNode(parent);
    }
}

// Perform a left or right rotation if node A is imbalanced.
// Returns the new root index.
template<typename T>
int32_t DynamicTree<T>::balance(int32_t i_a)
{
    assert(i_a != NULL_TREE_NODE);

    TreeNode<T> *a = m_nodes + i_a;
    if (a->isLeaf() || a->height < 2)
    {
        return i_a;
    }

    int32_t i_b = a->child1;
    int32_t i_c = a->child2;
    assert(0 <= i_b && i_b < m_node_capacity);
    assert(0 <= i_c && i_c < m_node_capacity);

    TreeNode<T> *b = m_nodes + i_b;
    TreeNode<T> *c = m_nodes + i_c;

    int32_t balance = c->height - b->height;

    // Rotate C up
    if (balance > 1)
    {
        int32_t i_f = c->child1;
        int32_t i_g = c->child2;
        TreeNode<T> *f = m_nodes + i_f;
        TreeNode<T> *g = m_nodes + i_g;
        assert(0 <= i_f && i_f < m_node_capacity);
        assert(0 <= i_g && i_g < m_node_capacity);

        // Swap A and C
        c->child1 = i_a;
        c->parent = a->parent;
        a->parent = i_c;

        // A's old parent should point to C
        if (c->parent != NULL_TREE_NODE)
        {
            if (m_nodes[c->parent].child1 == i_a)
            {
                m_nodes[c->parent].child1 = i_c;
            }
            else
            {
                assert(m_nodes[c->parent].child2 == i_a);
                m_nodes[c->parent].child2 = i_c;
            }
        }
        else
        {
            m_root = i_c;
        }

        // Rotate
        if (f->height > g->height)
        {
            c->child2 = i_f;
            a->child2 = i_g;
            g->parent = i_a;
            a->aabb.combine(b->aabb, g->aabb);
            c->aabb.combine(a->aabb, f->aabb);

            a->height = 1 + std::max(b->height, g->height);
            c->height = 1 + std::max(a->height, f->height);
        }
        else
        {
            c->child2 = i_g;
            a->child2 = i_f;
            f->parent = i_a;
            a->aabb.combine(b->aabb, f->aabb);
            c->aabb.combine(a->aabb, g->aabb);

            a->height = 1 + std::max(b->height, f->height);
            c->height = 1 + std::max(a->height, g->height);
        }

        return i_c;
    }

    // Rotate B up
    if (balance < -1)
    {
        int32_t i_d = b->child1;
        int32_t i_e = b->child2;
        TreeNode<T> *d = m_nodes + i_d;
        TreeNode<T> *e = m_nodes + i_e;
        assert(0 <= i_d && i_d < m_node_capacity);
        assert(0 <= i_e && i_e < m_node_capacity);

        // Swap A and B
        b->child1 = i_a;
        b->parent = a->parent;
        a->parent = i_b;

        // A's old parent should point to B
        if (b->parent != NULL_TREE_NODE)
        {
            if (m_nodes[b->parent].child1 == i_a)
            {
                m_nodes[b->parent].child1 = i_b;
            }
            else
            {
                assert(m_nodes[b->parent].child2 == i_a);
                m_nodes[b->parent].child2 = i_b;
            }
        }
        else
        {
            m_root = i_b;
        }

        // Rotate
        if (d->height > e->height)
        {
            b->child2 = i_d;
            a->child1 = i_e;
            e->parent = i_a;
            a->aabb.combine(c->aabb, e->aabb);
            b->aabb.combine(a->aabb, d->aabb);

            a->height = 1 + std::max(c->height, e->height);
            b->height = 1 + std::max(a->height, d->height);
        }
        else
        {
            b->child2 = i_e;
            a->child1 = i_d;
            d->parent = i_a;
            a->aabb.combine(c->aabb, d->aabb);
            b->aabb.combine(a->aabb, e->aabb);

            a->height = 1 + std::max(c->height, d->height);
            b->height = 1 + std::max(a->height, e->height);
        }

        return i_b;
    }

    return i_a;
}

// Compute the height of a sub-tree.
template<typename T>
int32_t DynamicTree<T>::computeHeight(int32_t node_id) const
{
    assert(0 <= node_id && node_id < m_node_capacity);
    TreeNode<T> *node = m_nodes + node_id;

    if (node->isLeaf())
    {
        return 0;
    }

    int32_t height1 = computeHeight(node->child1);
    int32_t height2 = computeHeight(node->child2);
    return 1 + std::max(height1, height2);
}

template<typename T>
int32_t DynamicTree<T>::computeHeight() const
{
    int32_t height = computeHeight(m_root);
    return height;
}

} // namespace fck::b2

#endif // B2DYNAMICTREE_DAYPZRBBUQHC_H
