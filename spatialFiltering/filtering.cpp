#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char ** argv){
    int dim, th;

    if(argc < 2){
        cout << "usage: " << argv[0] << " image_name" << endl;
        exit(0);
    }
    String imageName = argv[1];

    //Read image
    Mat image;
    image = imread(samples::findFile(imageName), IMREAD_GRAYSCALE);

    if(image.empty()){
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    //Read filter dimension
    cout << "Insert filter dimension: ";
    cin >> dim;

    //Alloacate filter and convulation
    Mat average_filter = Mat::ones(dim, dim, CV_32F)/(float)(dim*dim);
    //For convulation rotate the filter to 180 degrees
    //rotate(filter, r_filter, ROTATE_180)

    Mat output2D;
    filter2D(image, output2D, image.type(), average_filter);

    //Blurring average filter
    Mat outputBlur;
    blur(image, outputBlur, Size(dim, dim));

    Mat outputBox;
    boxFilter(image, outputBox, image.type(), Size(dim, dim));

    //Median filter
    Mat outputMedian;
    medianBlur(image, outputMedian, dim);

    //Gaussian filter
    Mat outputGaussian;
    GaussianBlur(image, outputGaussian, Size(dim, dim), 0,0);

    imshow("Original image", image);
    imshow("Filter2D image", output2D);
    imshow("Blur image", outputBlur);
    imshow("BoxFilter image", outputBox);
    imshow("MedianBlur image", outputMedian);
    imshow("GaussianBlur image", outputGaussian);

    cout << "Insert threshold: ";
    cin >> th;

    //Thresholding
    Mat mask;
    threshold(outputGaussian, mask, th, 1, CV_THRESH_BINARY);
    //Image * mask for rotate only the points of interest
    imshow("Image*Mak", image.mul(mask));

    Mat mask255;
    threshold(outputGaussian, mask255, th, 255, CV_THRESH_BINARY);
    imshow("Mask", mask255);

    waitKey(0);
    return 0;
    
}