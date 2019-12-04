#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
// #include <opencv2/video/tracking.hpp>
#include <opencv2/core/ocl.hpp>
//#include <opencv2/optflow/motempl.hpp>

using namespace cv;
using namespace std;
// using namespace Tracker;

// Convert to string
#define SSTR(x) static_cast<std::ostringstream &>(           \
                    (std::ostringstream() << std::dec << x)) \
                    .str()

int main(int argc, char **argv)
{
    // List of tracker types in OpenCV 3.2
    // NOTE : GOTURN implementation is buggy and does not work.
    string trackerTypes[6] = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN"};
    // vector <string> trackerTypes(types, std::end(types));
    // Create a tracker
    string trackerType = trackerTypes[2];
    Ptr<Tracker> tracker;

#if (CV_MINOR_VERSION < 3)
    {
        tracker = Tracker::create(trackerType);
    }
#else
    {
        if (trackerType == "BOOSTING")
            tracker = TrackerBoosting::create();
        if (trackerType == "MIL")
            tracker = TrackerMIL::create();
        if (trackerType == "KCF")
            tracker = TrackerKCF::create();
        if (trackerType == "TLD")
            tracker = TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            tracker = TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            tracker = TrackerGOTURN::create();
    }
#endif
    // Read video
    VideoCapture video("rtsp://admin:oeasy808@192.168.9.3:554/h264/ch1/main/av_stream");
    VideoWriter writer("jieguo2.avi",CV_FOURCC('h', '2', '6', '4'),25.0,Size(1280,720));

    // Exit if video is not opened
    if (!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }

    // Read first frame
    Mat frame;
   

    // Define initial boundibg box
    Rect2d bbox;

    // Uncomment the line below to select a different bounding box
    // bbox = selectROI(frame, false);

    // Display bounding box.
    // rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    //imshow("Tracking", frame);

    tracker->init(frame, bbox);

    while (video.read(frame))
    {
        
        resize(frame,frame,Size(1280,720));
        Mat select=frame;
        // Start timer
        double timer = (double)getTickCount();

        // Update the tracking result
        bool ok = tracker->update(frame, bbox);

        // Calculate Frames per second (FPS)
        float fps = getTickFrequency() / ((double)getTickCount() - timer);

        if (ok)
        {
            Mat qqq;
            //cvSegmentMotion()
            // Tracking success : Draw the tracked object
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            // Tracking failure detected.
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }

        // Display tracker type on frame
        putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display FPS on frame
        putText(frame, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        putText(frame, "new bbox " , Point(100, 100), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);

        // Display frame.
        imshow("Tracking", frame);
        writer<< frame;
        // Exit if ESC pressed.
        int k = waitKey(1);
        if (char(k) == '0')
        {
            //~tracker();
            bbox = selectROI(select, false);
            try
            {
                tracker = TrackerGOTURN::create();
                tracker->init(frame, bbox);
            }
            catch (const std::exception &e)
            {
                printf("nooooooooooooooooooooooooooooooooooooo!!!!");
            }
            continue;
        }

        if (k == 27)
        {
            break;
        }
    }
}