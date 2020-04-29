#ifndef ELIPTICCURVE_H
#define ELIPTICCURVE_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>

namespace mp = boost::multiprecision;

class elipticCurve
{
public:
    struct point
    {
    public:
        mp::cpp_int x;
        mp::cpp_int y;
        bool isUnit;
        void print() { std::cout << "P: (" << x << "," << y << ")" << std::endl; }
    };

    elipticCurve(mp::cpp_int a, mp::cpp_int b, mp::cpp_int p);
    point addPoint(point P, point Q);
    point scalarMultiplication(mp::cpp_int d, point P);
    elipticCurve::point generatePubKey(mp::cpp_int secret, mp::cpp_int p_x, mp::cpp_int p_y);
    mp::cpp_int add(mp::cpp_int a, mp::cpp_int b) { return _add(a, b, p); }
    mp::cpp_int sub(mp::cpp_int a, mp::cpp_int b) { return _sub(a, b, p); }
    mp::cpp_int mul(mp::cpp_int a, mp::cpp_int b) { return _mul(a, b, p); }
    mp::cpp_int div(mp::cpp_int a, mp::cpp_int b) { return _div(a, b, p); }
    mp::cpp_int mulInv(mp::cpp_int a) { return _mulInv(a, p); }
    mp::cpp_int addInv(mp::cpp_int a) { return _addInv(a, p); }
    void initECDSA(mp::cpp_int secret, mp::cpp_int p_x, mp::cpp_int p_y, mp::cpp_int order);
    void sign(mp::cpp_int hash, mp::cpp_int &u, mp::cpp_int &v);
    void verify(mp::cpp_int hash, mp::cpp_int u, mp::cpp_int v, elipticCurve::point pub);

private:
    //EC defined as y^2 = x^3 + ax + b (mod p)
    mp::cpp_int a;
    mp::cpp_int b;
    mp::cpp_int p;
    const point origin = { mp::cpp_int(0), mp::cpp_int(1), true };
    mp::cpp_int _add(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n);
    mp::cpp_int _sub(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n);
    mp::cpp_int _mul(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n);
    mp::cpp_int _div(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n);
    mp::cpp_int extgcd(mp::cpp_int a, mp::cpp_int b, mp::cpp_int &x, mp::cpp_int &y);
    mp::cpp_int _mulInv(mp::cpp_int a, mp::cpp_int n);
    mp::cpp_int _addInv(mp::cpp_int a, mp::cpp_int n);

    mp::cpp_int secret;
    elipticCurve::point P;
    mp::cpp_int order;
};

#endif // ELIPTICCURVE_H
