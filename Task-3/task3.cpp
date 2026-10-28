// Lab 4 - Task 3: Binary Arithmetic using Doubly Linked List

#include <iostream>
#include <string>
using namespace std;

struct Bit
{
    int val;
    Bit *next;
    Bit *prev;

    Bit(int v) { val = v; next = NULL; prev = NULL; }
};

class BinaryNumber
{
public:
    Bit *head;
    Bit *tail;
    int size;

    BinaryNumber() { head = tail = NULL; size = 0; }
    ~BinaryNumber() { clear(); }

    void clear()
    {
        while (head != NULL)
        {
            Bit *t = head;
            head = head->next;
            delete t;
        }
        tail = NULL;
        size = 0;
    }

    void addBack(int v) // add at LSB side
    {
        Bit *n = new Bit(v);
        if (tail == NULL) head = tail = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        size++;
    }

    void addFront(int v) // add at MSB side
    {
        Bit *n = new Bit(v);
        if (head == NULL) head = tail = n;
        else { n->next = head; head->prev = n; head = n; }
        size++;
    }

    // 1. store number, group into 8-bit blocks (pad zeros on the left)
    bool store(string s)
    {
        clear();
        for (int i = 0; i < (int)s.length(); i++)
        {
            if (s[i] != '0' && s[i] != '1') return false;
            addBack(s[i] - '0');
        }
        while (size % 8 != 0) addFront(0);
        return true;
    }

    void show(string label)
    {
        cout << label;
        int c = 0;
        for (Bit *t = head; t != NULL; t = t->next)
        {
            cout << t->val;
            c++;
            if (c % 8 == 0 && t->next != NULL) cout << " ";
        }
        cout << endl;
    }

    void copyFrom(BinaryNumber &o)
    {
        clear();
        for (Bit *t = o.head; t != NULL; t = t->next) addBack(t->val);
    }

    // 2. one's complement
    void onesComplement()
    {
        for (Bit *t = head; t != NULL; t = t->next)
            t->val = 1 - t->val;
    }

    // 3. two's complement = ones + 1 (add 1 starting from LSB, same width)
    void twosComplement()
    {
        onesComplement();
        int carry = 1;
        Bit *t = tail;
        while (t != NULL && carry == 1)
        {
            if (t->val == 1) t->val = 0;          // 1+1 = 0 carry 1
            else { t->val = 1; carry = 0; }       // 0+1 = 1 no carry
            t = t->prev;
        }
    }

    // 6. decimal value
    long long toDecimal()
    {
        long long total = 0, power = 1;
        for (Bit *t = tail; t != NULL; t = t->prev)
        {
            if (t->val == 1) total += power;
            power *= 2;
        }
        return total;
    }
};

// 4. binary addition, result stored in a new list
void addBinary(BinaryNumber &a, BinaryNumber &b, BinaryNumber &res)
{
    res.clear();
    Bit *p = a.tail;
    Bit *q = b.tail;
    int carry = 0;

    while (p != NULL || q != NULL)
    {
        int x = (p != NULL) ? p->val : 0;
        int y = (q != NULL) ? q->val : 0;
        int sum = x + y + carry;
        res.addFront(sum % 2);
        carry = sum / 2;
        if (p != NULL) p = p->prev;
        if (q != NULL) q = q->prev;
    }
    if (carry == 1) res.addFront(1);
    while (res.size % 8 != 0) res.addFront(0);
}

// 5. multiplication: repeated addition + shifting
void multiplyBinary(BinaryNumber &a, BinaryNumber &b, BinaryNumber &res)
{
    BinaryNumber shifted, temp;
    shifted.copyFrom(a);
    res.clear();
    for (int i = 0; i < 8; i++) res.addBack(0); // start from 0

    for (Bit *q = b.tail; q != NULL; q = q->prev)
    {
        if (q->val == 1) // add shifted copy when bit is 1
        {
            addBinary(res, shifted, temp);
            res.copyFrom(temp);
        }
        shifted.addBack(0); // shift left by one position
    }

    // remove extra all-zero 8-bit blocks on the left (keep at least one block)
    while (res.size > 8)
    {
        Bit *t = res.head;
        bool blockZero = true;
        for (int i = 0; i < 8; i++) { if (t->val == 1) blockZero = false; t = t->next; }
        if (!blockZero) break;

        for (int i = 0; i < 8; i++)
        {
            Bit *d = res.head;
            res.head = d->next;
            res.head->prev = NULL;
            delete d;
            res.size--;
            if (i == 7) break;
        }
    }
}

int main()
{
    BinaryNumber A, B, R;
    string s1, s2;
    int choice;

    do
    {
        cout << "\n===== BINARY CALCULATOR (DLL) =====\n";
        cout << "1. Store Binary Number\n2. 1's Complement\n3. 2's Complement\n";
        cout << "4. Addition\n5. Multiplication\n6. Convert to Decimal\n0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Enter first binary number: "; cin >> s1;
            cout << "Enter second binary number: "; cin >> s2;
            if (A.store(s1) && B.store(s2))
            {
                A.show("First number  : ");
                B.show("Second number : ");
            }
            else cout << "Only 0 and 1 are allowed!\n";
        }
        else if (choice == 2)
        {
            int which;
            cout << "Which number? (1 = first, 2 = second): ";
            cin >> which;
            BinaryNumber &X = (which == 2) ? B : A; // pick A or B
            R.copyFrom(X);
            R.onesComplement();
            X.show("Original       : ");
            R.show("1's complement : ");
        }
        else if (choice == 3)
        {
            int which;
            cout << "Which number? (1 = first, 2 = second): ";
            cin >> which;
            BinaryNumber &X = (which == 2) ? B : A; // pick A or B
            R.copyFrom(X);
            R.twosComplement();
            X.show("Original       : ");
            R.show("2's complement : ");
        }
        else if (choice == 4)
        {
            addBinary(A, B, R);
            A.show("A     : ");
            B.show("B     : ");
            R.show("A + B : ");
            cout << "Decimal: " << A.toDecimal() << " + " << B.toDecimal()
                 << " = " << R.toDecimal() << endl;
        }
        else if (choice == 5)
        {
            multiplyBinary(A, B, R);
            A.show("A     : ");
            B.show("B     : ");
            R.show("A * B : ");
            cout << "Decimal: " << A.toDecimal() << " x " << B.toDecimal()
                 << " = " << R.toDecimal() << endl;
        }
        else if (choice == 6)
        {
            cout << "First number in decimal = " << A.toDecimal() << endl;
            cout << "Second number in decimal = " << B.toDecimal() << endl;
        }
        else if (choice != 0) cout << "Invalid choice.\n";

    } while (choice != 0);

    cout << "Program by Mohammad Haroon Ahmadzai (578328)\n";
    return 0;
}
