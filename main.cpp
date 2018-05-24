#include <iostream>
/// #include <cstdio>
#include <ctime>
#include <fstream>

#include "sphere.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "float.h"

#define time double(clock())/CLOCKS_PER_SEC

/// vec3 random_in_unit_sphere(){
///   vec3 p;
///   do {
///     p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1,1,1);
///   } while (p.squared_length() >= 1.0);
///   return p;
/// }

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

int main() {
  std::cout << "Starting Raytracer...\n";
  double a=time;
  std::ofstream outputFile;
  outputFile.open("output.ppm", std::ios::out);

  int nx = 200;
  int ny = 100;
  int ns = 100;
  /// std::cout << "Writing to disk...\n";
  outputFile << "P3\n" << nx << " " << ny << "\n255\n";

  /// vec3 lower_left_corner(-2.0, -1.0, -1.0);
  /// vec3 horizontal(4.0, 0.0, 0.0);
  /// vec3 vertical(0.0, 2.0, 0.0);
  /// vec3 origin(0.0, 0.0, 0.0);

  /// std::cout << "Making a list...\n";
  hitable *list[4];
  list[0] = new sphere(vec3( 0,0,-1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
  list[1] = new sphere(vec3( 0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
  list[2] = new sphere(vec3( 1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
  list[3] = new sphere(vec3(-1,0,-1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));

  hitable *world = new hitable_list(list, 4);

  camera cam;

  /// std::cout << "For loop, making pixels...\n";
  for (int j = ny-1; j >= 0; j--){
    for (int i = 0; i < nx; i++){
      vec3 col(0, 0, 0);
      for (int s=0; s < ns; s++){
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);

        ray r = cam.get_ray(u, v);
        vec3 p = r.point_at_parameter(2.0);
        col += color(r, world, 0);
      }

      /// std::cout << "Testing(j , i): " << j << " , " << i << "\n";
      col /= float(ns);
      col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );

      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);

      outputFile << ir << " " << ig << " " << ib << "\n";
    }
  }

  /// std::cout << "Done, writing to disk...\n";
  outputFile.close();

  std::cout << "Time: " << time - a << " sec" << std::endl;

  return 0;
}
