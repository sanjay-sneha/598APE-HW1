#include "triangle.h"

Triangle::Triangle(Vector c, Vector b, Vector a, Texture* t):Plane(Vector(0,0,0), t, 0., 0., 0., 0., 0.){
   center = c;
   Vector righta = (b-c);
   textureX = righta.mag();
   invTextureX = 1.0 / textureX;
   right = righta*invTextureX;
   vect = right.cross(b-a).normalize();

   xsin = -right.z;
   if(xsin<-1.)xsin = -1;
   else if (xsin>1.)xsin=1.; 
   yaw = asin(xsin);
   xcos = sqrt(1.-xsin*xsin);

   zcos = right.x/xcos;
   zsin = -right.y/xcos;
   if(zsin<-1.)zsin = -1;
   else if (zsin>1.)zsin=1.;
   if(zcos<-1.)zcos = -1;
   else if (zcos>1.)zcos=1.;
   roll = asin(zsin);

   ycos = vect.z/xcos;
   if(ycos<-1.)ycos = -1;
   else if (ycos>1.)ycos=1.;
   pitch = acos(ycos);
   ysin = sqrt(1-ycos*ycos);

   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   Vector temp = vect.cross(right);

   Vector delta = a - c;
	double npX = delta.dot(right);
	double npY = delta.dot(up);
	textureY = npY;
   invTextureY = 1.0 / textureY;
	thirdX = npX;
   
   d = -vect.dot(center);
}

double Triangle::getIntersection(const Ray& ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;

   const double px = ray.point.x + ray.vector.x * time - center.x;
   const double py = ray.point.y + ray.vector.y * time - center.y;
   const double pz = ray.point.z + ray.vector.z * time - center.z;
   const double dx = px * right.x + py * right.y + pz * right.z;
   const double dy = px * up.x + py * up.y + pz * up.z;

   unsigned char tmp = (thirdX - dx) * textureY + (thirdX-textureX) * (dy - textureY) < 0.0;
   return((tmp!=(textureX * dy < 0.0)) || (tmp != (dx * textureY - thirdX * dy < 0.0)))?inf:time;
}

bool Triangle::getLightIntersection(const Ray& ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   
   Vector delta = ray.point + ray.vector * r - center;
   const double px = ray.point.x + ray.vector.x * r - center.x;
   const double py = ray.point.y + ray.vector.y * r - center.y;
   const double pz = ray.point.z + ray.vector.z * r - center.z;
   const double dx = px * right.x + py * right.y + pz * right.z;
   const double dy = px * up.x + py * up.y + pz * up.z;

   unsigned char tmp = (thirdX - dx) * textureY + (thirdX-textureX) * (dy - textureY) < 0.0;
   if ((tmp!=(textureX * dy < 0.0)) || (tmp != (dx * textureY - thirdX * dy < 0.0))) return false;
   
   if(texture->opacity>1-1E-6) return true;   
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dx*invTextureX-.5), fix(dy*invTextureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}
