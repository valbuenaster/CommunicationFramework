/*
 * robot.h
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <QObject>
#include <QVector>
#include <QMutex>
#include <iostream>

#include "string.h"
#include "nodesstates.h"
#include "servidor.h"
#include "sockettransmision.h"
#include "plentyfunctions.h"

const int ExtPort[]={6000,7000,8000};//7000
const QString ArrayIPs[]={"192.168.1.105","192.168.1.116","192.168.1.108"};
//const QString ArrayIPs[]={"127.0.0.1","127.0.0.1","127.0.0.1"};

class Robot:public QObject
{
	Q_OBJECT
  public:

	int t,n,RobotID;
	int numberArrivals;
	int iterator_Varsha;
	int iterator_Jacobi;
	int*node_number;
	int sizeColumnasAlmacenadas;
	double *state;

	double MatrixPrasanna[NSIZE][NSIZE];

	bool flag_Nodes_already_Sent,flag_Nodes_already_Received;

	Servidor *Server;

	SocketTransmision * arregloDeSockets[NPROC];

	FILE* pointerIniNodes;
	FILE* pointerIniStates;

	FILE* OutputStates;

	QVector<int> IniNodesV;
	QVector<double> IniStatesV;

	QVector<double> d;
	QVector<double> q;
	QVector<double> results;

	NodesStates otherProcessors[NPROC];
	LLegadaColumnasIndOtherProc bufferColumnsArrival[2];

	Robot(int RbID);
	~Robot();

	void ComputeOutput(int n, double *RobState, double *d);
	void ComputeNextState( int p, double *RobState, int* RobNodeNumber,double *results, double* q);
	void ArrangeOutput(int l, int m, int n,  double *d1, double *d2,double *d3,int *Rob1NodeNumber, int *Rob2NodeNumber,int *Rob3NodeNumber, double *result);

  private:
	int iteradorColumnasJacobi;
	bool VarshaOption;
	bool JacobiOption;
	QVector<int> ColumnasAlmacenadas;
	QVector<int> top;
	QVector<int> botton;
	QVector<int> Ntop;
	QVector<int> Nbotton;
	ParamsVSmallOtherProc Buffer[NPROC];
	bool AllgotVsMatrices[NPROC];
	bool AllfinnishJacobiI[NPROC];
	bool AllfinnishJacobiII[NPROC];
	bool finalstage_JacobiII;
	bool bothcolumns_received;
	bool FlagRefere;
	bool FlagAllProc;
	int witnessJacobiIEnd;
	int witnessJacobiIIEnd;
	int witnessVsmallArrivals;
	int witnessStageBefJacobiII;
	double co;
	double si;
	double cp;
	double cq;

//	QMutex mutex1;
//	QMutex mutex2;
//	QMutex mutex3;
//	QMutex mutex4;

	QMutex mJMI;
	QMutex mJMII;
	QMutex mRefere;

  signals:
    void ack_GoNextPhase();
    void go_NextPhase();
    void signal_MakeSending();
    void go_JacobiII();
    void finalJacobiII();
    void signalEndJacobiI();
    void signalEndJacobiII();
    void signalsLeft(int columnsP);
    void signalsRight(int columnsP);
    void signalSinCos(double si,double co,int p,int q);
	void GotAllVs();
	void signalJacobiIagain();
	void signalPosColandENDCycle();
	void signalCheckRefere();

  private slots:
	void countingArrivals();
	void nextPhase();
	void hacerEnvio();//Borrar despues
	void SendWord();
	void SendSpark();
	void SendSpark_Jacobi();
	void SendAck();
	void SendPMidJM();
	void arrivalNodes();
	void arrivalStates();
	void verificationStageIIJM(int id);
	void verificationFinJacI(int id);
	void verificationFinJacII(int id);
    void JacobiMethod();
    void SendEndJacobiI();
    void SendEndJacobiII();
    void JacobiMethodII();

    void setJacobiOption();
    void setVarshaOption();
    void beginSelectedAlgorithm();

    void arrivalVsMatrixSiCos(double s,double c,int p,int q,int id);
    void SendSinCosine(double s,double c,int p,int q);
    void SendLeft(int columnsP);
    void SendRight(int columnsP);
    void positionColumn();
    void PosColandENDCycle();
    void final_Stage_Refere();

    void imprimirMP();
};

#endif /* ROBOT_H_ */
