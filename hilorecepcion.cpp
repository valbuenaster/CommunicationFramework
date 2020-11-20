/*
 * hilorecepcion.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#include "hilorecepcion.h"
#include "socketrecepcion.h"

HiloRecepcion::HiloRecepcion(QObject *parent,int socketID,int id,NodesStates *pointerOtStates,LLegadaColumnasIndOtherProc *pointerColumnsOtherProc):QThread(parent)
{
	this->socketID=socketID;
	//imprimirMensaje("estamos en el constructor del hilo de recepcion");
	this->pointerOtStatesH=pointerOtStates;
	this->pointerColumnsOtherProcH=pointerColumnsOtherProc;
	this->ID=id;
}
void HiloRecepcion::run()
{
	this->socketrecepcion= new SocketRecepcion(0,ID,this->pointerOtStatesH,this->pointerColumnsOtherProcH);

	connect(this->socketrecepcion,SIGNAL(llegadaVectorInt()),this,SIGNAL(llegadaVectorIntH()));
	connect(this->socketrecepcion,SIGNAL(llegadaVectorDouble()),this,SIGNAL(llegadaVectorDoubleH()));
	connect(this->socketrecepcion,SIGNAL(ReceivedCol()),this,SIGNAL(ReceivedColH()));
	connect(this->socketrecepcion,SIGNAL(columnasRecibidas()),this,SIGNAL(columnasRecibidasH()));
	connect(this->socketrecepcion,SIGNAL(llegadaVsmall(double ,double ,int ,int ,int )),this,SIGNAL(llegadaVsmallH(double ,double ,int ,int ,int )));
	connect(this->socketrecepcion,SIGNAL(signalFillTableComp(int )),this,SIGNAL(signalFillTableCompH(int )));
	connect(this->socketrecepcion,SIGNAL(signalJabMeth_again()),this,SIGNAL(signalJabMeth_againH()));
	connect(this->socketrecepcion,SIGNAL(signalInit_VarshaAlg()),this,SIGNAL(signalInit_VarshaAlgH()));
	connect(this->socketrecepcion,SIGNAL(disconnected()),this,SLOT(quit()));
	connect(this->socketrecepcion,SIGNAL(signalAllVss(int )),this,SIGNAL(signalAllVssH(int )));
	connect(this->socketrecepcion,SIGNAL(signalAllJacobiIEnd(int )),this,SIGNAL(signalAllJacobiIEndH(int )));
	connect(this->socketrecepcion,SIGNAL(signalAllJacobiIIEnd(int )),this,SIGNAL(signalAllJacobiIIEndH(int )));
	connect(this,SIGNAL(finished ()),this,SLOT(decirQueSalio()));


	if((this->socketrecepcion->setSocketDescriptor(socketID)))
	{
		//imprimirMensaje("Listo, ya hizo el link...");
		//imprimirMensaje("Socket Descriptor = "+QString::number(socketrecepcion->socketDescriptor(),10));
	}
	this->exec();
/*	forever
	{

	}
*/
}
void HiloRecepcion::decirQueSalio()
{
	//imprimirMensaje("El socket se desconecto, deberia acabarse el hilo");
	this->terminate();
	this->wait(90);
}

