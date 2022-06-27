#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

int main(int argc, char**argv){

    if( argc < 2)
    {
        cout<< "usage: " << argv[0] <<" image_name" << endl;
        exit(-1);
    }

    String imageName = argv[1];

    Mat image;
    
    image = imread(imageName, IMREAD_COLOR );
    
    if( image.empty() )
    {
        cout <<  "Could not open or find the image" << endl ;
        exit(-1);
    }
    int padding = 2;
    int padding2 = padding/2;

    Mat copy = Mat::zeros(image.rows+padding, image.cols+padding, image.type());
    image.copyTo(copy(Rect(padding2, padding2, image.cols, image.rows)));

    int sumB, sumG, sumR;
    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            sumB = 0;
            sumG = 0;
            sumR = 0;
            for(int r=-1; r<=1; r++){
                for(int c= -1; c<=1; c++){
                    sumB += image.at<Vec3b>(i+1+r, j+1+c)[0];
                    sumG += image.at<Vec3b>(i+1+r, j+1+c)[1];
                    sumR += image.at<Vec3b>(i+1+r, j+1+c)[2];

                }
            }
            copy.at<Vec3b>(i+1, j+1)[0] = sumB/9;
            copy.at<Vec3b>(i+1, j+1)[1] = sumG/9;
            copy.at<Vec3b>(i+1, j+1)[2] = sumR/9;

        }
    }

    imshow("Original image",image);
    imshow("Copy", copy);

    waitKey(0);
    destroyAllWindows();
    return 0;
}
