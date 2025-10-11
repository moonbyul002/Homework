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
