#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

cv::Mat image;
int thresh1 = 52;
int max_thresh1 = 500;

int thresh2 = 445;
int max_thresh2 = 500;

int gauKsize = 11;

char* source_window = "Source image";

void filterCanny( int, void* );

int main( int argc, char** argv )
{
  if( argc != 2 ) {
    std::cerr << "Usage: " << argv[0] << " <InputImage>" << std::endl;
    return EXIT_FAILURE;
  }

  image = cv::imread( argv[1], 1 );
  if(!image.data) {
    return EXIT_FAILURE;
  }

  cv::namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  cv::createTrackbar( "Threshold 1: ", source_window, &thresh1, max_thresh1, filterCanny );
  cv::createTrackbar( "Threshold 2: ", source_window, &thresh2, max_thresh2, filterCanny );
  cv::imshow( source_window, image );

  filterCanny(0, 0);

  cv::waitKey(0);
  return EXIT_SUCCESS;
}

void filterCanny( int, void* )
{
  cv::Mat canny; 
  cv::Mat dst = image.clone();

  cv::Canny(image, canny, thresh1, thresh2, 3, false );

  std::cout << thresh1 << " : " << thresh2 << std::endl;

  for( int j = 0; j < canny.rows ; j++ ) { 
    for( int i = 0; i < canny.cols; i++ ) {
      uchar pixel = canny.at<uchar>(j, i);

      if( pixel > uchar(10) ) {
        circle( dst, cv::Point( i, j ), 1, cv::Scalar(0, 0, 255), 2, 8, 0 );
      }
    }
  }

  cv::namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  cv::imshow( source_window, dst );
}
