#ifndef GLG_H
#define GLG_H

class GLG{
private:
    void mergeLevels(CvHistogram*, float *, int*, float[], float[]);
    void lookUpTable(float *, float [], float [], int*, float []);
public:
    void glg(IplImage*);
    GLG();
};

#endif // GLG_H
