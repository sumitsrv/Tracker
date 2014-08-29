RESOURCES += \
    test.qrc

SOURCES += \
    main.cpp \
    glg.cpp

QT += quick sensors svg xml qml opengl

TEMPLATE = app

INCLUDEPATH += /usr/local/include/
LIBS += /usr/local/lib/*.so.2.4.8

HEADERS += \
    cvimagewidget.h \
    glg.h

CONFIG += console

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

contains(ANDROID_TARGET_ARCH,armeabi) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libnative_camera_r4.1.1.so
    INCLUDEPATH += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/
}

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        /home/sumit/test/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libnative_camera_r4.4.0.so \
        $$ANDROID_PACKAGE_SOURCE_DIR/libs/armeabi-v7a/libnative_camera_r2.2.0.so
    LIBS += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/* \
         $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a/*
    INCLUDEPATH += $$PWD/../../../mnt/Signi/Softwares/Linux/OpenCV-2.4.9-android-sdk/sdk/native/jni/include/
}
