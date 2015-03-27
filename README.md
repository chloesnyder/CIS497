Brief README… I will be updating my assignment tomorrow to work better.

So currently my assignment works up to deleting a second vertex. Because the code I wrote for deleting a second vertex crashes everything the first time I call delete, I have commented that out in my mesh.cpp file. This way, it runs so that there is no issue deleting a first vertex but there is an issue deleting a second. 

The pseudo code for deleting the second is as follows: 
//for all the vertices that you don't delete, set the incoming edge to the previous edge
//that prev is guaranteed to be a qualified edge
//set its next to it's next.sym.next

Until I can get these pointers to all update properly, I’m leaving it out. I’ve done something where there’s some null pointer trying to be accessed and I’m having a lot of difficulty debugging on my own.

How it works:
- Add Vertex: select an edge from the list, click add vertex. This adds a vertex at the midpoint of the two, and creates 2 new half edges as appropriate
- Split face: select a face from the list, click split face. Triangulates by creating a new half edge going diagonally across the quad. Only works for quads!
- Delete vertex: select a vertex from the list, click delete vertex. This deletes the vertex and it’s incident edges/faces.