## B3MP

Goal is to parse .bmp (bitmap) format (only uncompressed ones for now),
and turn them into image data, this could be useful for my machine learning library or in general.

#### Helpful Websites :

- [Useful website I found](https://users.cs.fiu.edu/~czhang/teaching/cop4225/project_files/bitmap_format.htm)

- [wikipedia](https://en.wikipedia.org/wiki/BMP_file_format)

#### NOTES

- This could eventually be part of [lib3man](https://github.com/3manuel0/lib3man)
- I used PPM format, since needed a fast way to write an image to test things.
- First I will handle the uncompressed formats then the easy ones, and I could create my own parser for PNG and JPEG evetually after.
- I will only be using the C standard library and [lib3man](https://github.com/3manuel0/lib3man).
