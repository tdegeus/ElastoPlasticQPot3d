/*

(c - MIT) T.W.J. de Geus (Tom) | www.geus.me | github.com/tdegeus/GMatElastoPlasticQPot3d

*/

#ifndef GMATELASTOPLASTICQPOT3D_CARTESIAN3D_H
#define GMATELASTOPLASTICQPOT3D_CARTESIAN3D_H

// use "M_PI" from "math.h"
#define _USE_MATH_DEFINES

#include <QPot/Static.hpp>
#include <GMatTensor/Cartesian3d.h>
#include <GMatElastic/Cartesian3d.h>
#include <math.h>
#include <xtensor/xsort.hpp>

#include "config.h"

namespace GMatElastoPlasticQPot3d {
namespace Cartesian3d {

// Unit tensors

using GMatTensor::Cartesian3d::O2;
using GMatTensor::Cartesian3d::O4;
using GMatTensor::Cartesian3d::I2;
using GMatTensor::Cartesian3d::II;
using GMatTensor::Cartesian3d::I4;
using GMatTensor::Cartesian3d::I4rt;
using GMatTensor::Cartesian3d::I4s;
using GMatTensor::Cartesian3d::I4d;

// Tensor decomposition

using GMatTensor::Cartesian3d::hydrostatic;
using GMatTensor::Cartesian3d::Hydrostatic;
using GMatTensor::Cartesian3d::deviatoric;
using GMatTensor::Cartesian3d::Deviatoric;

// Equivalent strain

template <class T, class U>
inline void epsd(const T& A, U& B);

template <class T>
inline auto Epsd(const T& A);

// Equivalent stress

template <class T, class U>
inline void sigd(const T& A, U& B);

template <class T>
inline auto Sigd(const T& A);

// Material point

class Elastic : public GMatElastic::Cartesian3d::Elastic
{
public:
    Elastic() = default;
    Elastic(double K, double G);
};

// Material point

class Cusp
{
public:
    Cusp() = default;
    Cusp(double K, double G, const xt::xtensor<double, 1>& epsy, bool init_elastic = true);

    double K() const; // bulk modulus
    double G() const; // shear modulus
    xt::xtensor<double, 1> epsy() const; // yield strains

    auto getQPot() const; // underlying QPot model
    auto* refQPot();      // reference to underlying QPot model

    size_t currentIndex() const;      // yield index
    double currentYieldLeft() const;  // yield strain left epsy[index]
    double currentYieldRight() const; // yield strain right epsy[index + 1]
    double epsp() const;   // "plastic strain" = 0.5 * (currentYieldLeft + currentYieldRight)
    double energy() const; // potential energy

    // Check that 'the particle' is at least "n" wells from the far-left/right
    bool checkYieldBoundLeft(size_t n = 0) const;
    bool checkYieldBoundRight(size_t n = 0) const;

    template <class T> void setStrain(const T& arg);
    template <class T> void strain(T& ret) const;
    template <class T> void stress(T& ret) const;
    template <class T> void tangent(T& ret) const;

    template <class T> void setStrainPtr(const T* arg);
    template <class T> void strainPtr(T* ret) const;
    template <class T> void stressPtr(T* ret) const;
    template <class T> void tangentPtr(T* ret) const;

    xt::xtensor<double, 2> Strain() const;
    xt::xtensor<double, 2> Stress() const;
    xt::xtensor<double, 4> Tangent() const;

private:
    double m_K;                  // bulk modulus
    double m_G;                  // shear modulus
    QPot::Static m_yield;        // potential energy landscape
    std::array<double, 9> m_Eps; // strain tensor [xx, xy, xz, yx, yy, yz, zx, zy, zz]
    std::array<double, 9> m_Sig; // stress tensor ,,
};

// Material point

class Smooth
{
public:
    Smooth() = default;
    Smooth(double K, double G, const xt::xtensor<double, 1>& epsy, bool init_elastic = true);

    double K() const; // bulk modulus
    double G() const; // shear modulus
    xt::xtensor<double, 1> epsy() const; // yield strains

    auto getQPot() const; // underlying QPot model
    auto* refQPot();      // reference to underlying QPot model

    size_t currentIndex() const;      // yield index
    double currentYieldLeft() const;  // yield strain left epsy[index]
    double currentYieldRight() const; // yield strain right epsy[index + 1]
    double epsp() const;   // "plastic strain" = 0.5 * (currentYieldLeft + currentYieldRight)
    double energy() const; // potential energy

    // Check that 'the particle' is at least "n" wells from the far-left/right
    bool checkYieldBoundLeft(size_t n = 0) const;
    bool checkYieldBoundRight(size_t n = 0) const;

    template <class T> void setStrain(const T& arg);
    template <class T> void strain(T& ret) const;
    template <class T> void stress(T& ret) const;
    template <class T> void tangent(T& ret) const;

    template <class T> void setStrainPtr(const T* arg);
    template <class T> void strainPtr(T* ret) const;
    template <class T> void stressPtr(T* ret) const;
    template <class T> void tangentPtr(T* ret) const;

    xt::xtensor<double, 2> Strain() const;
    xt::xtensor<double, 2> Stress() const;
    xt::xtensor<double, 4> Tangent() const;

private:
    double m_K;                  // bulk modulus
    double m_G;                  // shear modulus
    QPot::Static m_yield;        // potential energy landscape
    std::array<double, 9> m_Eps; // strain tensor [xx, xy, xz, yx, yy, yz, zx, zy, zz]
    std::array<double, 9> m_Sig; // stress tensor ,,
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

// Array of material points

template <size_t N>
class Array : public GMatTensor::Cartesian3d::Array<N>
{
public:
    using GMatTensor::Cartesian3d::Array<N>::rank;

    // Constructors

    Array() = default;
    Array(const std::array<size_t, N>& shape);

    // Overloaded methods:
    // - "shape"
    // - unit tensors: "I2", "II", "I4", "I4rt", "I4s", "I4d"

    // Type

    xt::xtensor<size_t, N> type() const;
    xt::xtensor<size_t, N> isElastic() const;
    xt::xtensor<size_t, N> isPlastic() const;
    xt::xtensor<size_t, N> isCusp() const;
    xt::xtensor<size_t, N> isSmooth() const;

    // Parameters

    xt::xtensor<double, N> K() const;
    xt::xtensor<double, N> G() const;

    // Set purely elastic

    void setElastic(
        const xt::xtensor<double, N>& K,
        const xt::xtensor<double, N>& G);

    // Set parameters for a batch of points
    // (uniform for all points specified: that have "I(i, j) == 1")

    void setElastic(
        const xt::xtensor<size_t, N>& I,
        double K,
        double G);

    void setCusp(
        const xt::xtensor<size_t, N>& I,
        double K,
        double G,
        const xt::xtensor<double, 1>& epsy,
        bool init_elastic = true);

    void setSmooth(
        const xt::xtensor<size_t, N>& I,
        double K,
        double G,
        const xt::xtensor<double, 1>& epsy,
        bool init_elastic = true);

    // Set parameters for a batch of points:
    // each to the same material, but with different parameters:
    // the matrix "idx" refers to a which entry to use: "K(idx)", "G(idx)", or "epsy(idx,:)"

    void setElastic(
        const xt::xtensor<size_t, N>& I,
        const xt::xtensor<size_t, N>& idx,
        const xt::xtensor<double, 1>& K,
        const xt::xtensor<double, 1>& G);

    void setCusp(
        const xt::xtensor<size_t, N>& I,
        const xt::xtensor<size_t, N>& idx,
        const xt::xtensor<double, 1>& K,
        const xt::xtensor<double, 1>& G,
        const xt::xtensor<double, 2>& epsy,
        bool init_elastic = true);

    void setSmooth(
        const xt::xtensor<size_t, N>& I,
        const xt::xtensor<size_t, N>& idx,
        const xt::xtensor<double, 1>& K,
        const xt::xtensor<double, 1>& G,
        const xt::xtensor<double, 2>& epsy,
        bool init_elastic = true);

    // Set strain tensor, get the response

    void setStrain(const xt::xtensor<double, N + 2>& arg);
    void strain(xt::xtensor<double, N + 2>& ret) const;
    void stress(xt::xtensor<double, N + 2>& ret) const;
    void tangent(xt::xtensor<double, N + 4>& ret) const;
    void currentIndex(xt::xtensor<size_t, N>& ret) const;
    void currentYieldLeft(xt::xtensor<double, N>& ret) const;
    void currentYieldRight(xt::xtensor<double, N>& ret) const;
    bool checkYieldBoundLeft(size_t n = 0) const;
    bool checkYieldBoundRight(size_t n = 0) const;
    void epsp(xt::xtensor<double, N>& ret) const;
    void energy(xt::xtensor<double, N>& ret) const;

    // Auto-allocation of the functions above

    xt::xtensor<double, N + 2> Strain() const;
    xt::xtensor<double, N + 2> Stress() const;
    xt::xtensor<double, N + 4> Tangent() const;
    xt::xtensor<size_t, N> CurrentIndex() const;
    xt::xtensor<double, N> CurrentYieldLeft() const;
    xt::xtensor<double, N> CurrentYieldRight() const;
    xt::xtensor<double, N> Epsp() const;
    xt::xtensor<double, N> Energy() const;

    // Get copy or reference to the underlying model at on point

    auto getElastic(const std::array<size_t, N>& index) const;
    auto getCusp(const std::array<size_t, N>& index) const;
    auto getSmooth(const std::array<size_t, N>& index) const;
    auto* refElastic(const std::array<size_t, N>& index);
    auto* refCusp(const std::array<size_t, N>& index);
    auto* refSmooth(const std::array<size_t, N>& index);

private:
    // Material vectors
    std::vector<Elastic> m_Elastic;
    std::vector<Cusp> m_Cusp;
    std::vector<Smooth> m_Smooth;

    // Identifiers for each matrix entry
    xt::xtensor<size_t, N> m_type;  // type (e.g. "Type::Elastic")
    xt::xtensor<size_t, N> m_index; // index from the relevant material vector (e.g. "m_Elastic")

    // Shape
    using GMatTensor::Cartesian3d::Array<N>::m_ndim;
    using GMatTensor::Cartesian3d::Array<N>::m_stride_tensor2;
    using GMatTensor::Cartesian3d::Array<N>::m_stride_tensor4;
    using GMatTensor::Cartesian3d::Array<N>::m_size;
    using GMatTensor::Cartesian3d::Array<N>::m_shape;
    using GMatTensor::Cartesian3d::Array<N>::m_shape_tensor2;
    using GMatTensor::Cartesian3d::Array<N>::m_shape_tensor4;
};

} // namespace Cartesian3d
} // namespace GMatElastoPlasticQPot3d

#include "Cartesian3d.hpp"
#include "Cartesian3d_Array.hpp"
#include "Cartesian3d_Cusp.hpp"
#include "Cartesian3d_Elastic.hpp"
#include "Cartesian3d_Smooth.hpp"

#endif
