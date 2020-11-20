/*
 * nodesstates.h
 *
 *  Created on: Mar 27, 2011
 *      Author: ariel
 */

#ifndef NODESSTATES_H_
#define NODESSTATES_H_

#include <QObject>
#include <QVector>
#include "plentyfunctions.h"
/*
class Robot:public QObject
{
	Q_OBJECT
  public:
*/
class NodesStates:public QObject
{
	Q_OBJECT
  public:

	QVector<int> Nodes;
	QVector<double> States;

	int sizeA;

	NodesStates();
	void resizeArrays(int);
	~NodesStates();
};
#endif /* NODESSTATES_H_ */
