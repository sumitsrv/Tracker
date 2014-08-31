TEMPLATE = app
TARGET = Tracker

RESOURCES += \
    test.qrc

SOURCES += \
    main.cpp

QT += quick svg xml qml opengl declarative sensors

#unix: PKGCONFIG += opencv

TARGET = $$qtLibraryTarget($$TARGET)
#uri = opencv.components

#INCLUDEPATH += /usr/local/include/
#LIBS += /usr/local/lib/*.so.2.4.8

#HEADERS += \
#    cvimagewidget.h \
#    glg.h

#CONFIG += console

OTHER_FILES += \
    android/AndroidManifest.xml

contains(ANDROID_TARGET_ARCH,armeabi) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libnative_camera_r4.1.1.so
    INCLUDEPATH += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    LIBS += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java.so \
$$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libnative_camera_r4.4.0.so

    INCLUDEPATH += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/ \
$$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/opencv \
$$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/opencv2

}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
