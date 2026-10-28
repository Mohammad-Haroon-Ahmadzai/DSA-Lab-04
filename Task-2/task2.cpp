// Lab 4 - Task 2: Josephus Problem using Circular Linked List

#include <iostream>
using namespace std;

struct Person
{
    int id;
    Person *next;

    Person(int i) { id = i; next = NULL; }
};

// Build circle of n people (1..n), return pointer to the last person
Person* createCircle(int n)
{
    Person *first = new Person(1);
    Person *last = first;
    for (int i = 2; i <= n; i++)
    {
        last->next = new Person(i);
        last = last->next;
    }
    last->next = first; // close the circle
    return last;        // last->next is person 1
}

void showCircle(Person *last)
{
    Person *t = last->next;
    do
    {
        cout << t->id << " ";
        t = t->next;
    } while (t != last->next);
    cout << endl;
}

int main()
{
    int n, k;
    cout << "Enter number of people (N): ";
    cin >> n;
    cout << "Enter step count (k): ";
    cin >> k;

    if (n <= 0 || k <= 0)
    {
        cout << "N and k must be positive.\n";
        return 0;
    }

    Person *last = createCircle(n); // 'last' stays just behind the counting start
    cout << "Circle: ";
    showCircle(last);

    int order[100]; // stores elimination order
    int count = 0;
    int remaining = n;

    while (remaining > 1)
    {
        // move (k-1) steps so last->next is the k-th person
        for (int i = 1; i < k; i++) last = last->next;

        Person *victim = last->next;
        order[count++] = victim->id;
        last->next = victim->next; // unlink
        delete victim;
        remaining--;
    }

    cout << "Elimination order: ";
    for (int i = 0; i < count; i++) cout << order[i] << " ";
    cout << endl;

    cout << "Survivor: Person " << last->id << endl;
    delete last; // free the survivor

    cout << "Done by Mohammad Haroon Ahmadzai (578328)\n";
    return 0;
}
