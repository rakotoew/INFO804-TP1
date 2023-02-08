//
// Created by ikanatsu on 2/3/23.
//

#ifndef TP1_UTILS_H
#define TP1_UTILS_H
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

//Classe Vecteur
struct Vecteur {
    float xyz[ 3 ]; // les composantes
    Vecteur( float x, float y, float z ); // constructeur
    Vecteur(); // constructeur
    float  operator[]( int i ) const;     // accesseur en lecture
    float& operator[]( int i );           // accesseur en ecriture
    Vecteur operator-(Vecteur other) const; // soustraction
    // Retourne le vecteur dont les composantes sont les minima des
    // composantes de soi-même et de other.
    Vecteur inf( const Vecteur& other ) const;
    // Retourne le vecteur dont les composantes sont les maxima des
    // composantes de soi-même et de other.
    Vecteur sup( const Vecteur& other ) const;
    // Retourne le produit vectoriel de soi-même et de v.
    Vecteur cross( const Vecteur& v ) const;
    // Retourne l
    Vecteur normalize() const;
};
std::ostream& operator<<( std::ostream& out, Vecteur v );
std::istream& operator>>( std::istream& in, Vecteur& v );


//Classe Triangle
struct Triangle {
    Vecteur sommets[ 3 ]; // les indices des sommets
    Triangle(); // constructeur
    Triangle(Vecteur s0, Vecteur s1, Vecteur s2); // constructeur
    Vecteur  operator[]( int i ) const;     // accesseur en lecture
    Vecteur& operator[]( int i );           // accesseur en ecriture
    Vecteur normal() const; // retourne la normale au triangle
};
std::ostream& operator<<( std::ostream& out, Triangle t );
std::istream& operator>>( std::istream& in, Triangle& t );


//Classe TriangleSoup
struct TriangleSoup {
    std::vector<Triangle> triangles; // les triangles
    TriangleSoup() {}
    void read( std::istream& in );
    void boundingBox( Vecteur& low, Vecteur& up);
};
std::ostream& operator<<( std::ostream& out, TriangleSoup ts );
std::istream& operator>>( std::istream& in, TriangleSoup& ts );


//Classe Index
/// Définit un index sur 3 entiers. Toutes les opérations usuelles
/// sont surchargées (accès, comparaisons, égalité).
struct Index {
    int idx[ 3 ];
    Index() {}
    Index( int i0, int i1, int i2 )
    {
        idx[ 0 ] = i0;
        idx[ 1 ] = i1;
        idx[ 2 ] = i2;
    }
    Index( int indices[] )
    {
        idx[ 0 ] = indices[ 0 ];
        idx[ 1 ] = indices[ 1 ];
        idx[ 2 ] = indices[ 2 ];
    }
    int  operator[]( int i ) const { return idx[ i ]; }
    int& operator[]( int i )       { return idx[ i ]; }
    bool operator<( const Index& other ) const
    {
        return ( idx[ 0 ] < other.idx[ 0 ] )
               || ( ( idx[ 0 ] == other.idx[ 0 ] )
                    && ( ( idx[ 1 ] < other.idx[ 1 ] )
                         || ( ( idx[ 1 ] == other.idx[ 1 ] )
                              && ( idx[ 2 ] < other.idx[ 2 ] ) ) ) );
    }

    bool operator==(const Index& other) const {
        return ( idx[ 0 ] == other.idx[ 0 ] )
               && ( ( idx[ 1 ] == other.idx[ 1 ] )
                    && ( idx[ 2 ] ==other.idx[ 2 ] ) );
    }



};
std::ostream& operator<<( std::ostream& out, Index i );
std::istream& operator>>( std::istream& in, Index& i );

//Classe TriangleSoupZipper
struct TriangleSoupZipper {
    // Construit le zipper avec une soupe de triangle en entrée \a
    // anInput, une soupe de triangle en sortie \a anOutput, et un index \a size
    // qui est le nombre de cellules de la boîte découpée selon les 3 directions.
    TriangleSoupZipper( const TriangleSoup& anInput, TriangleSoup& anOuput, Index size );
};


#endif //TP1_UTILS_H
