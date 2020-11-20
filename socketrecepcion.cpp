/*
 * socketrecepcion.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#include <QTcpSocket>
#include <QVector>

#include "socketrecepcion.h"

SocketRecepcion::SocketRecepcion(QObject *parent,int id,NodesStates * pointerOtStates,LLegadaColumnasIndOtherProc *pointerColumnsOtherProc):QTcpSocket(parent)
{
	connect(this,SIGNAL(readyRead()),this,SLOT(LeerCliente()));
	connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
	this->setReadBufferSize(6300);
	this->pointerOtStatesSk=pointerOtStates;
	this->pointerColumnsOtherProcSR=pointerColumnsOtherProc;
	this->ID=id;
	nextBlockSize=0;
}
void SocketRecepcion::LeerCliente()
{
	QDataStream in(this);
	in.setVersion(QDataStream::Qt_4_1);
	if(nextBlockSize==0)
	{
		if (bytesAvailable()<sizeof(quint16))
			return;
		in>>nextBlockSize;
	}
	if(bytesAvailable()<nextBlockSize)return;

	int RecID;
	QString Caracteristica;
	QVector<int> columns;
	int Colum;
	QVector<double> VectorNumbers(NSIZE*NSIZE);
	QVector<double> Vector(NSIZE);
	QVector<double> DatosD;
	QVector<int> DatosI;

	int hh=0;
	double sn=0,cs=0;
	int i=0,j=0;
	int p=0,q=0;
	int temp;
	int Tama=0;

	in>>RecID;
	in>>Caracteristica;

	if(Caracteristica=="ColumnasMatSim")
	{
		in>>columns;
		in>>VectorNumbers;

		Tama=columns.size();

		for(i=0;i<columns.size();i++)
		{
			for(hh=0;hh<NSIZE;hh++)
			{
				j++;
			}
		}
		emit columnasRecibidas();
	}
	if(Caracteristica=="SingleColumn")
	{
		in>>Colum;
		in>>Vector;

		if(RecID==(this->ID - 1))
		{
			for(hh=0;hh<NSIZE;hh++)
			{
				this->pointerColumnsOtherProcSR[0].Columna[hh]=Vector[hh];
			}
			this->pointerColumnsOtherProcSR[0].NColumn=Colum;
			this->pointerColumnsOtherProcSR[0].newColumn=true;
		}
		if(RecID==(this->ID + 1))
		{
			for(hh=0;hh<NSIZE;hh++)
			{
				this->pointerColumnsOtherProcSR[1].Columna[hh]=Vector[hh];
			}
			this->pointerColumnsOtherProcSR[1].NColumn=Colum;
			this->pointerColumnsOtherProcSR[1].newColumn=true;
		}
//		imprimirMensaje("LLEGO DE "+QString::number(RecID,10)+" COLUMNA "+QString::number(Colum,10));
		emit ReceivedCol();//Crear otra seNal
	}
	if (Caracteristica=="Vsmallsc")
	{
		in>>cs;
		in>>sn;
		in>>p;
		in>>q;
//		imprimirMensaje("llego de "+QString::number(RecID,10)+" s "+QString::number(sn,'g',6)+" c "+QString::number(cs,'g',6)+" p "+QString::number(p,10)+" q "+QString::number(q,10));
		emit llegadaVsmall(sn,cs,p,q,RecID);
	}
	if((Caracteristica=="JacSec_Over"))
	{
		//imprimirMensaje("Emite la se~nal de que todos llegaron...");
		emit signalFillTableComp(RecID);
	}
	if((Caracteristica=="Init_JacSec"))
	{
//		imprimirMensaje("Emite la se~nal de que hay que empezar con Method again...");
		emit signalJabMeth_again();
	}
	if((Caracteristica=="Init_VarshaAlg"))
	{
//		imprimirMensaje("Emite la se~nal de que hay que empezar con Method again...");
		emit signalInit_VarshaAlg();
	}
	if((Caracteristica=="GotAllVs"))
	{
//		imprimirMensaje("Los demas ya tienen sus matrices VSmall...");
		emit signalAllVss(RecID);
	}
	if((Caracteristica=="signalEndJacobiI"))
	{
//		imprimirMensaje("Los demas ya tienen sus matrices VSmall...");
		emit signalAllJacobiIEnd(RecID);
	}
	if((Caracteristica=="signalEndJacobiII"))
	{
//		imprimirMensaje("Los demas ya tienen sus matrices VSmall...");
		emit signalAllJacobiIIEnd(RecID);
	}
	if((Caracteristica=="VectorD"))
	{
		in>>Tama;

		DatosD.resize(Tama);
		in>>DatosD;

		this->pointerOtStatesSk[RecID-1].resizeArrays(Tama);

  		for(hh=0;hh<Tama;hh++)
		{
  			this->pointerOtStatesSk[RecID-1].States[hh]=DatosD[hh];
//			imprimirMensaje("Datos que llegaron en socketreception "+QString::number(this->pointerOtStatesSk[RecID-1].States[hh],'g',6)+"\n");
		}
  		IdSender=RecID;
		emit llegadaVectorDouble();
	}
	if((Caracteristica=="VectorI"))
	{
		in>>Tama;

		DatosI.resize(Tama);
		in>>DatosI;

		this->pointerOtStatesSk[RecID-1].resizeArrays(Tama);

		for(hh=0;hh<Tama;hh++)
		{
			this->pointerOtStatesSk[RecID-1].Nodes[hh]=DatosI[hh];
//			imprimirMensaje("Datos que llegaron en socketreception "+QString::number(this->pointerOtStatesSk[RecID-1].Nodes[hh],10)+"\n");
		}
		IdSender=RecID;
		emit llegadaVectorInt();
	}
	nextBlockSize=0;
	close();
}
SocketRecepcion::~SocketRecepcion()
{
	//imprimirMensaje("Estamos en el destructor del socket de recepcion!");
}
