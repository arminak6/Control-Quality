#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat image, original_image, mask;
Point center(-1, -1);  
int radius = -1;

void crop_circle();

void mouse_callback(int event, int x, int y, int flags, void* userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        if (center.x == -1 && center.y == -1) {
            center = Point(x, y);
            cout<<center<<endl;
            circle(image, center, 5, Scalar(0, 255, 0), -1); 
            imshow("Select Center and Radius", image);
        } else if (radius == -1) {
            int dx = x - center.x;
            int dy = y - center.y;
            radius = static_cast<int>(sqrt(dx * dx + dy * dy));
            cout<<"\n"<<radius<<endl;
            circle(image, center, radius, Scalar(0, 255, 0), 2);  
            imshow("Select Center and Radius", image);
            crop_circle();
        }
    }
}

void crop_circle() {
    mask = Mat::zeros(original_image.size(), original_image.type());
    circle(mask, center, radius, Scalar(255, 255, 255), -1);

    Mat circular_crop;
    bitwise_and(original_image, mask, circular_crop);

    Rect bounding_box(center.x - radius, center.y - radius, radius * 2, radius * 2);
    Mat cropped_img = circular_crop(bounding_box);

    imshow("Cropped Circular Region", cropped_img);
}

int main(int argc, char* argv[]) {
    image = imread(argv[1]); 
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    original_image = image.clone();

    namedWindow("Select Center and Radius", WINDOW_NORMAL);
    setMouseCallback("Select Center and Radius", mouse_callback);

    imshow("Select Center and Radius", image);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
