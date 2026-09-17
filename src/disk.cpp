#include "disk.h"
Disk::Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}


double Disk::getIntersection(const Ray& ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;

   Vector delta = ray.point + ray.vector * time - center;
   Vector dist(delta.dot(right), delta.dot(up), delta.dot(vect));

   return (  dist.x*dist.x*invTextureX*invTextureX+dist.y*dist.y*invTextureY*invTextureY>1  )?inf:time;
}

bool Disk::getLightIntersection(const Ray& ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;

   Vector delta = ray.point + ray.vector * r - center;
   Vector dist(delta.dot(right), delta.dot(up), delta.dot(vect));
   
   if(  dist.x*dist.x*invTextureX*invTextureX+dist.y*dist.y*invTextureY*invTextureY>1  )return false;
   if(texture->opacity>1-1E-6) return true;   
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x*invTextureX-.5), fix(dist.y*invTextureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}