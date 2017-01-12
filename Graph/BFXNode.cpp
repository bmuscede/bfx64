/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXNode.cpp
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Defines the format of an node in the object file dependency graph.
// Contains an enum with a collection of node types. These define how
// the TA file is laid out.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "BFXNode.h"

using namespace std;

/**
 * Creates a new BFX node.
 * @param ID The ID of the node.
 * @param type The type of node.
 * @param name The name of the node (unmangled).
 * @param mangledNames A vector of unmangled values (all should tranlsate to name).
 */
BFXNode::BFXNode(string ID, NodeType type, string name, vector<string> mangledNames){
    //Sets up all the fields.
    this->ID = ID;
    this->type = type;
    this->name = name;
    this->mangledNames = mangledNames;
}

/**
 * Default destructor.
 */
BFXNode::~BFXNode(){ }

/**
 * Lookup to translate the enum for node type to a string.
 * @param type The type to lookup.
 * @return The string equivalent.
 */
string BFXNode::getTypeString(NodeType type){
    //Simple switch statement for lookup.
    if (type == NodeType::FILE){
        return "cObjectFile";
    } else if (type == NodeType::FUNCTION){
        return "cFunction";
    } else if (type == NodeType::OBJECT){
        return "cObject";
    } else if (type == NodeType::SUBSYSTEM){
        return "cSubSystem";
    }

    //If the lookup fails, puts item at top level.
    //This shouldn't happen.
    return "cRoot";
}

/**
 * Gets the node ID.
 * @return The node ID.
 */
string BFXNode::getID() {
    return ID;
}

/**
 * Gets the type of node.
 * @return The type of node.
 */
BFXNode::NodeType BFXNode::getType(){
    return type;
}

/**
 * Gets the name (unmangled).
 * @return The unmangled name.
 */
string BFXNode::getName(){
    return name;
}

/**
 * Gets all mangled names that translate over.
 * @return All mangled names.
 */
vector<string> BFXNode::getMangledNames() {
    return mangledNames;
}

/**
 * Sets the node ID.
 * @param nID The new node ID.
 */
void BFXNode::setID(string nID){
    ID = nID;
}

/**
 * Sets the node type.
 * @param nType The new node type.
 */
void BFXNode::setType(NodeType nType){
    type = nType;
}

/**
 * Sets the name.
 * @param nName The new node unmangled name.
 */
void BFXNode::setName(std::string nName){
    name = nName;
}

/**
 * Adds a new mangled name to the mangle list.
 * @param nMangle The name to add.
 */
void BFXNode::addMangledName(string nMangle) {
    mangledNames.push_back(nMangle);
}

/**
 * Checks whether a mangled name exists in the mangle list.
 * @param name The name to look for.
 * @return Boolean indicating whether it was found.
 */
bool BFXNode::doesMangledNameExist(std::string name){
    //Iterate through our vector and look.
    for (int i = 0; i < mangledNames.size(); i++){
        if (mangledNames.at(i).compare(name) == 0)
            return true;
    }

    return false;
}