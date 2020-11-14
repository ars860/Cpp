#include "victor.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool is_equal(vector<unsigned int> const &vec, victor const &vic) {
    if (vic.size() != 0) {
        victor _vic(vic);
        vector<unsigned int> vic_to_vec;
        while (_vic.size() > 0) {
            vic_to_vec.push_back(_vic.back());
            _vic.pop_back();
        }
        auto tmp = vec;
        reverse(tmp.begin(), tmp.end());
        if (vic_to_vec != tmp) {
            cout << "ERROR\n";
            cout << "vic\n";
            for (int i:vic_to_vec) {
                cout << i << " ";
            }
            cout << '\n';
            cout << "vec\n";
            for (int i:vec) {
                cout << i << " ";
            }
            cout << '\n';
            int hren;
            cin >> hren;
        }
    }
}

vector<unsigned int> convert(victor vic) {
    vector<unsigned int> vic_to_vec;
    while (vic.size() > 0) {
        vic_to_vec.push_back(vic.back());
        vic.pop_back();
    }
    return vic_to_vec;
}

bool is_equal(victor vec, victor vic) {
    auto tmp = convert(vec);
    reverse(tmp.begin(), tmp.end());
    return is_equal(tmp, vic);
}

int main() {
    victor a;

    cout << "second tests\n";


    victor test;
    test.print();
    for (int i = 1; i < 11; i++) {
        test.push_back(i);
        test.print();
    }

    while (test.size() > 0) {
        test.pop_back();
        test.print();
    }

    int stop = 1;

    cout << "random push/pop size() back() test\n";
    vector<unsigned int> cmp;
    victor victr;
    for (int i = 0; i < 100; i++) {
//        cout<<i<<'\n';
        int type = rand() % 2;
        if (type == 1) {
            //cout << "push " << i << endl;
            cmp.push_back(i);
            victr.push_back(i);
            is_equal(cmp, victr);

        } else {
            //cout << "pop " << endl;
            if (cmp.size() > 0)
                cmp.pop_back();
            victr.pop_back();
            is_equal(cmp, victr);
        }
        if (cmp.size() != victr.size()) {
            cout << "ERROR\n";
            cout << cmp.size() << " != " << victr.size() << '\n';
            int k;
            cin >> k;
        }
        if (cmp.size() > 0 && (cmp.back() != victr.back())) {
            cout << "ERROR\n";
            cout << cmp.back() << " != " << victr.back() << '\n';
            victr.print();
            int k;
            cin >> k;
        }
    }
    cout << "done\n";

    // push, pop - working

    cout << "Constructor[vector] [] test\n";

    for (int i = 0; i < 100; i++) {
        vector<unsigned int> a(rand() % 1000);
        generate(a.begin(), a.end(), rand);
        victor v(a);

        if (a.size() > 0)
            for (int j = 0; j < 10; j++) {
                int pos = (rand() % a.size());
                if (a[pos] != v[pos]) {
                    cout << "ERROE\n";
                    int k;
                    cin >> k;
                }
            }

        is_equal(a, v);
    }

    cout << "Done!\n";

    cout << "resize test\n";

    for (int i = 0; i < 100; i++) {
        int size = rand() % 1000;
        vector<unsigned int> a(size);
        generate(a.begin(), a.end(), rand);
        victor v(a);

        int newsize = size + (rand() % 1000);
        a.resize(newsize);
        v.resize(newsize);
        is_equal(a, v);
    }

    cout << "Done!\n";

    cout << "swap tests\n";

    for (int i = 0; i < 100; i++) {
        vector<unsigned int> a(rand() % 1000);
        generate(a.begin(), a.end(), rand);
        victor first(a), first_copy(a);
        generate(a.begin(), a.end(), rand);
        victor second(a), second_copy(a);

        first.swap(second);

//        first.print();
//        second.print();

        is_equal(first, second_copy);
        is_equal(second, first_copy);
    }
}
