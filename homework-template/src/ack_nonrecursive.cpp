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
