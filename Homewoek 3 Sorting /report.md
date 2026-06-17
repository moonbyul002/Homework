# 41343110 and 41343153
# Homework Sorting Project
## 解題說明
首先我們先了解本次作業的目標是由 
- 插入排序
- 快速排序 
- 歸併排序 
- 堆排序

這些去設計出一個複合排序函數
，使其在最壞時間準則下表現良好。
## 程式實作
```
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std; 


// Insertion Sort 
void InsertionSortRange(vector<int>& arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void InsertionSort(vector<int>& arr) {
    if (arr.empty()) return;
    InsertionSortRange(arr, 0, arr.size() - 1);
}

// Quick Sort 
int medianOfThree(vector<int>& arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[left] > arr[mid]) swap(arr[left], arr[mid]);
    if (arr[left] > arr[right]) swap(arr[left], arr[right]);
    if (arr[mid] > arr[right]) swap(arr[mid], arr[right]);
    return mid; 
}

void QuickSortHelper(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int pivotIdx = medianOfThree(arr, left, right);
    int pivotVal = arr[pivotIdx]; 


    swap(arr[pivotIdx], arr[right - 1]);

    int i = left;
    int j = right - 1;

    while (true) {
        while (arr[++i] < pivotVal) {}
        while (arr[--j] > pivotVal) {}
        if (i < j) {
            swap(arr[i], arr[j]);
        }
        else {
            break;
        }
    }
 
    swap(arr[i], arr[right - 1]);

    QuickSortHelper(arr, left, i - 1);
    QuickSortHelper(arr, i + 1, right);
}

void QuickSort(vector<int>& arr) {
    if (arr.size() <= 1) return;
    QuickSortHelper(arr, 0, arr.size() - 1);
}

// Merge Sort 
void Merge(vector<int>& arr, int left, int mid, int right, vector<int>& tmp) {
    int i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= right) tmp[k++] = arr[j++];
    for (int l = left; l <= right; ++l) arr[l] = tmp[l];
}

void MergeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    vector<int> tmp(n);
    for (int sz = 1; sz < n; sz *= 2) {
        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}

// Heap Sort (Max-Heap)
void Heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        swap(arr[i], arr[largest]);
        Heapify(arr, n, largest);
    }
}
// Heap Sort
void HeapSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;
    for (int i = n / 2 - 1; i >= 0; --i) Heapify(arr, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);
        Heapify(arr, i, 0); 
    }
}

// Composite Sort function 
void compositeSort(vector<int>& arr) {
    int n = arr.size();
    if (n <= 1) return;

    int threshold = 20;
    vector<int> tmp(n);

    for (int sz = 1; sz < n; sz *= 2) {
        if (sz < threshold) {
            for (int left = 0; left < n; left += 2 * sz) {
                int right = min(left + 2 * sz - 1, n - 1);
                InsertionSortRange(arr, left, right);
            }
            continue; 
        }

        for (int left = 0; left < n - sz; left += 2 * sz) {
            int mid = left + sz - 1;
            int right = min(left + 2 * sz - 1, n - 1);
            Merge(arr, left, mid, right, tmp);
        }
    }
}


// make a test 
vector<int> worstCaseInsertion(int n) {
    vector<int> arr(n);
    for (int i = 0; i < i; ++i) arr[i] = n - i; 
    
    for (int i = 0; i < n; ++i) arr[i] = n - i;
    return arr;
}

void generateMergeWorst(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    if (right - left == 1) {
        swap(arr[left], arr[right]);
        return;
    }
    vector<int> tmp(right - left + 1);
    int mid = left + (right - left) / 2;
    int lIdx = 0, rIdx = (mid - left + 1);

    for (int i = left; i <= right; i += 2) {
        tmp[lIdx++] = arr[i];
        if (i + 1 <= right) tmp[rIdx++] = arr[i + 1];
    }
    for (int i = left; i <= right; ++i) arr[i] = tmp[i - left];

    generateMergeWorst(arr, left, mid);
    generateMergeWorst(arr, mid + 1, right);
}

vector<int> worstCaseMerge(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    generateMergeWorst(arr, 0, n - 1);
    return arr;
}

void Permute(vector<int>& arr) {
    int n = arr.size();
    for (int i = n - 1; i >= 1; i--) {
        int j = rand() % (i + 1);
        swap(arr[j], arr[i]);
    }
}

vector<int> randomPermutation(int n) {
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) arr[i] = i + 1;
    Permute(arr);
    return arr;
}


typedef void(*SortFunc)(vector<int>&);

double testSort(SortFunc sortFunc, const vector<int>& baseArr, int repeat = 1) {
    clock_t startTotal = clock();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = baseArr; 
        sortFunc(temp);             
    }
    clock_t endTotal = clock();

    clock_t startOverhead = clock();
    for (int i = 0; i < repeat; ++i) {
        vector<int> temp = baseArr; 
    }
    clock_t endOverhead = clock();

    long totalTicks = (endTotal - startTotal) - (endOverhead - startOverhead);
    if (totalTicks < 0) totalTicks = 0;

    double ms = 1000.0 * totalTicks / CLOCKS_PER_SEC;
    return ms / repeat;
}


int main() {
    srand(2026);

    vector<int> ns = { 500, 1000, 2000, 3000, 4000, 5000 };
    int heapPerms = 15;    
    int averagePerms = 20; 

  
    cout << " 最壞狀況分析 (Worst-case Runtimes in ms)\n";
   

    for (int n : ns) {
        int r = (n <= 1000) ? 200 : 20;

        vector<int> insWorst = worstCaseInsertion(n);
        vector<int> mergeWorst = worstCaseMerge(n);

        double tIns = testSort(InsertionSort, insWorst, r);
        double tM = testSort(MergeSort, mergeWorst, r);
        double tC = testSort(compositeSort, insWorst, r);

        double tQ_max = 0;
        double tH_max = 0;
        for (int i = 0; i < heapPerms; ++i) {
            vector<int> randArr = randomPermutation(n);
            tQ_max = max(tQ_max, testSort(QuickSort, randArr, r));
            tH_max = max(tH_max, testSort(HeapSort, randArr, r));
        }

        cout << "n = " << n << " ->\n";
        cout << "  Insertion: " << tIns << " ms\tQuick(Max): " << tQ_max << " ms\tMerge: " << tM 
             << " ms\tHeap(Max): " << tH_max << " ms\tComposite: " << tC << " ms\n\n";
    }

    cout << "平均狀況分析 (Average Runtimes in ms)\n";

    for (int n : ns) {
        double aIns = 0, aQ = 0, aM = 0, aH = 0, aC = 0;
        int r = (n <= 1000) ? 50 : 5; 

        for (int i = 0; i < averagePerms; ++i) {
            vector<int> randArr = randomPermutation(n);

            aIns += testSort(InsertionSort, randArr, r);
            aQ += testSort(QuickSort, randArr, r);
            aM += testSort(MergeSort, randArr, r);
            aH += testSort(HeapSort, randArr, r);
            aC += testSort(compositeSort, randArr, r);
        }

        cout << "n = " << n << " -> \n";
        cout << "  Insertion: " << aIns / averagePerms << " ms\tQuick: " << aQ / averagePerms
             << " ms\tMerge: " << aM / averagePerms << " ms\tHeap: " << aH / averagePerms
             << " ms\tComposite: " << aC / averagePerms << " ms\n\n";
    }

    return 0;
}
```
## 效能分析
先了解在不同的排序方法中，每個的時間複雜度在平均、最壞以及最好的情況下為多少。
| Sorting                  | 平均                 | 最壞                                           |最好|
|-----------------------|-----------------------------|---------------------------|------------------------------------|
|Insertion Sort|$O(n^2)$|$O(n^2)$|$O(n)$|
|Quick Sort||||
|Merge Sort||||
|Heap Sort||||
|Composite Sort||||

## 測試與驗證
| 順序 | 輸入參數                |  預期輸出     | 說明                             | 
|----|-------------------------|----------------------------------------|----------------------------------------|
| 一 ||
| 二 ||
| 三 ||
| 四 ||
| 五 ||
| 六 ||
| 七 ||
| 八 ||
| 九 ||
| 十 ||
| 十一 ||
| 十二 ||
| 十三 ||
| 十四 ||
| 十五 ||
| 十六 ||  
| 十七 ||
| 十八 ||    
| 十九 ||      
| 二十 ||       
### 測試輸出
```
Clock accuracy: 0.0001 ms
n = 500 (worst-case ms)
Insertion: 0    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 500 (average-case ms)
Insertion: 0.05 Quick: 0        Merge: 0.05     Heap: 0.1       Composite: 0.05
n = 1000 (worst-case ms)
Insertion: 2    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 1000 (average-case ms)
Insertion: 0    Quick: 0.1      Merge: 0.1      Heap: 0.2       Composite: 0.2
n = 2000 (worst-case ms)
Insertion: 9    Quick: 0        Merge: 0        Heap: 1 Composite: 0
n = 2000 (average-case ms)
Insertion: 0.15 Quick: 0.15     Merge: 0.25     Heap: 0.35      Composite: 0.35
n = 3000 (worst-case ms)
Insertion: 18   Quick: 1        Merge: 0        Heap: 1 Composite: 0
n = 3000 (average-case ms)
Insertion: 0.1  Quick: 0.45     Merge: 0.35     Heap: 0.7       Composite: 0.4
n = 4000 (worst-case ms)
Insertion: 34   Quick: 1        Merge: 0        Heap: 1 Composite: 0
n = 4000 (average-case ms)
Insertion: 0.35 Quick: 0.55     Merge: 0.55     Heap: 0.95      Composite: 0.55
n = 5000 (worst-case ms)
Insertion: 49   Quick: 1        Merge: 1        Heap: 2 Composite: 1
n = 5000 (average-case ms)
Insertion: 0.15 Quick: 0.65     Merge: 0.8      Heap: 1.15      Composite: 0.8
```
## 申論及開發報告
### 結論
