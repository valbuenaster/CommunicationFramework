/*
 * hilorecepcion.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#ifndef HILORECEPCION_H_
#define HILORECEPCION_H_

#include <QThread>

#include "socketrecepcion.h"
#include "nodesstates.h"

class HiloRecepcion:public QThread
{
	Q_OBJECT
  public:
	HiloRecepcion(QObject *parent=0,int socketID=0,int id=0,NodesStates * pointerOtStates=0,LLegadaColumnasIndOtherProc *pointerColumnsOtherProc=0);
	SocketRecepcion *socketrecepcion;
	NodesStates *pointerOtStatesH;
	LLegadaColumnasIndOtherProc *pointerColumnsOtherProcH;
	int ID;

	int IdSender;
	QVector<int> IniNodesH;
	QVector<double> IniStatesH;

	void run();
  signals:
    void ReceivedColH();
	void columnasRecibidasH();
	void llegadaVsmallH(double s,double c,int p,int q,int provID);
	void signalFillTableCompH(int RecID);
	void signalJabMeth_againH();
	void signalInit_VarshaAlgH();
	void signalAllVssH(int RecID);
	void signalAllJacobiIEndH(int RecID);
	void signalAllJacobiIIEndH(int id);

    void llegadaVectorIntH();
    void llegadaVectorDoubleH();

  private:
	int socketID;
  private slots:
    void decirQueSalio();
};

#endif /* HILORECEPCION_H_ */
