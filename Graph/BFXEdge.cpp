/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXEdge.cpp
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Defines the format of an edge in the object file dependency graph.
// Contains an enum with a collection of edge types. These define how
// the TA file is laid out.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <string>
#include "BFXEdge.h"

using namespace std;

/**
 * Creates a new edge.
 * @param src The source node.
 * @param dst The destination node.
 * @param type The type of edge.
 */
BFXEdge::BFXEdge(BFXNode* src, BFXNode* dst, EdgeType type){
    this->src = src;
    this->dst = dst;
    this->type = type;
}

/**
 * Default destructor.
 */
BFXEdge::~BFXEdge() { }

/**
 * Lookup function to translate the type enum
 * to a string value.
 * @param type The enum value.
 * @return The string equivalent of that enum.
 */
string BFXEdge::getTypeString(EdgeType type){
    //Simply lookup the edge type value.
    if (type == EdgeType::CONTAINS){
        return "contain";
    } else if (type == EdgeType::LINK){
        return "reference";
    }

    //If all else fails, return default.
    return "unknown";
}

/**
 * Gets the source node.
 * @return The source node.
 */
BFXNode* BFXEdge::getSource(){
    return src;
}

/**
 * Gets the destination node.
 * @return The destination node.
 */
BFXNode* BFXEdge::getDestination(){
    return dst;
}

/**
 * Gets the edge type.
 * @return The edge type.
 */
BFXEdge::EdgeType BFXEdge::getType(){
    return type;
}

/**
 * Sets the source node.
 * @param nSrc A pointer to the source node.
 */
void BFXEdge::setSource(BFXNode* nSrc){
    src = nSrc;
}

/**
 * Sets the destination node.
 * @param nDst A pointer to the destination node.
 */
void BFXEdge::setDestination(BFXNode* nDst){
    dst = nDst;
}

/**
 * Sets the edge type.
 * @param nType The new edge type.
 */
void BFXEdge::setType(EdgeType nType){
    type = nType;
}