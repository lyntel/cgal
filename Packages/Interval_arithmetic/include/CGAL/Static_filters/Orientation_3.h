// Copyright (c) 2001,2004  Utrecht University (The Netherlands),
// ETH Zurich (Switzerland), Freie Universitaet Berlin (Germany),
// INRIA Sophia-Antipolis (France), Martin-Luther-University Halle-Wittenberg
// (Germany), Max-Planck-Institute Saarbruecken (Germany), RISC Linz (Austria),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Sylvain Pion

#ifndef CGAL_STATIC_FILTERS_ORIENTATION_3_H
#define CGAL_STATIC_FILTERS_ORIENTATION_3_H

#include <CGAL/Profile_counter.h>
// #include <CGAL/Static_filter_error.h> // Only used to precompute constants

CGAL_BEGIN_NAMESPACE

template < typename K_base >
class SF_Orientation_3
  : public K_base::Orientation_3
{
#if 0
  // Computes the epsilon for Orientation_3.
  static double ori_3()
  {
    typedef Static_filter_error F;
    F t1 = F(1)-F(1);         // First translation
    F det = det3x3_by_formula(t1, t1, t1,
                              t1, t1, t1,
                              t1, t1, t1); // Full det
    double err = det.error();
    std::cerr << "*** epsilon for Orientation_3 = " << err << std::endl;
    return err;
  }
#endif

  typedef typename K_base::Point_3          Point_3;
  typedef typename K_base::Orientation_3    Base;

public:

  Orientation operator()(const Point_3 &p, const Point_3 &q,
                         const Point_3 &r, const Point_3 &s) const
  {
      if (fit_in_double(p.x()) && fit_in_double(p.y()) && fit_in_double(p.z()) &&
          fit_in_double(q.x()) && fit_in_double(q.y()) && fit_in_double(q.z()) &&
          fit_in_double(r.x()) && fit_in_double(r.y()) && fit_in_double(r.z()) &&
          fit_in_double(s.x()) && fit_in_double(s.y()) && fit_in_double(s.z()))
      {
          double px = p.x();
          double py = p.y();
          double pz = p.z();
          double qx = q.x();
          double qy = q.y();
          double qz = q.z();
          double rx = r.x();
          double ry = r.y();
          double rz = r.z();
          double sx = s.x();
          double sy = s.y();
          double sz = s.z();

          CGAL_PROFILER("Orientation_3 calls");

          double pqx = qx-px;
          double pqy = qy-py;
          double pqz = qz-pz;
          double prx = rx-px;
          double pry = ry-py;
          double prz = rz-pz;
          double psx = sx-px;
          double psy = sy-py;
          double psz = sz-pz;

          double det = det3x3_by_formula(pqx, pqy, pqz,
                                         prx, pry, prz,
                                         psx, psy, psz);

#if 1
          // Then semi-static filter.
          double maxx = fabs(px);
          if (maxx < fabs(qx)) maxx = fabs(qx);
          if (maxx < fabs(rx)) maxx = fabs(rx);
          if (maxx < fabs(sx)) maxx = fabs(sx);
          double maxy = fabs(py);
          if (maxy < fabs(qy)) maxy = fabs(qy);
          if (maxy < fabs(ry)) maxy = fabs(ry);
          if (maxy < fabs(sy)) maxy = fabs(sy);
          double maxz = fabs(pz);
          if (maxz < fabs(qz)) maxz = fabs(qz);
          if (maxz < fabs(rz)) maxz = fabs(rz);
          if (maxz < fabs(sz)) maxz = fabs(sz);
          double eps = 3.90799e-14 * maxx * maxy * maxz;

          if (det > eps)  return POSITIVE;
          if (det < -eps) return NEGATIVE;

          CGAL_PROFILER("Orientation_3 semi-static failures");
#endif

          // Experiments showed that there's practically no benefit for testing when
          // the initial substractions were done exactly.  In most cases where the
          // first filter fails, the determinant is null.  Most of those cases are
          // caught by the IA filter, but not by a second stage semi-static filter,
          // because in most cases one of the coefficient is null, and IA takes
          // advantage of this, though it's going through an inexact temporary
          // computation (which is zeroed later).
      }

      return Base::operator()(p, q, r, s);
  }

};

CGAL_END_NAMESPACE

#endif // CGAL_STATIC_FILTERS_ORIENTATION_3_H
