/* 
 * This file is function part of Colour Transfer De-Noise.
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

#include "deNoise.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <limits>
#include <array>
#include "opencv2/highgui.hpp"

Mat solve(const Mat &IRR, const Mat &I0, const Mat &IR, \
          const int nbits, double smoothness, int level)
{
    double eps = numeric_limits<double>::epsilon();

    // get image size
    int Nrow = I0.rows;
    int Ncol = I0.cols;
    int Nch = I0.channels();

    Mat bIRR, bI0, bIR;

    // make border
    copyMakeBorder(I0, bI0, 1,1,1,1, BORDER_REPLICATE);
    copyMakeBorder(IR, bIR, 1,1,1,1, BORDER_REPLICATE);

    // shifted coordinates (0, top, bottom, left, right)
    array<Range, 4> x, y;
    y[0] = Range(1, Nrow+1);
    y[1] = Range(2, Nrow+2);
    y[2] = Range(1, Nrow+1);
    y[3] = Range(0, Nrow);

    x[0] = Range(2, Ncol+2);
    x[1] = Range(1, Ncol+1);
    x[2] = Range(0, Ncol);
    x[3] = Range(1, Ncol+1);

    // get greyscale gradient magnitude dI0 of I0
    auto getGrad = [&](Mat const &in)->Mat{ 
        Mat G0x, G0y, g;
        cvtColor(in,g,CV_BGR2GRAY);
        Sobel(g, G0x, -1, 1, 0 ,3);
        Sobel(g, G0y, -1, 0, 1 ,3);
        Mat G0 = G0x.mul(G0x) + G0y.mul(G0y);
        sqrt(G0, G0);
        return G0;
    };
    Mat dI0 = getGrad(I0);

    Mat tmp[] = {dI0,dI0,dI0};
    merge(tmp, Nch, dI0);

    // current pyramid scale
    double h = pow(2.,double(-level));
    // gradient difference weight (heristic function)
    Mat psi = min(255.*dI0/5., 1.);
    // intensity difference weight (heuristic function)
    Mat phi = 30./(1. + 10.*dI0/max(smoothness, eps))*h;
    Mat bphi;
    copyMakeBorder(phi, bphi, 1,1,1,1, BORDER_REPLICATE);

    array<Mat, 6> a;
    for(int i=0; i<4; i++)
    {
        Range tRanges[] = {y[i],x[i],Range::all()};
        a[i] = (bphi(tRanges) + phi)/2.;
    }
    a[4] = psi + a[0] + a[1] + a[2] + a[3];

    // GI0 derivatives in 4 directions
    array<Mat, 4> GI0;
    for(int i=0; i<4; i++)
    {
        Range tRanges[] = {y[i],x[i],Range::all()};
        GI0[i] = -bI0(tRanges) + I0;
    }

    double rho = 0.2; // alpha blend factor
    Mat a5_const = psi.mul(IR);

    Mat IRR_out = IRR.clone(); // DEEP copy
    for(int i=0; i<nbits; i++) // just iterate
    {
        copyMakeBorder(IRR_out, bIRR, 1,1,1,1, BORDER_REPLICATE);
        // pack a[5]
        a[5] = a5_const.clone(); // DEEP copy
        //cout<<IRR.cols<<I0.rows<<endl;
        for(int j=0; j<4; j++)
        {
            Range tRanges[] = {y[j],x[j],Range::all()};
            //cout<< nbits << ":" << y[j] << "," << x[j] <<endl;
            a[5] += a[j].mul( bIRR(tRanges) + GI0[j] );
        }

        // solve for output image of this iteration
        Mat IRR0;
        divide(a[5],a[4]+eps,IRR0);

        // alpha blend the previous iteration's result
        IRR_out = IRR0*(1-rho) + IRR_out*rho;
    }

    /* DEBUG USE
    cout<<nbits<<endl;
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", IRR_out );                   // Show our image inside it.
    waitKey(0);                                          // Wait for a keystroke in the window
    */

    return IRR_out;
}

Mat deNoise_rec(const Mat &IRR, const Mat &I0, const Mat &IR, \
          const int *nbits, double smoothness, int level)
{
    int Nrow = I0.rows;
    int Ncol = I0.cols;

    // half size (ceil)
    int Nrow2 = 1 + (Nrow-1)/2;
    int Ncol2 = 1 + (Ncol-1)/2;

    Mat IRR_out;
    if (*(nbits+1) && Ncol2 > 20 && Nrow2>20)
    {
        Mat I02, IR2, IRR2;
        resize(I0,I02,Size(Ncol2,Nrow2));
        resize(IR,IR2,Size(Ncol2,Nrow2));
        resize(IRR,IRR2,Size(Ncol2,Nrow2));
        IRR2 = deNoise_rec(IRR2, I02, IR2, nbits+1, smoothness, level+1);
        resize(IRR2,IRR_out,Size(Ncol,Nrow));
    }
    else
        IRR_out = IRR;

    return solve(IRR_out, I0, IR, *nbits, smoothness, level);
}

Mat deNoise(const Mat &I0, const Mat &IR, double smoothness)
{
    const int nbits[] = {4,16,32,64,64,64,0};

    Mat I0_in, IR_in;
    I0.convertTo(I0_in,CV_32FC3,1./255.);
    IR.convertTo(IR_in,CV_32FC3,1./255.);

    return deNoise_rec(I0_in, I0_in, IR_in, nbits, smoothness, 0);
}

