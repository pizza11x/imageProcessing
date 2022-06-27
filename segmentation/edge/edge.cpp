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

    // With Sobel's filters I get the x and y components of the gradient vector.
    //Dx e Dy sono le componenti per ogni pixel del vettore gradiente.
    Mat Dx;
    Sobel(imgGaussian, Dx, CV_32FC1, 0,1);

    Mat Dy;
    Sobel(imgGaussian, Dy, CV_32FC1, 1, 0);

    //Calculate magnitude
    Mat magnitude = abs(Dx)+abs(Dy);
    normalize(magnitude, magnitude, 0, 255, NORM_MINMAX, CV_8U);
    
    Mat orientations;

    /* Calculate the arctangent of the gradient vectors in each position. 
    The directions of the gradient vector at each position are calculated, then the angle. */
    phase(Dx, Dy, orientations, true);

    /*Phase, e quindi l'arcotangente, restituisce valori a -pigreco/2 e +pigreco/2, 
    ma con la funzione "normalize" li riportiamo nell'intervallo 0-255. */
    normalize(orientations, orientations, 0, 255, NORM_MINMAX, CV_8U);

    imshow("Original image", image);
    imshow("Magnitudo", magnitude);
    imshow("Orientations", orientations);
    waitKey(0);
    destroyAllWindows();


    double th;
    cout<<"Insert threshold value: ";
    cin >> th; //75

    // Used to threshold the magnitude and get a cleaner image.
    Mat Th;
    threshold(magnitude, Th, th, 255, CV_THRESH_BINARY);

    imshow("Magnitudo threshold", Th);
    waitKey(0);
    destroyAllWindows();

    return 0;
}