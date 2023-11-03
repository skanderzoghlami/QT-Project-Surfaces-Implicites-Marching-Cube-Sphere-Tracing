// Implicits

#pragma once

#include <iostream>

#include "mesh.h"




class AnalyticScalarField
{
public:
    double tx=0 ;
    double ty=0;
    double tz=0 ;
public:
    AnalyticScalarField() : tx(0), ty(0), tz(0) {}

  virtual double Value(const Vector&) const;
  virtual Vector Gradient(const Vector&) const;

  // Normal
  virtual Vector Normal(const Vector&) const;

  // Dichotomy
  Vector Dichotomy(Vector, Vector, double, double, double, const double& = 1.0e-4) const;

  virtual void Polygonize(int, Mesh&, const Box&, const double& = 1e-4) const;
protected:
  static const double Epsilon; //!< Epsilon value for partial derivatives
protected:
  static int TriangleTable[256][16]; //!< Two dimensionnal array storing the straddling edges for every marching cubes configuration.
  static int edgeTable[256];    //!< Array storing straddling edges for every marching cubes configuration.
};
class TorusSDF :  public AnalyticScalarField
{
  public:
  // Constructor for the torus SDF class
  TorusSDF(double majorRadius, double minorRadius)
      : majorRadius_(majorRadius), minorRadius_(minorRadius) {}

  // Override the Value function to define the torus SDF
  double Value(const Vector& p) const override
  {
    double majorRadius = majorRadius_ ; // Replace with your actual major radius
    double minorRadius = minorRadius_; // Replace with your actual minor radius

    // Calculate the signed distance to a torus
    double x = p[0]+tx;
    double y = p[1]+ty;
    double z = p[2]+tz;

    double distanceInXZ = sqrt(x * x + z * z) - majorRadius;
    double signedDistance = sqrt(distanceInXZ * distanceInXZ + y * y) - minorRadius;

    return signedDistance;
  }
  private:
  double majorRadius_;  // Major radius of the torus
  double minorRadius_;  // Minor radius of the torus
};

class CapsuleSDF :  public AnalyticScalarField
{
  public:

  CapsuleSDF(double majorRadius, double minorRadius)
      : r(majorRadius), h(minorRadius) {}

  // Override the Value function to define the torus SDF
  double Value(const Vector& point) const override {
    // Parameters for the capsule
    double radius = r ;      // Radius of the capsule's cylindrical part
    double halfLength = h;  // Half of the length of the capsule along the y-axis

    double x = point[0] +tx ;
    double y = point[1] +ty ;
    double z = point[2] +tz ;

    // Calculate the distance from the center line (cylinder part)
    double d = sqrt(x * x + z * z) - radius;

    // Calculate the distance from the end caps
    double distanceToEndCaps = std::max(0.0, std::abs(y) - halfLength);

    // Combine both distances to define the capsule
    double signedDistance = std::sqrt(d * d + distanceToEndCaps * distanceToEndCaps) - radius;

    return signedDistance;
  }

  private:
  double r;  // Major radius of the torus
  double h;  // Minor radius of the torus
};

class BoiteSDF :  public AnalyticScalarField
{
  public:

  BoiteSDF(double halfSizeX, double halfSizeY , double halfSizeZ )
      : X(halfSizeX), Y(halfSizeY ) ,Z(halfSizeZ ) {}

  // Override the Value function to define the torus SDF
  double Value(const Vector& point) const override
  {

    // Parameters for the cube
    double halfSizeX = X; // Half of the size along the x-axis
    double halfSizeY = Y; // Half of the size along the y-axis
    double halfSizeZ = Z; // Half of the size along the z-axis

    double x = std::abs(point[0]+tx);
    double y = std::abs(point[1]+ty);
    double z = std::abs(point[2]+tz);

    double signedDistanceX = x - halfSizeX;
    double signedDistanceY = y - halfSizeY;
    double signedDistanceZ = z - halfSizeZ;

    double maxDistance = std::max(signedDistanceX, std::max(signedDistanceY, signedDistanceZ));

    return maxDistance;
  }
  private:
  double X;  // Major radius of the torus
  double Y;  // Minor radius of the torus
  double Z;  // Minor radius of the torus
};

class ConeSDF : public AnalyticScalarField
{
  public:
  ConeSDF(float cosAngle, float sinAngle, float height) : cosAngle_(cosAngle), sinAngle_(sinAngle), h_(height) {}

  double Value(const Vector& point) const override
  {
    double x = point[0] +tx;
    double y = point[1] +ty;
    double z = point[2] + 0.5 * h_ +tz; // Offset by half the height

    double q = sqrt(x * x + z * z);

    return std::max(cosAngle_ * q + sinAngle_ * y, -h_ - y);
  }

  private:
  float cosAngle_;
  float sinAngle_;
  float h_;
};

class SphereSDF : public AnalyticScalarField
{
  public:
  SphereSDF(float rayon ) :  rayon_(rayon) {}

  double Value(const Vector& p) const override
  {
    Vector p2(p[0] + tx , p[1] + ty , p[2] + tz);
    return Norm(p2) - rayon_;
  }

  private:
  float rayon_;
};
