#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
    int dim;
	
	if( argc < 2){
        cout<<"usage: "<<argv[0]<<" image_name"<<endl;
        exit(0);
    }
    String imageName = argv[1];

	//Read image    
    Mat image;
    image = imread(imageName, IMREAD_GRAYSCALE );
    if( image.empty() ){
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

    //Read filter dimension
    cout <<"Insert filter dimension: ";
    cin>>dim; //5

    //Gaussian filter
    Mat imgGaussian;
    GaussianBlur(image, imgGaussian, Size(dim, dim), 0,0);

    Mat Dx;
    Sobel(imgGaussian, Dx, CV_32FC1, 0,1);

    Mat Dy;
    Sobel(imgGaussian, Dy, CV_32FC1, 1, 0);

    Mat magnitudo = abs(Dx)+abs(Dy);
    normalize(magnitudo, magnitudo, 0, 255, NORM_MINMAX, CV_8U);
    
    Mat orientations;
    phase(Dx, Dy, orientations, true);
    normalize(orientations, orientations, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Original image", image);
    imshow("Magnitudo", magnitudo);
    imshow("Orientations", orientations);
    waitKey(0);
    destroyAllWindows();


    double th;
    cout<<"Insert threshold value: ";
    cin >> th; //75

    Mat Th;
    threshold(magnitudo, Th, th, 255, CV_THRESH_BINARY);

    imshow("Magnitudo threshold", Th);
    waitKey(0);
    destroyAllWindows();

    return 0;
}