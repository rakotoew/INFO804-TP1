//
// Created by ikanatsu on 2/3/23.
//
#include "Utils.h"

/*
 * Vecteur
 */

Vecteur::Vecteur(float x, float y, float z) {
    xyz[0] = x;
    xyz[1] = y;
    xyz[2] = z;
}

Vecteur::Vecteur() {
    xyz[0] = 0;
    xyz[1] = 0;
    xyz[2] = 0;
}

float Vecteur::operator[](int i) const {
    return xyz[i];
}

float &Vecteur::operator[](int i) {
    return xyz[i];
}

Vecteur Vecteur::operator-(Vecteur other) const {
    return Vecteur((*this)[0] - other[0],
                   (*this)[1] - other[1],
                   (*this)[2] - other[2]
    );
}

std::ostream& operator<<( std::ostream& out, Vecteur v )
{
    out << v[ 0 ] << " " << v[ 1 ] << " " << v[ 2 ];
    return out;
}

std::istream& operator>>( std::istream& in, Vecteur& v )
{
    in >> v[ 0 ] >> v[ 1 ] >> v[ 2 ];
    return in;
}

Vecteur Vecteur::inf(const Vecteur& other) const {
    float x,y,z;
    (xyz[0] < other.xyz[0]) ? x=xyz[0] : x = other.xyz[0];
    (xyz[1] < other.xyz[1]) ? y=xyz[1] : y = other.xyz[1];
    (xyz[2] < other.xyz[2]) ? z=xyz[2] : z = other.xyz[2];
    return Vecteur(x,y,z);
}

Vecteur Vecteur::sup(const Vecteur &other) const {
    float x,y,z;
    (other.xyz[0] < xyz[0]) ? x=xyz[0] : x = other.xyz[0];
    (other.xyz[1] < xyz[1]) ? y=xyz[1] : y = other.xyz[1];
    (other.xyz[2] < xyz[2]) ? z=xyz[2] : z = other.xyz[2];
    return Vecteur(x,y,z);
}

Vecteur Vecteur::cross(const Vecteur &v) const {
    Vecteur vOut = Vecteur(
            (*this)[1] * (v[2]) - (*this)[2] * v[1],
            (*this)[2] * v[0] - (*this)[0] * v[2],
            (*this)[0] * v[1] - (*this)[1] * v[0]
            );
    return vOut;
}

Vecteur Vecteur::normalize() const {
    float norm = sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    return Vecteur(xyz[0]/norm, xyz[1]/norm, xyz[2]/norm);
}

/*
 * Triangle
 */
Triangle::Triangle() { }

Triangle::Triangle(Vecteur s0, Vecteur s1, Vecteur s2)  {
    sommets[0] = s0;
    sommets[1] = s1;
    sommets[2] = s2;

}

Vecteur Triangle::operator[](int i) const {
    return sommets[i];
}

Vecteur &Triangle::operator[](int i) {
    return sommets[i];
}

Vecteur Triangle::normal() const {
    Vecteur v1 = sommets[1] - sommets[0];
    Vecteur v2 = sommets[2] - sommets[0];
    Vecteur vOut = v1.cross(v2);
    vOut.normalize();
    return vOut;
}

/*
 * TriangleSoup
 */

void TriangleSoup::read(std::istream &in) {
    if ( ! in.good() ) {
        std::cerr << "ERROR" << std::endl;
    }else {
        std::string line;
        while (std::getline(in, line)) {
            if ((line.size() > 0) && (line[0] != '#')) {
                std::istringstream inputLine(line);
                float x1, y1, z1, x2, y2, z2, x3, y3, z3;
                inputLine >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> x3 >> y3 >> z3;
                Vecteur v1(x1, y1, z1);
                Vecteur v2(x2, y2, z2);
                Vecteur v3(x3, y3, z3);
                Triangle t(v1, v2, v3);
                TriangleSoup::triangles.push_back(t);
            }
        }
        std::cout << "Read " << TriangleSoup::triangles.size() << " triangles" << std::endl;

    }
}

void TriangleSoup::write(std::ostream &output) {
    for (std::vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        output << (*it)[0] << " " << (*it)[1] << " " << (*it)[2] << std::endl;
    }
}

void TriangleSoup::boundingBox(Vecteur &low, Vecteur &up) {
    low = triangles[0][0];
    up = triangles[0][0];
    for (std::vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
        low = low.inf((*it)[0]);
        low = low.inf((*it)[1]);
        low = low.inf((*it)[2]);
        up = up.sup((*it)[0]);
        up = up.sup((*it)[1]);
        up = up.sup((*it)[2]);
    }
}

/*
 * CellData
 */

void CellData::add(const Vecteur &v) {
    acc.xyz[0] += v.xyz[0];
    acc.xyz[1] += v.xyz[1];
    acc.xyz[2] += v.xyz[2];
    nb++;
}

Vecteur CellData::barycenter() const {
    return Vecteur(acc[0] / nb,
                   acc[1] / nb,
                   acc[2] / nb
                   );
}

/*
 * TriangleSoupZipper
 */

TriangleSoupZipper::TriangleSoupZipper(const TriangleSoup &anInput, TriangleSoup &anOuput, Index size) {
    this->_anInput = anInput;
    this->_anOutput = anOuput;
    this->size = size;
    this->low = Vecteur();
    this->up = Vecteur();
    this->_anInput.boundingBox(this->low,this->up);
    this->index2data = std::map<Index, CellData>();
    this->cellSize = Vecteur((up[0] - low[0])/size[0],
                             (up[1] - low[1])/size[1],
                             (up[2] - low[2])/size[2]
    );

}

Index TriangleSoupZipper::index(const Vecteur &p) const {
    Index i;
    i.idx[0] = (int)(p.xyz[0] - low[0]) / cellSize[0];
    i.idx[1] = (int)(p.xyz[1] - low[1]) / cellSize[1];
    i.idx[2] = (int)(p.xyz[2] - low[2]) / cellSize[2];
    return i;
}

Vecteur TriangleSoupZipper::centroid(const Index &idx) const {
    Vecteur v;
    //calcul du centroid
    v.xyz[0] = idx.idx[0]*cellSize[0]+(cellSize[0]/2)+low[0];
    v.xyz[1] = idx.idx[1]*cellSize[1]+(cellSize[1]/2)+low[1];
    v.xyz[2] = idx.idx[2]*cellSize[2]+(cellSize[2]/2)+low[2];
    return v;
}

void TriangleSoupZipper::zip() {
    for (std::vector<Triangle>::iterator it = _anInput.triangles.begin(); it != _anInput.triangles.end(); ++it) {
        Index idx[3];
        idx[0] = index((*it)[0]);
        idx[1] = index((*it)[1]);
        idx[2] = index((*it)[2]);
        index2data[idx[0]].add((*it)[0]);
        index2data[idx[1]].add((*it)[1]);
        index2data[idx[2]].add((*it)[2]);

        if(!(idx[0] == idx[1] || idx[0] == idx[2] || idx[1] == idx[2]))
        {
            Triangle newT;
            newT[0] = centroid(idx[0]);
            newT[1] = centroid(idx[1]);
            newT[2] = centroid(idx[2]);
            //on l'ajoute a la liste des triangles en sortie
            this->_anOutput.triangles.push_back(newT);
        }

    }
}

void TriangleSoupZipper::advancedZip() {
    index2data.clear();
    this->zip();
    for(std::vector<Triangle>::iterator it = this->_anOutput.triangles.begin(), itE = this->_anOutput.triangles.end(); it != itE; ++it ){
        (*it).sommets[0] = index2data[index((*it).sommets[0])].barycenter();
        (*it).sommets[1] = index2data[index((*it).sommets[1])].barycenter();
        (*it).sommets[2] = index2data[index((*it).sommets[2])].barycenter();
    }
}
