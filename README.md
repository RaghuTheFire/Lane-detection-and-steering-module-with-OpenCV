# Lane-detection-and-steering-module-with-OpenCV
# lane_detect_new.cpp
This C++ code is using the OpenCV library to process a video file and detect lane lines on the road. 
Here's a breakdown of what the code is doing: 
1. The code starts by including the necessary OpenCV header files and the `cmath` library.
2. The `main()` function is the entry point of the program.
3. It opens a video capture object using the file "drive.mp4".
4. The code then enters a loop that processes each frame of the video.
5. Inside the loop, the code reads the current frame from the video capture object.
6. It converts the frame from the BGR color space to the HSV color space and applies a color range filter to detect the lane lines.
7. The code then uses the Canny edge detection algorithm to find the edges in the filtered image.
8. Next, it applies the Hough line transform to detect the lines in the edge image.
9. The detected lines are then drawn on the original frame.
10. The code then changes the perspective of the frame to a "bird's eye" view, which is a top-down view of the road.
11. It applies Gaussian blur to the bird's eye view image and then uses Canny edge detection to find the edges.
12. The Hough line transform is applied again to detect the lines in the bird's eye view image.
13. The code then classifies the detected lines as either left or right lane lines and calculates the angle between the middle line and the detected lines.
14. Based on the calculated angle, the code displays a "LEFT" or "RIGHT" message on the original frame.
15. Finally, the code displays the original frame and the bird's eye view frame in separate windows.
16. The loop continues until the user presses the 'q' key to exit the program.
Overall, this code is designed to detect and analyze the lane lines in a video feed, which could be useful for applications such as autonomous vehicle control or driver assistance systems.

# lane_detection_with_steer.cpp
This C++ code is using the OpenCV library to perform image processing on a video file. 
Here's a breakdown of what the code is doing: 
1. The code includes the necessary OpenCV header files.
2. The `read_vid()` function is defined, which creates a `cv::VideoCapture` object and opens the video file "drive.mp4".
3. In the `main()` function, the `read_vid()` function is called to create a `cv::VideoCapture` object named `img`.
4. The code then enters a loop that continues until the user presses the 'q' key or the video ends.
5. Inside the loop, the code reads a frame from the video using `img.read(frame)` and converts it to grayscale using `cv::cvtColor()`.
6. The grayscale image is then processed using the Canny edge detection algorithm to create an edge map.
7. The code then defines two sets of points, `pts1` and `pts2`, which are used to perform a perspective transformation on the frame. This is likely done to create a "bird's-eye" view of the scene.
8. The perspective transformation is applied using `cv::getPerspectiveTransform()` and `cv::warpPerspective()`, and the resulting "bird's-eye" view is stored in the `birds_eye` variable.
9. The "bird's-eye" view is then split into two halves, `birds_eye_r` and `birds_eye_l`, which are likely used for further processing.
10. The code then draws circles on the original frame at the locations of the `pts1` points, and creates a region of interest (ROI) in the frame.
11. The "bird's-eye" view is converted to grayscale, and a threshold is applied to create binary images `thresh`, `thresh_r`, and `thresh_l`.
12. The code then uses the `cv::HoughLinesP()` function to detect lines in the binary images, and draws these lines on the "bird's-eye" view.
13. Finally, the code displays the original frame and the "bird's-eye" view using `cv::imshow()`, and waits for the user to press the 'q' key or for the video to end.
Overall, this code appears to be performing some kind of lane detection or road analysis on a video feed, using OpenCV's image processing capabilities.

