#include "disk.h"
Disk::Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}


double Disk::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;

   Vector delta = ray.point + ray.vector * time - center;
   const double dx = delta.dot(right);
   const double dy = delta.dot(up);

   return (  dx*dx/(textureX*textureX)+dy*dy/(textureY*textureY)>1  )?inf:time;
}

bool Disk::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;

   Vector delta = ray.point + ray.vector * r - center;
   const double dx = delta.dot(right);
   const double dy = delta.dot(up);
   
   if(  dx*dx/(textureX*textureX)+dy*dy/(textureY*textureY)>1  )return false;
   if(texture->opacity>1-1E-6) return true;   
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dx/textureX-.5), fix(dy/textureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}