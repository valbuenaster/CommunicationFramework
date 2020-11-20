/*
 * servidor.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include "nodesstates.h"
#include "servidor.h"
#include "hilorecepcion.h"
#include "plentyfunctions.h"

Servidor::Servidor(int rID,bool leading,NodesStates * pointerOtStates,LLegadaColumnasIndOtherProc * pointerColumnsOtherProc):QTcpServer()
{
	int iter=0;
	Conexiones=0;
	this->ID=rID;
	this->pointerOtStatesSER=pointerOtStates;
	this->pointerColumnsOtherProcSER=pointerColumnsOtherProc;
	imprimirMensaje("Creamos un servidor");
}

Servidor::~Servidor()
{
	imprimirMensaje("Se murio el servidor");
	this->close();
}
void Servidor::incomingConnection(int socketID)
{
 	HiloRecepcion *Temp=new HiloRecepcion(this,socketID,this->ID,this->pointerOtStatesSER,this->pointerColumnsOtherProcSER);
	connect(Temp,SIGNAL(llegadaVectorIntH()),this,SIGNAL(llegadaVectorIntSER()));
	connect(Temp,SIGNAL(llegadaVectorDoubleH()),this,SIGNAL(llegadaVectorDoubleSER()));
	connect(Temp,SIGNAL(signalJabMeth_againH()),this,SIGNAL(signalJabMeth_againSER()));
	connect(Temp,SIGNAL(signalInit_VarshaAlgH()),this,SIGNAL(signalInit_VarshaAlgSER()));
	connect(Temp,SIGNAL(llegadaVsmallH(double ,double ,int ,int ,int )),this,SIGNAL(llegadaVsmallSER(double ,double ,int ,int ,int )));
	connect(Temp,SIGNAL(signalAllVssH(int )),this,SIGNAL(signalAllVssSER(int )));
	connect(Temp,SIGNAL(signalAllJacobiIEndH(int )),this,SIGNAL(signalAllJacobiIEndSER(int )));
	connect(Temp,SIGNAL(signalAllJacobiIIEndH(int )),this,SIGNAL(signalAllJacobiIIEndSER(int )));
	connect(Temp,SIGNAL(ReceivedColH()),this,SIGNAL(ReceivedColSER()));
	connect(Temp, SIGNAL(finished()), Temp, SLOT(deleteLater()));
	Temp->start();
	Conexiones+=1;

	if(Temp->isFinished())imprimirMensaje("El hilo ya esta muerto");
}

void Servidor::impMat()
{
//	imprimirMatriz(MatrizRecibida);
}
