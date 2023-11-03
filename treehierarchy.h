#ifndef TREEHIERARCHY_H
#define TREEHIERARCHY_H

#include "implicits.h"

class TreeHierarchy : public AnalyticScalarField
{
private:
    AnalyticScalarField *leftSon;
    AnalyticScalarField *rightSon;
protected:
    inline AnalyticScalarField *getLeftSon() const {return leftSon;}
    inline AnalyticScalarField *getRightSon() const {return rightSon;}
public:
    TreeHierarchy(AnalyticScalarField *leftSon = nullptr, AnalyticScalarField *rightSon = nullptr) :
        leftSon(leftSon),
        rightSon(rightSon)
    {}
};


class UnionNode : public TreeHierarchy
{
public:
    UnionNode(AnalyticScalarField *leftSon, AnalyticScalarField *rightSon) :
        TreeHierarchy(leftSon, rightSon)
    {}
    double Value(const Vector&) const override;
};

class IntersectionNode : public TreeHierarchy
{
private:
    /* data */
public:
    IntersectionNode(AnalyticScalarField *leftSon, AnalyticScalarField *rightSon) :
        TreeHierarchy(leftSon, rightSon)
    {}

    double Value(const Vector&) const override;
};

class DifferenceNode: public TreeHierarchy
{
private:
    /* data */
public:
    DifferenceNode(AnalyticScalarField *leftSon, AnalyticScalarField *rightSon) :
        TreeHierarchy(leftSon, rightSon)
    {}

    double Value(const Vector&) const;
};

class SmoothUnionNode: public TreeHierarchy
{
private:
    /* data */
public:
    SmoothUnionNode(AnalyticScalarField *leftSon, AnalyticScalarField *rightSon) :
        TreeHierarchy(leftSon, rightSon)
    {}

    double Value(const Vector&) const;
};

class SmoothIntersecionNode: public TreeHierarchy
{
private:
    /* data */
public:
    SmoothIntersecionNode(AnalyticScalarField *leftSon, AnalyticScalarField *rightSon) :
        TreeHierarchy(leftSon, rightSon)
    {}

    double Value(const Vector&) const;
};


#endif // TREEHIERARCHY_H
