# Colour_Transfer_deNoise
A C++ implementation (based on OpenCV) of an image colour transfer noise deduction algorithm.

# Examples

J = deNoise(I_s, It) // I_s and I_t are the inputs.

Source Image (I_s)

![source image](https://github.com/hangong/Colour_Transfer_deNoise/blob/master/org.jpg?raw=true)

Original Bad Colour Transfer Result (I_t)

![bad result](https://github.com/hangong/Colour_Transfer_deNoise/blob/master/rendered.jpg?raw=true)

De-Noised Colour Transfer Output (J)

![Fixture](https://github.com/hangong/Colour_Transfer_deNoise/blob/master/fixed.jpg?raw=true)

# Usage
## Compile
Please check the Makefile to edit your OpenCV library path. This code was tested on OSX.

$ make all
## Test

$ make test

# Copyright
Enjoy and feel free to use the code for your purposes. But please keep my original copyright statements.

Han Gong <gong@fedorapoject.org> 2016, University of East Anglia

# Reference
 Automated colour grading using colour distribution transfer, 2007
 Section 4, CVIU.
