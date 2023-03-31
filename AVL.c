
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <limits.h>
#include <time.h>

int Max(int a, int b)
{
    return (a > b) ? a : b ;
}

struct BSTNode // Binary Search Tree Node
{
        int val;
    
        int index;
        int index_even;
        int index_odd;
    
        int _dup_left_val;
        int *index_dup_left;
        
        int height;
    
        struct BSTNode *parent;

        struct BSTNode *left;
        struct BSTNode *right;
};

struct BST // Binary Search Tree
{
        struct BSTNode *root;
};

int BSTNodeBalance(struct BSTNode *bst_node)
{
    int left = (bst_node->left == NULL) ? 0 : bst_node->left->height ;
    int right = (bst_node->right == NULL) ? 0 : bst_node->right->height ;
    
    bst_node->height = Max(left, right) + 1;
    
    return left - right;
}

struct BSTNode* BSTNodeMin(struct BSTNode *bst_node)
{       
        while (bst_node->left != NULL)
                bst_node = bst_node->left;
        
        return bst_node;
}

struct BSTNode* BSTNodeMax(struct BSTNode *bst_node)
{       
        while (bst_node->right != NULL)
                bst_node = bst_node->right;
        
        return bst_node;
}

struct BSTNode* BSTNodeSuccessor(struct BSTNode *bst_node)
{       
        struct BSTNode *parent = NULL;
        
        if (bst_node->right != NULL)
                return BSTNodeMin(bst_node->right);
        
        while ((parent = bst_node->parent) != NULL)
        {       
                if (parent->left == bst_node)
                        return parent;
                
                bst_node = parent;
        }
        
        return NULL;
}

struct BSTNode* BSTNodePredecessor(struct BSTNode *bst_node)
{       
        struct BSTNode *parent = NULL;
        
        if (bst_node->left != NULL)
                return BSTNodeMax(bst_node->left);
        
        while ((parent = bst_node->parent) != NULL)
        {       
                if (parent->right == bst_node)
                        return parent;
                
                bst_node = parent;
        }
        
        return NULL;
}

void BSTNodeConnect(struct BSTNode *child, struct BSTNode *parent)
{       
        if (child->val <= parent->val)
                parent->left = child;
        else    
                parent->right = child;
        
        child->parent = parent;
}

void BSTNodeDump(struct BSTNode *bst_node, int shift)
{       
        int i = 0;
        struct BSTNode *predecessor = NULL, *successor = NULL;
        
        if (bst_node == NULL)
                return;
        
        predecessor = BSTNodePredecessor(bst_node);
        successor = BSTNodeSuccessor(bst_node);
        
        for (i = 0; i < shift; ++i)
                printf("  ");
        
        printf("%d h %d pre %d suc %d dup %d\n", bst_node->val, bst_node->height
                , (predecessor == NULL) ? -1 : predecessor->val
                , (successor == NULL) ? -1 : successor->val
                , *bst_node->index_dup_left);
        
        BSTNodeDump(bst_node->left, shift + 1);
        BSTNodeDump(bst_node->right, shift + 1);
}

void BSTNodeReset(struct BSTNode *bst_node)
{
        bst_node->parent = NULL;
        bst_node->left = NULL;
        bst_node->right = NULL;
}

void BSTNodeInit(struct BSTNode *bst_node, int val, int index)
{
        bst_node->val = val;
    
        bst_node->index = index;
        bst_node->index_even = -1;
        bst_node->index_odd = -1;
    
        bst_node->height = 1;
    
        bst_node->_dup_left_val = bst_node->index;
        bst_node->index_dup_left = &bst_node->_dup_left_val;
    
        BSTNodeReset(bst_node);
}

/*
            y              x

          x   z	         a   y

        a   b    		   b   z

 */

void LeftRotate(struct BST *bst, struct BSTNode* x)
{
    struct BSTNode *y = x->right;
    
    if (y == NULL)
        return;
    
    // x parent - y
    if (x->parent == NULL)
        bst->root = y;
    else
    {
        if (x->parent->right == x)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    
    y->parent = x->parent;
    
    // x - b
    if (y->left != NULL)
        y->left->parent = x;
    
    x->right = y->left;
    
    // y - x
    x->parent = y;
    y->left = x;
    
    BSTNodeUpdateHeight(x);
    BSTNodeUpdateHeight(y);
}

void RightRotate(struct BST *bst, struct BSTNode* y)
{
    struct BSTNode *x = y->left;
    
    if (x == NULL)
        return;
    
    // y parent - x
    if (y->parent == NULL)
        bst->root = x;
    else
    {
        if (y->parent->right == y)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    
    x->parent = y->parent;
    
    // y - b
    if (x->right != NULL)
        x->right->parent = y;
    
    y->left = x->right;
    
    // x - y
    y->parent = x;
    x->right = y;
    
    BSTNodeUpdateHeight(x);
    BSTNodeUpdateHeight(y);
}

struct BSTNode* BSTInsert(struct BST *bst, struct BSTNode* node)
{
        struct BSTNode *bst_node = bst->root, *parent = NULL;
        int balance = 0;
    
        while (bst_node != NULL)
        {
                parent = bst_node;

                if (node->val <= bst_node->val)
                        bst_node = bst_node->left;
                else
                        bst_node = bst_node->right;
        }

        bst_node = node;

        if (parent == NULL) // tree empty
                bst->root = bst_node;
        else
                BSTNodeConnect(bst_node, parent);

        while (parent != NULL)
        {
            balance = BSTNodeBalance(parent);
                      
            // L
            if (balance > 1)
            {
                // L - R
                if (parent->left->val < bst_node->val)
                    LeftRotate(bst, parent->left);
                
                // both L - L / L - R
                RightRotate(bst, parent);
            }
            
            // R
            if (balance < -1)
            {
                // R - L
                if (parent->right->val >= bst_node->val)
                    RightRotate(bst, parent->right);
                
                // both R - L / R - R
                LeftRotate(bst, parent);
            }
            
            bst_node = parent;
            parent = parent->parent;
        }
    
        return node;
}

void BSTDump(struct BST *bst)
{
        BSTNodeDump(bst->root, 0);
        printf("\n");
}

void BSTInit(struct BST *bst)
{
        bst->root = NULL;
}

int oddEvenJumps(int* arr, int arrSize)
{
    int i = arrSize - 1, j = 0, cn_good = 1;
    
    struct BSTNode *nodes = malloc(arrSize * sizeof(struct BSTNode)), *node = NULL;
    struct BST bst;
    
    BSTInit(&bst);
    
    BSTNodeInit(&nodes[i], arr[i], i);
    BSTInsert(&bst, &nodes[i]);
    
    nodes[i].index_even = arrSize - 1;
    nodes[i].index_odd = arrSize - 1;
    
    BSTDump(&bst);
    
    --i;
    
    for (; i >= 0; --i) // n-2 ~ 0
    {
        BSTNodeInit(&nodes[i], arr[i], i);
        BSTInsert(&bst, &nodes[i]);
        
        if ((node = BSTNodePredecessor(&nodes[i])) != NULL)
            nodes[i].index_even = *node->index_dup_left;
        
        if ((node = BSTNodeSuccessor(&nodes[i])) != NULL)
        {
            nodes[i].index_odd = node->index;
            
            if (node->val == nodes[i].val)
            {
                nodes[i].index_even = node->index;
                
                nodes[i].index_dup_left = node->index_dup_left;
                *node->index_dup_left = nodes[i].index;
            }
        }
        BSTDump(&bst);
        
//        printf("val %d odd %d even %d\n", arr[i], nodes[i].index_odd, nodes[i].index_even);
        
        if (nodes[i].index_even != -1)
        {
            if (nodes[nodes[i].index_even].index_odd == -1)
                nodes[i].index_even = -1;
        }
        
        if (nodes[i].index_odd != -1)
        {
            if (nodes[nodes[i].index_odd].index_even == -1)
                nodes[i].index_odd = -1;
        }
        
        if (/*(nodes[i].index_even != -1) || */(nodes[i].index_odd != -1)) // must start from 1st
            ++cn_good;

//        if (nodes[i].index_odd == -1)
//            printf("index %d val %d odd %d even %d good %d\n", i, arr[i], nodes[i].index_odd, nodes[i].index_even, cn_good);
    }

    return cn_good;
}

int main(int argc, char *argv[])
{
	return 0;
}

