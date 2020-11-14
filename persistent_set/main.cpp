#include <iostream>
#include <set>
#include "persistent_set.h"

using namespace std;

int main() {
    persistent_set<int> a;
    a.insert(5);
    a.insert(3);
    a.insert(8);
    a.insert(1);
    a.insert(2);

    a.erase(a.find(5));


    auto it = a.find(1);
    ++it;

    auto as = *a.rbegin();


    persistent_set<int> x;
    set<int> used;
    int amount = 5;

    for (int i = 0; i < amount; i++) {
        int r = 5;
        while(used.count(r)) {
           r = rand() % amount;
        }
        used.insert(r);

        x.insert(r);
    }

    for(auto i : x){
        cout << i << " ";
    }

    a = x;

    auto itt = x.end();
    itt--;
    itt++;
    bool ans = (it == x.end());


//    persistent_set<int> x_tmp;
//    swap(x, x_tmp);

    const persistent_set<int>& y = x;

    int z = *(y.begin());

    if(y.begin() == x.end())
        cout << "x.begin() == x.end()\n";

    used.clear();
    for (int i = 0; i < amount; i++) {
        int r = 5;
        while(used.count(r)) {
            r = rand() % amount;
        }
        used.insert(r);


        x.erase(x.find(r));
    }

////    persistent_set<int> x;
//    x.insert(5);
//    persistent_set<int> y;
//    y.insert(10);
//    y.insert(100);
//    for(auto i:x){
//        cout<<i << " ";
//    }
//    cout<<'\n';
//    for(auto i:y){
//        cout << i << " ";
//    }
//
//    persistent_set<int> p;
//    p.insert(5);
//    p.insert(3);
//    p.insert(4);
//    p.insert(1);
//    p.insert(6);
//
//    for(persistent_set<int>::iterator it = p.begin(); it != p.end() ; it++){
//        cout<< *it << " ";
//    }
//
//    persistent_set<int> p_copy(p);
//
//    auto nd = p.find(5);
//
//    p.erase(persistent_set<int>::iterator(nd));
//    p.erase(persistent_set<int>::iterator(p.find(4)));
//    p.erase(persistent_set<int>::iterator(p.find(1)));
//    p.erase(persistent_set<int>::iterator(p.find(3)));
//    p.erase(persistent_set<int>::iterator(p.find(6)));
//
//    //random tests
//
//    persistent_set<int> random;
////    set<int> used;
//    for (int i = 0; i < 100000; i++) {
//        int num = rand();
//        used.insert(num);
//        random.insert(num);
//    }
//
//    set<int> to_sravnit;
//    for(persistent_set<int>::iterator it = random.begin(); it != random.end() ; it++){
//        to_sravnit.insert(*it);
//    }
//    if(to_sravnit == used){
//        cout<<"ALL IS OKEY\n";
//    }
//
//
//    for (auto i: used) {
//        random.erase(random.find(i));
//    }
//
//    if(random.is_empty())
//        cout<<"EMPTY\n";
//
//    struct int_ {
//        int_() = delete;
//
//        int_(int x):x(x){}
//
//        bool operator>(int_ const& other){
//            return x > other.x;
//        }
//
//        bool operator<(int_ const& other){
//            return x < other.x;
//        }
//
//        bool operator==(int_ const other){
//            return x == other.x;
//        }
//    private:
//        int x;
//    };
//
//    persistent_set<int_> sadsa;
//
//    sadsa.insert(int_(1));
//
//    sadsa.erase(sadsa.find(int_(1)));
}