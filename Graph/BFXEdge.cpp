/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXEdge.cpp
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Defines the format of an edge in the object file dependency graph.
// Contains an enum with a collection of edge types. These define how
// the TA file is laid out.
//
// Copyright (C) 2017, Bryan J. Muscedere
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
    this->lowMem = false;
    this->src = src;
    this->dst = dst;
    this->type = type;
}

BFXEdge::BFXEdge(string src, string dst, EdgeType type){
    this->lowMem = true;
    this->sourceID = src;
    this->destinationID = dst;
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

string BFXEdge::getSrcID(){
    if (lowMem) return sourceID;
    return src->getID();
}

string BFXEdge::getDstID(){
    if (lowMem) return destinationID;
    return dst->getID();
}

/**
 * Sets the source node.
 * @param nSrc A pointer to the source node.
 */
void BFXEdge::setSource(BFXNode* nSrc){
    lowMem = false;
    src = nSrc;
}

/**
 * Sets the destination node.
 * @param nDst A pointer to the destination node.
 */
void BFXEdge::setDestination(BFXNode* nDst){
    lowMem = false;
    dst = nDst;
}

/**
 * Sets the edge type.
 * @param nType The new edge type.
 */
void BFXEdge::setType(EdgeType nType){
    type = nType;
}