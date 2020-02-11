
#include <catch2/catch.hpp>

#define EQ(a,b) REQUIRE_THAT((a), Catch::WithinAbs((b), 1.e-12));

#include <GMatElastoPlasticQPot3d/Cartesian3d.h>

namespace GM = GMatElastoPlasticQPot3d::Cartesian3d;

TEST_CASE("GMatElastoPlasticQPot3d::Cartesian3d", "Cartesian3d.h")
{

double K = 12.3;
double G = 45.6;

GM::Tensor2 Eps;
GM::Tensor2 Sig;

double gamma = 0.02;
double epsm = 0.12;

Eps.fill(0.0);
Eps(0,0) = Eps(1,1) = Eps(2,2) = epsm;
Eps(0,1) = Eps(1,0) = gamma;

SECTION("Elastic")
{
    GM::Elastic mat(K, G);
    Sig = mat.Stress(Eps);

    EQ(Sig(0,0), 3.0 * K * epsm);
    EQ(Sig(1,1), 3.0 * K * epsm);
    EQ(Sig(2,2), 3.0 * K * epsm);
    EQ(Sig(0,1), 2.0 * G * gamma);
    EQ(Sig(1,0), 2.0 * G * gamma);
}

SECTION("Cusp")
{
    GM::Cusp mat(K, G, {0.01, 0.03, 0.10});
    Sig = mat.Stress(Eps);

    EQ(Sig(0,0), 3.0 * K * epsm);
    EQ(Sig(1,1), 3.0 * K * epsm);
    EQ(Sig(2,2), 3.0 * K * epsm);
    EQ(Sig(0,1), 0.0);
    EQ(Sig(1,0), 0.0);
    EQ(mat.epsp(Eps), 0.02);
    REQUIRE(mat.find(Eps) == 1);
}

SECTION("Smooth")
{
    GM::Smooth mat(K, G, {0.01, 0.03, 0.10});
    Sig = mat.Stress(Eps);

    EQ(Sig(0,0), 3.0 * K * epsm);
    EQ(Sig(1,1), 3.0 * K * epsm);
    EQ(Sig(2,2), 3.0 * K * epsm);
    EQ(Sig(0,1), 0.0);
    EQ(Sig(1,0), 0.0);
    EQ(mat.epsp(Eps), 0.02);
    REQUIRE(mat.find(Eps) == 1);
}

SECTION("Matrix")
{
    size_t nelem = 3;
    size_t nip = 2;

    GM::Matrix mat(nelem, nip);

    {
        xt::xtensor<size_t,2> I = xt::zeros<size_t>({nelem, nip});
        xt::view(I, 0, xt::all()) = 1;
        mat.setElastic(I, K, G);
    }

    {
        xt::xtensor<size_t,2> I = xt::zeros<size_t>({nelem, nip});
        xt::xtensor<double,1> epsy = {0.01, 0.03, 0.10};
        xt::view(I, 1, xt::all()) = 1;
        mat.setCusp(I, K, G, epsy);
    }

    {
        xt::xtensor<size_t,2> I = xt::zeros<size_t>({nelem, nip});
        xt::xtensor<double,1> epsy = {0.01, 0.03, 0.10};
        xt::view(I, 2, xt::all()) = 1;
        mat.setCusp(I, K, G, epsy);
    }

    xt::xtensor<double,4> eps = xt::empty<double>({nelem, nip, 3ul, 3ul});

    for (size_t e = 0; e < nelem; ++e) {
        for (size_t q = 0; q < nip; ++q) {
            xt::view(eps, e, q) = Eps;
        }
    }

    auto sig = mat.Stress(eps);
    auto epsp = mat.Epsp(eps);

    for (size_t q = 0; q < nip; ++q) {

        EQ(sig(0,q,0,0), 3.0 * K * epsm);
        EQ(sig(0,q,1,1), 3.0 * K * epsm);
        EQ(sig(0,q,2,2), 3.0 * K * epsm);
        EQ(sig(0,q,0,1), 2.0 * G * gamma);
        EQ(sig(0,q,1,0), 2.0 * G * gamma);
        EQ(epsp(0,q), 0.0);

        EQ(sig(1,q,0,0), 3.0 * K * epsm);
        EQ(sig(1,q,1,1), 3.0 * K * epsm);
        EQ(sig(1,q,2,2), 3.0 * K * epsm);
        EQ(sig(1,q,0,1), 0.0);
        EQ(sig(1,q,1,0), 0.0);
        EQ(epsp(1,q), gamma);

        EQ(sig(2,q,0,0), 3.0 * K * epsm);
        EQ(sig(2,q,1,1), 3.0 * K * epsm);
        EQ(sig(2,q,2,2), 3.0 * K * epsm);
        EQ(sig(2,q,0,1), 0.0);
        EQ(sig(2,q,1,0), 0.0);
        EQ(epsp(2,q), gamma);
    }
}

}
