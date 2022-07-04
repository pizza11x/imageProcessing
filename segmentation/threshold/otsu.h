#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

vector <double> normalizedHistogram(Mat img);
void multipleThrehosld(Mat img, Mat &output, vector<int> thresh);

vector <double> normalizedHistogram(Mat img){
    vector<double> his(256, 0.0f);
    for(int y = 0; y < img.rows; y++){
        for(int x = 0; x < img.cols; x++){
            his[img.at<uchar>(y, x)]++;
        }
    }
    for(int i = 0; i < 256; i++)
        his[i] /= img.rows * img.cols;
    return his;
}

void multipleThrehosld(Mat img, Mat &output, vector<int> thresh){
    output = Mat::zeros(img.size(), img.type());
    for(int y = 0; y < img.rows; y ++){
        for(int x = 0; x < img.cols; x++){
            if(img.at<uchar>(y, x) >= thresh[1]){
                output.at<uchar>(y, x) = 255;
            }else if(img.at<uchar>(y, x) >= thresh[0])
                output.at<uchar>(y, x) = 127;
        }
    }
}