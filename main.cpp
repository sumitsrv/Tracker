#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QQuickView>
#include <QDialog>
#include <QApplication>
#include <QMainWindow>
#include <QDebug>

#include<stdlib.h>
#include<ctype.h>
#include<iostream>
#include<vector>

#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/features2d/features2d.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#ifndef ANDROID
    #include "opencv2/nonfree/nonfree.hpp"
#endif
#include <opencv2/ml/ml.hpp>

//#include <cvimagewidget.h>
//#include <glg.h>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QObject::connect(view.engine(), SIGNAL(quit()), qApp, SLOT(quit()));
    view.setSource(QUrl("qrc:///test.qml"));
    view.resize(800, 480);
    view.show();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl("qrc:///test.qml"));

//    QMainWindow sceneWindow;
//    QMainWindow objectWindow;

//    CVImageWidget* sceneWidget = new CVImageWidget();
//    sceneWindow.setCentralWidget(sceneWidget);

//    CVImageWidget* objectWidget = new CVImageWidget();
//    objectWindow.setCentralWidget(objectWidget);

    VideoCapture capture(0);
    Mat img;

//    capture >> img;

//    capture.open(CV_CAP_ANDROID_FRONT);
    capture.grab();
    capture.retrieve(img, CV_CAP_ANDROID_COLOR_FRAME_RGBA);

//    img = cvRetrieveFrame(capture);

    Mat gray;
    Mat prev;
    Mat diff;
    cvtColor(img, prev, CV_BGR2GRAY);

    //    IplImage *compImage = cvCreateImage(cvSize(sz.width, sz.height), img->depth, 1);
    //    cvSet(compImage, cvScalarAll(255));
    //    cvSub(compImage, maskImageOrig, maskImage);
//    Mat Object;
    Mat object = imread("/mnt/Signi/tracker/remote3.jpeg" , CV_LOAD_IMAGE_GRAYSCALE );
    //        GLG contrast;
    //        contrast.glg(object);
    //        SIFT detector(100,3,0.04,10,1.6);
    //    detector.operator()(Mat(object) , noArray(), keypts, descrpts, false);

#ifdef ANDROID
    ORB features(500,1.2f, 8, 31, 0, 2, ORB::HARRIS_SCORE, 31 );
#else
    SURF features(600, 3, 2, true, false);
#endif
    vector<KeyPoint> keypts;
    Mat descrpts;
    features.operator ()(object, noArray(), keypts, descrpts, false);

    FlannBasedMatcher matcher(new flann::CompositeIndexParams(5, 32, 11, cvflann::FLANN_CENTERS_RANDOM, 0.2), new flann::SearchParams(32));
    std::vector<cv::Mat> descriptor_vector;

    descriptor_vector.push_back(descrpts);
    matcher.add(descriptor_vector);
    matcher.train();

//    objectWindow.show();
//    sceneWindow.show();

    int k = 2;
    //    objectWidget->showImage(maskImage);

    while(cvWaitKey(10)!= 32){
        vector<KeyPoint> graykeypts;
        Mat graydescrpts;

        capture.grab();
        capture.retrieve(img, CV_CAP_ANDROID_COLOR_FRAME_RGBA);

        cvtColor(img, gray, CV_BGR2GRAY, 1);
        subtract(prev, gray, diff, noArray(), -1);
        prev = gray.clone();

        //        contrast.glg(gray);
        //        surf.operator ()(Mat(gray), Mat(maskImage), graykeypts, graydescrpts, false);
        features.operator ()(gray, noArray(), graykeypts, graydescrpts, false);
        //        detector.operator()(Mat(gray) , noArray(), graykeypts, graydescrpts, false);

        vector<vector<DMatch> > matches;
        vector<Mat> mask;
        matcher.knnMatch(graydescrpts, matches, k, mask, false);
        //        flannIndex.knnSearch(graydescrpts, results, dists, 2, cv::flann::SearchParams(32));
        //        matcher.radiusMatch(graydescrpts, descrpts, matches, 0.5, Mat());
        //        matcher.match(descrpts, graydescrpts, matches);
        //        qDebug() << keypts.size() << "\t" << matches.size() << "\n";

        double max_dist = 0; double min_dist = 100;

        for( int i = 0; i < graydescrpts.rows; i++ )
        {
            vector<DMatch> match = matches[i];
            for(int j = 0; j<k; j++){
                double dist = match[j].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }
        }

        vector<DMatch> good_matches;

        for( int i = 0; i < graydescrpts.rows; i++ )
        {
            vector<DMatch> match = matches[i];
            for(int j=0; j<k; j++){
                if( match[j].distance <= max(2*min_dist, 0.02))
                {
                    good_matches.push_back(match[j]);
                }
            }
        }

        Mat img_matches = img.clone();
        drawKeypoints(gray, graykeypts, img_matches);
        //        drawMatches(gray, graykeypts, object, keypts, good_matches, img_matches);

        std::vector<Point2f> obj;
        std::vector<Point2f> scene;

        for( unsigned int i = 0; i < good_matches.size(); i++ )
        {
            //-- Get the keypoints from the good matches
            obj.push_back( keypts[ good_matches[i].trainIdx ].pt );
            scene.push_back( graykeypts[ good_matches[i].queryIdx ].pt );
        }

        std::vector<uchar> outlier_mask;
        //        qDebug() << "matches-------" << good_matches.size() << "\t" << matches.size();
        //        qDebug() << "objets----------" << obj.size() << "\t" << scene.size();

        unsigned int min_matches = 8;
        if(obj.size() >= min_matches && scene.size() >= min_matches){
            Mat H = findHomography( obj, scene, CV_RANSAC, 1.0, outlier_mask );

            //-- Get the corners from the image_1 ( the object to be "detected" )
            std::vector<Point2f> obj_corners(4);
            obj_corners[0] = cvPoint(0,0);
            obj_corners[1] = cvPoint( Mat(object).cols, 0 );
            obj_corners[2] = cvPoint( Mat(object).cols, Mat(object).rows );
            obj_corners[3] = cvPoint( 0, Mat(object).rows );
            std::vector<Point2f> scene_corners(4);

            perspectiveTransform(obj_corners, scene_corners, H);

            //-- Draw lines between the corners (the mapped object in the scene - image_2 )
            line( img_matches, scene_corners[0], scene_corners[1], Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1], scene_corners[2], Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2], scene_corners[3], Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3], scene_corners[0], Scalar( 0, 255, 0), 4 );
        }
//        sceneWidget->showImage(img_matches);

    }

    capture.release();
    img.release();
    gray.release();
    prev.release();
    diff.release();

    return app.exec();
}
