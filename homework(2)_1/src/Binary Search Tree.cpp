#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);

    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);

    return root;
}
int height(Node* root) {
    if (!root) return 0;
    return 1 + max(height(root->left), height(root->right));
}
int main() {
    for (int n = 100; n <= 10000; n += 1000) {
        Node* root = nullptr;

        for (int i = 0; i < n; i++) {
            int val = rand(); // 隨機數
            root = insert(root, val);
        }

        int h = height(root);
        double ratio = h / log2(n);

        cout << "n = " << n
            << ", height = " << h
            << ", height/log2(n) = " << ratio << endl;
    }
}
///(b)
Node* findMin(Node* root) {
    while (root->left)
        root = root->left;
    return root;
}

Node* deleteNode(Node* root, int key) {
    if (!root) return nullptr;

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        // 找到節點
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        // 兩個子節點
        Node* temp = findMin(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    return root;
}
