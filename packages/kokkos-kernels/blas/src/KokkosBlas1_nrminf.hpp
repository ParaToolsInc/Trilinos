//@HEADER
// ************************************************************************
//
//                        Kokkos v. 4.0
//       Copyright (2022) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Part of Kokkos, under the Apache License v2.0 with LLVM Exceptions.
// See https://kokkos.org/LICENSE for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//@HEADER

#ifndef KOKKOSBLAS1_NRMINF_HPP_
#define KOKKOSBLAS1_NRMINF_HPP_

#include <KokkosBlas1_nrminf_spec.hpp>
#include <KokkosKernels_helpers.hpp>
#include <KokkosKernels_Error.hpp>

namespace KokkosBlas {

/// \brief Return the nrminf of the vector x.
///
/// \tparam XVector Type of the first vector x; a 1-D Kokkos::View.
///
/// \param x [in] Input 1-D View.
///
/// \return The nrminf product result; a single value.
template <class XVector>
typename Kokkos::Details::InnerProductSpaceTraits<
    typename XVector::non_const_value_type>::mag_type
nrminf(const XVector& x) {
  static_assert(Kokkos::is_view<XVector>::value,
                "KokkosBlas::nrminf: XVector must be a Kokkos::View.");
  static_assert(XVector::rank == 1,
                "KokkosBlas::nrminf: "
                "Both Vector inputs must have rank 1.");
  typedef typename Kokkos::Details::InnerProductSpaceTraits<
      typename XVector::non_const_value_type>::mag_type mag_type;

  typedef Kokkos::View<
      typename XVector::const_value_type*,
      typename KokkosKernels::Impl::GetUnifiedLayout<XVector>::array_layout,
      typename XVector::device_type, Kokkos::MemoryTraits<Kokkos::Unmanaged> >
      XVector_Internal;

  typedef Kokkos::View<mag_type, typename XVector_Internal::array_layout,
                       Kokkos::HostSpace,
                       Kokkos::MemoryTraits<Kokkos::Unmanaged> >
      RVector_Internal;

  mag_type result;
  RVector_Internal R = RVector_Internal(&result);
  XVector_Internal X = x;

  Impl::NrmInf<RVector_Internal, XVector_Internal>::nrminf(R, X);
  Kokkos::fence();
  return result;
}

/// \brief R(j) = nrminf(X(i,j))
///
/// Replace each entry in R with the nrminfolute value (magnitude) of the
/// corresponding entry in X.
///
/// \tparam RMV 1-D or 2-D Kokkos::View specialization.
/// \tparam XMV 1-D or 2-D Kokkos::View specialization.  It must have
///   the same rank as RMV, and its entries must be assignable to
///   those of RMV.
template <class RV, class XMV>
void nrminf(
    const RV& R, const XMV& X,
    typename std::enable_if<Kokkos::is_view<RV>::value, int>::type = 0) {
  static_assert(Kokkos::is_view<RV>::value,
                "KokkosBlas::nrminf: "
                "R is not a Kokkos::View.");
  static_assert(Kokkos::is_view<XMV>::value,
                "KokkosBlas::nrminf: "
                "X is not a Kokkos::View.");
  static_assert(std::is_same<typename RV::value_type,
                             typename RV::non_const_value_type>::value,
                "KokkosBlas::nrminf: R is const.  "
                "It must be nonconst, because it is an output argument "
                "(we have to be able to write to its entries).");
  static_assert(((RV::rank == 0) && (XMV::rank == 1)) ||
                    ((RV::rank == 1) && (XMV::rank == 2)),
                "KokkosBlas::nrminf: "
                "RV and XMV must either have rank 0 and 1 or rank 1 and 2.");

  typedef typename Kokkos::Details::InnerProductSpaceTraits<
      typename XMV::non_const_value_type>::mag_type mag_type;
  static_assert(std::is_same<typename RV::value_type, mag_type>::value,
                "KokkosBlas::nrminf: R must have the magnitude type of"
                "the xvectors value_type it is an output argument "
                "(we have to be able to write to its entries).");

  // Check compatibility of dimensions at run time.
  if (X.extent(1) != R.extent(0)) {
    std::ostringstream os;
    os << "KokkosBlas::nrminf (MV): Dimensions of R and X do not match: "
       << "R: " << R.extent(0) << ", X: " << X.extent(0) << " x "
       << X.extent(1);
    KokkosKernels::Impl::throw_runtime_exception(os.str());
  }

  using UnifiedXLayout =
      typename KokkosKernels::Impl::GetUnifiedLayout<XMV>::array_layout;
  using UnifiedRVLayout =
      typename KokkosKernels::Impl::GetUnifiedLayoutPreferring<
          RV, UnifiedXLayout>::array_layout;

  // Create unmanaged versions of the input Views.  RV and XMV may be
  // rank 1 or rank 2.
  typedef Kokkos::View<typename std::conditional<
                           RV::rank == 0, typename RV::non_const_value_type,
                           typename RV::non_const_value_type*>::type,
                       UnifiedRVLayout, typename RV::device_type,
                       Kokkos::MemoryTraits<Kokkos::Unmanaged> >
      RV_Internal;
  typedef Kokkos::View<
      typename std::conditional<XMV::rank == 1, typename XMV::const_value_type*,
                                typename XMV::const_value_type**>::type,
      UnifiedXLayout, typename XMV::device_type,
      Kokkos::MemoryTraits<Kokkos::Unmanaged> >
      XMV_Internal;

  RV_Internal R_internal  = R;
  XMV_Internal X_internal = X;

  Impl::NrmInf<RV_Internal, XMV_Internal>::nrminf(R_internal, X_internal);
}

}  // namespace KokkosBlas

#endif  // KOKKOSBLAS1_NRMINF_HPP_
