/*******************************************
   Author        : Jun Zhang
   Email         : ewalker.zj@gmail.com
   Last Modified : 2019-11-05 11:33
   Filename      : main.cc
   Description   : 
*******************************************/

#include <iostream>
#include "blurpre.h"
#include <chrono>

int main() {

    cv::Mat img = cv::imread("../dst.jpg");

    auto begin = std::chrono::high_resolution_clock::now();
    int status = blurpre(img); 
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "status: " << status << ", cost: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;

    return 0;
}
