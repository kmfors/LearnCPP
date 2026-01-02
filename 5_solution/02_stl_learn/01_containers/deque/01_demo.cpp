#include <iostream>
#include <string>
#include <deque>
using namespace std;

/**
 *
*/

int main(int argc, char* argv[])
{
    deque<int> dq{1, 2, 3};

    dq.push_front(4);
    dq.push_front(0);

    dq.pop_back();
    dq.pop_front();

    cout << "First of elem:" << dq.front() << endl;
    cout << "End of elem:" << dq.back() << endl;
    cout << "Second of elem:" << dq[1] << endl;
    cout << "Third of elem:" << dq.at(2) << endl;

    for(int num: dq) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}