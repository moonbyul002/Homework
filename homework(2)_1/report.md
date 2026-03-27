# 41343110 
# Quit_1 Max/Min Heap

## 申論及開發報告
## 解題說明
本題目要求實作一個 最小優先佇列（Min Priority Queue），並使用 最小堆積（Min Heap） 作為底層資料結構。

### 解題策略
#### 資料結構
- Min Priority Queue 抽象類別
- MinHeap 類別設計
#### 主要操作方法
1.插入元素（Push):當新元素加入時，先放在陣列最後，再進行「向上調整（Heapify Up）」：
- 與父節點比較
- 若較小則交換
- 重複直到符合 MinHeap 性質

2.刪除最小值（Pop:最小值位於根節點（heap[1]），刪除後：
- 以最後一個元素補上根節點
- 進行「向下調整（Heapify Down）」
- 與較小子節點比較並交換

3.取得最小值（Top）
```cpp
  heap[1]
```

4.動態擴充（Resize):當陣列空間不足時：
- 建立新陣列（容量加倍）
- 複製原資料

## 程式實作
```cpp
  #include <iostream>
#include <stdexcept>
using namespace std;

template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int size;

    void Resize() {
        capacity *= 2;
        T* newHeap = new T[capacity + 1];
        for (int i = 1; i <= size; i++)
            newHeap[i] = heap[i];
        delete[] heap;
        heap = newHeap;
    }

public:
    MinHeap(int cap = 10) {
        capacity = cap;
        heap = new T[capacity + 1]; //  index從1開始
        size = 0;
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    const T& Top() const {
        if (IsEmpty())
            throw runtime_error("Heap is empty");
        return heap[1];
    }

    void Push(const T& x) {
        if (size + 1 == capacity)
            Resize();

        int i = ++size;

        while (i != 1 && x < heap[i / 2]) {
            heap[i] = heap[i / 2];
            i /= 2;
        }

        heap[i] = x;
    }

    void Pop() {
        if (IsEmpty())
            throw runtime_error("Heap is empty");

        T last = heap[size--];

        int parent = 1;
        int child = 2;

        while (child <= size) {
            if (child < size && heap[child] > heap[child + 1])
                child++;

            if (last <= heap[child])
                break;

            heap[parent] = heap[child];
            parent = child;
            child *= 2;
        }

        heap[parent] = last;
    }
    void PrintByIndex() const {
        for (int i = 1; i <= size; i++) {
            cout << heap[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    MinHeap<int> h;

    int n, x;
    cout << "輸入元素個素:";
    cin >> n;
    cout << "輸入元素:";
    for (int i = 0; i < n; i++) {
        cin >> x;
        h.Push(x);
    }

    cout << "依 index 輸出: ";
    h.PrintByIndex();

    return 0;
}
```
## 效能分析
### 時間複雜度分析
1. 插入操作（Push）:插入新元素時，需進行向上調整
```cpp
O(log n)
```
2. 刪除最小值（Pop）:刪除 root 後，需進行向下調整
```cpp
O(log n)
```
3. 取得最小值（Top）:直接存取根節點
```cpp
O(1)
```
4. 判斷是否為空（IsEmpty）:僅檢查 size
```cpp
O(1)
```
## 測試與驗證

### 測試案例
| 測試案例 | 輸入參數   | 預期輸出 | 實際輸出 | 
|----------|--------------|----------|----------|
| 測試一   |   5 10 4 15 2 8   |     依 index 輸出: 2 4 15 10 8       |     依 index 輸出: 2 4 15 10 8    | 

### 結論
本題成功以 MinHeap 實作最小優先佇列，透過陣列表示完全二元樹，並利用向上調整與向下調整維持堆積性質，使得各項操作皆能有效率地完成。


# Quit_2 Binary Search Tree
## 申論及開發報告
## 解題說明

## 程式實作
```cpp
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
```
## 效能分析
## 測試與驗證
