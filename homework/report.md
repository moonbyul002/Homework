# 41343110 

## 作業一 Ackermann Function

# 遞迴方式

## 解題說明

Ackermann 函數是一個典型的遞迴函數，其數學定義如下： 1.​ n+1 m=0 A(m,n)= 2. A(m−1,1) m>0,n=0 3. A(m-1,A(m,n-1)) m>0,n>0

### Ackermann 函數定義如下 :
| A(m,n)                 | 條件                  |
|------------------------|----------------------|
| n + 1                  | 當 m = 0             |
| A(m-1, 1)              | 當 n = 0             |
| A(m-1, A(m, n-1))      | 其他                 |

### 解題策略

#### 題目邏輯

1.先將 Ackermann 函數的數學定義轉化為程式邏輯。

2.使用 if/else if 依條件決定不同遞迴路徑。

#### 程式流程

1.主程式讀入使用者輸入的 m、n 值

2.呼叫遞迴函式計算結果

3.輸出結果

#### 測試策略

1.從最小值測試（m=0, n=0）開始，確保基礎情況正確。

2.逐步增加 m 和 n，驗證程式正確性與堆疊使用。

3.記錄結果，與數學公式計算結果比對。

## 程式實作

以下為主要程式碼：

```cpp
#include<iostream>
using namespace std;
int ack(int m, int n) {
	if (m == 0)return n + 1;
	else if ((m > 0) && (n == 0)) return ack(m - 1, 1);
	else if ((m > 0) && (n > 0)) return ack(m - 1, ack(m, n - 1));

}
int main() {
	int A;
	int m, n;
	cout << "A(m,n)" << endl;
	cout << "輸入m值" << endl;
	cin >> m;
	cout << "輸入m值" << endl;
	cin >> n;
	A = ack(m, n);
	cout << A << endl;
	return 0;

}
```
## 效能分析
1. 時間複雜度：沒有一個多項式或指數函數可以準確描述它的時間複雜度。例如:
	ack(1, n) ≈ O(n),
	ack(2, n) ≈ O(2n),
	ack(3, n) ≈ O(2^n),
	ack(4, n) ≈ 雙重指數 (2^(2^(...n...))),ack(5, n) 幾乎無法計算，遞迴次數巨大。

2. 空間複雜度：由於是純遞迴實作，呼叫堆疊深度與遞迴層數有關。
	每次呼叫都會：
	建立一個新的「呼叫紀錄」（activation record）
	直到達成 m == 0 為止
	所以：
	空間複雜度 = 遞迴深度
	若以 A(3, n) 為例，深度約為 O(2n)
	若以 A(4, 1) 為例，可能需要數萬層遞迴，容易造成 Stack Overflow。

### 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $m$ |  輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|----------|
| 測試一   | $m = 0$      | $n = 0$   | 1 (n+1)  | 1     |
| 測試二   | $m = 1$      | $n = 3$ |    5      |5      |
| 測試三   | $m = 2$      |$n = 3$|      9    |9      |
| 測試四   | $m = 3$      |$n = 4$|     125    |125    |
| 測試五   | $m = 4$     |$n = 1$|    65533     |異常拋出   |

### 編譯與執行指令

```shell
$ g++ ack.cpp -std=c++14 -o ack (Visual Studio 2022 為C++14)
$ ack.exe
A(m,n)
輸入 m 值:
(輸入)2
輸入 n 值:
(輸入)3
(結果): 9
```

## 開發報告

### 程式目的

本程式實作 Ackermann 函數（Ackermann Function），
此函數是理論計算機科學中著名的遞迴函數之一，用來展示「遞迴深度」與「非原始遞迴（non-primitive recursive）」的概念。

### 程式設計與說明

程式使用 C++14，核心是遞迴函數 ack：
```cpp

int ack(int m, int n) {
    if (m == 0) return n + 1;
    else if (n == 0) return ack(m - 1, 1);
    else return ack(m - 1, ack(m, n - 1));
}
```
m = 0 → 回傳 n+1
n = 0 → 遞迴呼叫 ack(m-1, 1)
其他情況 → 雙層遞迴 ack(m-1, ack(m, n-1))

#### main 主程式流程：

1.輸入 m 和 n

2.呼叫 ack(m,n) 計算結果

3.輸出結果

## 開發心得與申論

#### 程式改進：

1.可加入呼叫計數器以觀察遞迴呼叫次數。

2.可加入最大遞迴層數警告，避免 Stack Overflow。

#### 實務應用：

Ackermann 函數不適合用於實務計算，主要用於理論分析。

# 非遞迴方式

## 解題說明

Ackermann 函數是一個典型的遞迴函數，其數學定義如下： 1.​ n+1 m=0 A(m,n)= 2. A(m−1,1) m>0,n=0 3. A(m-1,A(m,n-1)) m>0,n>0

### Ackermann 函數定義如下 :
| A(m,n)                 | 條件                  |
|------------------------|----------------------|
| n + 1                  | 當 m = 0             |
| A(m-1, 1)              | 當 n = 0             |
| A(m-1, A(m, n-1))      | 其他                 |

		 
### 解題策略

#### 題目邏輯

- 傳統遞迴版本每次呼叫 ack(m, n) 時，系統會自動建立新的函數堆疊。
  
- 非遞迴版本則是「手動模擬」這個堆疊，用陣列 stackM 取代系統堆疊。
  
- 每次遇到要「再呼叫自己」的情況時，改成：

- 把要計算的狀態（m 值）推入 stack

- 調整 n 的值

- 再進入下一輪迴圈模擬呼叫

#### 輸入階段

1.使用者輸入兩個整數 m、n，代表 Ackermann 函數的參數。

2.初始化堆疊

	- 宣告陣列 stackM[1000] 來模擬系統堆疊，
	
	- 並設變數 top = -1 作為堆疊頂端指標。
	
	- 將初始值 m 推入堆疊。
	
3.迴圈處理 (模擬遞迴)

	當堆疊不為空 $(top >= 0)$ 時：
	
	取出堆疊最上層的 m 值。
	
#### 測試策略

1.從最小值測試 $(m=0, n=0)$ 開始，確保基礎情況正確。

2.逐步增加 m 和 n，驗證程式正確性與堆疊使用。

3.記錄結果，與數學公式計算結果比對。

## 程式實作
以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

int ack_non_recursive(int m, int n) {
    int stackM[1000];  
    int top = -1;      

    stackM[++top] = m; 

    while (top >= 0) {
        m = stackM[top--]; 

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            n = 1;
            stackM[++top] = m - 1; 
        }
        else {
            
            stackM[++top] = m - 1;
            stackM[++top] = m;
            n = n - 1;
        }
    }

    return n;
}

int main() {
    int m, n;
    cout << "A(m,n)" << endl;
    cout << "輸入m值: ";
    cin >> m;
    cout << "輸入n值: ";
    cin >> n;

    int result = ack_non_recursive(m, n);
    cout << "Ackermann(" << m << "," << n << ") = " << result << endl;

    return 0;
}
```
## 效能分析

1. 時間複雜度：沒有封閉型時間公式，但可觀察到：

	- 當 m = 0 → 計算時間為 $O(1)$ 

	- 當 m = 1 → 時間複雜度約為 $O(n)$ 

	- 當 m = 2 → 時間約為 $O(2n)$ 

	- 當 m = 3 → 已呈指數級別增長

2. 空間複雜度：空間使用為 $O(k)$ ，其中 k 為堆疊深度。
   
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $m$ |  輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|----------|
| 測試一   | $m = 0$      | $n = 0$   | 1 (n+1)  | 1     |
| 測試二   | $m = 1$      | $n = 3$ |    5      |5      |
| 測試三   | $m = 2$      |$n = 3$|      9    |9      |
| 測試四   | $m = 3$      |$n = 4$|     125    |125    |
| 測試五   | $m = 4$     |$n = 1$|    65533     |異常拋出   |

### 編譯與執行指令

```shell
$ g++ ack_nonrecursive.cpp -std=c++14 -o ack_nonrecursive (Visual Studio 2022 為C++14)
$ ack_nonrecursive.exe
A(m,n)
輸入m值:(輸入) 0
輸入n值:(輸入) 0
Ackermann(0,0) = 1
```

### 結論

## 開發報告

### 程式目的

本程式實作 Ackermann 函數（Ackermann Function），
此函數是理論計算機科學中著名的遞迴函數之一，用來展示「遞迴深度」與「非原始遞迴（non-primitive recursive）」的概念。

### 程式設計與說明

#### 程式運作流程

1.將初始的 m 值推入堆疊。

2.當堆疊不為空時：

	取出最上層的 m。
	
	- 若 m == 0 → 直接令 n = n + 1。
	
	- 若 n == 0 → 推入 $(m - 1)$ ，並令 n = 1。
	
	- 否則同時推入 m 與 $(m - 1)$ ，並令 n = n - 1，模擬 A(m-1, A(m, n-1)) 的過程。
	
3. 直到堆疊為空時，n 即為最終結果。
   
#### 優缺點

1. 優點：

	- 避免系統遞迴造成的 Stack Overflow。

	- 可清楚觀察每層模擬過程。

2. 限制：
   
	- 若輸入值過大，仍會造成迴圈次數過多、執行時間過長。

	- 陣列堆疊大小固定為 1000，超出將導致記憶體錯誤。

## 開發心得與申論

在學習遞迴時，Ackermann 函數是一個極具代表性的範例，它能測試電腦在深層遞迴下的極限。
透過手動推入與彈出堆疊，能清楚掌握每一步運算的對應關係。
在實作過程中，也體會到堆疊是遞迴的底層實現機制，程式的運作其實就是「由堆疊控制的反覆運算」。
## 作業一 之Powerset

## 解題說明
本題目要求對一個給定集合 
S生成其所有子集合（powerset），並將每個子集合印出。
集合為 {a, b, c}，其子集合為 {}, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c}。

### 解題策略

對每個元素做二元決策：

1.不選擇 → 遞迴下一個元素

2.選擇 → 將元素加入子集合，遞迴下一個元素

遞迴終止條件：
當處理完最後一個元素，印出當前子集合

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;
void printSubset(char subset[], int size) {
    cout << "{";
    for (int i = 0; i < size; i++) {
        cout << subset[i];
        if (i < size - 1) cout << ",";
    }
    cout << "}" << endl;
}
void powerset(char set[], int n, int index, char subset[], int subsetSize) {
    if (index == n) {
        
        printSubset(subset, subsetSize);
        return;
    } 
    powerset(set, n, index + 1, subset, subsetSize);
    subset[subsetSize] = set[index];
    powerset(set, n, index + 1, subset, subsetSize + 1);
}
int main() {
    char set[] = { 'a', 'b', 'c' };
    int n = 3;
    char subset[10]; 
    powerset(set, n, 0, subset, 0);
    return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(n⋅2^n)$。
2. 空間複雜度：空間複雜度為 $O(n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出                 | 實際輸出 |
|----------|--------------|----------              |----------|
| 測試一   | 輸入程式已固定      |      ()                    | () |
 |         |                   |        (a)                      | (c)|
 |          |                   |         (b)                    | (b)|
 |         |                    |           (c)                  | (b,c)|
  |        |                    |             (a,b)                | (a)|
 |         |                    |               (a,c)              |(a,c)|
 |         |                    |                 (b,c)            |(a,b)|
 |         |                     |                  (a,b,c)          |(a,b,c)         |


### 編譯與執行指令

```shell
$ g++ powerset.cpp -std=c++14 -o powerset (Visual Studio 2022 為C+14)
$ powerset.exe
(輸出):
()
(c)
(b)
(b,c)
(a)
(a,c)
(a,b)
(a,b,c)
```

### 結論



    

## 開發報告

### 研究背景與目的

集合的子集合（powerset）在計算機科學中非常常見

本程式的目的是透過 遞迴方法，生成給定集合的所有子集合，並展示其內容。
### 程式邏輯

每個元素有兩種選擇：

1.不加入子集合

2.加入子集合

使用遞迴逐層選擇每個元素，直到處理完所有元素。
### 程式設計
#### printSubset
```cpp
void printSubset(char subset[], int size) {
    cout << "{";
    for (int i = 0; i < size; i++) {
        cout << subset[i];
        if (i < size - 1) cout << ",";
    }
    cout << "}" << endl;
}
```
功能：印出子集合內容

輸入：子集合陣列 subset[] 與大小 size
#### powerset
```cpp
void powerset(char set[], int n, int index, char subset[], int subsetSize) {
    if (index == n) {
        printSubset(subset, subsetSize);
        return;
    }
    // 不選擇當前元素
    powerset(set, n, index + 1, subset, subsetSize);
    // 選擇當前元素
    subset[subsetSize] = set[index];
    powerset(set, n, index + 1, subset, subsetSize + 1);
}
```
功能：遞迴生成所有子集合

遞迴結束條件：index == n → 印出子集合
#### main 主程式
```cpp
int main() {
    char set[] = { 'a', 'b', 'c' };
    int n = sizeof(set)/sizeof(set[0]);
    char* subset = new char[n];
    powerset(set, n, 0, subset, 0);
    delete[] subset;
    return 0;
}
```
功能：初始化集合與子集合陣列

呼叫 powerset 遞迴函數
## 申論及開發報告
使用 subset 陣列存儲中間結果，易於理解
集合的子集合（powerset）是一個基礎而重要的概念，在資料結構與演算法中應用廣泛。
在程式設計上，我們利用一個陣列暫存當前子集合，遞迴過程中根據選擇動態更新陣列內容，當遞迴到底時，印出子集合。
