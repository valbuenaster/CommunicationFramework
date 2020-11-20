/*
 * nodesstates.cpp
 *
 *  Created on: Mar 27, 2011
 *      Author: Luis Ariel Valbuena Reyes
 */

#include "nodesstates.h"

NodesStates::NodesStates()
{

}
NodesStates::~NodesStates()
{

}
void NodesStates::resizeArrays(int newSize)
{
	this->Nodes.resize(newSize);
	this->States.resize(newSize);
	this->sizeA=newSize;
}

