#include "VideoTexture.hpp"

#ifndef OPENCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#define OPENCV
#endif

VideoTexture::VideoTexture() {
  glGenTextures(1, &m_RendererID);
//  std::cout << "(cosntructor) VideoTexture ID: " << m_RendererID << "\n";
}

VideoTexture::~VideoTexture() {}

void VideoTexture::UpdateTexture(cv::Mat &frame) {

  if (frame.empty()) {
    std::cout << "Empty frame."
              << "\n";
  } else {
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
    cv::flip(frame, frame, 0);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, frame.cols, frame.rows, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, frame.ptr());
  }
}