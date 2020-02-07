/*

(c - MIT) T.W.J. de Geus (Tom) | www.geus.me | github.com/tdegeus/GMatElastoPlasticQPot3d

*/

#ifndef GMATELASTOPLASTICQPOT3D_CARTESIAN3D_HPP
#define GMATELASTOPLASTICQPOT3D_CARTESIAN3D_HPP

#include "Cartesian3d.h"

namespace GMatElastoPlasticQPot3d {
namespace Cartesian3d {

inline Tensor2 I2()
{
    return Tensor2({{1.0, 0.0, 0.0},
                    {0.0, 1.0, 0.0},
                    {0.0, 0.0, 1.0}});
}

inline double Hydrostatic(const Tensor2& A)
{
    return trace(A) / 3.0;
}

inline Tensor2 Deviatoric(const Tensor2& A)
{
    return A - trace(A) / 3.0 * I2();
}

inline double Epsd(const Tensor2& Eps)
{
    Tensor2 Epsd = Eps - trace(Eps) / 3.0 * I2();
    return std::sqrt(0.5 * A2_ddot_B2(Epsd, Epsd));
}

inline double Sigd(const Tensor2& Sig)
{
    Tensor2 Sigd = Sig - trace(Sig) / 3.0 * I2();
    return std::sqrt(2.0 * A2_ddot_B2(Sigd, Sigd));
}

inline void hydrostatic(const xt::xtensor<double,3>& A, xt::xtensor<double,1>& Am)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Am.shape(0), 3, 3}));

    #pragma omp parallel
    {
        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            auto Ai = xt::adapt(&A(e, 0, 0), xt::xshape<3, 3>());
            Am(e) = trace(Ai) / 3.0;
        }
    }
}

inline void deviatoric(const xt::xtensor<double,3>& A, xt::xtensor<double,3>& Ad)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Ad.shape(0), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            auto Ai = xt::adapt(&A(e, 0, 0), xt::xshape<3, 3>());
            auto Aid = xt::adapt(&Ad(e, 0, 0), xt::xshape<3, 3>());
            xt::noalias(Aid) = Ai - trace(Ai) / 3.0 * I;
        }
    }
}

inline void epsd(const xt::xtensor<double,3>& A, xt::xtensor<double,1>& Aeq)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Aeq.shape(0), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            auto Ai = xt::adapt(&A(e, 0, 0), xt::xshape<3, 3>());
            auto Aid = Ai - trace(Ai) / 3.0 * I;
            Aeq(e) = std::sqrt(0.5 * A2_ddot_B2(Aid, Aid));
        }
    }
}

inline void sigd(const xt::xtensor<double,3>& A, xt::xtensor<double,1>& Aeq)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Aeq.shape(0), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            auto Ai = xt::adapt(&A(e, 0, 0), xt::xshape<3, 3>());
            auto Aid = Ai - trace(Ai) / 3.0 * I;
            Aeq(e) = std::sqrt(2.0 * A2_ddot_B2(Aid, Aid));
        }
    }
}

inline xt::xtensor<double,1> Hydrostatic(const xt::xtensor<double,3>& A)
{
    xt::xtensor<double,1> Am = xt::empty<double>({A.shape(0)});
    hydrostatic(A, Am);
    return Am;
}

inline xt::xtensor<double,3> Deviatoric(const xt::xtensor<double,3>& A)
{
    xt::xtensor<double,3> Ad = xt::empty<double>(A.shape());
    deviatoric(A, Ad);
    return Ad;
}

inline xt::xtensor<double,1> Epsd(const xt::xtensor<double,3>& A)
{
    xt::xtensor<double,1> Aeq = xt::empty<double>({A.shape(0)});
    epsd(A, Aeq);
    return Aeq;
}

inline xt::xtensor<double,1> Sigd(const xt::xtensor<double,3>& A)
{
    xt::xtensor<double,1> Aeq = xt::empty<double>({A.shape(0)});
    sigd(A, Aeq);
    return Aeq;
}

inline void hydrostatic(const xt::xtensor<double,4>& A, xt::xtensor<double,2>& Am)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Am.shape(0), Am.shape(1), 3, 3}));

    #pragma omp parallel
    {
        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            for (size_t q = 0; q < A.shape(1); ++q) {
                auto Ai = xt::adapt(&A(e, q, 0, 0), xt::xshape<3, 3>());
                Am(e, q) = trace(Ai) / 3.0;
            }
        }
    }
}

inline void deviatoric(const xt::xtensor<double,4>& A, xt::xtensor<double,4>& Ad)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Ad.shape(0), Ad.shape(1), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            for (size_t q = 0; q < A.shape(1); ++q) {
                auto Ai = xt::adapt(&A(e, q, 0, 0), xt::xshape<3, 3>());
                auto Aid = xt::adapt(&Ad(e, q, 0, 0), xt::xshape<3, 3>());
                xt::noalias(Aid) = Ai - trace(Ai) / 3.0 * I;
            }
        }
    }
}

inline void epsd(const xt::xtensor<double,4>& A, xt::xtensor<double,2>& Aeq)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Aeq.shape(0), Aeq.shape(1), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            for (size_t q = 0; q < A.shape(1); ++q) {
                auto Ai = xt::adapt(&A(e, q, 0, 0), xt::xshape<3, 3>());
                auto Aid = Ai - trace(Ai) / 3.0 * I;
                Aeq(e, q) = std::sqrt(0.5 * A2_ddot_B2(Aid, Aid));
            }
        }
    }
}

inline void sigd(const xt::xtensor<double,4>& A, xt::xtensor<double,2>& Aeq)
{
    GMATELASTOPLASTICQPOT3D_ASSERT(
        A.shape() == std::decay_t<decltype(A)>::shape_type({Aeq.shape(0), Aeq.shape(1), 3, 3}));

    #pragma omp parallel
    {
        Tensor2 I = I2();

        #pragma omp for
        for (size_t e = 0; e < A.shape(0); ++e) {
            for (size_t q = 0; q < A.shape(1); ++q) {
                auto Ai = xt::adapt(&A(e, q, 0, 0), xt::xshape<3, 3>());
                auto Aid = Ai - trace(Ai) / 3.0 * I;
                Aeq(e, q) = std::sqrt(2.0 * A2_ddot_B2(Aid, Aid));
            }
        }
    }
}

inline xt::xtensor<double,2> Hydrostatic(const xt::xtensor<double,4>& A)
{
    xt::xtensor<double,2> Am = xt::empty<double>({A.shape(0), A.shape(1)});
    hydrostatic(A, Am);
    return Am;
}

inline xt::xtensor<double,4> Deviatoric(const xt::xtensor<double,4>& A)
{
    xt::xtensor<double,4> Ad = xt::empty<double>(A.shape());
    deviatoric(A, Ad);
    return Ad;
}

inline xt::xtensor<double,2> Epsd(const xt::xtensor<double,4>& A)
{
    xt::xtensor<double,2> Aeq = xt::empty<double>({A.shape(0), A.shape(1)});
    epsd(A, Aeq);
    return Aeq;
}

inline xt::xtensor<double,2> Sigd(const xt::xtensor<double,4>& A)
{
    xt::xtensor<double,2> Aeq = xt::empty<double>({A.shape(0), A.shape(1)});
    sigd(A, Aeq);
    return Aeq;
}

template <class U>
inline double trace(const U& A)
{
    return A(0,0) + A(1,1) + A(2,2);
}

template <class U, class V>
inline double A2_ddot_B2(const U& A, const V& B)
{
    return A(0,0) * B(0,0) + A(1,1) * B(1,1) + A(2,2) * B(2,2)
        + 2.0 * A(0,1) * B(0,1)
        + 2.0 * A(0,2) * B(0,2)
        + 2.0 * A(1,2) * B(1,2);
}

} // namespace Cartesian3d
} // namespace GMatElastoPlasticQPot3d

#endif
