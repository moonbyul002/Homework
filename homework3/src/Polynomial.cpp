
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
