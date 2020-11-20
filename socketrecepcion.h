/*
 * socketrecepcion.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#ifndef SOCKETRECEPCION_H_
#define SOCKETRECEPCION_H_

#include <QTcpSocket>
#include "plentyfunctions.h"
#include "nodesstates.h"

class SocketRecepcion:public QTcpSocket
{
	Q_OBJECT
  public:
	SocketRecepcion(QObject *parent=0,int id=0,NodesStates * pointerOtStates=0,LLegadaColumnasIndOtherProc *pointerColumnsOtherProc=0);
	~SocketRecepcion();
	NodesStates *pointerOtStatesSk;
	int ID;
	LLegadaColumnasIndOtherProc *pointerColumnsOtherProcSR;

	int IdSender;


  signals:
    void ReceivedCol();
    void columnasRecibidas();
    void llegadaVsmall(double s,double c,int p,int q,int provID);

    void llegadaVectorInt();
    void llegadaVectorDouble();

    void signalFillTableComp(int RecID);
    void signalJabMeth_again();
    void signalAllVss(int RecID);
    void signalAllJacobiIEnd(int RecID);
    void signalAllJacobiIIEnd(int RecID);
    void signalInit_VarshaAlg();
  private slots:
	void LeerCliente();
  private:
	quint16 nextBlockSize;
};
#endif /* SOCKETRECEPCION_H_ */
