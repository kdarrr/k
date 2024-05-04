#include <stdio.h>
#include <stdlib.h>

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            fprintf(stderr, "Assertion failed: expected %d, actual %d\n", (expected), (actual)); \
            abort(); \
        } \
    } while (0)


#define RED 0
#define BLACK 1

struct Node {
    int data;
    int color; 
    struct Node* parent;
    struct Node* left;
    struct Node* right;
};


struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->color = RED;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


void leftRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}


void rightRotate(struct Node** root, struct Node* x) {
    struct Node* y = x->left;
    x->left = y->right;
    if (y->right != NULL)
        y->right->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}


void swapColors(struct Node* x1, struct Node* x2) {
    int temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}



void fixinsert(struct Node** root, struct Node* x) {
    struct Node* parent = NULL;
    struct Node* grandparent = NULL;

    while ((x != *root) && (x->color != BLACK) && (x->parent->color == RED)) {
        parent = x->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            struct Node* uncle = grandparent->right;

            if (uncle != NULL && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                x = grandparent;
            }
            else {
                if (x == parent->right) {
                    leftRotate(root, parent);
                    x = parent;
                    parent = x->parent;
                }
                rightRotate(root, grandparent);
                swapColors(parent, grandparent);
                x = parent;
            }
        }
        else {
            struct Node* uncle = grandparent->left;

            if ((uncle != NULL) && (uncle->color == RED)) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                x = grandparent;
            }
            else {
                if (x == parent->left) {
                    rightRotate(root, parent);
                    x = parent;
                    parent = x->parent;
                }
                leftRotate(root, grandparent);
                swapColors(parent, grandparent);
                x = parent;
            }
        }
    }

    (*root)->color = BLACK;
}

void insert(struct Node** root, int data) {
    struct Node* newNode = createNode(data);
    struct Node* parent = NULL;
    struct Node* x = *root;


    while (x != NULL) {
        parent = x;
        if (data < x->data)
            x = x->left;
        else if (data > x->data)
            x = x->right;
        else {
            free(newNode);
            return;  
        }
    }

    newNode->parent = parent;


    if (parent == NULL)
        *root = newNode;
    else if (data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    fixinsert(root, newNode);
}




struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}


void fixDeletion(struct Node** root, struct Node* x, struct Node* parent) {
    struct Node* sibling;

    while ((x != *root) && ((x == NULL) || (x->color == BLACK))) {
        if (x == parent->left) {
            sibling = parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                leftRotate(root, parent);
                sibling = parent->right;
            }

            if (((sibling->left == NULL) || (sibling->left->color == BLACK)) &&
                ((sibling->right == NULL) || (sibling->right->color == BLACK))) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            }
            else {
                if ((sibling->right == NULL) || (sibling->right->color == BLACK)) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(root, sibling);
                    sibling = parent->right;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->right->color = BLACK;
                leftRotate(root, parent);
                x = *root;
            }
        }
        else {
            sibling = parent->left;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rightRotate(root, parent);
                sibling = parent->left;
            }

            if (((sibling->right == NULL) || (sibling->right->color == BLACK)) &&
                ((sibling->left == NULL) || (sibling->left->color == BLACK))) {
                sibling->color = RED;
                x = parent;
                parent = x->parent;
            }
            else {
                if ((sibling->left == NULL) || (sibling->left->color == BLACK)) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(root, sibling);
                    sibling = parent->left;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                sibling->left->color = BLACK;
                rightRotate(root, parent);
                x = *root;
            }
        }
    }

    if (x != NULL)
        x->color = BLACK;
}
void deleteNode(struct Node** root, int data) {
    struct Node* z = NULL;
    struct Node* x, * y;
    struct Node* parent = NULL;
    z = *root;
    while (z != NULL) {
        if (data == z->data)
            break;
        parent = z;
        if (data < z->data)
            z = z->left;
        else
            z = z->right;
    }

    if (z == NULL) {
        printf("Pointer is null.\n");
        return;
    }


    if ((z->left == NULL) || (z->right == NULL)) {
        y = z;
    }
    else {
        y = minValueNode(z->right);
    }


    if (y->left != NULL) {
        x = y->left;
    }
    else {
        x = y->right;
    }

    if (x != NULL) {
        x->parent = y->parent;
    }

    if (y->parent == NULL) {
        *root = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }

    if (y != z) {
        z->data = y->data;
    }


    if (y->color == BLACK) {
        fixDeletion(root, x, y->parent);
    }

    free(y);
}


void print2DUtil(struct Node* root, int space) {
    if (root == NULL)
        return;
    int count = 5;
    space += count;

    print2DUtil(root->right, space);
    printf("\n");
    for (int i = count; i < space; i++)
        printf(" ");
    if (root->color == RED)
        printf("R%d\n", root->data);
    else
        printf("B%d\n", root->data);
    print2DUtil(root->left, space);
}
void print2D(struct Node* root) {
    print2DUtil(root, 0);
}
void test_insert_emptyTree() {
    printf("test_insert_emptyTree()\n");
    struct Node* root = NULL;
    insert(&root, 10);
    ASSERT_EQ(root->data, 10);
    ASSERT_EQ(root->color, BLACK); 
}



void test_insert_rebalance() {
    printf("test_insert_rebalance()\n");
    struct Node* root = NULL;
    insert(&root, 10);
    insert(&root, 5);
    insert(&root, 15);
    insert(&root, 20); 

    ASSERT_EQ(root->data, 10);
    ASSERT_EQ(root->color, BLACK);
    ASSERT_EQ(root->left->data, 5);
    ASSERT_EQ(root->right->data, 15);
    ASSERT_EQ(root->right->right->data, 20); 

}
void test_deleteNode_leafNode() {
    printf("test_deleteNode_leafNode()\n");
    struct Node* root = NULL;
    insert(&root, 10);
    insert(&root, 5);
    insert(&root, 15);

    deleteNode(&root, 5); 

    ASSERT_EQ(root->data, 10);
    ASSERT_EQ(root->left, NULL); 
    ASSERT_EQ(root->right->data, 15);
}
void testInsertRBTree() {
    printf("TestInsertRBTree\n");
    struct Node* root = NULL;
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 30);
    ASSERT_EQ(root->data, 20);
}

void testMinValueNode() {
    struct Node* root = NULL;
    printf("testMinValueNode()\n");
    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 30);
    struct Node* minNode = minValueNode(root);
    ASSERT_EQ(minNode->data, 10);
}
int main() {
    struct Node* root = NULL;

    insert(&root, 1);
    insert(&root, 2);
    insert(&root, 0);
    deleteNode(&root, 0);

    printf("Cay 2D:\n");
    print2D(root);
    deleteNode(&root, 5);
    print2D(root);
    test_insert_emptyTree();
    test_insert_rebalance(); 
    test_deleteNode_leafNode();
    testMinValueNode();
    testInsertRBTree();
    printf("All tests run successfully!\n");
    return 0;
}