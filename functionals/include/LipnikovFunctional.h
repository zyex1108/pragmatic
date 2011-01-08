/*  Copyright (C) 2010 Imperial College London and others.
    
    Please see the AUTHORS file in the main source directory for a full list
    of copyright holders.

    Gerard Gorman
    Applied Modelling and Computation Group
    Department of Earth Science and Engineering
    Imperial College London

    g.gorman@imperial.ac.uk
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation,
    version 2.1 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
    USA
*/
#ifndef LIPNIKOV_H
#define LIPNIKOV_H

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <cmath>

/*! \brief Evaluates Lipnikov functional.
 */
template<typename real_t>
class LipnikovFunctional{
 public:
  LipnikovFunctional(const real_t *x0, const real_t *x1, const real_t *x2){
    orientation = 1;

    real_t A = area(x0, x1, x2);
    if(A<0)
      orientation = -1;
    else
      orientation = 1;
  }

  LipnikovFunctional(const real_t *x0, const real_t *x1, const real_t *x2, const real_t *x3){
    orientation = 1;

    real_t V = volume(x0, x1, x2, x3);
    if(V<0)
      orientation = -1;
    else
      orientation = 1;
  }

  // Calculate area.
  real_t area(const real_t *x0, const real_t *x1, const real_t *x2){
    return orientation*0.5*((x0[1] - x2[1])*(x0[0] - x1[0]) - (x0[1] - x1[1])*(x0[0] - x2[0]));
  }

  // Calculate volume.
  real_t volume(const real_t *x0, const real_t *x1, const real_t *x2, const real_t *x3){
    return orientation*(-(x0[0] - x3[0])*((x0[2] - x2[2])*(x0[1] - x1[1]) - (x0[2] - x1[2])*(x0[1] - x2[1])) + (x0[0] - x2[0])*((x0[2] - x3[2])*(x0[1] - x1[1]) - (x0[2] - x1[2])*(x0[1] - x3[1])) - (x0[0] - x1[0])*((x0[2] - x3[2])*(x0[1] - x2[1]) - (x0[2] - x2[2])*(x0[1] - x3[1])))/6;
  }
  
  /// 2D
  real_t calculate(const real_t *x0, const real_t *x1, const real_t *x2,
                   const real_t *m0, const real_t *m1, const real_t *m2){
    // Metric tensor
    real_t m00 = (m0[0] + m1[0] + m2[0])/3;
    real_t m01 = (m0[1] + m1[1] + m2[1])/3;
    real_t m11 = (m0[3] + m1[3] + m2[3])/3;

    // Paremeter
    real_t l =
      sqrt((x0[1] - x1[1])*((x0[1] - x1[1])*m11 + (x0[0] - x1[0])*m01) + 
           (x0[0] - x1[0])*((x0[1] - x1[1])*m01 + (x0[0] - x1[0])*m00))+
      sqrt((x0[1] - x2[1])*((x0[1] - x2[1])*m11 + (x0[0] - x2[0])*m01) + 
           (x0[0] - x2[0])*((x0[1] - x2[1])*m01 + (x0[0] - x2[0])*m00))+
      sqrt((x2[1] - x1[1])*((x2[1] - x1[1])*m11 + (x2[0] - x1[0])*m01) + 
           (x2[0] - x1[0])*((x2[1] - x1[1])*m01 + (x2[0] - x1[0])*m00));

    // Area
    real_t a=area(x0, x1, x2);

    // Area in metric space
    real_t a_m = a*(m00*m11 - m01*m01);

    // Function
    real_t f = min(l/3, 3/l);
    real_t F = pow(f * (2.0 - f), 3);
    real_t quality = 12*sqrt(3)*a_m*F/(l*l);
    
    return quality;
  }

  /// 3D
  real_t calculate(const real_t *x0, const real_t *x1, const real_t *x2, const real_t *x3,
                   const real_t *m0, const real_t *m1, const real_t *m2, const real_t *m3){
    // Metric tensor
    real_t m00 = (m0[0] + m1[0] + m2[0] + m3[0])/4;
    real_t m01 = (m0[1] + m1[1] + m2[1] + m3[1])/4;
    real_t m02 = (m0[2] + m1[2] + m2[2] + m3[2])/4;
    real_t m11 = (m0[4] + m1[4] + m2[4] + m3[4])/4;
    real_t m12 = (m0[5] + m1[5] + m2[5] + m3[5])/4;
    real_t m22 = (m0[8] + m1[8] + m2[8] + m3[8])/4;

    // Paremeter
    real_t l =
      sqrt((x0[2] - x1[2])*((x0[2] - x1[2])*m22 + (x0[1] - x1[1])*m12 + (x0[0] - x1[0])*m02) + (x0[1] - x1[1])*((x0[2] - x1[2])*m12 + (x0[1] - x1[1])*m11 + (x0[0] - x1[0])*m01) + (x0[0] - x1[0])*((x0[2] - x1[2])*m02 + (x0[1] - x1[1])*m01 + (x0[0] - x1[0])*m00)) +
      sqrt((x1[2] - x2[2])*((x1[2] - x2[2])*m22 + (x1[1] - x2[1])*m12 + (x1[0] - x2[0])*m02) + (x1[1] - x2[1])*((x1[2] - x2[2])*m12 + (x1[1] - x2[1])*m11 + (x1[0] - x2[0])*m01) + (x1[0] - x2[0])*((x1[2] - x2[2])*m02 + (x1[1] - x2[1])*m01 + (x1[0] - x2[0])*m00)) +
      sqrt((x2[2] - x3[2])*((x2[2] - x3[2])*m22 + (x2[1] - x3[1])*m12 + (x2[0] - x3[0])*m02) + (x2[1] - x3[1])*((x2[2] - x3[2])*m12 + (x2[1] - x3[1])*m11 + (x2[0] - x3[0])*m01) + (x2[0] - x3[0])*((x2[2] - x3[2])*m02 + (x2[1] - x3[1])*m01 + (x2[0] - x3[0])*m00));

    // Volume
    real_t v=volume(x0, x1, x2, x3);

    // Volume in metric space
    real_t v_m = v*(((m11*m22 - m12*m12)*m00 - (m01*m22 - m02*m12)*m01 + (m01*m12 - m02*m11)*m02));

    // Function
    real_t f = min(l/6, 6/l);
    real_t F = pow(f * (2.0 - f), 3);
    real_t quality = pow(6.0, 4)*sqrt(2.0) * v_m * F / (l*l*l);

    return quality;
  }

 private:
  int orientation;
};
#endif