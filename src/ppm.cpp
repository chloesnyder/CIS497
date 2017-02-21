#include "ppm.h"

PPM::PPM()
{

}

/* Since img_t doesn't contain a constructor, destructor/finalize, or any other methods,
 * we need to build up a library of functions to create, destroy and manipulate images.
 */

// Create a new image of specified size.
img_t *PPM::new_img(int w, int h) {
  /* Crash if width or height is 0 or negative.  This is pretty drastic,
   * but very convenient for debugging.  You normally use it as a sanity
   * check for errors that should not occur in production.
   *
   * In the debugger, your program will break at the line with the failed assertion,
   * so you can your variables and figure out what went wrong.  Outside the debugger
   * your program will generally crash with an error that states the failed assertion's
   * condition and its file and line number in the source code.
   */
  assert(w > 0);
  assert(h > 0);

  // Allocate enough memory to hold an img_t struct
  // sizeof(img_t) returns the number of bytes required
  // for an img_t struct.  malloc allocates memory, similar
  // to new in Java/C++, except that *MALLOC DOES NOT INITIALIZE THE MEMORY*!!!
  // The block of memory returned by malloc is allowed to
  // contain garbage, or even a password or other sensitive
  // information left over in memory from a previous process.
  // It is up to you to initialize the memory's contents
  // in subsequent statements.
  //
  // Note that malloc's return type is "void *", which means
  // "pointer to a blcok of memory of no particular type".  You
  // need to explicitly cast this to (img_t *) to say that this
  // memory will be used to contain an img_t.
  //
  // (C doesn't require you to include the cast, but C++ does.)
  img_t *img = (img_t *) malloc(sizeof(img_t));

  // now initialize img appropriately
  img->w = w;
  img->h = h;

  // allocate memory for the image pixels
  img->data = (pixel_t *) malloc(w * h * sizeof(pixel_t));

  // zero out all the image pixels so they don't contain garbage
  // memset is quite handy for this
  memset(img->data, 0, w * h * sizeof(pixel_t));

  return img;
}

// Destroy an image and free up its memory
// Unlike Java, C does not release memory when you are no longer using it;
// you have to explicitly free the memory yourself.
//
// C also doesn't prevent you from accessing memory that you have freed, but the
// behavior is undefined when you do.  If you're lucky, your program will crash immediately,
// but the problems can be much subtler.  Your program might work find in the debugger, but
// crash outside the debugger.  It might work fine on your computer, but crash when your TA is grading, ...
//
// One way to reduce the likelihood of accessing freed memory is to set any pointer to NULL immediately after
// freeing it.
void PPM::destroy_img(img_t **img) {
  // step 1: free the image pixels
  free((*img)->data); // as long as you allocated img->data with malloc, free knows how big it is
  (*img)->data = NULL; // this is a sanity check to make sure we don't try and access img->data after deleting it
  free(*img); // now free the img_t structure
  *img = NULL; // finally, set the img pointer provided by the caller to NULL so the caller
  // can't accidentally access the freed memory
}

// Read in a PPM file
// RAW PPM DATA! So p6, NOT ASCII (p3)
// GIMP FILES -> edit out the gimp comment!
img_t *PPM::read_ppm(const char *fname) {
  int w, h;

  assert(fname != NULL); // crash if fname is NULL
  FILE *f = fopen(fname, "rb"); // open the ppm for reading in binary mode
  assert(f != NULL); // crash if the file didn't open

  fscanf(f, "P6 %d %d 255%*c", &w, &h); // read in the header and image width and height

  img_t *img = new_img(w, h); // create an empty image of the correct size

  fread(img->data, 3, w * h, f); // read the image data into img->data

  //fread(img->data, 3, w * h, f); // read the image data into img->data

  fclose(f); // close the file

  return img;
}

// Write out a PPM file
void PPM::write_ppm(const img_t *img, const char *fname) {
  assert(img != NULL); // crash if img is NULL
  assert(fname != NULL); // crash if fname is NULL

  FILE *f = fopen(fname, "wb"); // open fname for writing in binary mode; clobbers any existing file
  assert(f != NULL); // crash if file did not open

  fprintf(f, "P6\n%d %d 255\n", img->w, img->h); // write the image header
  fwrite(img->data, img->w * img->h, 3, f); // write the image data

  fclose(f); // close the file
}
