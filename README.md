# 3d_4d_viewer

## 3D és 4D testek megjelenítése.

Nagyjából refaktoráltam a kódot, remélem olvasható valamennyire.<br />
A geometriához/matekhoz/megjelenítéshez stb. tartozó dolgokat igyekeztem kommentezni.

### Irányítás:
Egérrel a 3D-s projekciót tudod forgatni, görgővel közelítesz-távolítasz.<br />
S/W ---> J/U billentyűkkel a negyedik dimenzióban forgathatsz a különböző síkok körül.

Használd, írd át nyugodtan :)

### Használat:
GLUT könyvtár szükséges!! Berakod a /usr/include-ba a GL mappát és műxik.<br />
Windowson nem tom holvan az include mappa.
```
g++ -c -g -std=c++11 -MMD -MP -MF "main.o.d" -o main.o main.cpp
g++ -o outfile main.o -lglut -lGLU -lGL
```
### Futtatás:
```
./outfile
```
