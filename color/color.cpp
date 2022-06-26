#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
    if( argc < 2){
        cout<<"usage: "<<argv[0]<<" image_name"<< endl;
        exit(0);
    }
    String imageName = argv[1];

	//Read image    
    Mat image;
    image = imread(imageName, IMREAD_COLOR );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

    Mat bgr[3];
    split(image, bgr);

    imshow("Orginal image", image);
    imshow("BGR_Blue image", bgr[0]);
    imshow("BGR_Green image", bgr[1]);
    imshow("BGR_Red image", bgr[2]);
    waitKey(0);
    destroyAllWindows();

    Mat imgBGR;
    merge(bgr, 3, imgBGR);
    Mat imgRGB;
    cvtColor(imgBGR, imgRGB, COLOR_BGR2RGB);
    Mat rgb[3];
    split(imgRGB, rgb);

    imshow("Orginal image", image);
    imshow("RGBlue image", rgb[0]);
    imshow("RGBGreen image", rgb[1]);
    imshow("RGB_Red image", rgb[2]);
    waitKey(0);
    destroyAllWindows();

    Mat imgHSV;
    cvtColor(imgRGB, imgHSV, COLOR_RGB2HSV);

    Mat hsv[3];
    split(imgHSV, hsv);
    imshow("Orginal image", image);
    imshow("Hue image", hsv[0]);
    imshow("Stauration image", hsv[1]);
    imshow("Value image", hsv[2]);

    waitKey(0);
    destroyAllWindows();

    return 0;
}