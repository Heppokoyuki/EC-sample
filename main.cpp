#include <iostream>
#include <elipticcurve.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>

using namespace std;

int main()
{
    elipticCurve *e = new elipticCurve(3, 4, 7);
    std::cout << e->addInv(2) << " " << e->mulInv(2) << std::endl;
    elipticCurve::point p_1 = {0, 2, false};
    elipticCurve::point p_4 = {5, 2, false};
    elipticCurve::point p_6 = {5, 5, false};
    elipticCurve::point result = e->addPoint(p_4, p_6);
    elipticCurve::point result2 = e->scalarMultiplication(3, p_1);
    result.print();
    result2.print();

    mp::cpp_int a("1461501637330902918203684832716283019653785059324");
    mp::cpp_int b("163235791306168110546604919403271579530548345413");
    mp::cpp_int p("1461501637330902918203684832716283019653785059327");
    mp::cpp_int P_x("598833001378563909320556562387727035658124457364");
    mp::cpp_int P_y("456273172676936625440583883939668862699127599796");
    mp::cpp_int l("1461501637330902918203687197606826779884643492439");
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<mp::cpp_int> dist(mp::cpp_int(1) << 160, mp::cpp_int(1) << 161);
    mp::cpp_int d = dist(gen);
    // "crypt" sha256 hash
    mp::cpp_int hash("98687292787671481785167033227923050838847915443362959793501195795748475571364");
    mp::cpp_int u, v;

    elipticCurve *ecdsa = new elipticCurve(a, b, p);
    elipticCurve::point pub = ecdsa->generatePubKey(d, P_x, P_y);
    ecdsa->initECDSA(d, P_x, P_y, l);
    ecdsa->sign(hash, u, v);
    std::cout << u << " " << v << std::endl;
    ecdsa->verify(hash, u, v, pub);
    return 0;
}
