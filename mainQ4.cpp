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
    if(argc<6){
        cout << "Nombre de parametre imcorrect\n";
        exit(-1);
    }
    // Read command lines arguments.
    QApplication application(argc,argv);
    // Instantiate the viewer.
    Viewer viewer;
    // Give a name
    viewer.setWindowTitle("Viewer triangle soup");
    // Make the viewer window visible on screen.
    ifstream input( argv[1]); // input est un flux en entrée.
    if ( ! input.good() ) std::cerr << "ERROR" << std::endl;
    TriangleSoup ts;
    ts.read(input);
    //sauvergarde des triangles avant compression
    std::vector<Triangle> inputTriangle =  ts.triangles;
    TriangleSoup anOutput;
    TriangleSoupZipper tsz = TriangleSoupZipper(ts, anOutput, Index(atoi(argv[3]), atoi(argv[4]), atoi(argv[5])));
    tsz.advancedZip();
    std::ofstream output( argv[2]);
    ts.triangles = tsz._anOutput.triangles;
    ts.write(output);

    viewer.ptrSoup = &tsz._anOutput;
    std::cout << "Triangles apres compression :  " <<  tsz._anOutput.triangles.size() << std::endl;
    std::cout << "Taux de compression : " <<   tsz._anOutput.triangles.size()*100 / inputTriangle.size()<< "%"<<endl;
    viewer.show();
    // Run main loop.
    application.exec();
    input.close(); // à la fin
    return 0;
}
