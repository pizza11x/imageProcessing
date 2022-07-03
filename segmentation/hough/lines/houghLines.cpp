#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void houghLines(Mat &src, Mat &output, int threshold);

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
    houghLines(image, output, 200);

    imshow("Output image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

void houghLines(Mat &src, Mat &output, int threshold){
    Mat gaussian;
    //Let's do some smoothing on the iage to reduce the noise
    GaussianBlur(src, gaussian, Size(7,7), 2,2);

    //First we apply Canny to get the image of the segmantation and then all the edges
    Mat canny;
    Canny(gaussian, canny, 50, 150, 3);

    double theta, rho;
    int x, y;

    /* 
        Calculate the maximum possible distance between two points given a number of rows
        and columns.
        It would basically be the hypotenuse
    */
    int dist = hypot(src.rows, src.cols);
    /*
        We initialize the "votes" matrix.
        On the lines we will have rho, and on the columns tetha
    */
    Mat votes = Mat::zeros(dist * 2, 180, CV_8U);

    //Scan every single pixel
    for( x = 0; x < src.rows; x++){
        for( y = 0; y < src.cols; y++){
            //Let's only consider the points of edge
            if(canny.at<uchar>(x, y) == 255){
                //Vote for the sinusoid of thet edge point
                for( theta = 0; theta < 180; theta++){
                    rho = y* cos((theta - 90) * CV_PI / 180) + x * sin((theta - 90) * CV_PI / 180);
                    votes.at<uchar>(rho,theta)++;
                }
            }
        }
    }

    //Analyze the voting phase, and check which cells pass the th
    for(int r = 0; r < votes.rows; r++){
        for(int t = 0; t < votes.cols; t++){
            //If the votes are considerable, calculate the angle theta and the x, y coordinates
            if(votes.at<uchar>(r, t) >= threshold){
                //Convert from degrees to radians
                theta = (t - 90) * CV_PI / 180;

                //compute the point which are the extremes of the line
                double cos_t = cos(theta);
                double sin_t = sin(theta);

                int x = r * cos_t;
                int y = r * sin_t;

                Point pt1(cvRound(x + dist * (-sin_t)), cvRound(y+dist*cos_t));
                Point pt2(cvRound(x - dist * (-sin_t)), cvRound(y-dist*cos_t));
                line(src, pt1, pt2, Scalar(0), 2, 0);
            }
        }
    }

    src.copyTo(output);


}