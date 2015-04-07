Catmull-Clark Subdivision and OBJ file importer:

- Everything works correctly, no known bugs.
- There’s no mesh originally, so you’ll have to press “import .obj” to import a mesh to play around with
- Press “subdivide” to subdivide the mesh

Catmull-Clark: 
I implemented the Catmull-Clark subdivision algorithm by creating a push button that connected to a slot in MyGL that calls a subdivision method in mesh.cpp. I copied all the original vertex and face data, and made a list to store all the half-edges I visited as I went through the mesh. I also created 2 maps that mapped a face to the set of half edges that point to the face, and a map that mapped the face to the vertices that created the face.
First, I went through each face in the mesh and created a vertex at its centroid. Then I created a list of all the half edges I needed to split in the mesh (this way I only split one edge, not 2 midpoints per edge). I called my split edge function from the last homework on these edges to get an unweighted midpoint, then updated the position based on whether there was 1 or 2 face centroids affecting the “weight” of the midpoint.
After this, I smoothed the original vertices based on the sum of adjacent midpoint positions and incident face centroids. I also created a map list of half edges that point to a midpoint on a face.
I pass this map, a face, and my map of edges and vertices to my quadrangulate function.
Quadrangulating: (quadrangulate per face in the mesh)
- get the vector of all half edges pointing to midpoints
- get the vector of all half edges pointing to original vertices
- find the number of vertices in the original face
- a half edge that points to a vertex will have a next that points to a midpoint. This one’s next is a new edge that points to a centroid, this one’s next is a new edge that goes from the centroid to a midpoint, and this one’s next points to the first edge.
- when setting up the next loop for the face is done,set the sum of the new half edges. The from_centroid[i] edges are sums with the to_centroid[i-1] edges

Import OBJ:
- in MyGL, the import obj push button calls the slot importObjFile()
- I delete all original mesh info if any exists
- Assumes that this is a wellformed file
- Use QFileDialogue to create a QTextStream. Read each line, and parse info based on whether the line begins with the letter “v “ or “f”. This gets either info about vertex positions and stores it in an array or vertices in a face. 
-Once all info is parsed, pass the list of vertices and a map of faces to their vertices to a createFromFile function in mesh.
- Here I create each one of the faces individually, then when I’m done wit that I “sew together” by making the edges have the correct sums. I go through every vertex, loop through all the edges pointing to that vertex, and find the previous edge to one pointing to the vert. If an edge’s next’s vert is the same as the vertex of the previous edge, it’s the sym to the current edge.
