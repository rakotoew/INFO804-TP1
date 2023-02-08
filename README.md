## TP1 Visualisation et compression "out-of-core" de soupes de triangles 
<p style="text-align: center;">Ewan Rakotoanosy, 2023</p>

### Soupes de triangles
- [X] 3.1 Classe de Base
- [X] 3.2.1 Classe TriangleSoup et chargement
- [X] 3.2.2 Classe TriangleSoup et chargement
- [X] 3.3 Ajustement de la Camera
- [X] 3.4 Flat Shading sur les faces
- [X] 3.5 Couleur ambiante / diffuse / spéculaire

Le fichier Q3Viewer.pro permet de generer le makefile correspondant a cette partie. \
L'executable est Q3Viewer.\
Il peut etre appele de la facon suivante:
```
./Q3Viewer bunny258.tri
```

### Compression par découpage sur une grille régulière
- [X] 4.2 Zipper et index d'une cellule
- [X] 4.3 Compression sans replacement des sommets des triangles 
- [X] 4.4 Compression avec replacement des sommets des triangles (Semble marcher mais ne donne exactement pas les memes resultats que ceux de l'enonce)

Le fichier Q4Viewer.pro permet de generer le makefile correspondant a cette partie. \
L'executable est Q4Viewer.\
Il peut etre appele de la facon suivante:
```
./Q4Viewer bunny258.tri out.tri 100 100 100
```