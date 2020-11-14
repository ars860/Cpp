#include <iostream>
#include "big_integer.h"
//#include <cmath>

int main() {

    unsigned long long MAX = (unsigned long long)UINT32_MAX + 1;
    int base2 = 32;

    unsigned int a = 1,b = 1,c = 1,x = 1,y = 1;

    unsigned long long tmp = ((a << base2) + b) / ((x << base2) + y);
    unsigned long long remainder =  ((a << base2) + b) - tmp * (((x << base2) + y));
    unsigned long long tmp1 = ((remainder << base2) + c) / ((x << base2) + y);
    unsigned long long res = (tmp << base2) + tmp1;
    if (res > MAX - 1) {
        res = MAX - 1;
    }

    cout<<res;

//    vector<unsigned int> x = {123, 123, 0}, y = {10101, 101};
//    big_integer tmp, xx(x), yy(y);
//    tmp.sub_equal_vectors(x, y);
//    xx -= yy;
//    for (auto i:x) {
//        cout << i << " ";
//    }
//    xx.stupid_print();
//    cout<<(long long)UINT32_MAX+1;
//    cout<<to_string(big_integer("100"))<<'\n';
//    cout<<to_string(big_integer("0100"))<<'\n';
//    cout<<to_string(big_integer("0"))<<'\n';
//    cout<<to_string(big_integer("-0"))<<'\n';
//    string str = "-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000";
//    big_integer a = big_integer(str);
//    cout<<'\n';
//    cout<<str<<" "<<str.size()<<"\n"<<to_string(a)<<" "<<to_string(a).size()<<" "<<(str == to_string(a))<<'\n';

    //
//    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    big_integer b(                                                     "100000000000000000000000000000000000000");
//    big_integer c("100000000000000000000000000000000000000000000000000000");
//
//    (a/b).stupid_print();
//    c.stupid_print();

//    big_integer a = 20;
//    big_integer b = 5;
//    big_integer c = 20;
//
//
//    (a / b).stupid_print();
//    cout<<"== 4\n";
//    (a % b).stupid_print();
//    cout<<"== 0\n";
//
//    (a /= b).stupid_print();
//    cout<<"a == 4\n";
//
//    (c %= b).stupid_print();
//    cout<<"c == 0\n";

//    big_integer a = 4;
//    big_integer b = 2;
//
//    a/=b;
//    a.stupid_print();

//    (a|(b-256)).stupid_print();
//    big_integer(-1).stupid_print();

//    big_integer a("-5");
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1);
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1);
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1);
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1);
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1);
//    a.stupid_print();
//    big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    big_integer b(                                                     "100000000000000000000000000000000000000");
//    big_integer c( "-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");
//
//    a.stupid_print();
//
//    a+=b;
//    a.stupid_print();
//    c.stupid_print();
//
//    cout << (unsigned long long) UINT32_MAX + (unsigned long long) 1;

//    big_integer a( "18446744073709551616");
//    big_integer b("-18446744073709551616");
//    big_integer c( "36893488147419103232");
//
//    (a+a).stupid_print();
//    cout<<" need to =  \n";
//    c.stupid_print();
//    (b+c).stupid_print();
//    cout<<" need to =  \n";
//    a.stupid_print();


//    big_integer a(1000000000);
//    for (int i = 0; i < 20; i++) {
//        a += a;
//        a.stupid_print();
//    }

//    big_integer test = 536870757; // == -155
//    test.stupid_print();
//    test.sign = -1;
//    test.make_dopolnenie_do_dvuh(1); // dopolnnie do dvuh - norm
//    test.stupid_print();
//    test.sign = -1;
//    test.make_dopolnenie_do_dvuh(1,0); // - from sign: -, data: data  to dopolneneie do dvuh
//    test.stupid_print();
//    test.sign = -1;
//    test.make_dopolnenie_do_dvuh(1); // dopolnnie do dvuh - norm
//    test.stupid_print();
//
//    big_integer a = -1234;
//    a >>= 3;
//    a.stupid_print();
//
//    unsigned long long x = 30985691264, y =18;
//    cout<<(unsigned int)x + (unsigned int)y;
//
//    //cout<<(int)pow(10,2);
//
//    big_integer a("1000000000000000000000000000000000000000000");
//    big_integer c("123123123123123123");
//    a.stupid_print();
//    a <<= 5;
//    a.stupid_print();
    ///cout<<a.insert_cell(8,12345);

//    c.stupid_print();
////    a.take_k_higher_symbols(21);
//    //big_integer b = big_integer(a.take_k_higher_symbols(16));
//    //b.stupid_print();
//    a.stupid_print();
//    //a.make_from_two(a,b);
//    //a = a.make_from_two(a,b);
//    a.stupid_print();
//    big_integer a = -10;
//    a.stupid_print();
//    a.make_dopolnenie_do_dvuh(1,0);
//    a.stupid_print();
//    a = -a;
//    a.make_dopolnenie_do_dvuh(1,0);
//    a=-a;
//    a.stupid_print();
//    big_integer b = 0xcc;
//    cout << 0xaa << " " << 0xcc << '\n';
//    (a ^ (b - 256)).stupid_print();
//    cout << " = " << (0x66 - 256) << '\n';
//    EXPECT_TRUE((a ^ (b - 256)) == (0x66 - 256));

//    big_integer a = 0x55;
//    big_integer b = 0xaa;
//
//    cout<<0x55<<" "<<0xaa<<'\n';
//    (a | (b - 256)).stupid_print();
//    cout<<" = "<<-1<<'\n';

//    EXPECT_TRUE((a | (b - 256)) == -1);

//    SIGNED AND TEST

//    big_integer a = 0x55;
//    big_integer b = 0xaa;
//
//    cout << 0x55 << " " << 0xaa << '\n';
//
//    big_integer minus(-1);
//    b &= minus;
//
//    (b & -1).stupid_print();
//    cout << " = " << 0xaa << '\n';
//    (a & (0xaa - 256)).stupid_print();
//    cout << " = " << 0 << '\n';
//    (a & (0xcc - 256)).stupid_print();
//    cout << " = " << 0x44 << '\n';
//    big_integer c = 0x55;
//    big_integer d = 0xcc;
//    (c & d).stupid_print();
//    cout << 0x44 << '\n';


//    EXPECT_EQ(big_integer(0x44), c & d);

//    cout<<(((unsigned long long)17179869180)>>((unsigned int)32))<<'\n';

//    cout << UINT32_MAX << '\n';
//    big_integer aa((unsigned int)3689348814);
//    big_integer desyat(10);
//    big_integer sem(7);
//    aa *= desyat;
//    aa += sem;
//    aa.stupid_print();
    //aa = aa * big_integer(10) + big_integer(7);

    //ERRROR::
    //1717986918 : 858993459 *10 + 2
    //Trying to reproduce:


//    vector<unsigned int> vec;
//    vec.push_back(1717986918);
//    vec.push_back(858993459);
//    big_integer tst(vec);
//    tst.stupid_print();
//    big_integer desyat(10);
//    tst*=desyat;
//    tst+=big_integer(2);
//    tst.stupid_print();

//  ETO TESTI ANDA

//    big_integer a = 0x55;
//    big_integer b = 0xaa;
//    a.stupid_print();
//    b.stupid_print();
//    cout << 0xcc << " " << 0x44 << '\n';
//
//    big_integer x = a & b;
//    if ((a & b) == 0) {
//        cout << "1=\n";
//    } else {
//        (a & b).stupid_print();
//        cout << " != " << 0 << '\n';
//    }
//    if ((a & 0xcc) == 0x44) {
//        cout << "2=\n";
//    } else {
//        cout << (a & 0xcc) << " != " << 0x44 << '\n';
//    }
//    x.stupid_print();
//    x = a & 0xcc;
//    x.stupid_print();
//    a &= b;
//    a.stupid_print();

//    EXPECT_TRUE((a & b) == 0);
//    EXPECT_TRUE((a & 0xcc) == 0x44);
//    a &= b;
//    EXPECT_TRUE(a == 0);

//    big_integer a( "18446744073709551616");
//    big_integer b("-18446744073709551616");
//    big_integer c( "36893488147419103232");
//
//
//    a.stupid_print();
//    b.stupid_print();
//    c.stupid_print();
//
//    big_integer x = a + a;
//    x.stupid_print();
//    x = b + c;
//    x.stupid_print();
//    x = c + b;
//    x.stupid_print();
//    big_integer x("100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    //x *= 123123;
//    x.stupid_print();
//    big_integer y("-100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    x.stupid_print();
//    y.stupid_print();
//
//    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
//    big_integer b(                                                     "100000000000000000000000000000000000000");
//    big_integer c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");
//
//    a+=b;
//
//    big_integer xx = a + b;
//    xx.stupid_print();
//    c.stupid_print();
//    if(xx != c){
//        cout<<"fuck\n";
//    }
//    xx -= c;
//    xx.stupid_print();
    return 0;
}