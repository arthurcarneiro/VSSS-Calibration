#ifndef GET_POINTS_H
#define GET_POINTS_H

#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class get_points
{
public:
    get_points();
    static void mouseHandleStatic( int event, int x, int y, int flags, void* that );
    void onMouse( int event, int x, int y, int flags);
    ~get_points();
};


#endif // GET_POINTS_H
