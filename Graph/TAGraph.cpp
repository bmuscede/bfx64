/////////////////////////////////////////////////////////////////////////////////////////////////////////
// TAGraph.cpp
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

#include "TAGraph.h"

using namespace std;

/**
 * Constructor that creates a blank graph of
 * nodes and edges. Takes no parameters to
 * initialize.
 */
TAGraph::TAGraph(){
    //Create a blank graph.
    nodeList = unordered_map<string, BFXNode*>();
    edgeList = vector<BFXEdge*>();
    mangleList = unordered_map<string, vector<string>>();
}

/**
 * Destructor for TAGraph. Deletes all the
 * nodes and edges in the graph.
 */
TAGraph::~TAGraph(){
    //Delete all sub elements in vectors.
    for (auto it = nodeList.begin(); it != nodeList.end(); it++)
        delete it->second;
    for (int i = 0; i < edgeList.size(); i++)
        delete edgeList.at(i);
}

/**
 * Adds a new node to the graph. No connections
 * are set. If the node exists, then checks if the
 * mangled name matches. If not, we add it.
 * @param ID The proper ID of the node.
 * @param type The node type (See BFXNode)
 * @param name The name of the node (unmangled).
 * @param mangledName The mangled name of the node.
 * @return Boolean indicating whether the node was added.
 */
bool TAGraph::addNode(string ID, BFXNode::NodeType type, string name, string mangledName) {
    BFXNode* curr = findNode(ID);

    //Search to see if ID exists.
    if (curr != nullptr){
        //Check if we should add the mangled name.
        if (!curr->doesMangledNameExist(mangledName)) {
            curr->addMangledName(mangledName);
            mangleList[mangledName].push_back(ID);
        }
        return false;
    }

    //Start by creating a new node.
    vector<string> mangledNames = vector<string>();
    mangledNames.push_back(mangledName);

    //Adds the mangled name to the list.
    mangleList[mangledName].push_back(ID);

    BFXNode* newNode = new BFXNode(ID, type, name, mangledNames);
    nodeList[ID] = newNode;
    return true;
}

/**
 * Removes a node from the graph based on it's ID.
 * @param ID The ID to remove.
 * @return Boolean indicating success.
 */
bool TAGraph::removeNode(string ID) {
    //Get the node.
    BFXNode* node = nodeList[ID];
    if (node == nullptr) return false;

    //Deletes the node.
    delete node;
    nodeList[ID] = nullptr;

    //Removes all edges.
    removeAllEdges(ID);

    return false;
}

/**
 * Adds an edge to the graph based on two already added nodes. The IDs
 * must exist for the edge to be created.
 * @param srcID The source node ID.
 * @param dstID The destination node ID.
 * @param type The type of edge (See BFXEdge)
 * @return Boolean indicating success.
 */
bool TAGraph::addEdge(string srcID, string dstID, BFXEdge::EdgeType type) {
    //Get the two nodes.
    BFXNode* src = findNode(srcID);
    BFXNode* dst = findNode(dstID);

    //Check if they exist.
    if (src == nullptr || dst == nullptr) return false;

    //Create the edge.
    BFXEdge* newEdge = new BFXEdge(src, dst, type);
    edgeList.push_back(newEdge);
    return true;
}

/**
 * Adds an edge to the graph based on two already added nodes. Checks
 * based on mangle name and NOT ID name.
 * @param srcID The mangle ID of the source.
 * @param dstID The mangle ID of the destination.
 * @param type The type of edge (See BFXEdge)
 * @return Boolean indicating success.
 */
bool TAGraph::addEdgeByMangle(string srcID, string dstID, BFXEdge::EdgeType type){
    //Get the two nodes.
    BFXNode* src = findNodeByMangle(srcID);
    BFXNode* dst = findNodeByMangle(dstID);

    //Check if they exist.
    if (src == nullptr || dst == nullptr) return false;

    //Create the edge.
    BFXEdge* newEdge = new BFXEdge(src, dst, type);
    edgeList.push_back(newEdge);
    return true;
}

/**
 * Removes an edge based on a source ID and destination
 * ID.
 * @param srcID Source ID of the edge to remove.
 * @param dstID Destination ID of the edge to remove.
 * @return Boolean indicating success.
 */
bool TAGraph::removeEdge(string srcID, string dstID) {
    //Iterate until we find it.
    for (int i = 0; i < edgeList.size(); i++){
        //Get the source and destination.
        string curSrc = edgeList.at(i)->getSource()->getID();
        string curDst = edgeList.at(i)->getDestination()->getID();

        //Now, check the src and dst.
        if (curSrc.compare(srcID) == 0 && curDst.compare(dstID) == 0){
            edgeList.erase(edgeList.begin() + i);
            return true;
        }
    }

    return false;
}

/**
 * Based on the TA standard, prints all instances of all
 * entities in the graph. Generates a string as per the
 * TA standard.
 * @return String with instances in TA format.
 */
string TAGraph::printInstances() {
    string instance = "";

    //Iterate through the nodes and print their details.
    for (auto it = nodeList.begin(); it != nodeList.end(); it++){
        BFXNode* curr = it->second;
        instance += INSTANCE_FLAG + " " + curr->getID() + " " + BFXNode::getTypeString(curr->getType()) + "\n";
    }

    return instance;
}

/**
 * Based on the TA standard, prints all edges in the graph
 * formatted in RSF. Generates the edge name, then the source
 * ID, followed by the destination ID.
 * @return String with relationships in TA format.
 */
string TAGraph::printRelationships() {
    string relationships = "";

    //Iterate through the edges and print the details of each edge.
    for (int i = 0; i < edgeList.size(); i++){
        BFXEdge* curr = edgeList.at(i);
        relationships += BFXEdge::getTypeString(curr->getType()) + " " + curr->getSource()->getID() + " " + curr->getDestination()->getID() + "\n";
    }

    return relationships;
}

/**
 * Based on the TA standard, prints all nodes along with their attributes. This
 * includes their label. Does not print mangled name since this is for internal
 * use only.
 * @return String with attributes in TA format.
 */
string TAGraph::printAttributes(){
    string attributes = "";

    //Iterate through the nodes and print their details.
    for (auto it = nodeList.begin(); it != nodeList.end(); it++){
        BFXNode* curr = it->second;

        //Check if we have no label.
        if (curr->getName().compare("") == 0) continue;

        //Otherwise, we add an attribute entry.
        attributes += curr->getID() + " { " + ATT_LABEL + " = \"" + curr->getName() + "\" }\n";
    }

    return attributes;
}

/**
 * Checks whether an edge exists based on the source and destination
 * IDs.
 * @param srcID The source ID
 * @param dstID The destination ID
 * @return Boolean indicating whether the edge was found.
 */
bool TAGraph::doesContainEdgeExist(string srcID, string dstID){
    //Check whether we can find edges of the type.
    vector<BFXEdge*> results = findEdges(srcID, dstID, BFXEdge::CONTAINS);
    if (results.size() > 0) return true;

    return false;
}

/**
 * Checks whether an edge exists based on the source and destination
 * mangle values. Checks if ONE of the mangle values exist.
 * @param srcID Source mangled value.
 * @param dstID Destination mangle value.
 * @return
 */
bool TAGraph::doesMangleEdgeExist(string srcID, string dstID){
    //Check whether we can find edges of the type.
    for (int i = 0; i < edgeList.size(); i++){
        BFXEdge* curEdge = edgeList.at(i);
        if (curEdge->getSource()->doesMangledNameExist(srcID) &&
            curEdge->getDestination()->doesMangledNameExist(dstID) &&
            curEdge->getType() == BFXEdge::LINK){
            return true;
        }
    }

    return false;
}

/**
 * Finds a node in the graph based on its ID.
 * @param ID The ID of the node.
 * @return A pointer to the BFXNode.
 */
BFXNode* TAGraph::findNode(std::string ID){
    //Iterate through and search.
    return nodeList[ID];
}

/**
 * Finds a node in the graph based on its mangle
 * value.
 * @param mangle The mangle value of the node.
 * @return A pointer to the BFXNode.
 */
BFXNode* TAGraph::findNodeByMangle(string mangle){
    //Goes through the mangle list.
    vector<string> nodeIDs = mangleList[mangle];
    if (nodeIDs.size() == 0) return nullptr;

    //TODO: What should we return if there is more than one?
    return nodeList[nodeIDs.at(0)];
}

/**
 * Finds all edges in the graph that match a source and destination and type.
 * @param src The source ID of the edge.
 * @param dst The destination ID of the edge.
 * @param type The type of edge. (See BFXEdge)
 * @return A vector with pointers to all found edges.
 */
vector<BFXEdge *> TAGraph::findEdges(string src, string dst, BFXEdge::EdgeType type) {
    vector<BFXEdge*> edges = vector<BFXEdge*>();

    //Iterate through the edge list to search.
    for (int i = 0; i < edgeList.size(); i++){
        BFXEdge* curEdge = edgeList.at(i);
        if (curEdge->getSource()->getID().compare(src) == 0 &&
            curEdge->getDestination()->getID().compare(dst) == 0 &&
            type == curEdge->getType()){
            edges.push_back(curEdge);
        }
    }

    return edges;
}

/**
 * Checks whether a particular ID exists.
 * @param ID The ID to look for.
 * @return Boolean indicating whether the node was found.
 */
bool TAGraph::IDExists(std::string ID){
    if (findNode(ID) == nullptr)
        return false;

    return true;
}

/**
 * Removes all edges based on some particular ID.
 * @param ID The ID to remove all edges by.
 */
void TAGraph::removeAllEdges(std::string ID){
    //Iterate through edge list and delete edges that match.
    for (int i = 0; i< edgeList.size(); i++){
        BFXEdge* edge = edgeList.at(i);

        //Check src and dst.
        if (edge->getDestination()->getID().compare(ID) == 0 ||
            edge->getSource()->getID().compare(ID) == 0){
            //Remove the edge list.
            edgeList.erase(edgeList.begin() + i);
        }
    }
}