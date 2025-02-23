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
#ifndef __KOKKOSBATCHED_COPY_IMPL_HPP__
#define __KOKKOSBATCHED_COPY_IMPL_HPP__

/// \author Kyungjoo Kim (kyukim@sandia.gov)

#include "KokkosBatched_Util.hpp"
#include "KokkosBatched_Copy_Internal.hpp"

namespace KokkosBatched {

///
/// Serial Impl
/// ===========

template <>
template <typename AViewType, typename BViewType>
KOKKOS_INLINE_FUNCTION int SerialCopy<Trans::NoTranspose, 1>::invoke(
    const AViewType &A, const BViewType &B) {
  return SerialCopyInternal::invoke(A.extent(0), A.data(), A.stride_0(),
                                    B.data(), B.stride_0());
}

template <>
template <typename AViewType, typename BViewType>
KOKKOS_INLINE_FUNCTION int SerialCopy<Trans::Transpose, 1>::invoke(
    const AViewType &A, const BViewType &B) {
  return SerialCopyInternal::invoke(A.extent(0), A.data(), A.stride_0(),
                                    B.data(), B.stride_0());
}

template <>
template <typename AViewType, typename BViewType>
KOKKOS_INLINE_FUNCTION int SerialCopy<Trans::NoTranspose, 2>::invoke(
    const AViewType &A, const BViewType &B) {
  return SerialCopyInternal::invoke(A.extent(0), A.extent(1), A.data(),
                                    A.stride_0(), A.stride_1(), B.data(),
                                    B.stride_0(), B.stride_1());
}

template <>
template <typename AViewType, typename BViewType>
KOKKOS_INLINE_FUNCTION int SerialCopy<Trans::Transpose, 2>::invoke(
    const AViewType &A, const BViewType &B) {
  return SerialCopyInternal::invoke(A.extent(1), A.extent(0), A.data(),
                                    A.stride_1(), A.stride_0(), B.data(),
                                    B.stride_0(), B.stride_1());
}

///
/// Team Impl
/// =========

template <typename MemberType>
struct TeamCopy<MemberType, Trans::NoTranspose, 1> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamCopyInternal::invoke(member, A.extent(0), A.data(), A.stride_0(),
                                    B.data(), B.stride_0());
  }
};

template <typename MemberType>
struct TeamCopy<MemberType, Trans::Transpose, 1> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamCopyInternal::invoke(member, A.extent(0), A.data(), A.stride_0(),
                                    B.data(), B.stride_0());
  }
};

template <typename MemberType>
struct TeamCopy<MemberType, Trans::NoTranspose, 2> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamCopyInternal::invoke(member, A.extent(0), A.extent(1), A.data(),
                                    A.stride_0(), A.stride_1(), B.data(),
                                    B.stride_0(), B.stride_1());
  }
};

template <typename MemberType>
struct TeamCopy<MemberType, Trans::Transpose, 2> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamCopyInternal::invoke(member, A.extent(1), A.extent(0), A.data(),
                                    A.stride_1(), A.stride_0(), B.data(),
                                    B.stride_0(), B.stride_1());
  }
};

///
/// TeamVector Impl
/// =========

template <typename MemberType>
struct TeamVectorCopy<MemberType, Trans::NoTranspose, 1> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamVectorCopyInternal::invoke(member, A.extent(0), A.data(),
                                          A.stride_0(), B.data(), B.stride_0());
  }
};

template <typename MemberType>
struct TeamVectorCopy<MemberType, Trans::Transpose, 1> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamVectorCopyInternal::invoke(member, A.extent(0), A.data(),
                                          A.stride_0(), B.data(), B.stride_0());
  }
};

template <typename MemberType>
struct TeamVectorCopy<MemberType, Trans::NoTranspose, 2> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamVectorCopyInternal::invoke(member, A.extent(0), A.extent(1),
                                          A.data(), A.stride_0(), A.stride_1(),
                                          B.data(), B.stride_0(), B.stride_1());
  }
};

template <typename MemberType>
struct TeamVectorCopy<MemberType, Trans::Transpose, 2> {
  template <typename AViewType, typename BViewType>
  KOKKOS_INLINE_FUNCTION static int invoke(const MemberType &member,
                                           const AViewType &A,
                                           const BViewType &B) {
    return TeamVectorCopyInternal::invoke(member, A.extent(1), A.extent(0),
                                          A.data(), A.stride_1(), A.stride_0(),
                                          B.data(), B.stride_0(), B.stride_1());
  }
};

}  // end namespace KokkosBatched

#endif
