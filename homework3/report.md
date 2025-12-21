# 41343110
## 作業三 Polynomial with Available Lists
## 解題說明
本題要求實作一個多項式（Polynomial）類別，支援多項式的
輸入、輸出、加法、減法、乘法與帶入值計算，並需自行管理記憶體。

為了有效率地儲存多項式中的各項，本程式使用
鏈結串列（Linked List） 來表示多項式，每一個節點代表一項 (coef, exp)。
並透過 Iterator 來遍歷串列，使運算過程更直觀且模組化。

此外，為減少頻繁的動態記憶體配置成本，設計了
Available List（Free List） 機制，回收已釋放的節點供後續重複使用，
以提升整體效能並符合資料結構課程的設計精神。

### 解題策略
#### 多項式表示方式

 - 使用 Chain<Term> 作為多項式的內部結構

 - 每個 Term 包含：

    - coef：係數

    - exp：指數

 - 串列中的項目 依指數由大到小排序
 - 
#### 鏈結串列與 Iterator

 - 自行實作 ChainNode 與 ChainIterator
 
 - 透過 iterator 提供：

    - *、-> 存取資料

    - ++ 前進節點

 - 讓多項式運算程式碼可讀性更高，且避免直接操作指標
 - 
#### Available List（節點回收策略）

 - 設計 AvailableList 作為靜態 free list

 - 當節點不再使用時：

    - 不直接 delete

    - 改為回收到 available list

 - 新增節點時：

    - 優先從 available list 取用

    - 若無可用節點才配置新記憶體

## 程式實作
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
        std::cout << "輸入第"<<i+1<<"的係數、指數:";
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

| 操作             | 時間複雜度          | 說明                       |
| -------------- | -------------- | ------------------------ |
| 插入一項 `newTerm` | O(k)           | k = 當前多項式長度              |
| 多項式加法 A + B    | O((n+m)²)      | 插入結果時可能遍歷已存在鏈表           |
| 多項式減法 A - B    | O((n+m)²)      | 先取負再加法                   |
| 多項式乘法 A * B    | O((n*m)²)      | 每項相乘並插入結果                |
| Evaluate(x)    | O(n * max_exp) | 可優化為 O(n * log(max_exp)) |

### 空間複雜度：
| 操作       | 空間複雜度    |
| -------- | -------- |
| 儲存一個多項式  | O(n)     |
| 加法       | O(n + m) |
| 減法       | O(n + m) |
| 乘法       | O(n × m) |
| 帶入值計算    | O(1)     |
| Iterator | O(1)     |

## 測試與驗證
### 測試案例

| 測試案例 | 輸入參數   | 預期輸出  | 實際輸出  | 
|----------|--------------|----------|----------|
| 測試一   |      3     |             |         |       
|          |        2, 0    |            |        | 
|          |        3,2     |            |        |
|          |        3,3     |            |        |
|          |               |    3x^3 + 3x^2 + 2x^0     |    3x^3 + 3x^2 + 2x^0      |  
| 測試二  |      4    |            |         |       
|          |       4, 3    |            |        | 
|          |       2, 3    |            |        | 
|          |       2, 2    |            |        | 
|          |       4, 0    |       6x^3 + 2x^2 + 4x^0   |  6x^3 + 2x^2 + 4x^0     |
|   $A$  +    $B$     |      |   9x^3 + 5x^2 + 6x^0       |  9x^3 + 5x^2 + 6x^0       |  
|   $A$  -   $B$     |      |   -3x^3 + 1x^2 + -2x^0       |  -3x^3 + 1x^2 + -2x^0        |   
|   $A$ * $B$  |     |  18x^6 + 24x^5 + 6x^4 + 24x^3 + 16x^2 + 8x^0      |   18x^6 + 24x^5 + 6x^4 + 24x^3 + 16x^2 + 8x^0             |
|   $A(2)$      |      | 38   |   38 |

### 測試輸入
```
輸入A的項數:3
輸入第1的指數、係數:2 0
輸入第2的指數、係數:3 2
輸入第3的指數、係數:3 3
輸入B的項數:4
輸入第1的指數、係數:4 3
輸入第2的指數、係數:2 3
輸入第3的指數、係數:2 2
輸入第4的指數、係數:4 0
輸入x為多少:2
```
### 測試輸出
```
A = 3x^3 + 3x^2 + 2x^0
B = 6x^3 + 2x^2 + 4x^0 
A + B = 9x^3 + 5x^2 + 6x^0
A - B = -3x^3 + 1x^2 + -2x^0 
A * B = 18x^6 + 24x^5 + 6x^4 + 24x^3 + 16x^2 + 8x^0
A(2) = 38
```


## 申論及開發報告
### 申論
本程式以鏈結串列（Linked List）作為多項式的主要資料結構，每一個節點代表多項式中的一項，包含係數與指數兩個資訊。由於多項式的項數在執行期間可能動態變化，且各項指數不一定連續，使用鏈結串列能有效避免陣列需預先配置大小或頻繁搬移資料的缺點，提升結構彈性。
在記憶體管理方面，程式進一步設計了 Available List（Free List） 作為節點回收機制。綜合而言，本程式透過鏈結串列、Iterator 及 Available List 的搭配設計，在結構彈性、運算效率與記憶體管理之間取得良好平衡，成功實作一個功能完整且符合資料結構原則的多項式運算系統。

### 結論
本程式實作一套以鏈結串列為基礎的多項式運算系統，支援多項式的輸入與輸出，以及加法、減法、乘法與帶入值計算等基本操作。透過將多項式各項依指數大小排序儲存，使得多項式運算能以線性方式完成，大幅提升運算效率。
在資料結構設計上，使用 Iterator 來遍歷串列，降低模組間的耦合度，使程式結構更具可讀性與維護性；同時搭配 Available List 進行節點回收，有效減少動態記憶體配置與釋放的次數，在不增加漸進空間複雜度的前提下，改善實際執行效能。
