# 41343121
## 作業三 Polynomial + Available Lists
## 解題說明
實現對多項式的：

1.加法 (+)

2.減法 (-)

3.乘法 (*)

4.計算在某個 x 的值 (Evaluate)

5.動態輸入多項式

6.使用鏈結串列管理多項式項目，提高效率（可回收節點）
### 解題策略
1.使用可用節點列表（AvailableList）

- 節點可回收，減少記憶體分配與釋放

- 適合需要頻繁插入/刪除的多項式操作

2.鏈結串列實現多項式

- 支援任意順序輸入

- 自動合併同指數項

- 動態增加項目

3.運算符重載

- +,-,* 可直接對 Polynomial 對象進行運算

符合直覺的多項式計算方式
## 程式實作
### IDE:
Microsoft Visual Studio Code C/C++

```cpp
#include <iostream>
#include <cstdlib>
using namespace std;

template <class T>
class ChainNode;

template <class T>
class ChainIterator;

template <class T>
class Chain;

template <class T>
class AvailableList;

template <class T>
class ChainNode {
    friend class Chain<T>;
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    T element;
    ChainNode<T>* next;
public:
    ChainNode() : next(nullptr) {}
    ChainNode(const T& elem) : element(elem), next(nullptr) {}
    ChainNode(const T& elem, ChainNode<T>* nextNode)
        : element(elem), next(nextNode) {}
    void setElement(const T& elem) {
        element = elem;
    }
    void setNext(ChainNode<T>* nextNode) {
        next = nextNode;
    }
};

// Iterator for Chain
template <class T>
class ChainIterator {
private:
    ChainNode<T>* currentNode;
public:
    ChainIterator(ChainNode<T>* startNode = nullptr) : currentNode(startNode) {}

    T& operator*() const { return currentNode->element; }
    T* operator->() const { return &currentNode->element; }
    
    ChainIterator& operator++() {
        if (currentNode) currentNode = currentNode->next;
        return *this;
    }

    ChainIterator operator++(int) {
        ChainIterator old = *this;
        if (currentNode) currentNode = currentNode->next;
        return old;
    }

    bool operator!=(const ChainIterator& right) const {
        return currentNode != right.currentNode;
    }
    bool operator==(const ChainIterator& right) const {
        return currentNode == right.currentNode;
    }

    int operator-(const ChainIterator& right) const {
        int distance = 0;
        ChainNode<T>* temp = right.currentNode;
        while (temp != currentNode) {
            if (temp == nullptr) {
                return -1;
            }
            temp = temp->next;
            ++distance;
        }
        return distance;
    }
};


template <class T>
class AvailableList {
private:
    static ChainNode<T>* availableHead;
public:
    AvailableList() {}
    ~AvailableList() {
       
        while (availableHead) {
            ChainNode<T>* tmp = availableHead;
            availableHead = availableHead->next;
            delete tmp;
        }
    }

    
    static ChainNode<T>* getNode() {
        if (!availableHead) return nullptr;
        ChainNode<T>* nodeToReturn = availableHead;
        availableHead = availableHead->next;
        nodeToReturn->next = nullptr;
        return nodeToReturn;
    }

   
    static void getBack(ChainNode<T>* firstNode) {
        if (!firstNode) return;
       
        ChainNode<T>* current = firstNode;
        while (current->next) {
            current = current->next;
        }
        
        current->next = availableHead;
        
        availableHead = firstNode;
    }

    static bool isEmpty() {
        return availableHead == nullptr;
    }

    
    static ChainNode<T>* getOneNodeOrNew(const T& elem) {
        ChainNode<T>* node = getNode();
        if (!node) node = new ChainNode<T>(elem);
        else node->setElement(elem);
        return node;
    }
};

template <class T>
ChainNode<T>* AvailableList<T>::availableHead = nullptr;

template <class T>
class Chain {
    friend class ChainIterator<T>;
    friend class AvailableList<T>;
private:
    ChainNode<T>* head;
public:
    Chain() : head(nullptr) {}
    ~Chain() {
        ChainNode<T>* current = head;
        while (current != nullptr) {
            ChainNode<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }

    ChainNode<T>* release() {
        ChainNode<T>* oldHead = head;
        head = nullptr;
        return oldHead;
    }

    ChainIterator<T> begin() const {
        return ChainIterator<T>(head);
    }
    ChainIterator<T> end() const {
        return ChainIterator<T>(nullptr);
    }

    void insert(int idx, const T& element) {
        ChainNode<T>* newNode = AvailableList<T>::getNode();
        if (!newNode) {
            newNode = new ChainNode<T>(element);
        }
        else {
            newNode->setElement(element);
        }

        if (idx == 0) {
            newNode->next = head;
            head = newNode;
        }
        else {
            ChainNode<T>* prev = head;

            for (int i = 0; i < idx - 1 && prev != nullptr; ++i) {
                prev = prev->next;
            }
            if (prev != nullptr) {
                newNode->next = prev->next;
                prev->next = newNode;
            }
            else {

                AvailableList<T>::getBack(newNode);
            }
        }
    }
};

struct Term {
    double coef;
    int exp;
    Term() : coef(0.0), exp(0) {}
    Term(double c, int e) : coef(c), exp(e) {}
};

AvailableList<Term> globalASL;

class Polynomial {
    friend ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend istream& operator>>(std::istream& is, Polynomial& poly);
private:
    Chain<Term> terms; 
public:
    Polynomial() {}

    // Copy constructor
    Polynomial(const Polynomial& other) {
        int index = 0;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index) {
            terms.insert(index, *it);
        }
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {

            AvailableList<Term>::getBack(terms.release());

            int index = 0;
            for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it, ++index) {
                terms.insert(index, *it);
            }
        }
        return *this;
    }

    ~Polynomial() {
        AvailableList<Term>::getBack(terms.release());
    }

    ChainIterator<Term> begin() const {
        return terms.begin();
    }
    ChainIterator<Term> end() const {
        return terms.end();
    }

    void newTerm(double coef, int exp) {

        if (terms.begin() == terms.end()) {
            terms.insert(0, Term(coef, exp));
            return;
        }

        int index = 0;
        for (ChainIterator<Term> it = terms.begin(); it != terms.end(); ++it, ++index) {
            if (it->exp < exp) {

                terms.insert(index, Term(coef, exp));
                return;
            }
            else if (it->exp == exp) {

                it->coef += coef;
                return;
            }
        }

        terms.insert(index, Term(coef, exp));
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        ChainIterator<Term> it1 = this->begin();
        ChainIterator<Term> it2 = other.begin();
        while (it1 != this->end() && it2 != other.end()) {
            if (it1->exp > it2->exp) {
                result.newTerm(it1->coef, it1->exp);
                ++it1;
            }
            else if (it1->exp < it2->exp) {
                result.newTerm(it2->coef, it2->exp);
                ++it2;
            }
            else {
                double newCoef = it1->coef + it2->coef;
                if (newCoef != 0) {
                    result.newTerm(newCoef, it1->exp);
                }
                ++it1;
                ++it2;
            }
        }
        while (it1 != this->end()) {
            result.newTerm(it1->coef, it1->exp);
            ++it1;
        }
        while (it2 != other.end()) {
            result.newTerm(it2->coef, it2->exp);
            ++it2;
        }
        return result;
    }

    Polynomial operator-(const Polynomial& other) const {
        Polynomial negOther;
        for (ChainIterator<Term> it = other.begin(); it != other.end(); ++it) {
            negOther.newTerm(-it->coef, it->exp);
        }
        return (*this) + negOther;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (ChainIterator<Term> it1 = this->begin(); it1 != this->end(); ++it1) {
            for (ChainIterator<Term> it2 = other.begin(); it2 != other.end(); ++it2) {
                double newCoef = it1->coef * it2->coef;
                int newExp = it1->exp + it2->exp;
                result.newTerm(newCoef, newExp);
            }
        }
        return result;
    }
    float Evaluate(float x) const {
        float result = 0.0f;

        for (ChainIterator<Term> it = begin(); it != end(); ++it) {
            float termValue = it->coef;
            int exp = it->exp;


            float power = 1.0f;
            for (int i = 0; i < exp; ++i) {
                power *= x;
            }

            result += termValue * power;
        }
        return result;
    }
};

istream& operator>>(std::istream& is, Polynomial& poly) {
    int numTerms;
    if (!(is >> numTerms)) return is;
    for (int i = 0; i < numTerms; ++i) {
        double coef;
        int exp;
        std::cout << "輸入第"<<i+1<<"的指數、係數:";
        is >> coef >> exp;
        poly.newTerm(coef, exp);
    }
    return is;
}

ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    bool first = true;
    for (ChainIterator<Term> it = poly.begin(); it != poly.end(); ++it) {
        if (!first) {
            os << " + ";
        }
        first = false;
        os << it->coef << "x^" << it->exp;
    }
    return os;
}

int main() {
    Polynomial A, B;
    float x;
    cout << "輸入A的項數:";
    cin >> A;
    cout << "輸入B的項數:";
    cin >> B;
    cout << "輸入x為多少:";
    cin >> x;

    cout << "A = " << A << '\n';
    cout << "B = " << B << '\n';

    cout << "A + B = " << (A + B) << '\n';
    cout << "A - B = " << (A - B) << '\n';
    cout << "A * B = " << (A * B) << '\n';
    
    

    cout << "A(" << x << ") = " << A.Evaluate(x) << '\n';

    return 0;
}


```
## 效能分析
### 時間複雜度：

1.插入新項 newTerm(coef, exp)

- 遍歷鏈表找到正確位置插入 → 最壞情況遍歷整個鏈表

- 時間複雜度：O(k)，k = 目前多項式項數

2.多項式加法 A + B

- 使用兩個迭代器遍歷兩個多項式

- 每個項目最多遍歷一次 → O(n + m)

- 每次插入 newTerm 還需要遍歷結果鏈表插入（最壞 O(n+m)）

- 總體（最壞情況）：O((n+m)²)
（因為結果鏈表每次插入可能需要遍歷已存在項目）

3.多項式減法 A - B

- 先將 B 取負 → O(m)

- 然後和 A 相加 → 同上 → O((n+m)²)

4.多項式乘法 A * B

- 雙層迴圈遍歷 A 與 B 的每個項 → O(n*m)

- 每次插入結果鏈表時，可能需要遍歷整個結果鏈表（最壞情況）

- 總體最壞情況：O(nm(nm)) = O((nm)²)

5.Evaluate(x)

- 遍歷每個項目 → 計算 x 的冪次 → O(n * max_exp)

- 若使用簡單乘法計算 x^exp

- 可改為快速冪 → O(n * log(max_exp))

6.輸入多項式

- 依項數 n → O(n)

- 每次 newTerm 插入 → O(n)

- 最壞：O(n²)
#### 總結
| 操作             | 時間複雜度          | 說明                       |
| -------------- | -------------- | ------------------------ |
| 插入一項 `newTerm` | O(k)           | k = 當前多項式長度              |
| 多項式加法 A + B    | O((n+m)²)      | 插入結果時可能遍歷已存在鏈表           |
| 多項式減法 A - B    | O((n+m)²)      | 先取負再加法                   |
| 多項式乘法 A * B    | O((n*m)²)      | 每項相乘並插入結果                |
| Evaluate(x)    | O(n * max_exp) | 可優化為 O(n * log(max_exp)) |

### 空間複雜度：
#### 主要消耗空間：

1.多項式項目：

- 每個多項式有 n 項 → 使用 ChainNode<Term> 儲存。

- 每個節點包含：

    - Term（coef, exp）

    - 指標 next

- 空間：O(n)

2.鏈結串列本身（Chain）：

- 只存 head 指標 → O(1)

3.可用節點列表（AvailableList）：

- 最多存回收的節點數 = 所有曾存在過的節點數 → O(n)
（不會超過目前多項式的總項數）

4.臨時變數：

- 迭代器、輔助變數 → O(1)

## 測試與驗證
### 測試案例

| 測試案例 | 輸入參數   | 預期輸出  | 實際輸出  | 
|----------|--------------|----------|----------|
| 測試一   |      4     |             |         |       
|          |        4, 3    |            |        | 
|          |        3,2     |            |        |
|          |        2,1     |            |        |
|          |        5, 0    |    4x^3 + 3x^2 + 2x^1 + 5x^0       |    4x^3 + 3x^2 + 2x^1 + 5x^0      |  
| 測試二  |      3    |            |         |       
|          |       2, 3    |            |        | 
|          |       1, 2    |            |        | 
|          |       5, 1    |       2x^3 + 1x^2 + 5x^1    |  2x^3 + 1x^2 + 5x^1     |
|   $A$  +    $B$     |      |  6x^3 + 4x^2 + 7x^1 + 5x^0       | 6x^3 + 4x^2 + 7x^1 + 5x^0       |  
|   $A$  -   $B$     |      |  2x^3 + 2x^2 + -3x^1 + 5x^0       | 2x^3 + 2x^2 + -3x^1 + 5x^0        |   
|   $A$ * $B$  |     |  8x^6 + 10x^5 + 27x^4 + 27x^3 + 15x^2 + 25x^1       |   8x^6 + 10x^5 + 27x^4 + 27x^3 + 15x^2 + 25x^1              |
|   $A(2)$      |      | 53    |   53  |

### 測試輸入
```
輸入A的項數:4
輸入第1的指數、係數:4 3
輸入第2的指數、係數:3 2
輸入第3的指數、係數:2 1
輸入第4的指數、係數:5 0
輸入B的項數:3
輸入第1的指數、係數:2 3
輸入第2的指數、係數:1 2
輸入第3的指數、係數:5 1
輸入x為多少:2
```
### 測試輸出
```
A = 4x^3 + 3x^2 + 2x^1 + 5x^0
B = 2x^3 + 1x^2 + 5x^1
A + B = 6x^3 + 4x^2 + 7x^1 + 5x^0
A - B = 2x^3 + 2x^2 + -3x^1 + 5x^0
A * B = 8x^6 + 10x^5 + 27x^4 + 27x^3 + 15x^2 + 25x^1
A(2) = 53
```


## 申論及開發報告
### 申論
#### 資料結構設計
1.Term 結構

- 儲存多項式單項的資訊：係數 coef 與指數 exp。

- 範例：3x^2 → coef=3, exp=2。

2.ChainNode 節點

- 用於構建單向鏈結串列。

- 每個節點包含一個 Term 與下一個節點指標 next。

- 與 AvailableList 搭配，可重複使用節點，減少動態分配與釋放的開銷。

3.Chain 鏈結串列

- 封裝整個多項式項目的鏈表。

- 提供：

    - 插入項目（insert）

    - 釋放整個鏈表（release）

    - 迭代器遍歷（begin / end）

4.AvailableList 可用節點列表

- 管理已釋放但可重用的節點。

- 核心功能：

    - getNode()：取得可用節點

    - getBack(node)：回收節點

    - getOneNodeOrNew(elem)：取得節點或創建新節點

5.ChainIterator 迭代器

- 用於遍歷 Chain。

- 支援操作：*、->、++、==、!=。

- 方便在多項式運算中逐項操作。

6.Polynomial 類別

- 封裝多項式操作，核心成員為 Chain<Term>。

- 功能：

    - 新增項目 newTerm(coef, exp)，自動合併同指數項。

    - 運算符重載：+, -, *。

    - 計算多項式在某個 x 值的數值 Evaluate(x)。

### 結論

本程式透過鏈結串列與可回收節點列表實現多項式的動態管理與運算。
多項式的加、減、乘運算及在指定 x 值的求值功能均可正確執行，且利用迭代器與節點回收機制，提高了記憶體使用效率。
程式的空間複雜度主要取決於多項式項數。整體而言，該程式設計兼具資源管理效益，可作為多項式計算與鏈結串列應用的範例。
