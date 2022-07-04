#include "../otsu.h"

int otsu(vector<double> histogram);

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
    Mat blur;
    GaussianBlur(image, blur, Size(3, 3), 0);

    vector <double> hist = normalizedHistogram(blur);

    Mat output;
    threshold(blur, output, otsu(hist), 255, THRESH_BINARY);
    imshow("Otsu image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

int otsu(vector<double> his){
    double avgCum = 0.0f;
    for(int i = 0; i < 256; i++){
        avgCum += i * his[i];
    }
    double prob = 0.0f;
    double currAvgCum = 0.0f;
    double currVariance = 0.0f;
    double maxVariance = 0.0f;
    int threshold;

    for(int i = 0; i < 256; i++){
        prob += his[i];
        currAvgCum += i * his[i];
        currVariance = pow(avgCum * prob - currAvgCum, 2) / (prob *(1-prob));
        if(currVariance > maxVariance){
            maxVariance = currVariance;
            threshold = i;
        }
    }

    return threshold;

}