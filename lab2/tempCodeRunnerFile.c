void test_insert_rightChild() {
    printf("test_insert_rightChild()\n");
    struct Node* root = createNode(10);
    insert(&root, 15);
    ASSERT_EQ(root->right->data, 15);
    ASSERT_EQ(root->right->color, RED); 
}
