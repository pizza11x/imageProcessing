#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
    if(argc < 2){
        cout<<"Usage: "<<argv[0]<<" image_name"<<endl;
        exit(-1);
    }
    String imageName = argv[1];

    //Read image
    Mat image;
    image = imread(samples::findFile(imageName), IMREAD_GRAYSCALE);
    if(image.empty()){
        cout << "Could not open or find the image"<<endl;
        exit(-1);
    }

    Mat imgLaplacian;
    Laplacian(image, imgLaplacian, image.type());

    Mat imgSobel;
    Sobel(image, imgSobel, image.type(), 1,1);

    Mat imgSobelX;
    Sobel(image, imgSobelX, image.type(), 0,1);

    Mat imgSobelY;
    Sobel(image, imgSobelY, image.type(), 1,0);

    imshow("Original image", image);
    imshow("Laplacian image", imgLaplacian);
    imshow("Sobel image", imgSobel);
    imshow("SobelX image", imgSobelX);
    imshow("SobelY image", imgSobelY);

    waitKey(0);
    return 0;
}