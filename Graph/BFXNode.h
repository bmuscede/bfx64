/////////////////////////////////////////////////////////////////////////////////////////////////////////
// BFXNode.h
//
// Created By: Bryan J Muscedere
// Date: 09/14/2016
//
// Defines the format of an node in the object file dependency graph.
// Contains an enum with a collection of node types. These define how
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

#ifndef BFX64_NODE_H
#define BFX64_NODE_H

#include <string>
#include <vector>

class BFXNode {
public:
    /** Enum For Type of Node */
    enum NodeType { FILE, OBJECT, FUNCTION, SUBSYSTEM };

    /** Constructor / Destructor */
    BFXNode(std::string ID, NodeType type, std::string name, std::vector<std::string> mangledNames);
    ~BFXNode();

    /** Lookup Method */
    static std::string getTypeString(NodeType type);

    /** Getters and Setters */
    std::string getID();
    BFXNode::NodeType getType();
    std::string getName();
    std::vector<std::string> getMangledNames();
    void setID(std::string nID);
    void setType(NodeType nType);
    void setName(std::string nName);
    void addMangledName(std::string nMangle);

    /** Lookup Methods */
    bool doesMangledNameExist(std::string name);

private:
    /** Private Variables */
    std::string ID;
    NodeType type;
    std::string name;
    std::vector<std::string> mangledNames;
};


#endif //BFX64_NODE_H
