#include "treehierarchy.h"
#include "math.h"


double UnionNode::Value(const Vector &v) const
{
    return Math::Min(getLeftSon()->Value(v), getRightSon()->Value(v));
}

double IntersectionNode::Value(const Vector &v) const
{
    return Math::Max(getLeftSon()->Value(v), getRightSon()->Value(v));
}

double DifferenceNode::Value(const Vector &v) const
{
    return Math::Max(getLeftSon()->Value(v), -(getRightSon()->Value(v)));
}

double SmoothIntersecionNode::Value(const Vector &v) const
{
    double a =getLeftSon()->Value(v), b = (getRightSon()->Value(v));
    double k = 0.1 ;
    float h = Math::Max( k-abs(a-b), 0.0 )/k;

    return Math::Max( a, b ) + h*h*k*(1.0/4.0) ;
}

double SmoothUnionNode::Value(const Vector &v) const
{
    double a = (getLeftSon()->Value(v)) ;
    double b = (getRightSon()->Value(v));
    double k = 0.1 ;
    double h = Math::Max( k-abs(a-b), 0.0 )/k;

    return Math::Min( a, b ) - h*h*k*(1.0/4.0) ;
}









