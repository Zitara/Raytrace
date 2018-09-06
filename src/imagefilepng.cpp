#include "imagefilepng.hpp"

imageFilepng::imageFilepng(){

}

imageFilepng::imageFilepng( unsigned int image_width, unsigned int image_height){

    width = image_width;
    height = image_height;

//    row_pointers = new png_bytep[image_height];
    row_pointers = (png_bytep*)malloc(sizeof (png_bytep)*image_height);
    for (unsigned int h = 0; h < image_height ; h++) {
//        row_pointers[h] = new png_byte[3*image_width];
        row_pointers[h] = (png_byte*)malloc(3 * image_width * sizeof (png_byte));
    }


}

void imageFilepng::read_png_file(char *filename){
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) abort();

    png_infop info = png_create_info_struct(png);
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
      png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_palette_to_rgb(png);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
      png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
      png_set_tRNS_to_alpha(png);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if(color_type == PNG_COLOR_TYPE_RGB ||
       color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
       color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = new png_bytep[height];
    for (unsigned int h = 0; h < height ; h++) {
        row_pointers[h] = new png_byte[3*width];
    }

    png_read_image(png, row_pointers);

    fclose(fp);
}

void imageFilepng::write_png_file(char *filename){
    FILE *fp = fopen(filename, "wb");   // Open file for writing (binary mode)
    if(!fp) abort();

    png_structp png = png_create_write_struct(
                PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);    // Initialize write structure
    if (!png) abort();

    png_infop info = png_create_info_struct(png);   // Initialize info structure
    if (!info) abort();

    if (setjmp(png_jmpbuf(png))) abort();   // Setup Exception handling

    png_init_io(png, fp);

    // Write header (8 bit colour depth)
    // Output is 8bit depth, RGB format.
    png_set_IHDR(
      png,
      info,
      width, height,
//      8,
      bit_depth,
      color_type,
//      PNG_COLOR_TYPE_RGBA,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
//    png_set_filler(png, 0, PNG_FILLER_AFTER);

    png_write_image(png, row_pointers); // Write image data
    png_write_end(png, nullptr);        // End write

//    for(unsigned int h = 0 ; h < height; h++) {
//      free(row_pointers[h]);
//    }
//    free(row_pointers);

//    std::cout << "image height: " << height << std::endl;

    for(unsigned int h = 0 ; h < height; ++h) {
//        delete[] row_pointers[h];
        free(row_pointers[h]);
    }
//    delete[] row_pointers;
    free(row_pointers);
//    row_pointers = nullptr;

    fclose(fp);
}
