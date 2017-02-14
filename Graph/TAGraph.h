/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TAGraph.h
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Class that manages the graph containing the layout of
// the components stored in the object files. From this
// graph, this class can generate sections of a Tuple-Attribute
// file.
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

#ifndef BFX64_TAGRAPH_H
#define BFX64_TAGRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <bits/unordered_map.h>
#include "BFXNode.h"
#include "BFXEdge.h"

class TAGraph {
public:
    /** Constructor / Destructor */
    TAGraph(bool lowMemory = false);
    ~TAGraph();

    /** Node Operations */
    bool addNode(std::string ID, BFXNode::NodeType type, std::string name, std::string mangledName);
    bool removeNode(std::string ID);
    bool removeAllNodes();

    /** Edge Operations */
    bool addEdge(std::string srcID, std::string dstID, BFXEdge::EdgeType type);
    bool addEdgeByMangle(std::string srcID, std::string dstID, BFXEdge::EdgeType type);
    bool removeEdge(std::string srcID, std::string dstID, BFXEdge::EdgeType type);

    /** TA Generation Operations */
    std::string printInstances();
    std::string printRelationships();
    std::string printAttributes();

    /** Find Methods */
    bool doesContainEdgeExist(std::string srcID, std::string dstID);
    bool doesMangleEdgeExist(std::string srcID, std::string dstID);
private:
    /** Private Variables */
    std::unordered_map<std::string, BFXNode*> nodeList;
    std::unordered_map<std::string, BFXEdge*> edgeList;
    std::unordered_map<std::string, std::vector<std::string>> mangleList;
    std::unordered_map<std::string, bool> nodeBitList;
    std::unordered_map<std::string, bool> edgeBitList;

    bool lowMem = false;
    const std::string INSTANCE_FLAG = "$INSTANCE";
    const std::string ATT_LABEL = "label";

    /** Helper Methods */
    BFXNode* findNode(std::string ID);
    std::string findNodeIDByMangle(std::string mangle);
    BFXNode* findNodeByMangle(std::string mangle);
    BFXEdge* findEdge(std::string src, std::string dst, BFXEdge::EdgeType type);
    bool IDExists(std::string ID);
    void removeAllEdges(std::string ID);
};

#endif //BFX64_TAGRAPH_H
