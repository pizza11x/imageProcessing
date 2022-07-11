/* 
    STEPS :
    - Convolution of the input image with a Gaussian filter
    - Calculate the magnitude and orientation of the gradient
    - Apply non maxima suppression
    - Apply thresholding with insteresis
*/

#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static void canny(Mat &src, Mat &output, int kSize);
static void nonMaxSuppression(Mat &magnitude, Mat &orientations, Mat &nms);
void insteresis(Mat &nms, Mat &thresholdInsteresis, int lowTh, int highTh);

const int kSize = 3;

int main(int argc, char** argv){

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

    Mat imgCanny;
    canny(image, imgCanny, kSize);

    imshow("Original image", image);
    imshow("Canny image", imgCanny);

    waitKey(0);
    destroyAllWindows();
    return 0;
}

static void canny(Mat &src, Mat &output, int kSize){
    //Apply Gaussian
    Mat imgGaussian;
    GaussianBlur(src, imgGaussian, Size(5, 5), 0, 0);

    /* Apply Sobel's filters to get the two images in which 
    I have the two components of the gradient vector */
    Mat dx, dy;
    Sobel(imgGaussian, dx, CV_32FC1, 1, 0, kSize);
    Sobel(imgGaussian, dy, CV_32FC1, 0, 1, kSize);

    /* Calculation the magnitute M(square root of the sum of the components squared 
    or with sum of the absolute values of the two components) */
    Mat magnitude, dx2, dy2;
    pow(dx, 2, dx2);
    pow(dy, 2, dy2);
    sqrt(dx2+dy2, magnitude);
    normalize(magnitude, magnitude, 0, 255, NORM_MINMAX, CV_8U);

    //Calculate orientations
    Mat orientations;
    phase(dx, dy, orientations, true);

    Mat nms = Mat::zeros(magnitude.rows, magnitude.cols, CV_8U);
    nonMaxSuppression(magnitude, orientations, nms);

    Mat thresholdInsteresis = Mat::zeros(magnitude.rows, magnitude.cols, CV_8U);
    insteresis(nms, thresholdInsteresis, 40, 100);

    output = thresholdInsteresis;
}

static void nonMaxSuppression(Mat &magnitude, Mat &orientations, Mat &nms){
    for(int i = 1; i< magnitude.rows -1; i++){
        for(int j= 1; j< magnitude.cols -1; j++){
            float angle = orientations.at<float>(i, j);
            angle = angle > 180? angle -360 : angle;

            //Analyze 4 ranges of angles
            //Check if the central pixel magnitude.at(i, j) is greater than its neighbors in that direction
            if((angle < -157.5 && angle >= 157.5) || (angle > -22.5 && angle <= 22.5)){
                if(magnitude.at<uchar>(i,j) > magnitude.at<uchar>(i, j-1) &&
                    magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i, j-1)){
                        nms.at<uchar>(i, j) = magnitude.at<uchar>(i,j);
                }
            }
            if((angle < -112.5 && angle >= -157.5) || (angle >= 22.5 && angle < 67.5)){
                if(magnitude.at<uchar>(i,j) > magnitude.at<uchar>(i+1, j-1) &&
                    magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i-1, j+1)){
                        nms.at<uchar>(i, j) = magnitude.at<uchar>(i,j);
                }
            }
            if((angle < -67.5 && angle >= -112.5) || (angle <= 112.5 && angle > 67.5)){
                if(magnitude.at<uchar>(i,j) > magnitude.at<uchar>(i+1, j) &&
                    magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i-1, j)){
                        nms.at<uchar>(i, j) = magnitude.at<uchar>(i,j);
                }
            }
            if((angle < -22.5 && angle >= -67.5) || (angle >= 112.5 && angle < 157.5)){
                if(magnitude.at<uchar>(i,j) > magnitude.at<uchar>(i-1, j-1) &&
                    magnitude.at<uchar>(i, j) > magnitude.at<uchar>(i+1, j+1)){
                        nms.at<uchar>(i, j) = magnitude.at<uchar>(i,j);
                }
            }
        }
    }
    imshow("NMS", nms);
    imshow("Angle", orientations);
    imshow("Magnitude", magnitude);
    waitKey(0);
}

void insteresis(Mat &nms, Mat &thresholdInsteresis, int lowTh, int higTh){
    for(int i = 1; i< nms.rows; i++){
        for(int j=1; j<nms.cols; j++){
            if(nms.at<uchar>(i,j)>higTh){
                    thresholdInsteresis.at<uchar>(i,j)=255;
            }
            else if(nms.at<uchar>(i,j)<lowTh){
                thresholdInsteresis.at<uchar>(i,j)=0;
            }
            else if(nms.at<uchar>(i,j)>lowTh && nms.at<uchar>(i,j)<higTh){
                for(int rows=-1;rows<2;rows++){
                    for(int cols=-1;cols<2;cols++){
                        if(nms.at<uchar>(i+righe,j+colonne)>higTh){
                            thresholdInsteresis.at<uchar>(i+righe,j+colonne)=255;
                        }
                    }
                }
            }
        }
    }
}
