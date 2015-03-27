
How it works:
- Add Vertex: select an edge from the list, click add vertex. This adds a vertex at the midpoint of the two, and creates 2 new half edges as appropriate
- Split face: select a face from the list, click split face. Triangulates by creating a new half edge going diagonally across the quad. Only works for quads!
- Delete vertex: select a vertex from the list, click delete vertex. This deletes the vertex and it’s incident edges/faces.
- The RGB spin boxes update the color of the currently selected face. You will have to click off the face to see the new color, because selected faces are always white.
- The XYZ spin boxes update the position of the currently selected vertex