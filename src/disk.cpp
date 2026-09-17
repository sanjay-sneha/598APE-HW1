#include "disk.h"
Disk::Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}


double Disk::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;

   const double px = ray.point.x + ray.vector.x * time - center.x;
   const double py = ray.point.y + ray.vector.y * time - center.y;
   const double pz = ray.point.z + ray.vector.z * time - center.z;
   const double dx = px * right.x + py * right.y + pz * right.z;
   const double dy = px * up.x + py * up.y + pz * up.z;

   return (  dx*dx/(textureX*textureX)+dy*dy/(textureY*textureY)>1  )?inf:time;
}

bool Disk::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;

   const double px = ray.point.x + ray.vector.x * r - center.x;
   const double py = ray.point.y + ray.vector.y * r - center.y;
   const double pz = ray.point.z + ray.vector.z * r - center.z;
   const double dx = px * right.x + py * right.y + pz * right.z;
   const double dy = px * up.x + py * up.y + pz * up.z;
   
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