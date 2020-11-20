/*
 * servidor.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <QTcpServer>
#include "plentyfunctions.h"
#include "nodesstates.h"
#include "socketrecepcion.h"

class Servidor: public QTcpServer
{
	Q_OBJECT
  public:
	Servidor(int rID,bool leading,NodesStates * pointerOtStates,LLegadaColumnasIndOtherProc *pointerColumnsOtherProc);
	~Servidor();
	int Conexiones;

	int ID;
	double estimador;
	NodesStates *pointerOtStatesSER;
	LLegadaColumnasIndOtherProc *pointerColumnsOtherProcSER;

  protected:
	void incomingConnection(int socketID);
  signals:
	void makeSpark();
	void signalPMidJM();
	void signalJabMeth_againSER();
	void signalInit_VarshaAlgSER();
	void llegadaVsmallSER(double s,double c,int p,int q,int id);

	void llegadaVectorIntSER();
	void llegadaVectorDoubleSER();
	void signalAllVssSER(int id);
	void signalAllJacobiIEndSER(int id);
	void signalAllJacobiIIEndSER(int id);
	void ReceivedColSER();

  private:
	SocketRecepcion *socketL;
	int a;
  private slots:
//    void organizarMatriz();
    void impMat();//Borrar esta se~nal despues
//    void moveNodesSER(int sID);
//    void moveStatesSER(int sID);
};

#endif /* SERVIDOR_H_ */
