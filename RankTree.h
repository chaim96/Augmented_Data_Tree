#ifndef CODE_RANKTREE_H
#define CODE_RANKTREE_H
#include "AVLTree.h"


//class for tree nodes
template <class ptr_type>
class RankNode
{
public:
    ptr_type* data;
    int height;
    int rank;
    RankNode* left;
    RankNode* right;
    RankNode(ptr_type* data_to_copy) : data(data_to_copy), height(0), rank(1), left(nullptr), right(nullptr) {}
    RankNode() = default;
};


//overall class for AVL Rank tree
template <class ptr_type, class condition>
class RankTree
{
private:
    // - sub function for insert: adds a layer for passing root
    RankNode<ptr_type>* insert_node(RankNode<ptr_type>*& r, ptr_type* data, RankNode<ptr_type>*& r_new_junction);

    // - sub function for insert: balance the tree with proper rotations
    RankNode<ptr_type>* balance_tree(RankNode<ptr_type>*& r);

    // -- sub function for balance: makes an RR rotation
    RankNode<ptr_type>* make_RR_rotation(RankNode<ptr_type>*& r);

    // -- sub function for balance: makes an LL rotation
    RankNode<ptr_type>* make_LL_rotation(RankNode<ptr_type>*& r);

    // -- sub function for balance: makes an RL rotation
    RankNode<ptr_type>* make_RL_rotation(RankNode<ptr_type>*& r);

    // -- sub function for balance: makes an LR rotation
    RankNode<ptr_type>* make_LR_rotation(RankNode<ptr_type>*& r);

    // -- sub function for balance: calculates balance factor of node
    int get_bf(RankNode<ptr_type>*& r);

    // - sub function for insert: updates height of node
    void update_height(RankNode<ptr_type>*& r);

    // - sub function for insert: updates rank of node
    void update_rank(RankNode<ptr_type>*& r);

    // - sub function for remove: adds a layer for passing root and result of operation
    RankNode<ptr_type>* remove_node(RankNode<ptr_type>*& r, ptr_type* data, bool*& result, bool erase);

    // - sub function for remove: finds a successor for removed node
    RankNode<ptr_type>* find_successor(RankNode<ptr_type>* b);

    // - sub function for search: adds a layer for passing root and result of operation
    RankNode<ptr_type>* search_node(RankNode<ptr_type>*& r, ptr_type* data, RankNode<ptr_type>*& requested);

    // - sub function for search_by_index: adds a layer for passing root and result of operation
    ptr_type* retrieve_by_index(RankNode<ptr_type> *&r, int index);

    // - sub function for search: adds a layer for passing root and result of operation
    void erase_data_in_node(RankNode<ptr_type>*& r);

    // - sub function for destructor: frees all nodes (without freeing the data in every node)
    void destructor(RankNode<ptr_type>*& root);

    // - sub function for inorder: adds layer of root
    void inorder_travel(RankNode<ptr_type>* r, ptr_type**& elements_by_order, int*& index);

    // - sub function for get_max_node: returns max node for any tree that starts with a given root
    RankNode<ptr_type>* get_max_node_by_root(RankNode<ptr_type>* given_root);

    // - sub function for get_min_node: returns min node for any tree that starts with a given root
    RankNode<ptr_type>* get_min_node_by_root(RankNode<ptr_type>* given_root);

    // - sub function for get_closest_left and get_closest_right: returns father of key object, returns nullptr if not
    RankNode<ptr_type>* get_father(RankNode<ptr_type>* r, ptr_type* data);

    // -- sub function for get_father: confirms if passed node to function is father
    bool check_if_father(RankNode<ptr_type>* r, ptr_type* data);

    // -- sub function for build_from_array: constructs the tree from array
    RankNode<ptr_type>* build_tree_from_array(ptr_type** array, int start, int end);

    RankNode<ptr_type>* root;
    int num_of_nodes;

    static const int EMPTY_TREE = -1;
    static const int UNBALANCED_POSITIVE_BF = 2;
    static const int UNBALANCED_NEGATIVE_BF = -2;

public:
    // constructor
    RankTree() : root(nullptr), num_of_nodes(0) {}

    // builds tree from sorted array without duplicates
    void build_from_array(ptr_type** data_array, int size);

    // returns how many nodes the tree consists
    int get_num_of_nodes();

    // returns the height of the tree
    int get_tree_height();

    /** returns pointer to the tree's max node
     * returns nullptr - if tree is empty
     */
    RankNode<ptr_type>* get_max_node();

    /** inserts a new node to the tree
     * receives &address of new object created
     * returns pointer to node created
     * returns nullptr - if node already exists
     */
    RankNode<ptr_type>* insert(ptr_type* data);

    /** removes the node that points to 'data'
     * returns true - if node is found and removed
     * returns false - if node doesn't exist
     */
    bool remove(ptr_type* data);

    /** removes the node that points to 'data' and calls its destructor
     * returns true - if node is found and removed, and data is erased
     * returns false - if node doesn't exist
     */
    bool remove_and_erase(ptr_type* data);

    /** returns a pointer to node
     *  returns nullptr - if node doesn't exist
     */
    RankNode<ptr_type>* search(ptr_type* data);

    /** returns a pointer to element with the given index when sorting elements by size
     *  returns nullptr - if such index doesn't exist
     */
    ptr_type* search_by_index(int index);

    /**
     * returns a pointer to the closest left neighbor node (smaller then the node)
     * returns nullptr - if doesn't exist
     */
    RankNode<ptr_type>* get_closest_left(ptr_type* data);

    /**
    * returns a pointer to the closest left neighbor node (smaller then the node)
    * returns nullptr - if doesn't exist
    */
    RankNode<ptr_type>* get_closest_right(ptr_type* data);

    // returns an array with pointers to nodes' data, by order of template condition
    ptr_type** inorder();

    /** calls for the destructor of the data pointed to at every node
     * does not remove the node itself (that's the destructors job)
     */
    void erase_data();

    // destructor for the tree - DOES NOT erase the data pointed to
    ~RankTree();

};


/******************************************************* build tree from array functions *******************************************************/


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::build_from_array(ptr_type **data_array, int size)
{
    if (size < 1 || data_array == nullptr || root != nullptr)
    {
        return;
    }
    root = build_tree_from_array(data_array, 0, size-1);
    num_of_nodes = size;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::build_tree_from_array(ptr_type** array, int start, int end)
{
    if (start > end)
    {
        return nullptr;
    }
    int mid = (start + end) / 2;
    RankNode<ptr_type> *r = new RankNode<ptr_type>(array[mid]);
    r->left = build_tree_from_array(array, start, mid - 1);
    r->right = build_tree_from_array(array, mid + 1, end);
    update_height(r);
    update_rank(r);
    return r;
}


/******************************************************* tree details functions *******************************************************/


template <class ptr_type, class condition>
int RankTree<ptr_type, condition>::get_tree_height()
{
    if (root->right == nullptr && root->left == nullptr)
    {
        return EMPTY_TREE;
    }
    return root->height;
}


template <class ptr_type, class condition>
int RankTree<ptr_type, condition>::get_num_of_nodes()
{
    return num_of_nodes;
}

template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_max_node()
{
    return get_max_node_by_root(root);
}

template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_max_node_by_root(RankNode<ptr_type>* given_root)
{
    RankNode<ptr_type>* r;
    if (given_root == nullptr)
    {
        return nullptr;
    }
    r = given_root;
    while(r->right != nullptr)
    {
        r = r->right;
    }
    return r;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_min_node_by_root(RankNode<ptr_type>* given_root)
{
    RankNode<ptr_type>* r;
    if (given_root == nullptr)
    {
        return nullptr;
    }
    r = given_root;
    while(r->left != nullptr)
    {
        r = r->left;
    }
    return r;
}


/******************************************************* balancing functions *******************************************************/


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::make_LL_rotation(RankNode<ptr_type>*& r)
{
    RankNode<ptr_type>* A = r->left;
    r->left = r->left->right;
    A->right = r;
    update_height(r);
    update_rank(r);
    update_height(A);
    update_rank(A);
    return A;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::make_RR_rotation(RankNode<ptr_type>*& r)
{
    RankNode<ptr_type>* A = r->right;
    r->right = r->right->left;
    A->left = r;
    update_height(r);
    update_rank(r);
    update_height(A);
    update_rank(A);
    return A;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::make_RL_rotation(RankNode<ptr_type>*& r)
{
    r->right = make_LL_rotation(r->right);
    update_height(r);
    update_rank(r);
    return make_RR_rotation(r);
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::make_LR_rotation(RankNode<ptr_type>*& r)
{
    r->left = make_RR_rotation(r->left);
    update_height(r);
    update_rank(r);
    return make_LL_rotation(r);
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::balance_tree(RankNode<ptr_type>*& r)
{
    int bf = get_bf(r);
    if (bf == UNBALANCED_POSITIVE_BF)
    {
        if (get_bf(r->left) >= 0)
        {
            r = make_LL_rotation(r);
            return r;
        }
        else
        {
            r = make_LR_rotation(r);
            return r;
        }
    }
    if (bf == UNBALANCED_NEGATIVE_BF)
    {
        if (get_bf(r->right) <= 0)
        {
            r = make_RR_rotation(r);
            return r;
        }
        else
        {
            r = make_RL_rotation(r);
            return r;
        }
    }
    return r;
}


template <class ptr_type, class condition>
int RankTree<ptr_type, condition>::get_bf(RankNode<ptr_type>*& r)
{
    if (r->left == nullptr && r->right != nullptr)
    {
        return (0 - 1 - r->right->height);
    }
    if (r->left != nullptr && r->right == nullptr)
    {
        return (r->left->height + 1);
    }
    if (r->left != nullptr && r->right != nullptr)
    {
        return (r->left->height - r->right->height);
    }
    return 0;
}


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::update_height(RankNode<ptr_type>*& r)
{
    if (r->left == nullptr && r->right != nullptr)
    {
        r->height = 1 + r->right->height;
    }
    if (r->left != nullptr && r->right == nullptr)
    {
        r->height = 1 + r->left->height;
    }
    if (r->left != nullptr && r->right != nullptr)
    {
        if (r->left->height > r->right->height)
        {
            r->height = r->left->height + 1;
        }
        else
        {
            r->height = r->right->height + 1;
        }
    }
    if (r->left == nullptr && r->right == nullptr)
    {
        r->height = 0;
    }
}



template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::update_rank(RankNode<ptr_type>*& r)
{
    if (r->left == nullptr && r->right != nullptr)
    {
        r->rank = 1 + r->right->rank;
    }
    if (r->left != nullptr && r->right == nullptr)
    {
        r->rank = 1 + r->left->rank;
    }
    if (r->left != nullptr && r->right != nullptr)
    {
        r->rank = 1 + r->left->rank + r->right->rank;
    }
    if (r->left == nullptr && r->right == nullptr)
    {
        r->rank = 1;
    }
}


/******************************************************* insert functions *******************************************************/


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::insert(ptr_type* data)
{
    RankNode<ptr_type>* r_new_junction = nullptr;
    root = insert_node(root, data, r_new_junction);
    return r_new_junction;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::insert_node(RankNode<ptr_type>*& r, ptr_type* data, RankNode<ptr_type>*& r_new_junction)
{
    if (r == nullptr)
    {
        r = new RankNode<ptr_type>(data);
        r_new_junction = r;
        num_of_nodes++;
        return r;
    }
    else
    {
        condition cond;
        Comparison result = cond(data, r->data);
        if (result == Comparison::LESS_THAN)
        {
            r->left = insert_node(r->left, data, r_new_junction);
            update_height(r);
            update_rank(r);
            return balance_tree(r);
        }
        if (result == Comparison::GREATER_THAN)
        {
            r->right = insert_node(r->right, data, r_new_junction);
            update_height(r);
            update_rank(r);
            return balance_tree(r);
        }
        if (result == Comparison::EQUAL)
        {
            r_new_junction = nullptr;
            return r;
        }
        return r;
    }
}


/******************************************************* search functions *******************************************************/


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::search(ptr_type* data)
{
    RankNode<ptr_type>* requested = nullptr;
    return search_node(root, data, requested);
}

template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::search_node(RankNode<ptr_type> *&r, ptr_type* data, RankNode<ptr_type> *&requested)
{
    if (r == nullptr)
    {
        return nullptr;
    }
    else
    {
        condition cond;
        RankNode<ptr_type>* temp;
        Comparison result = cond(data, r->data);
        if (result == Comparison::LESS_THAN)
        {
            temp = search_node(r->left, data, requested);
            if (temp != nullptr)
            {
                return temp;
            }
            return nullptr;
        }
        if (result == Comparison::GREATER_THAN)
        {
            temp = search_node(r->right, data, requested);
            if (temp != nullptr)
            {
                return temp;
            }
            return nullptr;
        }
        if (result == Comparison::EQUAL)
        {
            requested = r;
            return r;
        }
    }
    return nullptr;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_closest_left(ptr_type* data)
{
    RankNode<ptr_type>* requested = search(data);
    if (requested == nullptr)
    {
        return nullptr;
    }
    if (requested->left == nullptr)
    {
        condition cond;
        RankNode<ptr_type>* father = get_father(root, requested->data);
        while (father != nullptr)
        {
            Comparison result_cond = cond(data, father->data);
            if (result_cond == Comparison::GREATER_THAN)
            {
                return father;
            }
            if (result_cond == Comparison::LESS_THAN)
            {
                father = get_father(root, father->data);
            }
        }
        return nullptr;
    }
    if (requested->left != nullptr)
    {
        return get_max_node_by_root(requested->left);
    }
    return nullptr;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_closest_right(ptr_type* data)
{
    RankNode<ptr_type>* requested = search(data);
    if (requested == nullptr)
    {
        return nullptr;
    }
    if (requested->right == nullptr)
    {
        condition cond;
        RankNode<ptr_type>* father = get_father(root, requested->data);
        while (father != nullptr)
        {
            Comparison result_cond = cond(data, father->data);
            if (result_cond == Comparison::LESS_THAN)
            {
                return father;
            }
            if (result_cond == Comparison::GREATER_THAN)
            {
                father = get_father(root, father->data);
            }
        }
        return nullptr;
    }
    if (requested->right != nullptr)
    {
        return get_min_node_by_root(requested->right);
    }
    return nullptr;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::get_father(RankNode<ptr_type>* r, ptr_type* data)
{
    if (r == nullptr || data == root->data)
    {
        return nullptr;
    }
    else
    {
        condition cond;
        if (check_if_father(r, data))
        {
            return r;
        }
        if (cond(data, r->data) == Comparison::LESS_THAN)
        {
            return get_father(r->left, data);
        }
        if (cond(data, r->data) == Comparison::GREATER_THAN)
        {
            return get_father(r->right, data);
        }
    }
    return nullptr;
}


template <class ptr_type, class condition>
bool RankTree<ptr_type, condition>::check_if_father(RankNode<ptr_type>* r, ptr_type* data)
{
    condition cond;
    if (r->right != nullptr)
    {
        if (cond(data, r->right->data) == Comparison::EQUAL)
        {
            return true;
        }
    }
    if (r->left != nullptr)
    {
        if (cond(data, r->left->data) == Comparison::EQUAL)
        {
            return true;
        }
    }
    return false;
}


template <class ptr_type, class condition>
ptr_type* RankTree<ptr_type, condition>::search_by_index(int index)
{
    if (index > num_of_nodes || num_of_nodes == 0)
    {
        return nullptr;
    }
    return retrieve_by_index(root, index);
}


template <class ptr_type, class condition>
ptr_type* RankTree<ptr_type, condition>::retrieve_by_index(RankNode<ptr_type> *&r, int index)
{
    if ((r->left != nullptr && (index - r->left->rank - 1) == 0) || (r->left == nullptr && index-1 == 0))
    {
        return r->data;
    }
    if (r->left != nullptr && index - r->left->rank - 1 < 0)
    {
        return retrieve_by_index(r->left, index);
    }
    if (r->left != nullptr && index - r->left->rank - 1 > 0)
    {
        return retrieve_by_index(r->right, index - r->left->rank - 1);
    }
    if (r->left == nullptr && index-1 > 0)
    {
        return retrieve_by_index(r->right, index-1);
    }
    return nullptr;
}

/******************************************************* travel functions *******************************************************/


template <class ptr_type, class condition>
ptr_type** RankTree<ptr_type, condition>::inorder()
{
    if (root == nullptr)
    {
        return nullptr;
    }
    ptr_type** elements_by_order = new ptr_type*[num_of_nodes];
    int* i = new int(0);
    inorder_travel(root, elements_by_order, i);
    delete i;
    return elements_by_order;
}


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::inorder_travel(RankNode<ptr_type>* r, ptr_type**& elements_by_order, int*& index)
{
    if (r == nullptr)
    {
        return;
    }
    inorder_travel(r->left, elements_by_order, index);
    elements_by_order[*index] = r->data;
    (*index)++;
    inorder_travel(r->right, elements_by_order, index);
}


/******************************************************* removing functions *******************************************************/


template <class ptr_type, class condition>
bool RankTree<ptr_type, condition>::remove(ptr_type* data)
{
    bool* result = new bool();
    *result = false;
    remove_node(root, data, result, false);
    bool value = *result;
    delete result;
    if (value == true)
    {
        num_of_nodes--;
    }
    return value;
}


template <class ptr_type, class condition>
bool RankTree<ptr_type, condition>::remove_and_erase(ptr_type* data)
{
    bool* result = new bool();
    *result = false;
    remove_node(root, data, result, true);
    bool value = *result;
    delete result;
    if (value == true)
    {
        num_of_nodes--;
    }
    return value;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::find_successor(RankNode<ptr_type>* b)
{
    b = b->left;
    while(b->right != nullptr)
    {
        b = b->right;
    }
    return b;
}


template <class ptr_type, class condition>
RankNode<ptr_type>* RankTree<ptr_type, condition>::remove_node(RankNode<ptr_type> *&r, ptr_type* data, bool*& result, bool erase)
{
    if (r == nullptr)
    {
        *result = *result + false;
        return nullptr;
    }
    else
    {
        condition cond;
        Comparison comp_result = cond(data, r->data);
        if (comp_result == Comparison::EQUAL)
        {
            if (r->right == nullptr && r->left == nullptr) // junction is leaf
            {
                RankNode<ptr_type>* temp = r;
                if (erase)
                {
                    delete temp->data;
                }
                delete temp;
                r = nullptr;
                *result = true;
                return r;
            }
            if (r->right == nullptr && r->left != nullptr) // junction has only left child
            {
                RankNode<ptr_type>* temp = r;
                r = r->left;
                if (erase)
                {
                    delete temp->data;
                }
                delete temp;
                *result = true;
                return r;
            }
            if (r->right != nullptr && r->left == nullptr) // junction has only right child
            {
                RankNode<ptr_type>* temp = r;
                r = r->right;
                if (erase)
                {
                    delete temp->data;
                }
                delete temp;
                *result = true;
                return r;
            }
            if (r->right != nullptr && r->left != nullptr) // junction has both children
            {
                RankNode<ptr_type>* successor = find_successor(r);
                RankNode<ptr_type>* temp2;
                temp2 = r;
                if (erase)
                {
                    delete temp2->data;
                }
                r->data = successor->data;
                remove_node(r->left, successor->data, result, false);
                update_height(r);
                update_rank(r);
                *result = true;
                return balance_tree(r);
            }
        }
        if (comp_result == Comparison::LESS_THAN)
        {
            r->left = remove_node(r->left, data, result, erase);
            update_height(r);
            update_rank(r);
            return balance_tree(r);
        }
        if (comp_result == Comparison::GREATER_THAN)
        {
            r->right = remove_node(r->right, data, result, erase);
            update_height(r);
            update_rank(r);
            return balance_tree(r);
        }
    }
    return nullptr;
}


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::erase_data_in_node(RankNode<ptr_type>*& r)
{
    if (r == nullptr)
    {
        return;
    }
    erase_data_in_node(r->left);
    erase_data_in_node(r->right);
    r->height = 0;  // for elegance
    delete r->data;
}


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::erase_data()
{
    erase_data_in_node(root);
}


/******************************************************* destructor *******************************************************/


template <class ptr_type, class condition>
void RankTree<ptr_type, condition>::destructor(RankNode<ptr_type>*& r)
{
    if (r == nullptr)
    {
        return;
    }
    destructor(r->left);
    destructor(r->right);
    delete r;
}

template <class ptr_type, class condition>
RankTree<ptr_type, condition>::~RankTree()
{
    destructor(root);
}


#endif //CODE_RANKTREE_H
