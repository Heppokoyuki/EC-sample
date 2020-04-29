#include "elipticcurve.h"

elipticCurve::elipticCurve(mp::cpp_int a, mp::cpp_int b, mp::cpp_int p)
{
    this->a = a;
    this->b = b;
    this->p = p;
}

mp::cpp_int
elipticCurve::_add(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n)
{
    return (a + b) % n;
}

mp::cpp_int
elipticCurve::_sub(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n)
{
    return _add(a, _addInv(b, n), n);
}

mp::cpp_int
elipticCurve::_mul(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n)
{
    return (a * b) % n;
}

mp::cpp_int
elipticCurve::_div(mp::cpp_int a, mp::cpp_int b, mp::cpp_int n)
{
    return _mul(a, _mulInv(b, n), n);
}

mp::cpp_int
elipticCurve::extgcd(mp::cpp_int a, mp::cpp_int b, mp::cpp_int &x, mp::cpp_int &y)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    mp::cpp_int gcd = extgcd(b, a % b, x, y);
    mp::cpp_int oldX = x;
    x = y;
    y = oldX - a / b * y;
    return gcd;
}

mp::cpp_int
elipticCurve::_mulInv(mp::cpp_int a, mp::cpp_int n)
{
    mp::cpp_int x, y;
    extgcd(a, n, x, y);
    return x < 0 ? x + n : x;
}

mp::cpp_int
elipticCurve::_addInv(mp::cpp_int a, mp::cpp_int n)
{
    return n - a;
}

elipticCurve::point
elipticCurve::addPoint(elipticCurve::point P, elipticCurve::point Q)
{
    mp::cpp_int lambda;
    mp::cpp_int hoge;
    elipticCurve::point result;

    if(P.isUnit) return Q;
    if(Q.isUnit) return P;
    if(add(P.y, Q.y) == 0) return origin;
    if(P.x == Q.x) {
        hoge = add(mul(3, mul(P.x, P.x)), a);
        lambda = div(add(mul(3, mul(P.x, P.x)), a), mul(2, P.y));
    }
    else {
        lambda = div(sub(P.y, Q.y), sub(P.x, Q.x));
    }
    result.x = sub(sub(mul(lambda, lambda), P.x), Q.x);
    result.y = sub(mul(lambda, sub(P.x, result.x)), P.y);
    result.isUnit = false;
    return result;
}

//Right-to-Left binary method
elipticCurve::point
elipticCurve::scalarMultiplication(mp::cpp_int d, point P)
{
    point S = origin;

    while(d != 0) {
        if(d % 2 == 1) S = this->addPoint(S, P);
        P = this->addPoint(P, P);
        d >>= 1;
    }

    return S;
}

elipticCurve::point
elipticCurve::generatePubKey(mp::cpp_int secret, mp::cpp_int p_x, mp::cpp_int p_y)
{
    elipticCurve::point P = {p_x, p_y, false};
    return scalarMultiplication(secret, P);
}

void
elipticCurve::initECDSA(mp::cpp_int secret, mp::cpp_int p_x,
                        mp::cpp_int p_y, mp::cpp_int order)
{
    this->secret = secret;
    this->P.x = p_x;
    this->P.y = p_y;
    this->order = order;
}

void
elipticCurve::sign(mp::cpp_int hash, mp::cpp_int &u, mp::cpp_int &v)
{
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<mp::cpp_int> dist(0, mp::cpp_int(1) << 256);
    mp::cpp_int r = dist(gen);
    elipticCurve::point U = scalarMultiplication(r, P);

    u = U.x % order;
    v = _div(_add(hash, _mul(u, secret, order), order), r, order);
}

void
elipticCurve::verify(mp::cpp_int hash, mp::cpp_int u, mp::cpp_int v, elipticCurve::point pub)
{
    mp::cpp_int d = _div(1, v, order);
    elipticCurve::point V = addPoint(scalarMultiplication(d * hash, P), scalarMultiplication(d * u, pub));
    if(u == (V.x % order))
        std::cout << "verified!" << std::endl;
}
