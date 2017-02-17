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
TAGraph::TAGraph(bool lowMemory){
    //Create a blank graph.
    nodeList = unordered_map<string, BFXNode*>();
    edgeList = unordered_map<string, BFXEdge*>();
    mangleList = unordered_map<string, vector<string>>();

    //Sets the low memory flag.
    if (lowMemory) {
        this->lowMem = lowMemory;
        nodeBitList = unordered_map<string, bool>();
        edgeBitList = unordered_map<string, bool>();
    }
}

/**
 * Destructor for TAGraph. Deletes all the
 * nodes and edges in the graph.
 */
TAGraph::~TAGraph(){
    //Delete all sub elements in vectors.
    for (auto it = edgeList.begin(); it != edgeList.end(); it++)
        delete it->second;
    edgeList.erase(edgeList.begin(), edgeList.end());
    for (auto it = nodeList.begin(); it != nodeList.end(); it++)
        delete it->second;
    nodeList.erase(nodeList.begin(), nodeList.end());
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

    //Adds the node to the bit list.
    if (lowMem) nodeBitList[ID] = true;

    return true;
}

bool TAGraph::removeAllNodes(){
    for (auto it = edgeList.begin(); it != edgeList.end(); it++)
        delete it->second;
    edgeList.erase(edgeList.begin(), edgeList.end());
    for (auto it = nodeList.begin(); it != nodeList.end(); it++)
        delete it->second;
    nodeList.erase(nodeList.begin(), nodeList.end());

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
    nodeList.erase(ID);

    //Removes all edges.
    removeAllEdges(ID);
    delete node;
    return true;
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
    BFXEdge* newEdge;

    //Adds the edge depending on the memory type.
    if (lowMem){
        //Check if the edges exist.
        if (!nodeBitList[srcID] || !nodeBitList[dstID]) return false;

        //Add the edge by ID.
        newEdge = new BFXEdge(srcID, dstID, type);
    } else {
        //Get the two nodes.
        BFXNode* src = findNode(srcID);
        BFXNode* dst = findNode(dstID);

        //Check if they exist.
        if (src == nullptr || dst == nullptr) return false;

        //Create the edge.
        newEdge = new BFXEdge(src, dst, type);
    }

    edgeList[srcID + dstID + BFXEdge::getTypeString(type)] = newEdge;

    //If we're in low memory mode, set the edge field.
    if (lowMem) edgeBitList[srcID + dstID + BFXEdge::getTypeString(type)] = true;

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
    BFXEdge* newEdge;

    if (lowMem){
        string src = findNodeIDByMangle(srcID);
        string dst = findNodeIDByMangle(dstID);
        if (src.compare("") == 0 || dst.compare("") == 0) return false;

        //Check if the nodes exist.
        if (!nodeBitList[src] || !nodeBitList[dst]) return false;

        newEdge = new BFXEdge(src, dst, type);
    } else {
        //Get the two nodes.
        BFXNode* src = findNodeByMangle(srcID);
        BFXNode* dst = findNodeByMangle(dstID);

        //Check if they exist.
        if (src == nullptr || dst == nullptr) return false;

        //Create the edge.
        newEdge = new BFXEdge(src, dst, type);
    }

    edgeList[newEdge->getSrcID() + newEdge->getDstID() + BFXEdge::getTypeString(type)] = newEdge;

    //If we're in low memory mode, set the edge field.
    if (lowMem) edgeBitList[newEdge->getSrcID() + newEdge->getDstID() + BFXEdge::getTypeString(type)] = true;

    return true;
}

/**
 * Removes an edge based on a source ID and destination
 * ID.
 * @param srcID Source ID of the edge to remove.
 * @param dstID Destination ID of the edge to remove.
 * @param type The edge type to remove.
 * @return Boolean indicating success.
 */
bool TAGraph::removeEdge(string srcID, string dstID, BFXEdge::EdgeType type) {
    //Check if a node exists.
    BFXEdge* edge = edgeList[srcID + dstID + BFXEdge::getTypeString(type)];
    if (edge == nullptr) return false;

    //Set nullptr on the edge.
    edgeList.erase(srcID + dstID + BFXEdge::getTypeString(type));
    delete edge;

    return true;
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
    for (auto it = edgeList.begin(); it != edgeList.end(); it++){
        BFXEdge* currEdge = it->second;
        if (currEdge == nullptr) continue;
        relationships += BFXEdge::getTypeString(currEdge->getType()) + " " + currEdge->getSrcID() + " " +
                currEdge->getDstID() + "\n";
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
    if (lowMem) {
        if (edgeBitList[srcID + dstID + BFXEdge::getTypeString(BFXEdge::CONTAINS)]) return true;
        return false;
    }

    //Check whether we can find edges of the type.
    BFXEdge* results = findEdge(srcID, dstID, BFXEdge::CONTAINS);
    if (results != nullptr) return true;

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
    //Gets the mangled src and dst nodes.
    vector<string> src = mangleList[srcID];
    vector<string> dst = mangleList[dstID];
    if (src.size() == 0 || dst.size() == 0) return false;

    //Searches for the edge.
    for (auto curSrc : src){
        for (auto curDst : dst){
            //Check if we're low mem or not.
            if (lowMem){
                if (edgeBitList[curSrc + curDst + BFXEdge::getTypeString(BFXEdge::CONTAINS)] == true ||
                    edgeBitList[curSrc + curDst + BFXEdge::getTypeString(BFXEdge::LINK)] == true)
                    return true;
            } else {
                if (edgeList[curSrc + curDst + BFXEdge::getTypeString(BFXEdge::CONTAINS)] != nullptr ||
                    edgeList[curSrc + curDst + BFXEdge::getTypeString(BFXEdge::LINK)] != nullptr)
                    return true;
            }
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

string TAGraph::findNodeIDByMangle(string mangle){
    //Goes through the mangle list.
    vector<string> nodeIDs = mangleList[mangle];
    if (nodeIDs.size() == 0) return string();

    //TODO: What should we return if there is more than one?
    return nodeIDs.at(0);
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
 * Finds an edge in the graph that match a source and destination and type.
 * @param src The source ID of the edge.
 * @param dst The destination ID of the edge.
 * @param type The type of edge. (See BFXEdge)
 * @return A pointers to the found edge (or nullptr).
 */
BFXEdge* TAGraph::findEdge(string src, string dst, BFXEdge::EdgeType type) {
    //Gets the edge.
    BFXEdge* edge = edgeList[src + dst + BFXEdge::getTypeString(type)];
    return edge;
}

/**
 * Checks whether a particular ID exists.
 * @param ID The ID to look for.
 * @return Boolean indicating whether the node was found.
 */
bool TAGraph::IDExists(std::string ID){
    if (lowMem) return nodeBitList[ID];

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
    for (auto it = edgeList.begin(); it != edgeList.end(); it++){
        BFXEdge* current = it->second;
        if (current == nullptr) continue;

        //Checks if the edge is a source or destination.
        if (current->getSrcID().compare(ID) == 0 ||
                current->getDstID().compare(ID) == 0){
            BFXEdge* edge =
                    edgeList[current->getSrcID() + current->getDstID() + BFXEdge::getTypeString(current->getType())];

            //Set nullptr on the edge.
            it = edgeList.erase(it);
            delete edge;
        }
    }
}