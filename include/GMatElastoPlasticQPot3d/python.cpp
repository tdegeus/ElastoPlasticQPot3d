/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | www.geus.me | github.com/tdegeus/GMatElastoPlasticQPot3d

================================================================================================= */

#include <pybind11/pybind11.h>
#include <pyxtensor/pyxtensor.hpp>

#include "Cartesian3d.h"

// =================================================================================================

// abbreviate name-space
namespace py = pybind11;

// ==================================== GMatElastoPlasticQPot3d ====================================

PYBIND11_MODULE(GMatElastoPlasticQPot3d, m) {

m.doc() = "Elasto-plastic material models";

// ============================= GMatElastoPlasticQPot3d::Cartesian3d ==============================

{

// create submodule
py::module sm = m.def_submodule("Cartesian3d", "3d Cartesian coordinates");

// abbreviate name-space
namespace SM = GMatElastoPlasticQPot3d::Cartesian3d;

// abbreviate types(s)
typedef SM::T2s T2s;

// -------------------------------------------------------------------------------------------------

sm.def("epsm", py::overload_cast<const T2s &>(&SM::epsm), "Hydrostatic strain" , py::arg("Eps"));
sm.def("epsd", py::overload_cast<const T2s &>(&SM::epsd), "Eq. strain deviator", py::arg("Eps"));
sm.def("Epsd", py::overload_cast<const T2s &>(&SM::Epsd), "Strain deviator"    , py::arg("Eps"));

sm.def("sigm", py::overload_cast<const T2s &>(&SM::sigm), "Hydrostatic stress" , py::arg("Sig"));
sm.def("sigd", py::overload_cast<const T2s &>(&SM::sigd), "Eq. stress deviator", py::arg("Sig"));
sm.def("Sigd", py::overload_cast<const T2s &>(&SM::Sigd), "Stress deviator"    , py::arg("Sig"));

sm.def("epsm", py::overload_cast<const xt::xtensor<double,4> &>(&SM::epsm), "Hydrostatic strain" , py::arg("a_Eps"));
sm.def("epsd", py::overload_cast<const xt::xtensor<double,4> &>(&SM::epsd), "Eq. strain deviator", py::arg("a_Eps"));
sm.def("Epsd", py::overload_cast<const xt::xtensor<double,4> &>(&SM::Epsd), "Strain deviator"    , py::arg("a_Eps"));

sm.def("sigm", py::overload_cast<const xt::xtensor<double,4> &>(&SM::sigm), "Hydrostatic stress" , py::arg("a_Sig"));
sm.def("sigd", py::overload_cast<const xt::xtensor<double,4> &>(&SM::sigd), "Eq. stress deviator", py::arg("a_Sig"));
sm.def("Sigd", py::overload_cast<const xt::xtensor<double,4> &>(&SM::Sigd), "Stress deviator"    , py::arg("a_Sig"));

// -------------------------------------------------------------------------------------------------

py::class_<SM::Elastic>(sm, "Elastic")
  // constructor
  .def(
    py::init<double,double>(),
    "Elastic material",
    py::arg("kappa"),
    py::arg("mu")
  )
  // methods
  .def("Sig"   , &SM::Elastic::Sig   , py::arg("Eps"))
  .def("energy", &SM::Elastic::energy, py::arg("Eps"))
  .def("epsy"  , &SM::Elastic::epsy  , py::arg("idx"))
  .def("epsp"  , py::overload_cast<const T2s &>(&SM::Elastic::epsp, py::const_), py::arg("Eps" ))
  .def("epsp"  , py::overload_cast<double     >(&SM::Elastic::epsp, py::const_), py::arg("epsd"))
  .def("find"  , py::overload_cast<const T2s &>(&SM::Elastic::find, py::const_), py::arg("Eps" ))
  .def("find"  , py::overload_cast<double     >(&SM::Elastic::find, py::const_), py::arg("epsd"))
  // print to screen
  .def("__repr__", [](const SM::Elastic &){
    return "<GMatElastoPlasticQPot3d.Cartesian3d.Elastic>"; });

// -------------------------------------------------------------------------------------------------

py::class_<SM::Cusp>(sm, "Cusp")
  // constructor
  .def(
    py::init<double,double,const xt::xtensor<double,1>&, bool>(),
    "Cusp material",
    py::arg("kappa"),
    py::arg("mu"),
    py::arg("epsy"),
    py::arg("init_elastic")=true
  )
  // methods
  .def("Sig"   , &SM::Cusp::Sig   , py::arg("Eps"))
  .def("energy", &SM::Cusp::energy, py::arg("Eps"))
  .def("epsy"  , &SM::Cusp::epsy  , py::arg("idx"))
  .def("epsp"  , py::overload_cast<const T2s &>(&SM::Cusp::epsp, py::const_), py::arg("Eps" ))
  .def("epsp"  , py::overload_cast<double     >(&SM::Cusp::epsp, py::const_), py::arg("epsd"))
  .def("find"  , py::overload_cast<const T2s &>(&SM::Cusp::find, py::const_), py::arg("Eps" ))
  .def("find"  , py::overload_cast<double     >(&SM::Cusp::find, py::const_), py::arg("epsd"))
  // print to screen
  .def("__repr__", [](const SM::Cusp &){
    return "<GMatElastoPlasticQPot3d.Cartesian3d.Cusp>"; });

// -------------------------------------------------------------------------------------------------

py::class_<SM::Smooth>(m, "Smooth")
  // constructor
  .def(
    py::init<double,double,const xt::xtensor<double,1>&, bool>(),
    "Smooth material",
    py::arg("kappa"),
    py::arg("mu"),
    py::arg("epsy"),
    py::arg("init_elastic")=true
  )
  // methods
  .def("Sig"   , &SM::Smooth::Sig   , py::arg("Eps"))
  .def("energy", &SM::Smooth::energy, py::arg("Eps"))
  .def("epsy"  , &SM::Smooth::epsy  , py::arg("idx"))
  .def("epsp"  , py::overload_cast<const T2s &>(&SM::Smooth::epsp, py::const_), py::arg("Eps" ))
  .def("epsp"  , py::overload_cast<double     >(&SM::Smooth::epsp, py::const_), py::arg("epsd"))
  .def("find"  , py::overload_cast<const T2s &>(&SM::Smooth::find, py::const_), py::arg("Eps" ))
  .def("find"  , py::overload_cast<double     >(&SM::Smooth::find, py::const_), py::arg("epsd"))
  // print to screen
  .def("__repr__", [](const SM::Smooth &){
    return "<GMatElastoPlasticQPot3d.Cartesian3d.Smooth>"; });

// -------------------------------------------------------------------------------------------------

py::module smm = sm.def_submodule("Type", "Type enumerator");

py::enum_<SM::Type::Value>(smm, "Type")
    .value("Unset"       , SM::Type::Unset)
    .value("Elastic"     , SM::Type::Elastic)
    .value("Cusp"        , SM::Type::Cusp)
    .value("Smooth"      , SM::Type::Smooth)
    .value("PlanarCusp"  , SM::Type::PlanarCusp)
    .value("PlanarSmooth", SM::Type::PlanarSmooth)
    .export_values();

// -------------------------------------------------------------------------------------------------

py::class_<SM::Matrix>(sm, "Matrix")
  // constructor
  .def(
    py::init<size_t, size_t>(),
    "Matrix of materials",
    py::arg("nelem"),
    py::arg("nip")
  )
  // methods
  .def("type"      , &SM::Matrix::type )
  .def("nelem"     , &SM::Matrix::nelem)
  .def("nip"       , &SM::Matrix::nip  )
  .def("setElastic", py::overload_cast<const xt::xtensor<size_t,2> &,                                double,                        double                                                            >(&SM::Matrix::setElastic),py::arg("I"),               py::arg("K"),py::arg("G"))
  .def("setCusp"   , py::overload_cast<const xt::xtensor<size_t,2> &,                                double,                        double,                        const xt::xtensor<double,1> &, bool>(&SM::Matrix::setCusp   ),py::arg("I"),               py::arg("K"),py::arg("G"),py::arg("epsy"),py::arg("init_elastic")=true)
  .def("setSmooth" , py::overload_cast<const xt::xtensor<size_t,2> &,                                double,                        double,                        const xt::xtensor<double,1> &, bool>(&SM::Matrix::setSmooth ),py::arg("I"),               py::arg("K"),py::arg("G"),py::arg("epsy"),py::arg("init_elastic")=true)
  .def("setElastic", py::overload_cast<const xt::xtensor<size_t,2> &, const xt::xtensor<size_t,2> &, const xt::xtensor<double,1> &, const xt::xtensor<double,1> &                                     >(&SM::Matrix::setElastic),py::arg("I"),py::arg("idx"),py::arg("K"),py::arg("G"))
  .def("setCusp"   , py::overload_cast<const xt::xtensor<size_t,2> &, const xt::xtensor<size_t,2> &, const xt::xtensor<double,1> &, const xt::xtensor<double,1> &, const xt::xtensor<double,2> &, bool>(&SM::Matrix::setCusp   ),py::arg("I"),py::arg("idx"),py::arg("K"),py::arg("G"),py::arg("epsy"),py::arg("init_elastic")=true)
  .def("setSmooth" , py::overload_cast<const xt::xtensor<size_t,2> &, const xt::xtensor<size_t,2> &, const xt::xtensor<double,1> &, const xt::xtensor<double,1> &, const xt::xtensor<double,2> &, bool>(&SM::Matrix::setSmooth ),py::arg("I"),py::arg("idx"),py::arg("K"),py::arg("G"),py::arg("epsy"),py::arg("init_elastic")=true)
  .def("Sig"       , py::overload_cast<const xt::xtensor<double,4> &>(&SM::Matrix::Sig   , py::const_), py::arg("a_Eps"))
  .def("energy"    , py::overload_cast<const xt::xtensor<double,4> &>(&SM::Matrix::energy, py::const_), py::arg("a_Eps"))
  .def("find"      , py::overload_cast<const xt::xtensor<double,4> &>(&SM::Matrix::find  , py::const_), py::arg("a_Eps"))
  .def("epsy"      , py::overload_cast<const xt::xtensor<size_t,2> &>(&SM::Matrix::epsy  , py::const_), py::arg("a_idx"))
  .def("epsp"      , py::overload_cast<const xt::xtensor<double,4> &>(&SM::Matrix::epsp  , py::const_), py::arg("a_Eps"))
  // print to screen
  .def("__repr__", [](const SM::Matrix &){
    return "<GMatElastoPlasticQPot3d.Cartesian3d.Matrix>"; });

// -------------------------------------------------------------------------------------------------

}

// =================================================================================================

}

