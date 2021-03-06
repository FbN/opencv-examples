#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

cv::Mat image;
int thresh1 = 100;
int max_thresh1 = 10000;

int thresh2 = 5000;
int max_thresh2 = 10000;

int int_ratio = 9;
double max_ratio = 40;

int gauKsize = 11;

char* source_window = "Source image";

void filterHarris( int, void* );

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
  cv::createTrackbar( "Threshold 1: ", source_window, &thresh1, max_thresh1, filterHarris );
  cv::createTrackbar( "Threshold 2: ", source_window, &thresh2, max_thresh2, filterHarris );
  cv::createTrackbar( "Ratio: ", source_window, &int_ratio, max_ratio, filterHarris );
  cv::imshow( source_window, image );

  filterHarris(0, 0);

  cv::waitKey(0);
  return EXIT_SUCCESS;
}

void filterHarris( int, void* )
{
  cv::Mat dX, dY, dX2, dY2, dXY, response, CrCbImage;

  cvtColor(image, CrCbImage, CV_RGB2YCrCb);

  cv::Sobel(CrCbImage, dX, CV_32F, 1, 0);
  cv::Sobel(CrCbImage, dY, CV_32F, 0, 1);

  // Gaussian kernel with ksize = gauKsize and sigma = -1 (autocomputed internally)
  cv::Mat gau = cv::getGaussianKernel(gauKsize, -1, CV_32F);

  cv::sepFilter2D(dX.mul(dX), dX2, CV_32F, gau.t(), gau);
  cv::sepFilter2D(dY.mul(dY), dY2, CV_32F, gau.t(), gau);
  cv::sepFilter2D(dX.mul(dY), dXY, CV_32F, gau.t(), gau);

  double ratio = (double) int_ratio;
  double k = ratio / ((ratio + 1) * (ratio + 1));

  cv::Mat Det = dX2.mul(dY2) - dXY.mul(dXY);
  cv::Mat Tr  = dX2 + dY2;
  response = Det - k * Tr.mul(Tr);

  cv::Mat dst = image.clone();
  
  std::cout << -thresh1 << " : " << -thresh2 << " : " << ratio << std::endl;

  for( int j = 0; j < response.rows ; j++ ) { 
    for( int i = 0; i < response.cols; i++ ) {
      cv::Vec3f pixel = response.at<cv::Vec3f>(j, i);

      if( pixel[1] < -thresh1 && pixel[2] < -thresh2 ) {
        circle( dst, cv::Point( i, j ), 1, cv::Scalar(0, 0, 255), 1, 8, 0 );
      }
    }
  }
  
  // cv::resize(dst, dst, cv::Size(round(700 * dst.cols/dst.rows), 700));

  cv::namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  cv::imshow( source_window, dst );
}

