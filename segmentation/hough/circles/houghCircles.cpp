#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void houghCircles(Mat &src, Mat &output, int threshold);

int main(int argc, char **argv){
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
    houghCircles(image, output, 200);
    imshow("Output image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

void houghCircles(Mat &src, Mat &output, int threshold){
    //Smoothing
    GaussianBlur(src, src, Size(3,3), 0, 0);

    Mat imageCanny;
    Canny(src, imageCanny, 70, 180, 3);

    //Maximum ranges of rays
    int rMin = 40;
    int rMax = 80;

    //3D granding matrix
    int sizes[] = {imageCanny.rows, imageCanny.cols, rMax - rMin+1};
    
    //Create the 3D voting space of all 0's
    Mat votes = Mat(3, sizes, CV_8U, Scalar(0));

    //Canny's image scan
    for(int x = 0; x < imageCanny.rows; x++){
        for(int y = 0; y < imageCanny.cols; y++){
            if(imageCanny.at<uchar>(x, y) == 255){
                //Edge pixels will vote for each radius from rMin to rMax
                for(int r = rMin; r <= rMax; r++){
                    for( int theta = 0; theta < 360; theta++){
                        //Calculate the coordinates of the center(a, b)
                        int a = y - r * cos(theta * CV_PI / 180);
                        int b = x - r * sin(theta * CV_PI / 180);
                        //Check if the coordinates of the center are internal to the image
                        if(a >= 0 && a <imageCanny.cols && b >= 0 && b < imageCanny.rows){
                            /*
                                The grades go in the range rMax - rMin,
                                so subtract r - rMin to bring it back into the range
                            */
                            votes.at<uchar>(b, a, r-rMin)++;
                        }

                    }
                }
            }
        }
    }

    //Analyze the voting space
    //Loop for each ray
    for(int r = rMin; r < rMax; r++){
        //Loop on the dimensions of the voting space (a and b)
        for(int b = 0; b < imageCanny.rows; b++){
            for(int a = 0; a < imageCanny.cols; a++){
                //Check if the cell has a number of values greater than threshold
                if( votes.at<uchar>(b, a, r-rMin) >= threshold){
                    //Small circle to display the center
                    circle(src, Point(a, b), 3, Scalar(0), 2, 8, 0);
                    //Larger circle with the center (a, b) and radius r
                    circle(src, Point(a, b), r, Scalar(0), 2, 8, 0);

                }
            }
        }
    }

    src.copyTo(output);

}