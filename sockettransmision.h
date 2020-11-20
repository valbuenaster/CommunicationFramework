/*
 * sockettransmision.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#ifndef SOCKETTRANSMISION_H_
#define SOCKETTRANSMISION_H_

#include <QTcpSocket>
#include <QString>
#include <QVector>

#include "plentyfunctions.h"

class SocketTransmision:public QObject
{
	Q_OBJECT
  public:
	void cerrarConexion();
	SocketTransmision(QObject *parent=0,QString DirIP="0.0.0.0",int puerto=5000,int id=0);
	int enviarSolicitud(double  Matriz[][NSIZE],int *columnsP,int Nelems);
	int enviarSolicitud(double  Matriz[][NSIZE],int columnsP);
	int enviarSolicitud(double* Vector,int TamVector);
	int enviarSolicitud(int* Vector,int TamVector);
	int enviarSolicitud(double s,double c,int p,int q);
	int enviarSolicitud(QString Palabra);
	//QTimer *marcadorSignal;
	quint8 n,m;
	int proc_ID;
	QTcpSocket *elSocket;
	bool enlazado;
	void connectarseAlServidor();
  private slots:
//	void enviarSolicitud();
//    void procedimientoTimermuere();
//    void connectarseAlServidor();
	void conexionCerradaPorServidor();
	void error();
  private:
	int puertoTransmision;
	QString DirIP;
	quint16 nextBlockSize;
};

#endif /* SOCKETTRANSMISION_H_ */
