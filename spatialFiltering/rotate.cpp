#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

void printFilter(Mat Filter);

int main(int argc, char **argv){
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    Mat filter = Mat(3,3, CV_32S, data);
    Mat r_filter;

    cout << "Original matrix" << endl;
    printFilter(filter);

    cout << "Matrix rotated 90 degrees clockwise" << endl;
    rotate(filter, r_filter, ROTATE_90_CLOCKWISE);
    printFilter(r_filter);

    cout << "Matrix rotated 180 degrees" << endl;
    rotate(filter, r_filter, ROTATE_180);
    printFilter(r_filter);

    cout << "Matrix rotated 90 degrees counterclockwise" << endl;
    rotate(filter, r_filter, ROTATE_90_COUNTERCLOCKWISE);
    printFilter(r_filter);

    return 0;

}

void printFilter(Mat filter){
    for(int i = 0; i<filter.rows; i++){
        for(int j=0; j<filter.cols; j++){
            cout << filter.at<int>(i,j) << " ";
        }
        cout << endl;
    } 
    cout << endl;
}