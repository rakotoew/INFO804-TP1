#include <qapplication.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Viewer.h"
#include "Utils.h"
using namespace std;
int main(int argc, char** argv)
{
    TriangleSoup soup;
    std::ifstream in(argv[1]);
    soup.read(in);
    // Read command lines arguments.
    QApplication application(argc,argv);
    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Viewer triangle soup");
    // Make the viewer window visible on screen.
    viewer.ptrSoup = &soup;
    viewer.show();
    // Run main loop.
    application.exec();

    return 0;
}
