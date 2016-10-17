/* 
 * This file is the demo part of Colour Transfer De-Noise.
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
#include "opencv2/highgui.hpp"
#include "deNoise.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout <<" Usage: demo source.jpg rendered.jpg" << endl;
     return -1;
    }

    Mat I0, IR, IRR;
    I0 = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
    IR = imread(argv[2], CV_LOAD_IMAGE_COLOR);   // Read the file
    IRR = deNoise(I0, IR);

    namedWindow( "Result", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Result", IRR );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
    
}
