/*
 * Copyright (c) 2018 Zitar (vsvsq8@hotmail.com)
 * Distributed under the MIT License.
 * See accompanying file LICENSE.md or copy at http://opensource.org/licenses/MIT
 */

#include <iostream>
#include <ctime>
#include <fstream>

//#include <pthread.h>
#include <thread>
#include <png.h>

#include "sphere.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "float.h"
#include "progressBar.hpp"
#include "imagefilepng.hpp"

#define time double(clock())/CLOCKS_PER_SEC
#define NUM_THREADS 4  // power of 2; 0 for 1
#define IMAGEH 1080 // divides by 2, 6, 8
#define IMAGEW 1920
#define IMAGES 1000

//unsigned int jobLocation = 0; // image height, for other threads to keep working on.

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

void *PrintHello(void *threadid){
    long tid;
    tid = (long)threadid;
    std::cout << "Hello world! Thread ID, " << tid << std::endl;
    pthread_exit(NULL);
}

void pixelate(imageFilepng &image, hitable *world, camera &cam, ProgressBar 
              &progressBar, unsigned int ns, unsigned int &jobLocation){

    unsigned int j = jobLocation;
    while ( j < image.height){
      png_bytep row = image.row_pointers[j];
      for (unsigned int i = 0; i <= image.width; i++){
          vec3 col(0, 0, 0);
          png_bytep px = &(row[i * 3]);

          for (unsigned int s=0; s < ns; s++){
              float u = float(i + drand48()) / float(image.width);
              float v = float(j + drand48()) / float(image.height);

              ray r = cam.get_ray(u, v);
              col += color(r, world, 0);
          }

          col /= float(ns);
          col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

          px[0] = static_cast<unsigned char>(255.99*col[0]);
          px[1] = static_cast<unsigned char>(255.99*col[1]);
          px[2] = static_cast<unsigned char>(255.99*col[2]);

  //      outputFile << ir << " " << ig << " " << ib << "\n";
          ++progressBar;
      }
      progressBar.display();
          jobLocation++;
          j = jobLocation;
    }
//    jobLocation++;
}
//==================================================================================
int main(int argc, char *argv[]) {
  std::cout << "Starting Raytracer...\n";
  double a=time;
//  std::ofstream outputFile;
//  outputFile.open("output.ppm", std::ios::out);

  // Image initilization
  const unsigned int nx = IMAGEW; // 1920
  const unsigned int ny = IMAGEH; // 1080
  const unsigned int ns = IMAGES; // image resoultion
  ProgressBar progressBar(ny*nx, 70);
//  outputFile << "P3\n" << nx << " " << ny << "\n255\n";
  imageFilepng image(nx, ny);

  // Making the world
  hitable *list[5];
  list[0] = new sphere(vec3( 0,     0,-1),  0.5,      new lambertian(vec3(0.1, 0.2, 0.5)));
  list[1] = new sphere(vec3( 0,-100.5,-1),  100,      new lambertian(vec3(0.8, 0.8, 0.0)));
  list[2] = new sphere(vec3( 1,     0,-1),  0.5,      new metal(vec3(0.8, 0.6, 0.2), 0.0));
  list[3] = new sphere(vec3(-1,     0,-1),  0.5,      new dielectric(1.5));
  list[4] = new sphere(vec3(-1,     0,-1),-0.45,      new dielectric(1.5));

  hitable *world = new hitable_list(list, 5);
  world = random_scene();

  // Camera options
  vec3 lookfrom(13,2,3);
  vec3 lookat(0,0,0);
  float dist_to_focus = (lookfrom-lookat).length();
  float apture = 0.1;

  camera cam(lookfrom, lookat, vec3(0,-1,0), 20, float(nx)/float(ny), apture, dist_to_focus);

  // Making pixels

//  for (unsigned int j = 0; j < ny; j++){
//    png_bytep row = image.row_pointers[j];
//    for (unsigned int i = 0; i <= nx; i++){
//        vec3 col(0, 0, 0);
//        png_bytep px = &(row[i * 3]);

//        for (unsigned int s=0; s < ns; s++){
//            float u = float(i + drand48()) / float(nx);
//            float v = float(j + drand48()) / float(ny);

//            ray r = cam.get_ray(u, v);
//            col += color(r, world, 0);
//        }

//        col /= float(ns);
//        col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

//        px[0] = static_cast<unsigned char>(255.99*col[0]);
//        px[1] = static_cast<unsigned char>(255.99*col[1]);
//        px[2] = static_cast<unsigned char>(255.99*col[2]);

////      outputFile << ir << " " << ig << " " << ib << "\n";
//        ++progressBar;
//    }
//    progressBar.display();
//  }

//  std::thread t(pixelate, std::ref(image), std::ref(world), std::ref(cam),
//                std::ref(progressBar), std::ref(ns));

  std::thread threads[NUM_THREADS];
  /* Note:
   * Multi-task? or Multi-thread?
   * split work by deviding the image into square sections?
   * - what if parts take more work?
   * - if number of jobs is an odd number?
   * Or, assigne each jod to an image row?
   * - Then, overhead cost of assigning taks to each thread
   * ...
   * Depends :)
   *
   * see:
   * https://computing.llnl.gov/tutorials/parallel_comp/
   * Array Processing
   * Parallel Solution 2: Pool of Tasks:
   * "A more optimal solution might be to distribute more work with each job.
   * The "right" amount of work is problem dependent."
  */
  // spawn n threrads:
  unsigned int jobLocation =0;
  for (unsigned int i =0; i < NUM_THREADS; i++){
      threads[i] = std::thread(pixelate, std::ref(image), std::ref(world),
                               std::ref(cam), std::ref(progressBar), ns, std::ref(jobLocation));
  }

  for (auto & th : threads){
      th.join();
  }


//  pthread_t threads[NUM_THREADS];
//  int rc;

//  for (static unsigned int i =0; i < NUM_THREADS; i++){
//      std::cout << "Main(): creating thread, " << i << std::endl;
//      rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);
//      rc = pthread_create(&threads[i], NULL, pixelate, )

//      if (rc){
//          std::cout << "Error: unable to create thread, " << rc << std::endl;
//          exit(-1);
//      }
//  }
//  pthread_exit(NULL);

//  t.join();

  // Writing image to disk
  progressBar.done();
//  outputFile.close();
  std::cout << "Writing image: " << std::endl;
  image.write_png_file("outputImage.png");


  std::cout << "Time: " << time - a << " sec" << std::endl;

  return 0;
}
