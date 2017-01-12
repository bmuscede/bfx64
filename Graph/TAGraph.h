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
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFX64_TAGRAPH_H
#define BFX64_TAGRAPH_H

#include <string>
#include <vector>
#include "BFXNode.h"
#include "BFXEdge.h"

class TAGraph {
public:
    /** Constructor / Destructor */
    TAGraph();
    ~TAGraph();

    /** Node Operations */
    bool addNode(std::string ID, BFXNode::NodeType type, std::string name, std::string mangledName);
    bool removeNode(std::string ID);

    /** Edge Operations */
    bool addEdge(std::string srcID, std::string dstID, BFXEdge::EdgeType type);
    bool addEdgeByMangle(std::string srcID, std::string dstID, BFXEdge::EdgeType type);
    bool removeEdge(std::string srcID, std::string dstID);

    /** TA Generation Operations */
    std::string printInstances();
    std::string printRelationships();
    std::string printAttributes();

    /** Find Methods */
    bool doesContainEdgeExist(std::string srcID, std::string dstID);
    bool doesMangleEdgeExist(std::string srcID, std::string dstID);
private:
    /** Private Variables */
    std::vector<BFXNode*> nodeList;
    std::vector<BFXEdge*> edgeList;
    const std::string INSTANCE_FLAG = "$INSTANCE";
    const std::string ATT_LABEL = "label";

    /** Helper Methods */
    BFXNode* findNode(std::string ID);
    BFXNode* findNodeByMangle(std::string mangle);
    std::vector<BFXEdge*> findEdges(std::string src, std::string dst, BFXEdge::EdgeType type);
    bool IDExists(std::string ID);
    void removeAllEdges(std::string ID);
};

#endif //BFX64_TAGRAPH_H
