
#include <iostream>
#include <cmath>
using namespace std;

class Polynomial;  // forward declaration

// ------------------ Term 類別 ------------------
class Term {
    friend class Polynomial; // 讓 Polynomial 可以直接存取 Term 的成員
private:
    float coef;  // 係數
    int exp;     // 指數
};

// ------------------ Polynomial 類別 ------------------
class Polynomial {
private:
    Term* termArray;  // 儲存非零項的陣列
    int capacity;     // 陣列容量
    int terms;        // 實際使用中的項數

public:
    // 建構子：建立 p(x)=0
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // 拷貝建構子
    Polynomial(const Polynomial& poly) {
        capacity = poly.capacity;
        terms = poly.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = poly.termArray[i];
    }


    
    // 插入一個新項（內部使用）
    void NewTerm(float c, int e) {
        if (c == 0) return; // 不加入零係數

        // 若有相同的指數項，則合併
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    // 若合併後變成 0，則刪除此項
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }

        // 若陣列滿，擴充容量
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        // 插入新項
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        // 保持以指數遞減排序
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    // ------------------ Add ------------------
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
        // 剩餘項
        for (; aPos < terms; aPos++)
            result.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
        for (; bPos < poly.terms; bPos++)
            result.NewTerm(poly.termArray[bPos].coef, poly.termArray[bPos].exp);
        return result;
    }

    // ------------------ Mult ------------------
    Polynomial Mult(Polynomial poly) {
        Polynomial result;

        // 對 this 的每一項
        for (int i = 0; i < terms; i++) {
            Polynomial temp;

            // 對 poly 的每一項
            for (int j = 0; j < poly.terms; j++) {
                float newCoef = termArray[i].coef * poly.termArray[j].coef;
                int newExp = termArray[i].exp + poly.termArray[j].exp;

                // 暫時乘出的每一項放進 temp
                temp.NewTerm(newCoef, newExp);
            }

            // 累加到結果（用 Add() 合併同類項）
            result = result.Add(temp);
        }

        return result;
    }

    // ------------------ Eval ------------------
    float Eval(float f) {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(f, termArray[i].exp);
        return result;
    }

    // ------------------ 輸出多項式 ------------------
    void Show() {
        if (terms == 0) {
            cout << "0" << endl;
            return;
        }

        for (int i = 0; i < terms; i++) {
            float c = termArray[i].coef;
            int e = termArray[i].exp;

            // 印出符號
            if (i > 0) {
                if (c >= 0) cout << " + ";
                else {
                    cout << " - ";
                    c = -c; // 將係數轉為正數輸出
                }
            }
            else {
                if (c < 0) {
                    cout << "-";
                    c = -c;
                }
            }

            // 根據不同指數輸出格式
            if (e == 0)
                cout << c;               // 常數項
            else if (e == 1)
                cout << c << "x";        // 一次項
            else
                cout << c << "x^" << e;  // 其他次方
        }

        cout << endl;
    }
};


int main() {
    Polynomial p1, p2;
    p1.NewTerm(3, 2);  // 3x^2
    p1.NewTerm(2, 1);  // 2x
    p1.NewTerm(1, 0);  // 1

    p2.NewTerm(1, 1);  // x
    p2.NewTerm(1, 1);  // +1

    cout << "P1(x) = "; p1.Show();
    cout << "P2(x) = "; p2.Show();

    Polynomial sum = p1.Add(p2);
    cout << "Sum = "; sum.Show();

    Polynomial product = p1.Mult(p2);
    cout << "Product = "; product.Show();

    cout << "P1(2) = " << p1.Eval(2) << endl;

    return 0;
}
