/*
 * sockettransmision.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QIODevice>

#include "plentyfunctions.h"
#include "sockettransmision.h"

SocketTransmision::SocketTransmision(QObject *parent,QString DirIP,int puerto,int id):QObject(parent)
{

	elSocket=new QTcpSocket();
	nextBlockSize=0;
	this->puertoTransmision=puerto;
	this->DirIP=DirIP;
	this->proc_ID=id;

	connect(this->elSocket,SIGNAL(disconnected()),this,SLOT(conexionCerradaPorServidor()));
	connect(this->elSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error()));
//	imprimirMensaje("DirIP "+this->DirIP+" puerto "+QString::number(this->puertoTransmision,10)+" id "+QString::number(this->proc_ID,10));

	n=0;
	m=0;
	this->enlazado=false;
//	imprimirMensaje("Constructor del socket transmision");
}

int SocketTransmision::enviarSolicitud(double  Matriz[][NSIZE],int *columnsP,int Nelems)//Falta especificar los argumentos de esta funcion
{
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	QString Number,espacio=" ",Linea;
	QString Caracteristica="ColumnasMatSim";
	QVector<double>VectorNumbers(NSIZE*NSIZE);
	QVector<int> columns(Nelems);
	int estado;
	for(m=0;m<Nelems;m++)
	{
		columns[m]=columnsP[m];
	}

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);

	int contador=0;

	for(m=0;m<(columns.size());m++)
	{
		for(n=0;n<NSIZE;n++)
		{
			VectorNumbers[contador]=Matriz[n][columns[m]];
			contador++;
		}
	}

	salida<<Tam<<(this->proc_ID)<<Caracteristica<<columns<<VectorNumbers;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);
	imprimirMensaje("Tempo = "+QString::number(tempo,10));
	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
		//imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");imprimirMensaje("logro enlazarse");
	}
	return estado;
}

int SocketTransmision::enviarSolicitud(double  Matriz[][NSIZE],int columnsP)//Falta especificar los argumentos de esta funcion
{
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	QString Number,espacio=" ",Linea;
	QString Caracteristica="SingleColumn";
	QVector<double>VectorNumbers(NSIZE);
	int columns,nn;
	int estado;

	columns=columnsP;

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);


	for(nn=0;nn<NSIZE;nn++)
	{
		VectorNumbers[nn]=Matriz[nn][columns];
	}
	salida<<Tam<<(this->proc_ID)<<Caracteristica<<columns<<VectorNumbers;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);
	imprimirMensaje("Tempo = "+QString::number(tempo,10));
	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
		//imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");
	}
	return estado;
}

int SocketTransmision::enviarSolicitud(double* Vector,int TamVect)//Falta especificar los argumentos de esta funcion
{
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	QString Number,espacio=" ",Linea;
	QString Caracteristica="VectorD";

	QVector<double> VectorNumbers(TamVect);

	int columns,nn;
	int estado;

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);

	for(nn=0;nn<TamVect;nn++)
	{
		VectorNumbers[nn]=Vector[nn];
	}

	salida<<Tam<<(this->proc_ID)<<Caracteristica<<TamVect<<VectorNumbers;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);

	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
		//imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");
	}
	return estado;
}
int SocketTransmision::enviarSolicitud(int* Vector,int TamVect)//Falta especificar los argumentos de esta funcion
{
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	QString Number,espacio=" ",Linea;
	QString Caracteristica="VectorI";

	QVector<int>VectorNumbers(TamVect);

	int columns,nn;
	int estado;

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);

	for(nn=0;nn<TamVect;nn++)
	{
		VectorNumbers[nn]=Vector[nn];
	}

	salida<<Tam<<(this->proc_ID)<<Caracteristica<<TamVect<<VectorNumbers;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);

	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
		//imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");
	}
	return estado;
}

int SocketTransmision::enviarSolicitud(double s,double c,int p,int q)//Falta especificar los argumentos de esta funcion
{										 //probablemente sera overloaded
	//imprimirMensaje("Se envia Solicitud");
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	QString Caracteristica="Vsmallsc";
	int estado;

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);

	salida<<Tam<<(this->proc_ID)<<Caracteristica<<c<<s<<p<<q;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);

	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
//		imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");
	}
	return estado;
}
int SocketTransmision::enviarSolicitud(QString Palabra)//Falta especificar los argumentos de esta funcion
{										 //probablemente sera overloaded
	//imprimirMensaje("Se envia Solicitud");
	while(this->elSocket->state()==6) ;
	connectarseAlServidor();
	quint16 Tam=0,tempo=0;
	QByteArray bloque;
	int estado;

	QDataStream salida(&bloque,QIODevice::WriteOnly);
	salida.setVersion(QDataStream::Qt_4_1);

	salida<<Tam<<(this->proc_ID)<<Palabra;
	salida.device()->seek(0);
	tempo=quint16(bloque.size()-sizeof(quint16));
	salida<<tempo;
	estado=elSocket->write(bloque);

	if(this->elSocket->waitForBytesWritten(600)==false)
	{
		imprimirMensaje("Dio false");
	}else
	{
	//	imprimirMensaje("Dio true");
	}
	this->elSocket->disconnectFromHost();
	if (this->elSocket->state() == QAbstractSocket::UnconnectedState || this->elSocket->waitForDisconnected(110))
	{
		//imprimirMensaje("Desconectado...");
	}
	return estado;
}
void SocketTransmision::conexionCerradaPorServidor()
{
	//imprimirMensaje("El servidor cerro la conexion");
	if(nextBlockSize!=0xFFFF){}
	elSocket->close();
}
void SocketTransmision::connectarseAlServidor()
{
		//imprimirMensaje("Tratando...");
		elSocket->connectToHost(this->DirIP,this->puertoTransmision);
//		imprimirMensaje("conectandose al host "+this->DirIP+"al puerto "+QString::number(this->puertoTransmision,10));

		if (elSocket->waitForConnected(150))
		{
			//imprimirMensaje("entrando en si del if");
		}else{
			imprimirMensaje("La falla esta en que no se pudo conectar al server");
		}
}
void SocketTransmision::error()
{
	imprimirMensaje("Se desconecto por el error...");
	elSocket->close();
}
