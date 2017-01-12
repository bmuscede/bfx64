/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXEdge.h
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Defines the format of an edge in the object file dependency graph.
// Contains an enum with a collection of edge types. These define how
// the TA file is laid out.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFX64_EDGE_H
#define BFX64_EDGE_H

#include "BFXNode.h"

class BFXEdge {
public:
    /** Enum For Type of Edge */
    enum EdgeType { CONTAINS, LINK };

    /** Constructor / Destructor */
    BFXEdge(BFXNode* src, BFXNode* dst, EdgeType type);
    ~BFXEdge();

    /** Lookup Method */
    static std::string getTypeString(EdgeType type);

    /** Getters and Setters */
    BFXNode* getSource();
    BFXNode* getDestination();
    BFXEdge::EdgeType getType();
    void setSource(BFXNode* nSrc);
    void setDestination(BFXNode* nDst);
    void setType(EdgeType nType);

private:
    /** Private Variables */
    BFXNode* src;
    BFXNode* dst;
    EdgeType type;
};


#endif //BFX64_EDGE_H
