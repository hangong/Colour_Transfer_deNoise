/* 
 * This file is the header of Colour Transfer De-Noise.
 *
 * Copyright (c) 2016 Han Gong <gong@fedoraproject.org>, Univeristy of East Anglia.
 * 
 * It is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as   
 * published by the Free Software Foundation, version 3.
 *
 * It is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "opencv2/core.hpp"

using namespace std;
using namespace cv;

Mat solve(const Mat &IRR, const Mat &I0, const Mat &IR, \
          const int nbits, double smoothness, int level);
Mat deNoise_rec(const Mat &IRR, const Mat &I0, const Mat &IR, \
          const int *nbits, double smoothness, int level);
Mat deNoise(const Mat &I0, const Mat &IR, double smoothness=1.);
