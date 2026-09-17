#include "triangle.h"

Triangle::Triangle(Vector c, Vector b, Vector a, Texture* t):Plane(Vector(0,0,0), t, 0., 0., 0., 0., 0.){
   center = c;
   Vector righta = (b-c);
   textureX = righta.mag();
   right = righta/textureX;
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

double Triangle::getIntersection(Ray ray){
   Vector v0 = center;
   Vector v1 = center + right * textureX;
   Vector v2 = center + right * thirdX + up * textureY;
   Vector e1 = v1 - v0;
   Vector e2 = v2 - v0;
   Vector h = ray.vector.cross(e2);
   double a = e1.dot(h);

   if(fabs(a) < 1e-12)
      return inf;

   double f = 1.0 / a;
   Vector s = ray.point - v0;
   double u = f * s.dot(h);
   if(u < 0.0 || u > 1.0)
      return inf;

   Vector q = s.cross(e1);
   double v = f * ray.vector.dot(q);
   if(v < 0.0 || u + v > 1.0)
      return inf;

   double t = f * e2.dot(q);
   return (t > 1e-12) ? t : inf;
}

bool Triangle::getLightIntersection(Ray ray, double* fill){
   Vector v0 = center;
   Vector v1 = center + right * textureX;
   Vector v2 = center + right * thirdX + up * textureY;
   Vector e1 = v1 - v0;
   Vector e2 = v2 - v0;
   Vector h = ray.vector.cross(e2);
   double a = e1.dot(h);

   if(fabs(a) < 1e-12)
      return false;

   double f = 1.0 / a;
   Vector s = ray.point - v0;
   double u = f * s.dot(h);
   if(u < 0.0 || u > 1.0)
      return false;

   Vector q = s.cross(e1);
   double v = f * ray.vector.dot(q);
   if(v < 0.0 || u + v > 1.0)
      return false;

   double t = f * e2.dot(q);
   if(t <= 1e-12 || t >= 1.0)
      return false;

   Vector hit = ray.point + ray.vector * t;
   Vector local = hit - center;
   double dx = local.dot(right);
   double dy = local.dot(up);

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
