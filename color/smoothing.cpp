#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){

	int dim;
	
	if( argc < 2){
        cout<<"usage: "<<argv[0]<<" image_name"<<endl;
        exit(0);
    }
    String imageName = argv[1];

	//Lettura immagine    
    Mat image;
    image = imread(imageName, IMREAD_COLOR );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    //Read filter dimension
    cout << "Insert filter dimension:";
    cin >> dim;

    //Gaussian filter
    Mat imgGaussian;
    GaussianBlur(image, imgGaussian, Size(dim, dim), 0, 0);

    Mat imgHSV;
    cvtColor(image, imgHSV, COLOR_BGR2HSV);

    Mat hsv[3];
    split(imgHSV, hsv);
    GaussianBlur(hsv[2], hsv[2], Size(dim, dim), 0, 0);
    merge(hsv, 3, imgHSV);

    Mat imgBGR;
    cvtColor(imgHSV, imgBGR, COLOR_HSV2BGR);

    imshow("Original image", image);
    imshow("GaussianBlur image", imgGaussian);
    imshow("Guassian Vimage", imgBGR);

    waitKey(0);
    destroyAllWindows();
    return 0;
}