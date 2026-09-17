#include "triangle.h"

Triangle::Triangle(Vector c, Vector b, Vector a, Texture* t):Plane(Vector(0,0,0), t, 0., 0., 0., 0., 0.){
   center = c;
   Vector righta = (b-c);
   invTextureX = 1.0 / textureX;
   invTextureY = 1.0 / textureY;
   textureX = righta.mag();
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
	thirdX = npX;
   
   d = -vect.dot(center);
}

double Triangle::getIntersection(const Ray& ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;
   
   Vector delta = ray.point + ray.vector * time - center;
	const double dx = delta.dot(right);
   const double dy = delta.dot(up);

   unsigned char tmp = (thirdX - dx) * textureY + (thirdX-textureX) * (dy - textureY) < 0.0;
   return((tmp!=(textureX * dy < 0.0)) || (tmp != (dx * textureY - thirdX * dy < 0.0)))?inf:time;
}

bool Triangle::getLightIntersection(const Ray& ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   
   Vector delta = ray.point + ray.vector * r - center;
   const double dx = delta.dot(right);
   const double dy = delta.dot(up);
   
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
