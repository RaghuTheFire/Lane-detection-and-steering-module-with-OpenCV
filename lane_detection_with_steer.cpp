#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

cv::VideoCapture read_vid() 
{
  cv::VideoCapture img("drive.mp4");
  return img;
}

int main() 
{
  cv::VideoCapture img = read_vid();

  while (true) 
  {
    cv::Mat frame;
    bool ret = img.read(frame);

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Mat canny;
    cv::Canny(gray, canny, 170, 250);

    int width = frame.cols;
    int height = frame.rows;

    std::vector < cv::Point2f > pts1 = 
    {
      {
        590,
        440
      },
      {
        690,
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

    cv::Mat matrix = cv::getPerspectiveTransform(pts1, pts2);
    cv::Mat birds_eye;
    cv::warpPerspective(frame, birds_eye, matrix, cv::Size(width, height));

    cv::Mat birds_eye_r = birds_eye(cv::Rect(width / 2, 0, width / 2, height));
    cv::Mat birds_eye_l = birds_eye(cv::Rect(0, 0, width / 2, height));

    for (int i = 0; i < 4; i++) 
    {
      cv::circle(frame, cv::Point(pts1[i].x, pts1[i].y), 5, cv::Scalar(0, 0, 255), 2);
    }

    cv::Mat roi = frame(cv::Rect(350, 400, 550, 200));

    cv::Mat gray_birds_eye, gray_birds_eye_r, gray_birds_eye_l;
    cv::cvtColor(birds_eye, gray_birds_eye, cv::COLOR_BGR2GRAY);
    cv::cvtColor(birds_eye_r, gray_birds_eye_r, cv::COLOR_BGR2GRAY);
    cv::cvtColor(birds_eye_l, gray_birds_eye_l, cv::COLOR_BGR2GRAY);

    cv::Mat thresh, thresh_r, thresh_l;
    cv::threshold(gray_birds_eye, thresh, 170, 255, cv::THRESH_BINARY);
    cv::threshold(gray_birds_eye_r, thresh_r, 170, 255, cv::THRESH_BINARY);
    cv::threshold(gray_birds_eye_l, thresh_l, 170, 255, cv::THRESH_BINARY);

    std::vector < cv::Vec4i > linesP, linesP_r, linesP_l;
    cv::HoughLinesP(thresh, linesP, 1, CV_PI / 180, 50, 50, 10);
    cv::HoughLinesP(thresh_r, linesP_r, 1, CV_PI / 180, 50, 50, 10);
    cv::HoughLinesP(thresh_l, linesP_l, 1, CV_PI / 180, 50, 50, 10);

    for (size_t i = 0; i < linesP_r.size(); i++) 
    {
      cv::Vec4i l = linesP_r[i];
      cv::line(birds_eye, cv::Point(l[0] + width / 2, l[1]), cv::Point(l[2] + width / 2, l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    for (size_t i = 0; i < linesP_l.size(); i++) 
    {
      cv::Vec4i l = linesP_l[i];
      cv::line(birds_eye, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    for (size_t i = 0; i < linesP_r.size(); i++)
    {
      try 
      {
        cv::Vec4i ll = linesP_l[i];
        int x1 = (linesP_r[i][0] + width / 2 + ll[0]) / 2;
        int x2 = (linesP_r[i][2] + width / 2 + ll[2]) / 2;
        cv::line(birds_eye, cv::Point(640, 50), cv::Point(x1, ll[1]), cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
        std::cout << "x1 : " << x1 << " ll : " << ll[1] << std::endl;
      } 
      catch (const std::out_of_range & ) 
      {
        continue;
      }
    }

    std::vector < std::vector < cv::Point >> drawing_pts = {
      {
        {
          590,
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
    cv::polylines(frame, drawing_pts, true, cv::Scalar(0, 255, 0), 1);
    cv::imshow("frame", frame);
    cv::imshow("birdsEye", birds_eye);

    if (!ret || cv::waitKey(60) == 'q') 
    {
      break;
    }
  }

  img.release();
  cv2.destroyAllWindows();
  return 0;
}
