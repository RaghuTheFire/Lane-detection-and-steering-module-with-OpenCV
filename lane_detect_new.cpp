#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cmath>

int main() 
{
  cv::VideoCapture cap("drive.mp4");

  while (true) 
  {
    cv::Mat frame;
    bool ret = cap.read(frame);
    if (!ret) 
    {
      break;
    }

    int width = frame.cols;
    int height = frame.rows;

    // ------------
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Mat lower(1, 1, CV_8UC3, cv::Scalar(180, 18, 255));
    cv::Mat upper(1, 1, CV_8UC3, cv::Scalar(0, 0, 231));

    cv::Mat mask;
    cv::inRange(hsv, lower, upper, mask);
    cv::Mat edges;
    cv::Canny(mask, edges, 75, 250);

    cv::imshow("mask", edges);

    std::vector < cv::Vec4i > lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50);

    for (const auto & line: lines) 
    {
      int x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];
      cv::line(frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }
    // ----------

    // change perspective
    // perspective points
    std::vector < cv::Point2f > pts1 = 
    {
      {
        560,
        440
      },
      {
        710,
        440
      },
      {
        200,
        640
      },
      {
        1000,
        640
      }
    };
    std::vector < cv::Point2f > pts2 = 
    {
      {
        0,
        0
      },
      {
        width,
        0
      },
      {
        0,
        height
      },
      {
        width,
        height
      }
    };
    // end perspective points

    // perspective points draw circle
    std::vector < cv::Point2i > pts1_ = 
    {
      {
        560,
        440
      },
      {
        710,
        440
      },
      {
        200,
        640
      },
      {
        1000,
        640
      }
    };
    std::vector < cv::Point2i > pts2_ = 
    {
      {
        0,
        0
      },
      {
        width,
        0
      },
      {
        0,
        height
      },
      {
        width,
        height
      }
    };

    // for (int i = 0; i < 4; i++) {
    //     cv::circle(frame, pts1_[i], 5, cv::Scalar(0, 0, 255), 2);
    // }
    // end perspective points draw circle

    // change perspective
    cv::Mat matrix = cv::getPerspectiveTransform(pts1, pts2);
    cv::Mat birds_eye;
    cv::warpPerspective(frame, birds_eye, matrix, cv::Size(width, height));

    // end change birds eye perspective

    cv::Mat grayscale;
    cv::cvtColor(birds_eye, grayscale, cv::COLOR_BGR2GRAY);

    // smooth image
    cv::Mat blur;
    cv::GaussianBlur(grayscale, blur, cv::Size(5, 5), 0);

    // canny edge dedection
    cv::Mat edges2;
    cv::Canny(blur, edges2, 50, 95);

    // detect line with houg line transform
    std::vector < cv::Vec4i > lines2;
    cv::HoughLinesP(edges2, lines2, 1, CV_PI / 180, 50, 50);

    // draw lines
    int x1_left = 0, x2_left = 0, y1_left = 0, y2_left = 0;
    int x1_right = 0, x2_right = 0, y1_right = 0, y2_right = 0;

    for (const auto & line: lines2) 
    {
      int x1 = line[0], y1 = line[1], x2 = line[2], y2 = line[3];

      // classification right and left line
      if (x1 < 640 || x2 < 640) 
      {
        x1_left = x1;
        x2_left = x2;
        y1_left = y1;
        y2_left = y2;
      } 
      else 
      if (x1 > 640 || x2 > 640) 
      {
        x1_right = x1;
        x2_right = x2;
        y1_right = y1;
        y2_right = y2;
      }

      // lane draw line red
      cv::line(birds_eye, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2);
    }

    try 
    {
      // calculate middle points
      int x1_mid = (x1_right + x1_left) / 2;
      int x2_mid = (x2_right + x2_left) / 2;

      cv::line(birds_eye, cv::Point(640, 300), cv::Point(x2_mid, 420), cv::Scalar(0, 255, 0), 2);

      // create straight pipe line in middle of the frame
      int x_1 = 640, x_2 = 640;
      int y_1 = 300, y_2 = 420;
      cv::line(birds_eye, cv::Point(x_1, y_1), cv::Point(x_2, y_2), cv::Scalar(0, 0, 255), 2);

      // calculate 3 point beetween angle
      std::vector < int > point_1 = 
      {
        x_1,
        y_1
      };
      std::vector < int > point_2 = 
      {
        x_2,
        y_2
      };
      std::vector < int > point_3 = 
      {
        x2_mid,
        420
      };

      double radian = std::atan2(point_2[1] - point_1[1], point_2[0] - point_1[0]) - std::atan2(point_3[1] - point_1[1], point_3[0] - point_1[0]);
      double angle = radian * 180 / CV_PI;

      std::cout << "omega : " << angle << std::endl;

      // cv::putText(frame, std::to_string(static_cast<int>(angle)), cv::Point(15, 35), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);

      if (angle < -30) 
      {
        cv::putText(frame, "LEFT", cv::Point(15, 35), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
      } 
      else 
      if (angle > 25) 
      {
        cv::putText(frame, "RIGHT", cv::Point(1150, 35), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
      } else if (angle > -25 && angle < 25) 
      {
        // cv::putText(frame, "DUZ", cv::Point(600, 35), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
        // continue;
      }
    } 
    catch (const std::exception & e) 
    {
      // Handle the exception
    }

    std::vector < std::vector < cv::Point >> drawing_pts = 
    {
      {
        {
          550,
          440
        }, {
          690,
          440
        }, {
          1000,
          640
        }, {
          200,
          640
        }
      }
    };
    cv::polylines(frame, drawing_pts, true, cv::Scalar(0, 255), 3);
    // cv::fillPoly(frame, drawing_pts, cv::Scalar(0, 255, 0));

    cv::resize(birds_eye, birds_eye, cv::Size(640, 360));

    cv::imshow("birds_eye", birds_eye);
    cv::imshow("frame", frame);

    if ((cv::waitKey(30) & 0xFF) == 'q') 
    {
      break;
    }
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
