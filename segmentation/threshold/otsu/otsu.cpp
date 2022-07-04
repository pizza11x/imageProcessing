#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void otsu(Mat &src, Mat &output);

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

    imshow("Original image", image);

    Mat output;
    otsu(image, output);

    imshow("Otsu image", output);
    
    waitKey(0);
    destroyAllWindows();
    return 0;
}

void otsu(Mat &src, Mat &output){
    double normHistogram[256];
    for(int i=0; i<src.rows; i++){
        for(int j=0; j<src.cols; j++){
            normHistogram[src.at<uchar>(i, j)]+=1;
        }
    }
    double N = src.cols*src.rows;
    double globalMean = 0;

    for(int i =0; i < 256; i++){
        //Normalize histogram
        normHistogram[i] = normHistogram[i]/N;
        //Calculate global average 
        globalMean += i * normHistogram[i];
    }

    double P1k, p1k;
    double maxVariance = 0;
    int optimalTh = 0;
    for(int th = 0; th < 256; th++){
        P1k += normHistogram[th];
        p1k += th * normHistogram[th];

        double P2k = 1-P1k;
        double p2k = globalMean - p1k;

        double m1k = p1k / P1k;
        double m2k = p2k / P2k;

        double variance = P1k * P2k * (m1k-m2k) * (m1k-m2k);
        if(variance > maxVariance){
            maxVariance = variance;
            optimalTh = th;
        }
    }
    Mat out = Mat::zeros(src.rows, src.cols, CV_8UC1);
    for(int i = 0; i < src.rows; i++){
        for(int j = 0; j < src.cols; j++){
            if(src.at<uchar>(i, j) > optimalTh){
                out.at<uchar>(i,j) = 255;
            }
        }
    }

    out.copyTo(output);

}