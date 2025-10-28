# 41343110
# Homework2 Polynomial
# Problem1

## 解題說明
本題要求以物件導向的方式設計一個 Polynomial（多項式）類別，支援以下功能：

- 多項式的建立與儲存
使用動態陣列儲存多項式的各項（係數與指數）。

- 多項式的加法 (Add)
將兩個多項式相加，合併相同指數的項。

- 多項式的乘法 (Mult)
對兩個多項式的所有項做乘積運算，再合併同類項。

- 多項式的代入求值 (Eval)
將指定數值代入 x 中，計算多項式的值。

- 多項式的顯示 (Show)
將多項式以標準數學格式輸出，例如：3x^2+2x+1
## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }

    void NewTerm(float c, int e) {
        if (c == 0) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < poly.terms) {
            if (termArray[aPos].exp == poly.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + poly.termArray[bPos].coef;
                if (sum != 0)
                    result.NewTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > poly.termArray[bPos].exp) {
                result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
                bPos++;
            }
        }
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            Polynomial temp;
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;
                temp.NewTerm(newCoef, newExp);
            }
            result = result.Add(temp);
        }
        return result;
    }

    float Eval(float f) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(f, termArray[i].exp);
        return result;
    }

    void Show() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }
        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;
            if (i > 0) {
                if (c >= 0) cout << " + ";
                else {
                    cout << " - ";
                    c = -c;
                }
            } else {
                if (c < 0) {
                    cout << "-";
                    c = -c;
                }
            }
            if (e == 0)
                cout << c;
            else if (e == 1)
                cout << c << "x";
            else
                cout << c << "x^" << e;
        }
        cout << endl;
    }
};

int main() {
    Polynomial p1, p2;
    p1.NewTerm(3, 2);
    p1.NewTerm(2, 1);
    p1.NewTerm(1, 0);
    p2.NewTerm(1, 1);
    p2.NewTerm(1, 1);

    cout << "P1(x) = "; p1.Show();
    cout << "P2(x) = "; p2.Show();

    Polynomial sum = p1.Add(p2);
    cout << "Sum = "; sum.Show();

    Polynomial product = p1.Mult(p2);
    cout << "Product = "; product.Show();

    cout << "P1(2) = " << p1.Eval(2) << endl;
    return 0;
}

```
## 效能分析
在測量效能時，您可以觀察到以下幾點，並考慮潛在的最佳化：
- Add 函式：目前 Add 函式會依序遍歷兩個多項式的項，這是一個效率不錯的合併操作（類似合併排序）。其效能主要取決於兩個多項式的項數。
- Mult 函式：這個函式的效能會較差，因為它使用一個迴圈嵌套另一個迴圈，並且在每次內部迴圈中都呼叫 Add 函式來合併同類項。
- Eval 函式：這個函式使用 pow 函式來計算每個項的冪次。
- NewTerm 函式：這個函式在每次插入新項時都會進行排序（氣泡排序），這會增加效能負擔。

## 測試與驗證
| 測試案例 | 輸入參數 $p1(x)$  | 預期輸出P1(x)  | 實際輸出 P1(x) | 
|----------|--------------|----------|----------|
| 測試一   |      3, 2     |             |         |       
|          |        2, 1    |            |        |  
|          |        1, 0    |    $3x^2$ + 2x + 1        |    $3x^2$ + 2x + 1      |  

| 測試案例 | 輸入參數 $p2(x)$  | 預期輸出P2(x)  | 實際輸出 P2(x) | 
|----------|--------------|----------|----------|
| 測試二  |      1, 1    |             |         |       
|          |       1, 1    |      2x      |  2x      | 

| 測試案例 |   預期輸出$p1(x)$ +    $p2(x)$  | 實際輸出 $p1(x)$ +    $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ +    $p2(x)$     |    $3x^2$ + 4x + 1         |  $3x^2$ + 4x + 1       |   

| 測試案例 |   預期輸出$p1(x)$ * $p2(x)$  | 實際輸出 $p1(x)$ * $p2(x)$ | 
|----------|----------|----------|
|   $p1(x)$ * $p2(x)$     |    $6x^3$ + $4x^2$ + 2x       |  $6x^3$ + $4x^2$ + 2x       |   

| 測試案例 |   預期輸出$p1(2)$  | 實際輸出 $p1(2)$ | 
|----------|----------|----------|
|   $p1(2)$      |   17    | 17     |   

### 編譯與執行指令
#### 測試輸入
```cpp
p1.NewTerm(3, 2);  
p1.NewTerm(2, 1);  
p1.NewTerm(1, 0);  
p2.NewTerm(1, 1);  
p2.NewTerm(1, 0);  
```
#### 測試輸出
```shell
P1(x) = 3x^2 + 2x + 1
P2(x) = 2x
Sum = 3x^2 + 4x + 1
Product = 6x^3 + 4x^2 + 2x
P1(2) = 17
```
## 效能量測
| 功能          | 時間複雜度           | 空間複雜度      | 說明                     |
| ----------- | --------------- | ---------- | ---------------------- |
| `NewTerm()` | (O(n))          | (O(n))     | 搜尋相同指數並排序              |
| `Add()`     | (O(n + m))      | (O(n + m)) | 同步合併兩多項式               |
| `Mult()`    | (O(n \times m)) | (O(n + m)) | 每一項與另一多項式所有項相乘         |
| `Eval()`    | (O(n))          | (O(1))     | 對每一項計算 ( c_i x^{e_i} ) |
## 申論及開發報告
本程式以「物件導向設計 (OOP)」為核心，採用類別封裝多項式運算邏輯：

- Term 類別封裝單一項（係數與指數）。

- Polynomial 類別負責多項式整體結構與運算。

- 使用 友元類別 (friend)，讓 Polynomial 能夠直接存取 Term 成員。

- 所有運算 (Add, Mult, Eval) 均為成員函式，實現高內聚。

- 採用 動態陣列，在多項式項數增加時自動擴容，避免記憶體不足。

開發重點：

- 保持項目依照指數遞減排序。

- 相同指數自動合併。

- 顯示格式貼近數學習慣，支援正負號輸出。

# Problem2
## 解題說明
- Term 結構：每一項包含一個係數和一個指數。
= Polynomial 類別：使用動態陣列 termArray 儲存多項式的各項。
= capacity 代表陣列的總容量，terms 代表實際儲存的項數。
= 提供 AddTerm 方法來新增項次，並且自動處理擴充陣列、合併同指數的項次。
= 提供加法、乘法、求值等運算方法。
- 運算子多載 (operator<<, operator>>) 使得多項式的輸入和輸出更加便捷。
## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);
private:
    float coef;  
    int exp;    
};
class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }
    void NewTerm(float c, int e) {
        if (c == 0) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    cout << "Enter number of terms: ";
    in >> n;

    poly.terms = 0; 

    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        in >> c >> e;
        poly.NewTerm(c, e);
    }

    return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-";
            c = -c;
        }

        if (e == 0)
            out << c;
        else if (e == 1)
            out << c << "x";
        else
            out << c << "x^" << e;
    }

    return out;
}

int main() {
    Polynomial p;

    cin >> p;           
    cout << "p(x) = " << p << endl; 

    float x;
    cout << "Enter x value: ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Eval(x) << endl;

    return 0;
}

```
## 效能分析
#### 時間複雜度NewTerm()：合併項次：最差情況下，需遍歷所有現有項次，時間複雜度為 $\(O(\text{terms})\)$
- 陣列擴充：當需要擴充容量時，會建立新陣列並複製所有舊項次，時間複雜度為 $\(O(\text{terms})\)$
- 排序：在每次新增項次後都進行排序。使用的泡沫排序時間複雜度為 $\(O(\text{terms}^{2})\)$
  若多項式項數較多，這會成為效能瓶頸
- 總體：綜合來看，NewTerm()的最差時間複雜度為 $\(O(\text{terms}^{2})\)$，主要受排序操作影響。
- Eval()：該方法遍歷所有項次，並對每個項次進行pow()運算，時間複雜度為 $\(O(\text{terms}\cdot \log (\text{exp}))\)$，其中pow()運算的時間複雜度取決於其演算法。
- operator<<()：該方法遍歷所有項次進行輸出，時間複雜度為 $\(O(\text{terms})\)$。operator>>()：在輸入 $\(n\)$ 個項次時，會迴圈呼叫 NewTerm() \(n\) 次。
- operator>>() 的總時間複雜度為 $\(O(n^{3})\)$。
#### 空間複雜度
- Polynomial 物件：termArray 是動態陣列，其空間隨項次增加而擴充。當有 \(n\) 個項次時，空間複雜度為 \(O(n)\)。
- NewTerm()：當擴充容量時，會建立一個兩倍大的臨時陣列，因此需要額外的 \(O(n)\) 空間。
## 測試與驗證
| 測試案例 | 輸入資料 | 
|----------|--------------|
| 測試一   |        3(個資料)   |     
|  第一資料        |1 1|
|  第二資料        |   2 1       | 
|  第三資料        |   2 3      | 
|  x的值        |   4    |

| 輸出資料 |  p(x)|p(4)| 
|----------|--------------|------|
| 所有資料統整  |       2x^3 + 3x   |   140  |

### 編譯與執行指令
#### 測試輸入
```shell
Enter number of terms: 3
Enter coefficient and exponent for term 1: 1 1
Enter coefficient and exponent for term 2: 2 1
Enter coefficient and exponent for term 3: 2 3

Enter x value: 4
```
#### 測試輸出
```shell
p(x) = 2x^3 + 3x

p(4) = 140
```
## 效能量測
優化建議：
- 改善排序：將NewTerm中的泡沫排序替換為更高效的演算法，例如插入排序。由於新項次只插入一個，插入排序的效能會比完整重新排序要好。更好的方法是在插入時保持排序狀態。
- 優化求值：對於 Eval，可以考慮使用霍納法則（Horner's method），這能避免在每個迴圈中重複計算pow()，從而提高求值速度。
## 申論及開發報告
### 效能討論：
分析目前實作的效能瓶頸（特別是NewTerm中的排序）。
提出優化建議，如使用更高效的排序演算法或霍納法則，並討論這些優化對時間複雜度的影響。
### 心得討論
這份程式碼提供了一個基礎的多項式類別實作，展示了物件導向程式設計的核心概念。然而，它的效能（特別是在處理大量項次時）有待改進。透過替換低效的排序演算法和優化求值方法，可以顯著提升程式的效率和可擴展性。這份報告不僅解釋了程式碼的功能，也指出了其局限性，並提出了明確的改進方向。
