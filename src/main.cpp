/*
 * Copyright (c) <year> <author> (<email>)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

//#include <cstdlib>
//#include <iostream>
//#include <stdexcept>

//#include "cppbase/factorial.hpp"

//int main( const int numberOfInputs, const char* inputArguments[ ] )
//{
//	try
//	{
//		const int factorial = cppbase::computeFactorial( 10 );
//		std::cout << "10! = " << factorial << std::endl;
//	}
//	catch( std::exception& error )
//	{
//		std::cout << error.what( ) << std::endl;
//	}

//	try
//	{
//		const int factorial = cppbase::computeFactorial( -5 );
//		std::cout << "5! = " << factorial << std::endl;
//	}
//	catch( std::exception& error )
//	{
//		std::cout << error.what( ) << std::endl;
//	}

//	return EXIT_SUCCESS;
//}

#include <iostream>
#include <ctime>
#include <fstream>

#include <pthread.h>
#include <png.h>

#include "sphere.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "float.h"
#include "progressBar.hpp"
#include "imagefilepng.hpp"

#define time double(clock())/CLOCKS_PER_SEC
#define NUM_THREADS 5
#define IMAGEH 180
#define IMAGEW 50

vec3 color(const ray& r, hitable *world, int depth){
  hit_record rec;
  if (world->hit(r, 0.001, MAXFLOAT, rec)){
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
      return attenuation * color(scattered, world, depth+1);
    } else {
      return vec3(0,0,0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
  }
}

hitable *random_scene(){
  int n = 500;
  hitable **list = new hitable*[n+1];
  list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
  int i =1;

  for (int a = -11; a < 11; a++){
    for (int b = -11; b < 11; b++){
      float choose_mat = drand48();
      vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
      if ((center-vec3(4,0.2,0)).length() > 0.9){
        if (choose_mat < 0.8){  // diffuse
          list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())));
        } else if (choose_mat < 0.95){  // metal
          list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1+drand48()), 0.5*(1+drand48()), 0.5*(1+drand48())), 0.5*drand48()));
        } else {  // glass
          list[i++] = new sphere(center, 0.2, new dielectric(1.5));
        }
      }
    }
  }

  list[i++] = new sphere(vec3(0,1,0), 1.0, new dielectric(1.5));
  list[i++] = new sphere(vec3(-4,1,0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
  list[i++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

  return new hitable_list(list, i);
}



//==================================================================================
int main(int argc, char *argv[]) {

    std::cout << "argc: " << argc << std::endl;
    std::cout << "argv[1]: " << argv[1] << std::endl;
//    std::cout << "argv[2]: " << argv[2] << std::endl;

//    if(argc != 3) {
//        std::cout << "less than 2 inputs provided..";
//        abort();
//        return -1;
//    }
//    std::cout << "2 inputs provided..";
//    imageFilepng image;

    imageFilepng image(10, 10);//, PNG_COLOR_TYPE_RGB_ALPHA, 8); //, "outputImage.png");

//    image.read_png_file(argv[1]);

//    image.width  = 400;
//    image.height = 300;

//    image.color_type = PNG_COLOR_TYPE_RGB;
//    image.bit_depth = 8;

//    image.row_pointers = (png_bytep*)malloc(sizeof(png_bytep)*image.height);
//    for(int y = 0; y < image.height; y++) {
//      image.row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
//    }

    for (unsigned int y = 0; y < image.height; y++) {
        png_bytep row = image.row_pointers[y];
        for (unsigned int x = 0; x < image.width; x++) {
            png_bytep px = &(row[x*4]);

            // Process:
//            printf("%4d, %4d = RGBA(%d, %d, %d, 3d)\n", x, y, px[0], px[1], px[2]);//, px[3]);

            px[0] = 200 ;
//            px[1] = 20 ;
//            px[2] = 20 ;
//            px[3] = 100 ;
//            std::cout << "px: " << px << std::endl;

        }
    }

    image.write_png_file("outputImage.png");

//    return 0;



//  std::cout << "Starting Raytracer...\n";
//  double a=time;
//  std::ofstream outputFile;
//  outputFile.open("output.ppm", std::ios::out);

//  int nx = 192;
//  int ny = 108;
//  int ns = 100;
//  ProgressBar progressBar(ny, 70);
//  outputFile << "P3\n" << nx << " " << ny << "\n255\n";

//  hitable *list[5];
//  list[0] = new sphere(vec3( 0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
//  list[1] = new sphere(vec3( 0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
//  list[2] = new sphere(vec3( 1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
//  list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
//  list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));

//  hitable *world = new hitable_list(list, 5);
//  world = random_scene();

//  vec3 lookfrom(13,2,3);
//  vec3 lookat(0,0,0);
//  float dist_to_focus = (lookfrom-lookat).length();
//  float apture = 0.1;

//  camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), apture, dist_to_focus);

//  for (int j = ny-1; j >= 0; j--){
//    for (int i = 0; i < nx; i++){
//      vec3 col(0, 0, 0);
//      for (int s=0; s < ns; s++){
//        float u = float(i + drand48()) / float(nx);
//        float v = float(j + drand48()) / float(ny);

//        ray r = cam.get_ray(u, v);
//        col += color(r, world, 0);
//      }

//      col /= float(ns);
//      col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

//      int ir = int(255.99*col[0]);
//      int ig = int(255.99*col[1]);
//      int ib = int(255.99*col[2]);

//      outputFile << ir << " " << ig << " " << ib << "\n";
//    }
//    ++progressBar;
//    progressBar.display();
//  }

//  progressBar.done();
//  outputFile.close();

//  std::cout << "Time: " << time - a << " sec" << std::endl;

  return 0;
}
