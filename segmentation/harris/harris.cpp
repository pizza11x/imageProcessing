/* 
    STEPS :
    - Calculate the partial derivatives with respect to x e y (Dx e Dy).
    - Calculate Dx^2, Dy^2 e Dx * Dy.
    - Apply Gaussian filter to Dx^2, Dy^2 e Dx * Dy.
    - C00, C11, C01, C10 are obtained
    - Calculate the index R.
    - Normalize the index R to [0,255].
    - Threshold R.
*/

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void harrisCornering(Mat& src, Mat& output,float k, int thresholdValue);

int main(int argc, char** argv) {

    if(argc < 2){
        cout<<"usage: "<<argv[0]<<" image_name"<<endl;
        exit(0);
    }

    String imageName = argv[1];

    //Read image   
    Mat image;
    image = imread(imageName, IMREAD_GRAYSCALE);

    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat harris(image.rows, image.cols, image.type());
    image.copyTo(harris);

    harrisCornering(image, harris, 0.04, 100);

    imshow("Original image", image);
    imshow("Harris cornering", harris);
    waitKey(0);
    destroyAllWindows();

    return 0;
}

void harrisCornering(Mat& src, Mat &output, float k, int thresholdValue)
{
    //STEP 1: Apply Sobel to both directions
    Mat Dx, Dy;
    Sobel(src, Dx, CV_32FC1, 1, 0, 3, BORDER_DEFAULT);
    Sobel(src, Dy, CV_32FC1, 0, 1, 3, BORDER_DEFAULT);

    //STEP 2
    Mat Dx2, Dy2, Dxy;
    pow(Dx, 2.0, Dx2); //Calculate Dx^2.
    pow(Dy, 2.0, Dy2); //Calculate Dy^2.
    multiply(Dx, Dy, Dxy); //Calcolo Dx * Dy.

    //STEP 3: Smoothing
    GaussianBlur(Dx2, Dx2, Size(7, 7), 2.0, 0.0, BORDER_DEFAULT); //C00
    GaussianBlur(Dy2, Dy2, Size(7, 7), 0.0, 2.0, BORDER_DEFAULT); //C11
    GaussianBlur(Dxy, Dxy, Size(7, 7), 2.0, 2.0, BORDER_DEFAULT); //C01

    //STEP 5.
    Mat Dx2y2, Dxy2, trace2, R;
    multiply(Dx2, Dy2, Dx2y2); //C00 * C11
    multiply(Dxy, Dxy, Dxy2); //C10 * C01

    Mat detrm = Dx2y2 - Dxy2; //Determine = (C00 * C11) - (C10 * C01)
    pow((Dx2 + Dy2), 2.0, trace2); //Calculate trace.
    /*
        k is usually a low value (0.04 in our case).
        It is the contribution of the trace to the final calculation of R.
        (amount of trace that I subtract from the determinant).
    */
    R = detrm - k * trace2; 
    
    //STEP 6
    normalize(R, R, 0, 255, NORM_MINMAX, CV_32FC1);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            if ((int)R.at<float>(i, j) > thresholdValue)
            {
                //Circle on corner points
                circle(output, Point(j, i), 5, Scalar(0), 2, 8, 0); 
            }
        }
    }
}