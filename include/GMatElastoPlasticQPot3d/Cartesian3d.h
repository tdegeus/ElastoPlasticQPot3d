/*

(c - MIT) T.W.J. de Geus (Tom) | www.geus.me | github.com/tdegeus/GMatElastoPlasticQPot3d

*/

#ifndef GMATELASTOPLASTICQPOT3D_CARTESIAN3D_H
#define GMATELASTOPLASTICQPOT3D_CARTESIAN3D_H

#include "config.h"

namespace GMatElastoPlasticQPot3d {
namespace Cartesian3d {

// Alias

using Tensor2 = xt::xtensor_fixed<double, xt::xshape<3, 3>>;
using Tensor4 = xt::xtensor_fixed<double, xt::xshape<3, 3, 3, 3>>;

// Unit tensors

inline Tensor2 I2();
inline Tensor4 II();
inline Tensor4 I4();
inline Tensor4 I4rt();
inline Tensor4 I4s();
inline Tensor4 I4d();

// Hydrostatic part of a tensor

inline double Hydrostatic(const Tensor2& A);

// Deviatoric part of a tensor

inline Tensor2 Deviatoric(const Tensor2& A);

// Equivalent deviatoric stress/stress

inline double Sigd(const Tensor2& Sig);
inline double Epsd(const Tensor2& Eps);

// List version of the functions above (no allocation)

inline void hydrostatic(const xt::xtensor<double,3>& A, xt::xtensor<double,1>& Am);
inline void deviatoric(const xt::xtensor<double,3>& A, xt::xtensor<double,3>& Ad);
inline void sigd(const xt::xtensor<double,3>& Sig, xt::xtensor<double,1>& Sigeq);
inline void epsd(const xt::xtensor<double,3>& Eps, xt::xtensor<double,1>& Epseq);

// Auto-allocation allocation of the functions above

inline xt::xtensor<double,1> Hydrostatic(const xt::xtensor<double,3>& A);
inline xt::xtensor<double,3> Deviatoric(const xt::xtensor<double,3>& A);
inline xt::xtensor<double,1> Sigd(const xt::xtensor<double,3>& Sig);
inline xt::xtensor<double,1> Epsd(const xt::xtensor<double,3>& Eps);

// Matrix version of the functions above (no allocation)

inline void hydrostatic(const xt::xtensor<double,4>& A, xt::xtensor<double,2>& Am);
inline void deviatoric(const xt::xtensor<double,4>& A, xt::xtensor<double,4>& Ad);
inline void sigd(const xt::xtensor<double,4>& Sig, xt::xtensor<double,2>& Sigeq);
inline void epsd(const xt::xtensor<double,4>& Eps, xt::xtensor<double,2>& Epseq);

// Auto-allocation allocation of the functions above

inline xt::xtensor<double,2> Hydrostatic(const xt::xtensor<double,4>& A);
inline xt::xtensor<double,4> Deviatoric(const xt::xtensor<double,4>& A);
inline xt::xtensor<double,2> Sigd(const xt::xtensor<double,4>& Sig);
inline xt::xtensor<double,2> Epsd(const xt::xtensor<double,4>& Eps);

// Material point

class Elastic
{
public:

    // Constructors
    Elastic() = default;
    Elastic(double K, double G);

    // Get parameters
    double K() const;
    double G() const;

    // Update strain
    template <class U>
    void set_strain(const U&& Eps);

    // Get response based on the current strain (see "set_strain")
    template <class U> void stress(U&& Sig) const;
    template <class U> void tangent(V&& C) const;
    Tensor2 Stress() const; // stress tensor (fourth order)
    Tensor4 Tangent() const; // tangent tensor (fourth order)
    double energy() const; // potential energy

private:

    double m_K; // bulk modulus
    double m_G; // shear modulus
    Tensor2 m_Eps; // current strain
};


// Material point

class Cusp
{
public:

    // Constructors
    Cusp() = default;
    Cusp(double K, double G, const xt::xtensor<double,1>& epsy, bool init_elastic = true);

    // Get parameters
    double K() const;
    double G() const;
    xt::xtensor<double,1> epsy() const;
    double epsy(size_t idx) const;

    // Update strain
    template <class U>
    void set_strain(const U&& Eps);

    // Get response based on the current strain (see "set_strain")
    template <class U> void stress(U&& Sig) const;
    template <class U> void tangent(V&& C) const;
    Tensor2 Stress() const; // stress tensor (fourth order)
    Tensor4 Tangent() const; // tangent tensor (fourth order)
    double energy() const; // potential energy
    size_t index() const; // index of the current yield strain
    double epsy_pos() const; // yield strain for an increase of equivalent strain: epsy[index + 1]
    double epsy_neg() const; // yield strain for a decrease of equivalent strain: epsy[index]
    double epsp() const; // 'plastic' strain

private:

    double m_K; // bulk modulus
    double m_G; // shear modulus
    xt::xtensor<double,1> m_epsy; // yield strains
    Tensor2 m_Eps; // current strain
};

// Material point

class Smooth
{
public:

    // Constructors
    Smooth() = default;
    Smooth(double K, double G, const xt::xtensor<double,1>& epsy, bool init_elastic = true);

    // Get parameters
    double K() const;
    double G() const;
    xt::xtensor<double,1> epsy() const;
    double epsy(size_t idx) const;

    // Update strain
    template <class U>
    void set_strain(const U&& Eps);

    // Get response based on the current strain (see "set_strain")
    template <class U> void stress(U&& Sig) const;
    template <class U> void tangent(V&& C) const;
    Tensor2 Stress() const; // stress tensor (fourth order)
    Tensor4 Tangent() const; // tangent tensor (fourth order)
    double energy() const; // potential energy
    size_t index() const; // index of the current yield strain
    double epsy_pos() const; // yield strain for an increase of equivalent strain: epsy[index + 1]
    double epsy_neg() const; // yield strain for a decrease of equivalent strain: epsy[index]
    double epsp() const; // 'plastic' strain

private:

    double m_K; // bulk modulus
    double m_G; // shear modulus
    xt::xtensor<double,1> m_epsy; // yield strains
    Tensor2 m_Eps; // current strain
};

// Material identifier

struct Type {
    enum Value {
        Unset,
        Elastic,
        Cusp,
        Smooth,
    };
};

// Matrix of material points

class Matrix
{
public:

    // Constructors

    Matrix() = default;
    Matrix(size_t nelem, size_t nip);

    // Shape

    size_t ndim() const;
    size_t nelem() const;
    size_t nip() const;

    // Type

    xt::xtensor<size_t,2> type() const;
    xt::xtensor<size_t,2> isElastic() const;
    xt::xtensor<size_t,2> isPlastic() const;
    xt::xtensor<size_t,2> isCusp() const;
    xt::xtensor<size_t,2> isSmooth() const;

    // Parameters

    xt::xtensor<double,2> K() const;
    xt::xtensor<double,2> G() const;

    // Matrix of unit tensors

    xt::xtensor<double,4> I2() const;
    xt::xtensor<double,6> II() const;
    xt::xtensor<double,6> I4() const;
    xt::xtensor<double,6> I4rt() const;
    xt::xtensor<double,6> I4s() const;
    xt::xtensor<double,6> I4d() const;

    // Check that a type has been set everywhere (throws if unset points are found)

    void check() const;

    // Set parameters for a batch of points
    // (uniform for all points specified: that have "I(i,j) == 1")

    void setElastic(
        const xt::xtensor<size_t,2>& I,
        double K,
        double G);

    void setCusp(
        const xt::xtensor<size_t,2>& I,
        double K,
        double G,
        const xt::xtensor<double,1>& epsy,
        bool init_elastic = true);

    void setSmooth(
        const xt::xtensor<size_t,2>& I,
        double K,
        double G,
        const xt::xtensor<double,1>& epsy,
        bool init_elastic = true);

    // Set parameters for a batch of points:
    // each to the same material, but with different parameters:
    // the matrix "idx" refers to a which entry to use: "K[idx]", "G[idx]", or "epsy[idx,:]"

    void setElastic(
        const xt::xtensor<size_t,2>& I,
        const xt::xtensor<size_t,2>& idx,
        const xt::xtensor<double,1>& K,
        const xt::xtensor<double,1>& G);

    void setCusp(
        const xt::xtensor<size_t,2>& I,
        const xt::xtensor<size_t,2>& idx,
        const xt::xtensor<double,1>& K,
        const xt::xtensor<double,1>& G,
        const xt::xtensor<double,2>& epsy,
        bool init_elastic = true);

    void setSmooth(
        const xt::xtensor<size_t,2>& I,
        const xt::xtensor<size_t,2>& idx,
        const xt::xtensor<double,1>& K,
        const xt::xtensor<double,1>& G,
        const xt::xtensor<double,2>& epsy,
        bool init_elastic = true);

    // Compute response (see above)

    void set_strain(const xt::xtensor<double,4>& Eps);

    void stress(xt::xtensor<double,4>& Sig) const;
    void tangent(xt::xtensor<double,6>& C) const;
    void energy(xt::xtensor<double,2>& energy) const;
    void index(xt::xtensor<size_t,2>& idx) const;
    void epsy_pos(xt::xtensor<double,2>& epsy) const;
    void epsy_neg(xt::xtensor<double,2>& epsy) const;
    void epsp(xt::xtensor<double,2>& epsp) const;

    xt::xtensor<double,4> Stress() const;
    xt::xtensor<double,6> Tangent() const;
    xt::xtensor<double,2> Energy() const;
    xt::xtensor<size_t,2> Index() const;
    xt::xtensor<double,2> Epsy_pos() const;
    xt::xtensor<double,2> Epsy_neg() const;
    xt::xtensor<double,2> Epsp() const;

private:

    // Material vectors
    std::vector<Elastic> m_Elastic;
    std::vector<Cusp> m_Cusp;
    std::vector<Smooth> m_Smooth;

    // Identifiers for each matrix entry
    xt::xtensor<size_t,2> m_type; // type (e.g. "Type::Elastic")
    xt::xtensor<size_t,2> m_index; // index from the relevant material vector (e.g. "m_Elastic")

    // Shape
    size_t m_nelem;
    size_t m_nip;
    static const size_t m_ndim = 3;

    // Internal check
    bool m_allSet = false; // true if all points have a material assigned
    void checkAllSet(); // check if all points have a material assigned (modifies "m_allSet")
};


// Internal support functions

// Trace: "c = A_ii"
template <class U>
inline double trace(const U& A);

// Tensor contraction: "c = A_ij * B_ji"
// Symmetric tensors only, no assertion
template <class U, class V>
inline double A2_ddot_B2(const U& A, const V& B);

} // namespace Cartesian3d
} // namespace GMatElastoPlasticQPot3d

#include "Cartesian3d.hpp"
#include "Cartesian3d_Cusp.hpp"
#include "Cartesian3d_Elastic.hpp"
#include "Cartesian3d_Matrix.hpp"
#include "Cartesian3d_Smooth.hpp"

#endif
