#pragma once
#ifndef OPENCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#define OPENCV
#endif


class VideoTexture {
  unsigned int m_RendererID;

public:
  VideoTexture();
  ~VideoTexture();
  void UpdateTexture(cv::Mat &frame);
  unsigned int GetID() { return m_RendererID; }
};