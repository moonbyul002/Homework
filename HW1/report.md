# 41343110 
# Quit_1 Max/Min Heap


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
#include <string>
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
### 時間複雜度
- 插入操作（Push）:插入新元素時，需進行向上調整
```cpp
O(log n)
```
- 刪除最小值（Pop）:刪除 root 後，需進行向下調整
```cpp
O(log n)
```
- 取得最小值（Top）:直接存取根節點
```cpp
O(1)
```
- 判斷是否為空（IsEmpty）:僅檢查 size
```cpp
O(1)
```
### 空間複雜度
- 使用陣列儲存：O(n)
- 動態擴充後仍為：O(n)
## 測試與驗證

### 測試案例
| 測試案例 | 輸入參數   | 預期輸出 | 實際輸出 | 
|----------|--------------|----------|----------|
| 測試一   |   5 10 4 15 2 8   |     依 index 輸出: 2 4 15 10 8       |     依 index 輸出: 2 4 15 10 8    | 

### 結論
本題成功以 MinHeap 實作最小優先佇列，透過陣列表示完全二元樹，並利用向上調整與向下調整維持堆積性質，使得各項操作皆能有效率地完成。
## 申論及開發報告

# Quit_2 Binary Search Tree

## 解題說明
(a)
### 題目說明 
本程式主要在探討 二元搜尋樹（Binary Search Tree, BST）在隨機插入情況下的高度變化
### 實作方法
- 建立一棵 空的 Binary Search Tree (BST)
- 插入 n 個隨機數
- 計算樹的高度 height: h(n)=1+max(h(left),h(right))
- 計算比值：height/ $\log_2 n$
(b)
### 題目說明 
寫一個 C++ 函式，在 Binary Search Tree（BST，二元搜尋樹） 中：
- 給定一個 key k
- 把 key = k 的那個節點（pair）從 BST 刪掉
- 刪除後樹仍然要符合 BST 性質：
  - 左子樹所有 key 都 < 此節點 key
  - 右子樹所有 key 都 > 此節點 key
### 實作方法
- 要刪的節點是葉節點（沒有子樹）：直接刪掉
- 只有一個子樹：用那個子樹頂替被刪節點的位置
- 有兩個子樹：用「中序後繼」（右子樹最小值）或「中序前驅」（左子樹最大值）來替換，再把那個後繼/前驅節點刪掉
## 程式實作
(a)
```cpp
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, int key) {
    if (!root) return new Node(key);

    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    
    return root;
}

int height(Node* root) {
    if (!root) return 0;
    int hl = height(root->left);
    int hr = height(root->right);
    return 1 + (hl > hr ? hl : hr);
}

void destroy(Node* root) {
    if (!root) return;
    destroy(root->left);
    destroy(root->right);
    delete root;
}

// Fisher–Yates shuffle
void shuffleArray(int* a, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

int main() {
    srand((unsigned)time(nullptr)); 

    int ns[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

    for (int idx = 0; idx < (int)(sizeof(ns) / sizeof(ns[0])); ++idx) {
        int n = ns[idx];
        Node* root = nullptr;

        
        int* arr = new int[n];
        for (int i = 0; i < n; ++i) arr[i] = i + 1;

       
        shuffleArray(arr, n);
        for (int i = 0; i < n; ++i) {
            root = insert(root, arr[i]);
        }

        int h = height(root);
        double ratio = h / log2((double)n);

        cout << "n = " << n
            << ", height = " << h
            << ", height/log2(n) = " << ratio << "\n";

        delete[] arr;
        destroy(root);
    }

    return 0;
}
```
(b)
```cpp
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

static Node* findMin(Node* node) {
    while (node && node->left) node = node->left;
    return node;
}

Node* deleteKey(Node* root, int k) {
    if (!root) return nullptr;

    if (k < root->key) {
        root->left = deleteKey(root->left, k);
    } else if (k > root->key) {
        root->right = deleteKey(root->right, k);
    } else {
        // 找到要刪的節點 root
        if (!root->left && !root->right) {
            delete root;
            return nullptr;
        }
        if (!root->left) {
            Node* r = root->right;
            delete root;
            return r;
        }
        if (!root->right) {
            Node* l = root->left;
            delete root;
            return l;
        }

      
        Node* succ = findMin(root->right);
        root->key = succ->key;
        root->right = deleteKey(root->right, succ->key);
    }
    return root;
}
```
## 效能分析
(a)
### 時間複雜度
- 插入與搜尋時間複雜度:BST 的時間複雜度取決於樹的高度 
|高度| 時間複雜度  |
|----------|--------------|
| $\log_2 n$ | O(logn) |
|  n |  O(n)|
- deleteNode 效能:刪除操作時間複雜度：O(h)

### 空間複雜度
- 每個節點：𝑂(1)
- 總空間：𝑂(𝑛)

(b)
|程式|高度| 時間複雜度  |
|----------|--------------|--------------|
|deleteKey()|h|O(h)|
## 測試與驗證
| 測試案例 | 輸入參數 n   | 預期輸出  height, height/ $\log_2 n$  | 實際輸出 height, height/  $\log_2 n$ | 
|----------|--------------|----------|----------|
| 測試一   |   100    |  12         |   1.80618    | 
| 測試二   |    1100   |     23    |   2.27649    | 
| 測試三  |    2100   |     23    |  2.08406     | 
| 測試四  |  3100     |      30   |   2.58664    | 
| 測試五  |  4100     |    36     |  2.99965     | 
| 測試六  |   5100    |    27     |   2.19222    | 
| 測試七  |   6100    |      28   |  2.22671     | 
| 測試八 | 7100    |     30    |    2.34492   | 
| 測試九   |  8100     |   28      |    2.15655   | 
| 測試十   |    9100   |    36     |    2.7373   | 
## 結論
本程式顯示，在資料輸入下，二元搜尋樹的高度趨近於 𝑂(log⁡𝑛)但在極端情況下仍可能退化為 𝑂(𝑛)，因此實務上常需使用平衡樹以維持穩定效能。


驗證了：
- BST 在隨機插入下，高度約為 𝑂(log⁡ 𝑛)
- 實際高度  $\log_2 n$  與的比例為近似常數
- 刪除操作的複雜度同樣受樹高影響
- 
## 申論及開發報告
