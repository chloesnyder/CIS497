#ifndef PPM_H
#define PPM_H

#include <stdio.h>  // paste the contents of stdio.h here, including the prototype for printf()
#include <stdlib.h> // malloc and free are defined here
#include <string.h> // string.h contains the prototype for memset()
#include <assert.h> // needed to use the assert() function for debugging

#define NDEBUG

#ifndef NDEBUG
#define ARRIVED_HERE fprintf(stderr, "Arrived here: %s:%d\n", __FILE__, __LINE__)
#else
#define ARRIVED_HERE
#endif


/* C doesn't have classes, but it does have structs.  A struct is
 * a sort of "anti-interface".  It can include data fields, but
 * it can't include any methods.  There is also no inheritance.
 */

struct pixel_struct {
  unsigned char r, g, b; // a pixel contains three bytes, name r, g, and b
};  // don't forget the ; at the end of the struct declaration!

/* To declare a variable of type "pixel" in C, you have to write
 *   struct pixel_struct pixel;
 * which is clunky.  It's handy to "rename" this type to get around having
 * to write "struct" all over the place.  Do this with a typedef:
 */
typedef struct pixel_struct pixel_t; // from now on "pixel_t p" is a synonym for "struct pixel_struct p"

/* You can also combine the struct declaration with a typedef:
 */
typedef struct img_struct {
  pixel_t *data; // img is a pointer to a block of memory containing pixels, i.e. an array of pixels
  // see the discussion of argv below for an explanation of pointers
  int w, h; // image width and height
} img_t;
class PPM
{

public:

    PPM();

    /* Declare prototypes for functions that main may use, but whose
     * implementations come after main in the file. */
    img_t *new_img(int w, int h);  // create a new image of specified width and height
    void destroy_img(img_t **img); // delete img from memory

    // These functions are used in main, but are not defined in this file
    // The C compiler will just assume they are defined in a different file,
    // but the linker that assembles the compiled code from different C files into
    // an executable will fail when it doesn't find them.
    img_t *read_ppm(const char *fname); // read in an image in ppm format
    void  write_ppm(const img_t *img, const char *fname); // write an image in ppm format
};

#endif // PPM_H



