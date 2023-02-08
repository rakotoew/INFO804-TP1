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


