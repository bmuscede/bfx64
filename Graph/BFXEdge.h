/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXEdge.h
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

#ifndef BFX64_EDGE_H
#define BFX64_EDGE_H

#include "BFXNode.h"

class BFXEdge {
public:
    /** Enum For Type of Edge */
    enum EdgeType { CONTAINS, LINK };

    /** Constructor / Destructor */
    BFXEdge(BFXNode* src, BFXNode* dst, EdgeType type);
    BFXEdge(std::string src, std::string dst, EdgeType type);
    ~BFXEdge();

    /** Lookup Method */
    static std::string getTypeString(EdgeType type);

    /** Getters and Setters */
    BFXNode* getSource();
    BFXNode* getDestination();
    BFXEdge::EdgeType getType();
    std::string getSrcID();
    std::string getDstID();
    void setSource(BFXNode* nSrc);
    void setDestination(BFXNode* nDst);
    void setType(EdgeType nType);

private:
    /** Private Variables */
    BFXNode* src;
    BFXNode* dst;
    EdgeType type;
    bool lowMem;
    std::string sourceID;
    std::string destinationID;
};


#endif //BFX64_EDGE_H
