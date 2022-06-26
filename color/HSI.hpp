#include <opencv2/opencv.hpp>
#include <cmath>
#include<iostream>

using namespace std;
using namespace cv;

#define PI 3.14159265

Mat rgb2hsi(Mat RGB){
    Mat HSI(RGB.rows,RGB.cols,CV_32FC3);

    for(int i=0;i<RGB.rows;i++)
        for(int j=0;j<RGB.cols;j++){

            HSI.at<Vec3f>(i,j)[2]=(RGB.at<Vec3b>(i,j)[0]+RGB.at<Vec3b>(i,j)[1]+RGB.at<Vec3b>(i,j)[2])/3;

            if(HSI.at<Vec3f>(i,j)[2]==255){
                HSI.at<Vec3f>(i,j)[0]=0;
                HSI.at<Vec3f>(i,j)[1]=0;
            }

            if(HSI.at<Vec3f>(i,j)[2]>0){
                double minRGB=min(RGB.at<Vec3b>(i,j)[0],min(RGB.at<Vec3b>(i,j)[1],RGB.at<Vec3b>(i,j)[2]));
                HSI.at<Vec3f>(i,j)[1] = 1 - minRGB / HSI.at<Vec3f>(i,j)[2];
            }else{
                HSI.at<Vec3f>(i,j)[1]=0;
            }

            double num = RGB.at<Vec3b>(i,j)[0] - (RGB.at<Vec3b>(i,j)[1]/2) - (RGB.at<Vec3b>(i,j)[2]/2);
            double den = sqrt( pow(RGB.at<Vec3b>(i,j)[0] - RGB.at<Vec3b>(i,j)[1],2) + (RGB.at<Vec3b>(i,j)[0] - RGB.at<Vec3b>(i,j)[2]) * (RGB.at<Vec3b>(i,j)[1] - RGB.at<Vec3b>(i,j)[2]));
            if(den>0){
                float arg = num/den;
                HSI.at<Vec3f>(i,j)[0] = acos(arg>1?1:arg) * 180.0 / PI;
            }
            else
                HSI.at<Vec3f>(i,j)[0] = 0;

            if(RGB.at<Vec3b>(i,j)[2]>RGB.at<Vec3b>(i,j)[1])
                HSI.at<Vec3f>(i,j)[0]=360-HSI.at<Vec3f>(i,j)[0];

            //cout<<HSI.at<Vec3f>(i,j)<<endl;
            //cin.get();
        }
    return HSI;
}

Mat hsi2rgb(Mat HSI){
    Mat RGB(HSI.rows,HSI.cols,CV_8UC3);

    for(int i=0;i<RGB.rows;i++)
        for(int j=0;j<RGB.cols;j++){
            /*if(HSI.at<Vec3f>(i,j)[0]==0){
                RGB.at<Vec3b>(i,j)[0] = HSI.at<Vec3f>(i,j)[2] * (2 + HSI.at<Vec3f>(i,j)[2] * HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[1] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[2] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);    
            }
            else */if(0<=HSI.at<Vec3f>(i,j)[0] && HSI.at<Vec3f>(i,j)[0]<120){
                RGB.at<Vec3b>(i,j)[2] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[0] = HSI.at<Vec3f>(i,j)[2] * (1 + (HSI.at<Vec3f>(i,j)[1] * cos(HSI.at<Vec3f>(i,j)[0] * PI / 180.0) / cos((60 - HSI.at<Vec3f>(i,j)[0]) * PI / 180.0)));
                RGB.at<Vec3b>(i,j)[1] = 3 * HSI.at<Vec3f>(i,j)[2] - (RGB.at<Vec3b>(i,j)[0] + RGB.at<Vec3b>(i,j)[2]);
            }/*else if(HSI.at<Vec3f>(i,j)[0]==120){
                RGB.at<Vec3b>(i,j)[0] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[1] = HSI.at<Vec3f>(i,j)[2] * (2 + HSI.at<Vec3f>(i,j)[2] * HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[2] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);    
            }*/else if(120<=HSI.at<Vec3f>(i,j)[0] && HSI.at<Vec3f>(i,j)[0]<240){
                HSI.at<Vec3f>(i,j)[0]-=120;
                RGB.at<Vec3b>(i,j)[0] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[1] = HSI.at<Vec3f>(i,j)[2] * (1 + (HSI.at<Vec3f>(i,j)[1] * cos(HSI.at<Vec3f>(i,j)[0] * PI / 180.0) / cos((60 - HSI.at<Vec3f>(i,j)[0]) * PI / 180.0)));
                RGB.at<Vec3b>(i,j)[2] = 3 * HSI.at<Vec3f>(i,j)[2] - (RGB.at<Vec3b>(i,j)[0] + RGB.at<Vec3b>(i,j)[1]);
            }/*else if(HSI.at<Vec3f>(i,j)[0]==240){
                RGB.at<Vec3b>(i,j)[0] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[1] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[2] = HSI.at<Vec3f>(i,j)[2] * (2 + HSI.at<Vec3f>(i,j)[2] * HSI.at<Vec3f>(i,j)[1]);    
            }*/else{
                HSI.at<Vec3f>(i,j)[0]-=240;
                RGB.at<Vec3b>(i,j)[1] = HSI.at<Vec3f>(i,j)[2] * (1 - HSI.at<Vec3f>(i,j)[1]);
                RGB.at<Vec3b>(i,j)[2] = HSI.at<Vec3f>(i,j)[2] * (1 + (HSI.at<Vec3f>(i,j)[1] * cos(HSI.at<Vec3f>(i,j)[0] * PI / 180.0) / cos((60 - HSI.at<Vec3f>(i,j)[0]) * PI / 180.0)));
                RGB.at<Vec3b>(i,j)[0] = 3 * HSI.at<Vec3f>(i,j)[2] - (RGB.at<Vec3b>(i,j)[1] + RGB.at<Vec3b>(i,j)[2]);
            }
        }
    return RGB;
}
