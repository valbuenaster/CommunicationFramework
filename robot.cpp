/*
 * robot.cpp
 *
 *  Created on: Jul 10, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>

#include "robot.h"
#include "servidor.h"
#include "sockettransmision.h"

//USAR QVector
//US
Robot::Robot(int RbID):QObject()
{
	QVector<int> tempLoc(NSIZE/NPROC);
	/*******/
		//1
	this->RobotID=RbID;
	numberArrivals=0;
	witnessVsmallArrivals=1;
//TODO: THIS SHOULD BE IN A FUNCTION
	char * buffer1;
	char * buffer2;
	int size1,size2,joker,modulusresult;
	double resulta=0;
	char *Fi;
	char comma[]=",",Temp[18];
	size_t result1,result2;

	for(joker=0;joker<NSIZE;joker++)
	{
//		listcolumns.append(iter);
		modulusresult=joker % 2;
		if((modulusresult)==0)
		{
			this->top.append(joker);
		}else
		{
			this->botton.append(joker);
		}
	}
	for(joker=0;joker<NPROC;joker++)
	{
		this->Buffer[joker].libre=true;
		AllgotVsMatrices[joker]=false;
		AllfinnishJacobiI[joker]=false;
		AllfinnishJacobiII[joker]=false;
		this->bufferColumnsArrival[joker].newColumn=false;
	}

	this->Nbotton.resize(this->botton.size());
	this->Ntop.resize(this->top.size());

	pointerIniNodes=fopen("/home/robot/Dropbox/Luis/Lo\ de\ Jacobi\ Method/A1/InitNodes.txt","r");
	pointerIniStates=fopen("/home/robot/Dropbox/Luis/Lo\ de\ Jacobi\ Method/A1/InitStates.txt","r");

	fseek(pointerIniNodes,0,SEEK_END);
	fseek(pointerIniStates,0,SEEK_END);

	size1=ftell(pointerIniNodes);
	size2=ftell(pointerIniStates);

	rewind(pointerIniNodes);
	rewind(pointerIniStates);

	buffer1=(char*) malloc (sizeof(char)*(size1+1));
	buffer2=(char*) malloc (sizeof(char)*(size2+2));

	result1 = fread (buffer1,1,size1,pointerIniNodes);
	result2 = fread (buffer2,1,size2,pointerIniStates);
	buffer1[size1]='\0';
	buffer2[size2]='\0';

	fclose(pointerIniNodes);
	fclose(pointerIniStates);

	while(Fi != NULL)
	{
		Fi=strpbrk(buffer1,comma);
		if(Fi == NULL)break;
		memmove(Temp,buffer1,Fi-buffer1);
		Temp[Fi-buffer1]='\0';
		memmove(Fi,Fi+1,strlen(Fi)-1);
		Fi[strlen(Fi)-1]='\0';
		resulta=Cadena2double(Temp,int(strlen(Temp)));
		IniNodesV.append(int(resulta));
		memmove(buffer1,Fi,strlen(Fi));
		buffer1[strlen(Fi)]='\0';
	}
	Fi=strpbrk(buffer2,comma);
	while(Fi != NULL)
	{
		Fi=strpbrk(buffer2,comma);
		if(Fi == NULL)break;
		memmove(Temp,buffer2,Fi-buffer2);
		Temp[Fi-buffer2]='\0';
		memmove(Fi,Fi+1,strlen(Fi)-1);
		Fi[strlen(Fi)-1]='\0';
		resulta=Cadena2double(Temp,int(strlen(Temp)));
		IniStatesV.append(resulta);
		memmove(buffer2,Fi,strlen(Fi));
		buffer2[strlen(Fi)]='\0';
	}
//THIS SHOULD BE IN A FUNCTION
	this->flag_Nodes_already_Received=false;
	this->flag_Nodes_already_Sent=false;
	d.resize(IniStatesV.size());
	q.resize(IniStatesV.size());
	iterator_Varsha=0;
	iterator_Jacobi=0;
	finalstage_JacobiII=false;
	bothcolumns_received=false;
	FlagRefere=false;
	FlagAllProc=false;
	results.resize(NSIZE);

	int a=0;
	for(a=0;a<NSIZE/NPROC;a++)
	{
		tempLoc[a]=a;
	}

	int pp=0,tmpPos;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==(this->RobotID))
		{
			arregloDeSockets[(this->RobotID)-1]=NULL;
			continue;
		}

		if(ExtPort[pp-1]==7000)
		{
			tmpPos=2;
		}else if(ExtPort[pp-1]==8000)
		{
			tmpPos=3;
		}else if(ExtPort[pp-1]==6000)
		{
			tmpPos=1;
		}
		imprimirMensaje("tmpPos validos ="+QString::number(tmpPos,10));
		arregloDeSockets[tmpPos-1]=new SocketTransmision(this,ArrayIPs[pp-1],ExtPort[pp-1],RobotID);
	}

	Server=new Servidor(RobotID,true,this->otherProcessors,this->bufferColumnsArrival);
	if (!(Server->listen(QHostAddress::Any,ExtPort[RobotID-1])))imprimirMensaje("Fallo al hacer link con el puerto");

	connect(Server,SIGNAL(makeSpark()),this,SLOT(SendSpark()));
	connect(Server,SIGNAL(signalPMidJM()),this,SLOT(SendPMidJM()));
	connect(Server,SIGNAL(llegadaVectorIntSER()),this,SLOT(arrivalNodes()));
	connect(Server,SIGNAL(llegadaVectorDoubleSER()),this,SLOT(arrivalStates()));

	connect(this,SIGNAL(ack_GoNextPhase()),this,SLOT(countingArrivals()));
	connect(this,SIGNAL(go_NextPhase()),this,SLOT(nextPhase()));
	connect(this,SIGNAL(signal_MakeSending()),this,SLOT(hacerEnvio()));
	connect(this,SIGNAL(signalSinCos(double ,double ,int ,int )),this,SLOT(SendSinCosine(double ,double ,int ,int )));
	connect(this,SIGNAL(GotAllVs()),this,SLOT(SendAck()));
	connect(this,SIGNAL(go_JacobiII()),this,SLOT(JacobiMethodII()));
	connect(this,SIGNAL(finalJacobiII()),this,SLOT(JacobiMethod()));
	connect(this,SIGNAL(signalJacobiIagain()),this,SLOT(JacobiMethod()));
	connect(Server,SIGNAL(signalInit_VarshaAlgSER()),this,SLOT(hacerEnvio()));

	connect(Server,SIGNAL(signalJabMeth_againSER()),this,SLOT(JacobiMethod()));
	connect(Server,SIGNAL(llegadaVsmallSER(double ,double ,int ,int ,int )),this,SLOT(arrivalVsMatrixSiCos(double ,double ,int ,int ,int)));
	connect(Server,SIGNAL(signalAllVssSER(int )),this,SLOT(verificationStageIIJM(int)));
	connect(Server,SIGNAL(signalAllJacobiIEndSER(int )),this,SLOT(verificationFinJacI(int )));
	connect(Server,SIGNAL(ReceivedColSER()),this,SLOT(positionColumn()));
	connect(Server,SIGNAL(signalAllJacobiIIEndSER(int )),this,SLOT(verificationFinJacII(int )));

	connect(this,SIGNAL(signalsLeft(int )),this,SLOT(SendLeft(int )));
	connect(this,SIGNAL(signalsRight(int )),this,SLOT(SendRight(int )));
	connect(this,SIGNAL(signalEndJacobiI()),this,SLOT(SendEndJacobiI()));
	connect(this,SIGNAL(signalEndJacobiII()),this,SLOT(SendEndJacobiII()));
	connect(this,SIGNAL(signalPosColandENDCycle()),this,SLOT(PosColandENDCycle()));
	connect(this,SIGNAL(signalCheckRefere()),this,SLOT(final_Stage_Refere()));


	this->OutputStates=fopen("FileMatlabN1.txt","w");
	fprintf(this->OutputStates,"StatesIteration1=[\n");
	a=0;
	fprintf(this->OutputStates,"%d	",this->iterator_Varsha);
	for(a=0;a<(this->d.size());a++)
	{
			if(a<((d.size())-1))
						{
							printf("%6.4f	",this->IniStatesV[a]);
							fprintf(this->OutputStates,"%6.4f		",this->IniStatesV[a]);
						}else{
							printf("%6.4f;\n",this->IniStatesV[a]);
							fprintf(this->OutputStates,"%6.4f;\n",this->IniStatesV[a]);
						}
	}

	this->ComputeOutput(IniStatesV.size(),IniStatesV.data(),d.data());
	this->iterator_Varsha+=1;
	fprintf(this->OutputStates,"%d	",this->iterator_Varsha);
	imprimirMensaje("Data...");
	for(a=0;a<(this->d.size());a++)
	{
			if(a<((d.size())-1))
						{
							printf("%6.4f	",d[a]);
							fprintf(this->OutputStates,"%6.4f		",d[a]);
						}else{
							printf("%6.4f;\n",d[a]);
							fprintf(this->OutputStates,"%6.4f;\n",d[a]);
						}
	}
	witnessJacobiIEnd=0;
	witnessJacobiIIEnd=0;
	witnessStageBefJacobiII=0;
	iteradorColumnasJacobi=(NSIZE/(2*NPROC))*(this->RobotID - 1);
	this->JacobiOption=false;
	this->VarshaOption=true;

	MatrixPrasanna[0][0]=-4;MatrixPrasanna[0][1]=1;MatrixPrasanna[0][2]=0;MatrixPrasanna[0][3]=0;MatrixPrasanna[0][4]=1;MatrixPrasanna[0][5]=0;MatrixPrasanna[0][6]=1;MatrixPrasanna[0][7]=1;MatrixPrasanna[0][8]=0;MatrixPrasanna[0][9]=0;MatrixPrasanna[0][10]=0;MatrixPrasanna[0][11]=0;MatrixPrasanna[0][12]=0;MatrixPrasanna[0][13]=0;MatrixPrasanna[0][14]=0;MatrixPrasanna[0][15]=0;MatrixPrasanna[0][16]=0;MatrixPrasanna[0][17]=0;MatrixPrasanna[0][18]=0;MatrixPrasanna[0][19]=0;MatrixPrasanna[0][20]=0;MatrixPrasanna[0][21]=0;MatrixPrasanna[0][22]=0;MatrixPrasanna[0][23]=0;
	MatrixPrasanna[1][0]=1;MatrixPrasanna[1][1]=-4;MatrixPrasanna[1][2]=0;MatrixPrasanna[1][3]=1;MatrixPrasanna[1][4]=0;MatrixPrasanna[1][5]=1;MatrixPrasanna[1][6]=1;MatrixPrasanna[1][7]=0;MatrixPrasanna[1][8]=0;MatrixPrasanna[1][9]=0;MatrixPrasanna[1][10]=0;MatrixPrasanna[1][11]=0;MatrixPrasanna[1][12]=0;MatrixPrasanna[1][13]=0;MatrixPrasanna[1][14]=0;MatrixPrasanna[1][15]=0;MatrixPrasanna[1][16]=0;MatrixPrasanna[1][17]=0;MatrixPrasanna[1][18]=0;MatrixPrasanna[1][19]=0;MatrixPrasanna[1][20]=0;MatrixPrasanna[1][21]=0;MatrixPrasanna[1][22]=0;MatrixPrasanna[1][23]=0;
	MatrixPrasanna[2][0]=0;MatrixPrasanna[2][1]=0;MatrixPrasanna[2][2]=-4;MatrixPrasanna[2][3]=0;MatrixPrasanna[2][4]=1;MatrixPrasanna[2][5]=0;MatrixPrasanna[2][6]=1;MatrixPrasanna[2][7]=1;MatrixPrasanna[2][8]=0;MatrixPrasanna[2][9]=0;MatrixPrasanna[2][10]=0;MatrixPrasanna[2][11]=0;MatrixPrasanna[2][12]=0;MatrixPrasanna[2][13]=0;MatrixPrasanna[2][14]=0;MatrixPrasanna[2][15]=0;MatrixPrasanna[2][16]=0;MatrixPrasanna[2][17]=0;MatrixPrasanna[2][18]=0;MatrixPrasanna[2][19]=0;MatrixPrasanna[2][20]=0;MatrixPrasanna[2][21]=0;MatrixPrasanna[2][22]=0;MatrixPrasanna[2][23]=0;
	MatrixPrasanna[3][0]=0;MatrixPrasanna[3][1]=1;MatrixPrasanna[3][2]=0;MatrixPrasanna[3][3]=-4;MatrixPrasanna[3][4]=1;MatrixPrasanna[3][5]=0;MatrixPrasanna[3][6]=0;MatrixPrasanna[3][7]=0;MatrixPrasanna[3][8]=0;MatrixPrasanna[3][9]=0;MatrixPrasanna[3][10]=0;MatrixPrasanna[3][11]=0;MatrixPrasanna[3][12]=0;MatrixPrasanna[3][13]=0;MatrixPrasanna[3][14]=0;MatrixPrasanna[3][15]=0;MatrixPrasanna[3][16]=0;MatrixPrasanna[3][17]=0;MatrixPrasanna[3][18]=0;MatrixPrasanna[3][19]=0;MatrixPrasanna[3][20]=0;MatrixPrasanna[3][21]=0;MatrixPrasanna[3][22]=0;MatrixPrasanna[3][23]=0;
	MatrixPrasanna[4][0]=1;MatrixPrasanna[4][1]=0;MatrixPrasanna[4][2]=1;MatrixPrasanna[4][3]=1;MatrixPrasanna[4][4]=-4;MatrixPrasanna[4][5]=1;MatrixPrasanna[4][6]=1;MatrixPrasanna[4][7]=0;MatrixPrasanna[4][8]=0;MatrixPrasanna[4][9]=0;MatrixPrasanna[4][10]=0;MatrixPrasanna[4][11]=0;MatrixPrasanna[4][12]=0;MatrixPrasanna[4][13]=0;MatrixPrasanna[4][14]=0;MatrixPrasanna[4][15]=0;MatrixPrasanna[4][16]=0;MatrixPrasanna[4][17]=0;MatrixPrasanna[4][18]=0;MatrixPrasanna[4][19]=0;MatrixPrasanna[4][20]=0;MatrixPrasanna[4][21]=0;MatrixPrasanna[4][22]=0;MatrixPrasanna[4][23]=0;
	MatrixPrasanna[5][0]=0;MatrixPrasanna[5][1]=1;MatrixPrasanna[5][2]=0;MatrixPrasanna[5][3]=0;MatrixPrasanna[5][4]=1;MatrixPrasanna[5][5]=-4;MatrixPrasanna[5][6]=1;MatrixPrasanna[5][7]=0;MatrixPrasanna[5][8]=0;MatrixPrasanna[5][9]=0;MatrixPrasanna[5][10]=0;MatrixPrasanna[5][11]=0;MatrixPrasanna[5][12]=0;MatrixPrasanna[5][13]=0;MatrixPrasanna[5][14]=0;MatrixPrasanna[5][15]=0;MatrixPrasanna[5][16]=0;MatrixPrasanna[5][17]=0;MatrixPrasanna[5][18]=0;MatrixPrasanna[5][19]=0;MatrixPrasanna[5][20]=0;MatrixPrasanna[5][21]=0;MatrixPrasanna[5][22]=0;MatrixPrasanna[5][23]=0;
	MatrixPrasanna[6][0]=1;MatrixPrasanna[6][1]=1;MatrixPrasanna[6][2]=1;MatrixPrasanna[6][3]=0;MatrixPrasanna[6][4]=1;MatrixPrasanna[6][5]=1;MatrixPrasanna[6][6]=-4;MatrixPrasanna[6][7]=1;MatrixPrasanna[6][8]=0;MatrixPrasanna[6][9]=0;MatrixPrasanna[6][10]=0;MatrixPrasanna[6][11]=0;MatrixPrasanna[6][12]=0;MatrixPrasanna[6][13]=0;MatrixPrasanna[6][14]=0;MatrixPrasanna[6][15]=0;MatrixPrasanna[6][16]=0;MatrixPrasanna[6][17]=0;MatrixPrasanna[6][18]=0;MatrixPrasanna[6][19]=0;MatrixPrasanna[6][20]=0;MatrixPrasanna[6][21]=0;MatrixPrasanna[6][22]=0;MatrixPrasanna[6][23]=0;
	MatrixPrasanna[7][0]=1;MatrixPrasanna[7][1]=0;MatrixPrasanna[7][2]=1;MatrixPrasanna[7][3]=0;MatrixPrasanna[7][4]=0;MatrixPrasanna[7][5]=0;MatrixPrasanna[7][6]=1;MatrixPrasanna[7][7]=-4;MatrixPrasanna[7][8]=0;MatrixPrasanna[7][9]=0;MatrixPrasanna[7][10]=0;MatrixPrasanna[7][11]=0;MatrixPrasanna[7][12]=0;MatrixPrasanna[7][13]=0;MatrixPrasanna[7][14]=0;MatrixPrasanna[7][15]=0;MatrixPrasanna[7][16]=0;MatrixPrasanna[7][17]=0;MatrixPrasanna[7][18]=0;MatrixPrasanna[7][19]=0;MatrixPrasanna[7][20]=0;MatrixPrasanna[7][21]=0;MatrixPrasanna[7][22]=0;MatrixPrasanna[7][23]=0;
	MatrixPrasanna[8][0]=1;MatrixPrasanna[8][1]=0;MatrixPrasanna[8][2]=1;MatrixPrasanna[8][3]=0;MatrixPrasanna[8][4]=1;MatrixPrasanna[8][5]=1;MatrixPrasanna[8][6]=1;MatrixPrasanna[8][7]=1;MatrixPrasanna[8][8]=0;MatrixPrasanna[8][9]=0;MatrixPrasanna[8][10]=0;MatrixPrasanna[8][11]=0;MatrixPrasanna[8][12]=0;MatrixPrasanna[8][13]=0;MatrixPrasanna[8][14]=0;MatrixPrasanna[8][15]=0;MatrixPrasanna[8][16]=0;MatrixPrasanna[8][17]=0;MatrixPrasanna[8][18]=0;MatrixPrasanna[8][19]=0;MatrixPrasanna[8][20]=0;MatrixPrasanna[8][21]=0;MatrixPrasanna[8][22]=0;MatrixPrasanna[8][23]=0;
	MatrixPrasanna[9][0]=0;MatrixPrasanna[9][1]=0;MatrixPrasanna[9][2]=1;MatrixPrasanna[9][3]=0;MatrixPrasanna[9][4]=0;MatrixPrasanna[9][5]=1;MatrixPrasanna[9][6]=1;MatrixPrasanna[9][7]=0;MatrixPrasanna[9][8]=0;MatrixPrasanna[9][9]=0;MatrixPrasanna[9][10]=0;MatrixPrasanna[9][11]=0;MatrixPrasanna[9][12]=0;MatrixPrasanna[9][13]=0;MatrixPrasanna[9][14]=0;MatrixPrasanna[9][15]=0;MatrixPrasanna[9][16]=0;MatrixPrasanna[9][17]=0;MatrixPrasanna[9][18]=0;MatrixPrasanna[9][19]=0;MatrixPrasanna[9][20]=0;MatrixPrasanna[9][21]=0;MatrixPrasanna[9][22]=0;MatrixPrasanna[9][23]=0;
	MatrixPrasanna[10][0]=1;MatrixPrasanna[10][1]=1;MatrixPrasanna[10][2]=1;MatrixPrasanna[10][3]=1;MatrixPrasanna[10][4]=0;MatrixPrasanna[10][5]=1;MatrixPrasanna[10][6]=1;MatrixPrasanna[10][7]=1;MatrixPrasanna[10][8]=0;MatrixPrasanna[10][9]=0;MatrixPrasanna[10][10]=0;MatrixPrasanna[10][11]=0;MatrixPrasanna[10][12]=0;MatrixPrasanna[10][13]=0;MatrixPrasanna[10][14]=0;MatrixPrasanna[10][15]=0;MatrixPrasanna[10][16]=0;MatrixPrasanna[10][17]=0;MatrixPrasanna[10][18]=0;MatrixPrasanna[10][19]=0;MatrixPrasanna[10][20]=0;MatrixPrasanna[10][21]=0;MatrixPrasanna[10][22]=0;MatrixPrasanna[10][23]=0;
	MatrixPrasanna[11][0]=1;MatrixPrasanna[11][1]=0;MatrixPrasanna[11][2]=0;MatrixPrasanna[11][3]=0;MatrixPrasanna[11][4]=0;MatrixPrasanna[11][5]=1;MatrixPrasanna[11][6]=0;MatrixPrasanna[11][7]=0;MatrixPrasanna[11][8]=0;MatrixPrasanna[11][9]=0;MatrixPrasanna[11][10]=0;MatrixPrasanna[11][11]=0;MatrixPrasanna[11][12]=0;MatrixPrasanna[11][13]=0;MatrixPrasanna[11][14]=0;MatrixPrasanna[11][15]=0;MatrixPrasanna[11][16]=0;MatrixPrasanna[11][17]=0;MatrixPrasanna[11][18]=0;MatrixPrasanna[11][19]=0;MatrixPrasanna[11][20]=0;MatrixPrasanna[11][21]=0;MatrixPrasanna[11][22]=0;MatrixPrasanna[11][23]=0;
	MatrixPrasanna[12][0]=1;MatrixPrasanna[12][1]=0;MatrixPrasanna[12][2]=0;MatrixPrasanna[12][3]=0;MatrixPrasanna[12][4]=0;MatrixPrasanna[12][5]=0;MatrixPrasanna[12][6]=1;MatrixPrasanna[12][7]=0;MatrixPrasanna[12][8]=0;MatrixPrasanna[12][9]=0;MatrixPrasanna[12][10]=0;MatrixPrasanna[12][11]=0;MatrixPrasanna[12][12]=0;MatrixPrasanna[12][13]=0;MatrixPrasanna[12][14]=0;MatrixPrasanna[12][15]=0;MatrixPrasanna[12][16]=0;MatrixPrasanna[12][17]=0;MatrixPrasanna[12][18]=0;MatrixPrasanna[12][19]=0;MatrixPrasanna[12][20]=0;MatrixPrasanna[12][21]=0;MatrixPrasanna[12][22]=0;MatrixPrasanna[12][23]=0;
	MatrixPrasanna[13][0]=1;MatrixPrasanna[13][1]=0;MatrixPrasanna[13][2]=1;MatrixPrasanna[13][3]=1;MatrixPrasanna[13][4]=0;MatrixPrasanna[13][5]=0;MatrixPrasanna[13][6]=0;MatrixPrasanna[13][7]=0;MatrixPrasanna[13][8]=0;MatrixPrasanna[13][9]=0;MatrixPrasanna[13][10]=0;MatrixPrasanna[13][11]=0;MatrixPrasanna[13][12]=0;MatrixPrasanna[13][13]=0;MatrixPrasanna[13][14]=0;MatrixPrasanna[13][15]=0;MatrixPrasanna[13][16]=0;MatrixPrasanna[13][17]=0;MatrixPrasanna[13][18]=0;MatrixPrasanna[13][19]=0;MatrixPrasanna[13][20]=0;MatrixPrasanna[13][21]=0;MatrixPrasanna[13][22]=0;MatrixPrasanna[13][23]=0;
	MatrixPrasanna[14][0]=1;MatrixPrasanna[14][1]=0;MatrixPrasanna[14][2]=0;MatrixPrasanna[14][3]=0;MatrixPrasanna[14][4]=0;MatrixPrasanna[14][5]=1;MatrixPrasanna[14][6]=1;MatrixPrasanna[14][7]=0;MatrixPrasanna[14][8]=0;MatrixPrasanna[14][9]=0;MatrixPrasanna[14][10]=0;MatrixPrasanna[14][11]=0;MatrixPrasanna[14][12]=0;MatrixPrasanna[14][13]=0;MatrixPrasanna[14][14]=0;MatrixPrasanna[14][15]=0;MatrixPrasanna[14][16]=0;MatrixPrasanna[14][17]=0;MatrixPrasanna[14][18]=0;MatrixPrasanna[14][19]=0;MatrixPrasanna[14][20]=0;MatrixPrasanna[14][21]=0;MatrixPrasanna[14][22]=0;MatrixPrasanna[14][23]=0;
	MatrixPrasanna[15][0]=0;MatrixPrasanna[15][1]=0;MatrixPrasanna[15][2]=0;MatrixPrasanna[15][3]=0;MatrixPrasanna[15][4]=0;MatrixPrasanna[15][5]=1;MatrixPrasanna[15][6]=1;MatrixPrasanna[15][7]=1;MatrixPrasanna[15][8]=0;MatrixPrasanna[15][9]=0;MatrixPrasanna[15][10]=0;MatrixPrasanna[15][11]=0;MatrixPrasanna[15][12]=0;MatrixPrasanna[15][13]=0;MatrixPrasanna[15][14]=0;MatrixPrasanna[15][15]=0;MatrixPrasanna[15][16]=0;MatrixPrasanna[15][17]=0;MatrixPrasanna[15][18]=0;MatrixPrasanna[15][19]=0;MatrixPrasanna[15][20]=0;MatrixPrasanna[15][21]=0;MatrixPrasanna[15][22]=0;MatrixPrasanna[15][23]=0;
	MatrixPrasanna[16][0]=0;MatrixPrasanna[16][1]=0;MatrixPrasanna[16][2]=1;MatrixPrasanna[16][3]=1;MatrixPrasanna[16][4]=1;MatrixPrasanna[16][5]=0;MatrixPrasanna[16][6]=0;MatrixPrasanna[16][7]=0;MatrixPrasanna[16][8]=0;MatrixPrasanna[16][9]=0;MatrixPrasanna[16][10]=0;MatrixPrasanna[16][11]=0;MatrixPrasanna[16][12]=0;MatrixPrasanna[16][13]=0;MatrixPrasanna[16][14]=0;MatrixPrasanna[16][15]=0;MatrixPrasanna[16][16]=0;MatrixPrasanna[16][17]=0;MatrixPrasanna[16][18]=0;MatrixPrasanna[16][19]=0;MatrixPrasanna[16][20]=0;MatrixPrasanna[16][21]=0;MatrixPrasanna[16][22]=0;MatrixPrasanna[16][23]=0;
	MatrixPrasanna[17][0]=1;MatrixPrasanna[17][1]=1;MatrixPrasanna[17][2]=1;MatrixPrasanna[17][3]=0;MatrixPrasanna[17][4]=0;MatrixPrasanna[17][5]=0;MatrixPrasanna[17][6]=0;MatrixPrasanna[17][7]=1;MatrixPrasanna[17][8]=0;MatrixPrasanna[17][9]=0;MatrixPrasanna[17][10]=0;MatrixPrasanna[17][11]=0;MatrixPrasanna[17][12]=0;MatrixPrasanna[17][13]=0;MatrixPrasanna[17][14]=0;MatrixPrasanna[17][15]=0;MatrixPrasanna[17][16]=0;MatrixPrasanna[17][17]=0;MatrixPrasanna[17][18]=0;MatrixPrasanna[17][19]=0;MatrixPrasanna[17][20]=0;MatrixPrasanna[17][21]=0;MatrixPrasanna[17][22]=0;MatrixPrasanna[17][23]=0;
	MatrixPrasanna[18][0]=0;MatrixPrasanna[18][1]=1;MatrixPrasanna[18][2]=1;MatrixPrasanna[18][3]=1;MatrixPrasanna[18][4]=1;MatrixPrasanna[18][5]=0;MatrixPrasanna[18][6]=0;MatrixPrasanna[18][7]=0;MatrixPrasanna[18][8]=0;MatrixPrasanna[18][9]=0;MatrixPrasanna[18][10]=0;MatrixPrasanna[18][11]=0;MatrixPrasanna[18][12]=0;MatrixPrasanna[18][13]=0;MatrixPrasanna[18][14]=0;MatrixPrasanna[18][15]=0;MatrixPrasanna[18][16]=0;MatrixPrasanna[18][17]=0;MatrixPrasanna[18][18]=0;MatrixPrasanna[18][19]=0;MatrixPrasanna[18][20]=0;MatrixPrasanna[18][21]=0;MatrixPrasanna[18][22]=0;MatrixPrasanna[18][23]=0;
	MatrixPrasanna[19][0]=0;MatrixPrasanna[19][1]=0;MatrixPrasanna[19][2]=0;MatrixPrasanna[19][3]=1;MatrixPrasanna[19][4]=1;MatrixPrasanna[19][5]=0;MatrixPrasanna[19][6]=1;MatrixPrasanna[19][7]=0;MatrixPrasanna[19][8]=0;MatrixPrasanna[19][9]=0;MatrixPrasanna[19][10]=0;MatrixPrasanna[19][11]=0;MatrixPrasanna[19][12]=0;MatrixPrasanna[19][13]=0;MatrixPrasanna[19][14]=0;MatrixPrasanna[19][15]=0;MatrixPrasanna[19][16]=0;MatrixPrasanna[19][17]=0;MatrixPrasanna[19][18]=0;MatrixPrasanna[19][19]=0;MatrixPrasanna[19][20]=0;MatrixPrasanna[19][21]=0;MatrixPrasanna[19][22]=0;MatrixPrasanna[19][23]=0;
	MatrixPrasanna[20][0]=0;MatrixPrasanna[20][1]=1;MatrixPrasanna[20][2]=0;MatrixPrasanna[20][3]=1;MatrixPrasanna[20][4]=1;MatrixPrasanna[20][5]=0;MatrixPrasanna[20][6]=1;MatrixPrasanna[20][7]=0;MatrixPrasanna[20][8]=0;MatrixPrasanna[20][9]=0;MatrixPrasanna[20][10]=0;MatrixPrasanna[20][11]=0;MatrixPrasanna[20][12]=0;MatrixPrasanna[20][13]=0;MatrixPrasanna[20][14]=0;MatrixPrasanna[20][15]=0;MatrixPrasanna[20][16]=0;MatrixPrasanna[20][17]=0;MatrixPrasanna[20][18]=0;MatrixPrasanna[20][19]=0;MatrixPrasanna[20][20]=0;MatrixPrasanna[20][21]=0;MatrixPrasanna[20][22]=0;MatrixPrasanna[20][23]=0;
	MatrixPrasanna[21][0]=0;MatrixPrasanna[21][1]=0;MatrixPrasanna[21][2]=0;MatrixPrasanna[21][3]=0;MatrixPrasanna[21][4]=0;MatrixPrasanna[21][5]=0;MatrixPrasanna[21][6]=0;MatrixPrasanna[21][7]=1;MatrixPrasanna[21][8]=0;MatrixPrasanna[21][9]=0;MatrixPrasanna[21][10]=0;MatrixPrasanna[21][11]=0;MatrixPrasanna[21][12]=0;MatrixPrasanna[21][13]=0;MatrixPrasanna[21][14]=0;MatrixPrasanna[21][15]=0;MatrixPrasanna[21][16]=0;MatrixPrasanna[21][17]=0;MatrixPrasanna[21][18]=0;MatrixPrasanna[21][19]=0;MatrixPrasanna[21][20]=0;MatrixPrasanna[21][21]=0;MatrixPrasanna[21][22]=0;MatrixPrasanna[21][23]=0;
	MatrixPrasanna[22][0]=1;MatrixPrasanna[22][1]=0;MatrixPrasanna[22][2]=1;MatrixPrasanna[22][3]=1;MatrixPrasanna[22][4]=0;MatrixPrasanna[22][5]=1;MatrixPrasanna[22][6]=1;MatrixPrasanna[22][7]=0;MatrixPrasanna[22][8]=0;MatrixPrasanna[22][9]=0;MatrixPrasanna[22][10]=0;MatrixPrasanna[22][11]=0;MatrixPrasanna[22][12]=0;MatrixPrasanna[22][13]=0;MatrixPrasanna[22][14]=0;MatrixPrasanna[22][15]=0;MatrixPrasanna[22][16]=0;MatrixPrasanna[22][17]=0;MatrixPrasanna[22][18]=0;MatrixPrasanna[22][19]=0;MatrixPrasanna[22][20]=0;MatrixPrasanna[22][21]=0;MatrixPrasanna[22][22]=0;MatrixPrasanna[22][23]=0;
	MatrixPrasanna[23][0]=1;MatrixPrasanna[23][1]=1;MatrixPrasanna[23][2]=0;MatrixPrasanna[23][3]=0;MatrixPrasanna[23][4]=0;MatrixPrasanna[23][5]=0;MatrixPrasanna[23][6]=1;MatrixPrasanna[23][7]=0;MatrixPrasanna[23][8]=0;MatrixPrasanna[23][9]=0;MatrixPrasanna[23][10]=0;MatrixPrasanna[23][11]=0;MatrixPrasanna[23][12]=0;MatrixPrasanna[23][13]=0;MatrixPrasanna[23][14]=0;MatrixPrasanna[23][15]=0;MatrixPrasanna[23][16]=0;MatrixPrasanna[23][17]=0;MatrixPrasanna[23][18]=0;MatrixPrasanna[23][19]=0;MatrixPrasanna[23][20]=0;MatrixPrasanna[23][21]=0;MatrixPrasanna[23][22]=0;MatrixPrasanna[23][23]=0;


/*
	MatrixPrasanna[0][0]=0;MatrixPrasanna[0][1]=0;MatrixPrasanna[0][2]=0;MatrixPrasanna[0][3]=0;MatrixPrasanna[0][4]=0;MatrixPrasanna[0][5]=0;MatrixPrasanna[0][6]=0;MatrixPrasanna[0][7]=0;MatrixPrasanna[0][8]=1;MatrixPrasanna[0][9]=0;MatrixPrasanna[0][10]=1;MatrixPrasanna[0][11]=1;MatrixPrasanna[0][12]=1;MatrixPrasanna[0][13]=1;MatrixPrasanna[0][14]=1;MatrixPrasanna[0][15]=0;MatrixPrasanna[0][16]=0;MatrixPrasanna[0][17]=0;MatrixPrasanna[0][18]=0;MatrixPrasanna[0][19]=0;MatrixPrasanna[0][20]=0;MatrixPrasanna[0][21]=0;MatrixPrasanna[0][22]=0;MatrixPrasanna[0][23]=0;
	MatrixPrasanna[1][0]=0;MatrixPrasanna[1][1]=0;MatrixPrasanna[1][2]=0;MatrixPrasanna[1][3]=0;MatrixPrasanna[1][4]=0;MatrixPrasanna[1][5]=0;MatrixPrasanna[1][6]=0;MatrixPrasanna[1][7]=0;MatrixPrasanna[1][8]=0;MatrixPrasanna[1][9]=0;MatrixPrasanna[1][10]=1;MatrixPrasanna[1][11]=0;MatrixPrasanna[1][12]=0;MatrixPrasanna[1][13]=0;MatrixPrasanna[1][14]=0;MatrixPrasanna[1][15]=0;MatrixPrasanna[1][16]=0;MatrixPrasanna[1][17]=0;MatrixPrasanna[1][18]=0;MatrixPrasanna[1][19]=0;MatrixPrasanna[1][20]=0;MatrixPrasanna[1][21]=0;MatrixPrasanna[1][22]=0;MatrixPrasanna[1][23]=0;
	MatrixPrasanna[2][0]=0;MatrixPrasanna[2][1]=0;MatrixPrasanna[2][2]=0;MatrixPrasanna[2][3]=0;MatrixPrasanna[2][4]=0;MatrixPrasanna[2][5]=0;MatrixPrasanna[2][6]=0;MatrixPrasanna[2][7]=0;MatrixPrasanna[2][8]=1;MatrixPrasanna[2][9]=1;MatrixPrasanna[2][10]=1;MatrixPrasanna[2][11]=0;MatrixPrasanna[2][12]=0;MatrixPrasanna[2][13]=1;MatrixPrasanna[2][14]=0;MatrixPrasanna[2][15]=0;MatrixPrasanna[2][16]=0;MatrixPrasanna[2][17]=0;MatrixPrasanna[2][18]=0;MatrixPrasanna[2][19]=0;MatrixPrasanna[2][20]=0;MatrixPrasanna[2][21]=0;MatrixPrasanna[2][22]=0;MatrixPrasanna[2][23]=0;
	MatrixPrasanna[3][0]=0;MatrixPrasanna[3][1]=0;MatrixPrasanna[3][2]=0;MatrixPrasanna[3][3]=0;MatrixPrasanna[3][4]=0;MatrixPrasanna[3][5]=0;MatrixPrasanna[3][6]=0;MatrixPrasanna[3][7]=0;MatrixPrasanna[3][8]=0;MatrixPrasanna[3][9]=0;MatrixPrasanna[3][10]=1;MatrixPrasanna[3][11]=0;MatrixPrasanna[3][12]=0;MatrixPrasanna[3][13]=1;MatrixPrasanna[3][14]=0;MatrixPrasanna[3][15]=0;MatrixPrasanna[3][16]=0;MatrixPrasanna[3][17]=0;MatrixPrasanna[3][18]=0;MatrixPrasanna[3][19]=0;MatrixPrasanna[3][20]=0;MatrixPrasanna[3][21]=0;MatrixPrasanna[3][22]=0;MatrixPrasanna[3][23]=0;
	MatrixPrasanna[4][0]=0;MatrixPrasanna[4][1]=0;MatrixPrasanna[4][2]=0;MatrixPrasanna[4][3]=0;MatrixPrasanna[4][4]=0;MatrixPrasanna[4][5]=0;MatrixPrasanna[4][6]=0;MatrixPrasanna[4][7]=0;MatrixPrasanna[4][8]=1;MatrixPrasanna[4][9]=0;MatrixPrasanna[4][10]=0;MatrixPrasanna[4][11]=0;MatrixPrasanna[4][12]=0;MatrixPrasanna[4][13]=0;MatrixPrasanna[4][14]=0;MatrixPrasanna[4][15]=0;MatrixPrasanna[4][16]=0;MatrixPrasanna[4][17]=0;MatrixPrasanna[4][18]=0;MatrixPrasanna[4][19]=0;MatrixPrasanna[4][20]=0;MatrixPrasanna[4][21]=0;MatrixPrasanna[4][22]=0;MatrixPrasanna[4][23]=0;
	MatrixPrasanna[5][0]=0;MatrixPrasanna[5][1]=0;MatrixPrasanna[5][2]=0;MatrixPrasanna[5][3]=0;MatrixPrasanna[5][4]=0;MatrixPrasanna[5][5]=0;MatrixPrasanna[5][6]=0;MatrixPrasanna[5][7]=0;MatrixPrasanna[5][8]=1;MatrixPrasanna[5][9]=1;MatrixPrasanna[5][10]=1;MatrixPrasanna[5][11]=1;MatrixPrasanna[5][12]=0;MatrixPrasanna[5][13]=0;MatrixPrasanna[5][14]=1;MatrixPrasanna[5][15]=1;MatrixPrasanna[5][16]=0;MatrixPrasanna[5][17]=0;MatrixPrasanna[5][18]=0;MatrixPrasanna[5][19]=0;MatrixPrasanna[5][20]=0;MatrixPrasanna[5][21]=0;MatrixPrasanna[5][22]=0;MatrixPrasanna[5][23]=0;
	MatrixPrasanna[6][0]=0;MatrixPrasanna[6][1]=0;MatrixPrasanna[6][2]=0;MatrixPrasanna[6][3]=0;MatrixPrasanna[6][4]=0;MatrixPrasanna[6][5]=0;MatrixPrasanna[6][6]=0;MatrixPrasanna[6][7]=0;MatrixPrasanna[6][8]=1;MatrixPrasanna[6][9]=1;MatrixPrasanna[6][10]=1;MatrixPrasanna[6][11]=0;MatrixPrasanna[6][12]=1;MatrixPrasanna[6][13]=0;MatrixPrasanna[6][14]=1;MatrixPrasanna[6][15]=1;MatrixPrasanna[6][16]=0;MatrixPrasanna[6][17]=0;MatrixPrasanna[6][18]=0;MatrixPrasanna[6][19]=0;MatrixPrasanna[6][20]=0;MatrixPrasanna[6][21]=0;MatrixPrasanna[6][22]=0;MatrixPrasanna[6][23]=0;
	MatrixPrasanna[7][0]=0;MatrixPrasanna[7][1]=0;MatrixPrasanna[7][2]=0;MatrixPrasanna[7][3]=0;MatrixPrasanna[7][4]=0;MatrixPrasanna[7][5]=0;MatrixPrasanna[7][6]=0;MatrixPrasanna[7][7]=0;MatrixPrasanna[7][8]=1;MatrixPrasanna[7][9]=0;MatrixPrasanna[7][10]=1;MatrixPrasanna[7][11]=0;MatrixPrasanna[7][12]=0;MatrixPrasanna[7][13]=0;MatrixPrasanna[7][14]=0;MatrixPrasanna[7][15]=1;MatrixPrasanna[7][16]=0;MatrixPrasanna[7][17]=0;MatrixPrasanna[7][18]=0;MatrixPrasanna[7][19]=0;MatrixPrasanna[7][20]=0;MatrixPrasanna[7][21]=0;MatrixPrasanna[7][22]=0;MatrixPrasanna[7][23]=0;
	MatrixPrasanna[8][0]=0;MatrixPrasanna[8][1]=0;MatrixPrasanna[8][2]=0;MatrixPrasanna[8][3]=0;MatrixPrasanna[8][4]=0;MatrixPrasanna[8][5]=0;MatrixPrasanna[8][6]=0;MatrixPrasanna[8][7]=0;MatrixPrasanna[8][8]=-4;MatrixPrasanna[8][9]=1;MatrixPrasanna[8][10]=0;MatrixPrasanna[8][11]=1;MatrixPrasanna[8][12]=0;MatrixPrasanna[8][13]=1;MatrixPrasanna[8][14]=0;MatrixPrasanna[8][15]=1;MatrixPrasanna[8][16]=0;MatrixPrasanna[8][17]=0;MatrixPrasanna[8][18]=0;MatrixPrasanna[8][19]=0;MatrixPrasanna[8][20]=0;MatrixPrasanna[8][21]=0;MatrixPrasanna[8][22]=0;MatrixPrasanna[8][23]=0;
	MatrixPrasanna[9][0]=0;MatrixPrasanna[9][1]=0;MatrixPrasanna[9][2]=0;MatrixPrasanna[9][3]=0;MatrixPrasanna[9][4]=0;MatrixPrasanna[9][5]=0;MatrixPrasanna[9][6]=0;MatrixPrasanna[9][7]=0;MatrixPrasanna[9][8]=1;MatrixPrasanna[9][9]=-4;MatrixPrasanna[9][10]=1;MatrixPrasanna[9][11]=1;MatrixPrasanna[9][12]=1;MatrixPrasanna[9][13]=0;MatrixPrasanna[9][14]=1;MatrixPrasanna[9][15]=0;MatrixPrasanna[9][16]=0;MatrixPrasanna[9][17]=0;MatrixPrasanna[9][18]=0;MatrixPrasanna[9][19]=0;MatrixPrasanna[9][20]=0;MatrixPrasanna[9][21]=0;MatrixPrasanna[9][22]=0;MatrixPrasanna[9][23]=0;
	MatrixPrasanna[10][0]=0;MatrixPrasanna[10][1]=0;MatrixPrasanna[10][2]=0;MatrixPrasanna[10][3]=0;MatrixPrasanna[10][4]=0;MatrixPrasanna[10][5]=0;MatrixPrasanna[10][6]=0;MatrixPrasanna[10][7]=0;MatrixPrasanna[10][8]=0;MatrixPrasanna[10][9]=1;MatrixPrasanna[10][10]=-4;MatrixPrasanna[10][11]=1;MatrixPrasanna[10][12]=1;MatrixPrasanna[10][13]=0;MatrixPrasanna[10][14]=1;MatrixPrasanna[10][15]=1;MatrixPrasanna[10][16]=0;MatrixPrasanna[10][17]=0;MatrixPrasanna[10][18]=0;MatrixPrasanna[10][19]=0;MatrixPrasanna[10][20]=0;MatrixPrasanna[10][21]=0;MatrixPrasanna[10][22]=0;MatrixPrasanna[10][23]=0;
	MatrixPrasanna[11][0]=0;MatrixPrasanna[11][1]=0;MatrixPrasanna[11][2]=0;MatrixPrasanna[11][3]=0;MatrixPrasanna[11][4]=0;MatrixPrasanna[11][5]=0;MatrixPrasanna[11][6]=0;MatrixPrasanna[11][7]=0;MatrixPrasanna[11][8]=1;MatrixPrasanna[11][9]=1;MatrixPrasanna[11][10]=1;MatrixPrasanna[11][11]=-4;MatrixPrasanna[11][12]=1;MatrixPrasanna[11][13]=1;MatrixPrasanna[11][14]=1;MatrixPrasanna[11][15]=1;MatrixPrasanna[11][16]=0;MatrixPrasanna[11][17]=0;MatrixPrasanna[11][18]=0;MatrixPrasanna[11][19]=0;MatrixPrasanna[11][20]=0;MatrixPrasanna[11][21]=0;MatrixPrasanna[11][22]=0;MatrixPrasanna[11][23]=0;
	MatrixPrasanna[12][0]=0;MatrixPrasanna[12][1]=0;MatrixPrasanna[12][2]=0;MatrixPrasanna[12][3]=0;MatrixPrasanna[12][4]=0;MatrixPrasanna[12][5]=0;MatrixPrasanna[12][6]=0;MatrixPrasanna[12][7]=0;MatrixPrasanna[12][8]=0;MatrixPrasanna[12][9]=1;MatrixPrasanna[12][10]=1;MatrixPrasanna[12][11]=1;MatrixPrasanna[12][12]=-4;MatrixPrasanna[12][13]=1;MatrixPrasanna[12][14]=1;MatrixPrasanna[12][15]=1;MatrixPrasanna[12][16]=0;MatrixPrasanna[12][17]=0;MatrixPrasanna[12][18]=0;MatrixPrasanna[12][19]=0;MatrixPrasanna[12][20]=0;MatrixPrasanna[12][21]=0;MatrixPrasanna[12][22]=0;MatrixPrasanna[12][23]=0;
	MatrixPrasanna[13][0]=0;MatrixPrasanna[13][1]=0;MatrixPrasanna[13][2]=0;MatrixPrasanna[13][3]=0;MatrixPrasanna[13][4]=0;MatrixPrasanna[13][5]=0;MatrixPrasanna[13][6]=0;MatrixPrasanna[13][7]=0;MatrixPrasanna[13][8]=1;MatrixPrasanna[13][9]=0;MatrixPrasanna[13][10]=0;MatrixPrasanna[13][11]=1;MatrixPrasanna[13][12]=1;MatrixPrasanna[13][13]=-4;MatrixPrasanna[13][14]=0;MatrixPrasanna[13][15]=0;MatrixPrasanna[13][16]=0;MatrixPrasanna[13][17]=0;MatrixPrasanna[13][18]=0;MatrixPrasanna[13][19]=0;MatrixPrasanna[13][20]=0;MatrixPrasanna[13][21]=0;MatrixPrasanna[13][22]=0;MatrixPrasanna[13][23]=0;
	MatrixPrasanna[14][0]=0;MatrixPrasanna[14][1]=0;MatrixPrasanna[14][2]=0;MatrixPrasanna[14][3]=0;MatrixPrasanna[14][4]=0;MatrixPrasanna[14][5]=0;MatrixPrasanna[14][6]=0;MatrixPrasanna[14][7]=0;MatrixPrasanna[14][8]=0;MatrixPrasanna[14][9]=1;MatrixPrasanna[14][10]=1;MatrixPrasanna[14][11]=1;MatrixPrasanna[14][12]=1;MatrixPrasanna[14][13]=0;MatrixPrasanna[14][14]=-4;MatrixPrasanna[14][15]=0;MatrixPrasanna[14][16]=0;MatrixPrasanna[14][17]=0;MatrixPrasanna[14][18]=0;MatrixPrasanna[14][19]=0;MatrixPrasanna[14][20]=0;MatrixPrasanna[14][21]=0;MatrixPrasanna[14][22]=0;MatrixPrasanna[14][23]=0;
	MatrixPrasanna[15][0]=0;MatrixPrasanna[15][1]=0;MatrixPrasanna[15][2]=0;MatrixPrasanna[15][3]=0;MatrixPrasanna[15][4]=0;MatrixPrasanna[15][5]=0;MatrixPrasanna[15][6]=0;MatrixPrasanna[15][7]=0;MatrixPrasanna[15][8]=1;MatrixPrasanna[15][9]=0;MatrixPrasanna[15][10]=1;MatrixPrasanna[15][11]=1;MatrixPrasanna[15][12]=1;MatrixPrasanna[15][13]=0;MatrixPrasanna[15][14]=0;MatrixPrasanna[15][15]=-4;MatrixPrasanna[15][16]=0;MatrixPrasanna[15][17]=0;MatrixPrasanna[15][18]=0;MatrixPrasanna[15][19]=0;MatrixPrasanna[15][20]=0;MatrixPrasanna[15][21]=0;MatrixPrasanna[15][22]=0;MatrixPrasanna[15][23]=0;
	MatrixPrasanna[16][0]=0;MatrixPrasanna[16][1]=0;MatrixPrasanna[16][2]=0;MatrixPrasanna[16][3]=0;MatrixPrasanna[16][4]=0;MatrixPrasanna[16][5]=0;MatrixPrasanna[16][6]=0;MatrixPrasanna[16][7]=0;MatrixPrasanna[16][8]=0;MatrixPrasanna[16][9]=0;MatrixPrasanna[16][10]=1;MatrixPrasanna[16][11]=1;MatrixPrasanna[16][12]=1;MatrixPrasanna[16][13]=1;MatrixPrasanna[16][14]=0;MatrixPrasanna[16][15]=0;MatrixPrasanna[16][16]=0;MatrixPrasanna[16][17]=0;MatrixPrasanna[16][18]=0;MatrixPrasanna[16][19]=0;MatrixPrasanna[16][20]=0;MatrixPrasanna[16][21]=0;MatrixPrasanna[16][22]=0;MatrixPrasanna[16][23]=0;
	MatrixPrasanna[17][0]=0;MatrixPrasanna[17][1]=0;MatrixPrasanna[17][2]=0;MatrixPrasanna[17][3]=0;MatrixPrasanna[17][4]=0;MatrixPrasanna[17][5]=0;MatrixPrasanna[17][6]=0;MatrixPrasanna[17][7]=0;MatrixPrasanna[17][8]=0;MatrixPrasanna[17][9]=1;MatrixPrasanna[17][10]=1;MatrixPrasanna[17][11]=1;MatrixPrasanna[17][12]=1;MatrixPrasanna[17][13]=1;MatrixPrasanna[17][14]=1;MatrixPrasanna[17][15]=0;MatrixPrasanna[17][16]=0;MatrixPrasanna[17][17]=0;MatrixPrasanna[17][18]=0;MatrixPrasanna[17][19]=0;MatrixPrasanna[17][20]=0;MatrixPrasanna[17][21]=0;MatrixPrasanna[17][22]=0;MatrixPrasanna[17][23]=0;
	MatrixPrasanna[18][0]=0;MatrixPrasanna[18][1]=0;MatrixPrasanna[18][2]=0;MatrixPrasanna[18][3]=0;MatrixPrasanna[18][4]=0;MatrixPrasanna[18][5]=0;MatrixPrasanna[18][6]=0;MatrixPrasanna[18][7]=0;MatrixPrasanna[18][8]=0;MatrixPrasanna[18][9]=0;MatrixPrasanna[18][10]=0;MatrixPrasanna[18][11]=0;MatrixPrasanna[18][12]=0;MatrixPrasanna[18][13]=1;MatrixPrasanna[18][14]=0;MatrixPrasanna[18][15]=0;MatrixPrasanna[18][16]=0;MatrixPrasanna[18][17]=0;MatrixPrasanna[18][18]=0;MatrixPrasanna[18][19]=0;MatrixPrasanna[18][20]=0;MatrixPrasanna[18][21]=0;MatrixPrasanna[18][22]=0;MatrixPrasanna[18][23]=0;
	MatrixPrasanna[19][0]=0;MatrixPrasanna[19][1]=0;MatrixPrasanna[19][2]=0;MatrixPrasanna[19][3]=0;MatrixPrasanna[19][4]=0;MatrixPrasanna[19][5]=0;MatrixPrasanna[19][6]=0;MatrixPrasanna[19][7]=0;MatrixPrasanna[19][8]=1;MatrixPrasanna[19][9]=0;MatrixPrasanna[19][10]=0;MatrixPrasanna[19][11]=0;MatrixPrasanna[19][12]=0;MatrixPrasanna[19][13]=0;MatrixPrasanna[19][14]=1;MatrixPrasanna[19][15]=0;MatrixPrasanna[19][16]=0;MatrixPrasanna[19][17]=0;MatrixPrasanna[19][18]=0;MatrixPrasanna[19][19]=0;MatrixPrasanna[19][20]=0;MatrixPrasanna[19][21]=0;MatrixPrasanna[19][22]=0;MatrixPrasanna[19][23]=0;
	MatrixPrasanna[20][0]=0;MatrixPrasanna[20][1]=0;MatrixPrasanna[20][2]=0;MatrixPrasanna[20][3]=0;MatrixPrasanna[20][4]=0;MatrixPrasanna[20][5]=0;MatrixPrasanna[20][6]=0;MatrixPrasanna[20][7]=0;MatrixPrasanna[20][8]=1;MatrixPrasanna[20][9]=1;MatrixPrasanna[20][10]=0;MatrixPrasanna[20][11]=0;MatrixPrasanna[20][12]=1;MatrixPrasanna[20][13]=0;MatrixPrasanna[20][14]=0;MatrixPrasanna[20][15]=1;MatrixPrasanna[20][16]=0;MatrixPrasanna[20][17]=0;MatrixPrasanna[20][18]=0;MatrixPrasanna[20][19]=0;MatrixPrasanna[20][20]=0;MatrixPrasanna[20][21]=0;MatrixPrasanna[20][22]=0;MatrixPrasanna[20][23]=0;
	MatrixPrasanna[21][0]=0;MatrixPrasanna[21][1]=0;MatrixPrasanna[21][2]=0;MatrixPrasanna[21][3]=0;MatrixPrasanna[21][4]=0;MatrixPrasanna[21][5]=0;MatrixPrasanna[21][6]=0;MatrixPrasanna[21][7]=0;MatrixPrasanna[21][8]=1;MatrixPrasanna[21][9]=1;MatrixPrasanna[21][10]=1;MatrixPrasanna[21][11]=0;MatrixPrasanna[21][12]=0;MatrixPrasanna[21][13]=1;MatrixPrasanna[21][14]=1;MatrixPrasanna[21][15]=0;MatrixPrasanna[21][16]=0;MatrixPrasanna[21][17]=0;MatrixPrasanna[21][18]=0;MatrixPrasanna[21][19]=0;MatrixPrasanna[21][20]=0;MatrixPrasanna[21][21]=0;MatrixPrasanna[21][22]=0;MatrixPrasanna[21][23]=0;
	MatrixPrasanna[22][0]=0;MatrixPrasanna[22][1]=0;MatrixPrasanna[22][2]=0;MatrixPrasanna[22][3]=0;MatrixPrasanna[22][4]=0;MatrixPrasanna[22][5]=0;MatrixPrasanna[22][6]=0;MatrixPrasanna[22][7]=0;MatrixPrasanna[22][8]=0;MatrixPrasanna[22][9]=0;MatrixPrasanna[22][10]=1;MatrixPrasanna[22][11]=0;MatrixPrasanna[22][12]=1;MatrixPrasanna[22][13]=0;MatrixPrasanna[22][14]=1;MatrixPrasanna[22][15]=1;MatrixPrasanna[22][16]=0;MatrixPrasanna[22][17]=0;MatrixPrasanna[22][18]=0;MatrixPrasanna[22][19]=0;MatrixPrasanna[22][20]=0;MatrixPrasanna[22][21]=0;MatrixPrasanna[22][22]=0;MatrixPrasanna[22][23]=0;
	MatrixPrasanna[23][0]=0;MatrixPrasanna[23][1]=0;MatrixPrasanna[23][2]=0;MatrixPrasanna[23][3]=0;MatrixPrasanna[23][4]=0;MatrixPrasanna[23][5]=0;MatrixPrasanna[23][6]=0;MatrixPrasanna[23][7]=0;MatrixPrasanna[23][8]=1;MatrixPrasanna[23][9]=1;MatrixPrasanna[23][10]=0;MatrixPrasanna[23][11]=0;MatrixPrasanna[23][12]=1;MatrixPrasanna[23][13]=0;MatrixPrasanna[23][14]=0;MatrixPrasanna[23][15]=0;MatrixPrasanna[23][16]=0;MatrixPrasanna[23][17]=0;MatrixPrasanna[23][18]=0;MatrixPrasanna[23][19]=0;MatrixPrasanna[23][20]=0;MatrixPrasanna[23][21]=0;MatrixPrasanna[23][22]=0;MatrixPrasanna[23][23]=0;
*/

/*
	MatrixPrasanna[0][0]=0;MatrixPrasanna[0][1]=0;MatrixPrasanna[0][2]=0;MatrixPrasanna[0][3]=0;MatrixPrasanna[0][4]=0;MatrixPrasanna[0][5]=0;MatrixPrasanna[0][6]=0;MatrixPrasanna[0][7]=0;MatrixPrasanna[0][8]=0;MatrixPrasanna[0][9]=0;MatrixPrasanna[0][10]=0;MatrixPrasanna[0][11]=0;MatrixPrasanna[0][12]=0;MatrixPrasanna[0][13]=0;MatrixPrasanna[0][14]=0;MatrixPrasanna[0][15]=0;MatrixPrasanna[0][16]=0;MatrixPrasanna[0][17]=1;MatrixPrasanna[0][18]=0;MatrixPrasanna[0][19]=0;MatrixPrasanna[0][20]=0;MatrixPrasanna[0][21]=0;MatrixPrasanna[0][22]=1;MatrixPrasanna[0][23]=1;
	MatrixPrasanna[1][0]=0;MatrixPrasanna[1][1]=0;MatrixPrasanna[1][2]=0;MatrixPrasanna[1][3]=0;MatrixPrasanna[1][4]=0;MatrixPrasanna[1][5]=0;MatrixPrasanna[1][6]=0;MatrixPrasanna[1][7]=0;MatrixPrasanna[1][8]=0;MatrixPrasanna[1][9]=0;MatrixPrasanna[1][10]=0;MatrixPrasanna[1][11]=0;MatrixPrasanna[1][12]=0;MatrixPrasanna[1][13]=0;MatrixPrasanna[1][14]=0;MatrixPrasanna[1][15]=0;MatrixPrasanna[1][16]=0;MatrixPrasanna[1][17]=1;MatrixPrasanna[1][18]=1;MatrixPrasanna[1][19]=0;MatrixPrasanna[1][20]=1;MatrixPrasanna[1][21]=0;MatrixPrasanna[1][22]=0;MatrixPrasanna[1][23]=1;
	MatrixPrasanna[2][0]=0;MatrixPrasanna[2][1]=0;MatrixPrasanna[2][2]=0;MatrixPrasanna[2][3]=0;MatrixPrasanna[2][4]=0;MatrixPrasanna[2][5]=0;MatrixPrasanna[2][6]=0;MatrixPrasanna[2][7]=0;MatrixPrasanna[2][8]=0;MatrixPrasanna[2][9]=0;MatrixPrasanna[2][10]=0;MatrixPrasanna[2][11]=0;MatrixPrasanna[2][12]=0;MatrixPrasanna[2][13]=0;MatrixPrasanna[2][14]=0;MatrixPrasanna[2][15]=0;MatrixPrasanna[2][16]=1;MatrixPrasanna[2][17]=1;MatrixPrasanna[2][18]=1;MatrixPrasanna[2][19]=0;MatrixPrasanna[2][20]=0;MatrixPrasanna[2][21]=0;MatrixPrasanna[2][22]=1;MatrixPrasanna[2][23]=0;
	MatrixPrasanna[3][0]=0;MatrixPrasanna[3][1]=0;MatrixPrasanna[3][2]=0;MatrixPrasanna[3][3]=0;MatrixPrasanna[3][4]=0;MatrixPrasanna[3][5]=0;MatrixPrasanna[3][6]=0;MatrixPrasanna[3][7]=0;MatrixPrasanna[3][8]=0;MatrixPrasanna[3][9]=0;MatrixPrasanna[3][10]=0;MatrixPrasanna[3][11]=0;MatrixPrasanna[3][12]=0;MatrixPrasanna[3][13]=0;MatrixPrasanna[3][14]=0;MatrixPrasanna[3][15]=0;MatrixPrasanna[3][16]=1;MatrixPrasanna[3][17]=0;MatrixPrasanna[3][18]=1;MatrixPrasanna[3][19]=1;MatrixPrasanna[3][20]=1;MatrixPrasanna[3][21]=0;MatrixPrasanna[3][22]=1;MatrixPrasanna[3][23]=0;
	MatrixPrasanna[4][0]=0;MatrixPrasanna[4][1]=0;MatrixPrasanna[4][2]=0;MatrixPrasanna[4][3]=0;MatrixPrasanna[4][4]=0;MatrixPrasanna[4][5]=0;MatrixPrasanna[4][6]=0;MatrixPrasanna[4][7]=0;MatrixPrasanna[4][8]=0;MatrixPrasanna[4][9]=0;MatrixPrasanna[4][10]=0;MatrixPrasanna[4][11]=0;MatrixPrasanna[4][12]=0;MatrixPrasanna[4][13]=0;MatrixPrasanna[4][14]=0;MatrixPrasanna[4][15]=0;MatrixPrasanna[4][16]=1;MatrixPrasanna[4][17]=0;MatrixPrasanna[4][18]=1;MatrixPrasanna[4][19]=1;MatrixPrasanna[4][20]=1;MatrixPrasanna[4][21]=0;MatrixPrasanna[4][22]=0;MatrixPrasanna[4][23]=0;
	MatrixPrasanna[5][0]=0;MatrixPrasanna[5][1]=0;MatrixPrasanna[5][2]=0;MatrixPrasanna[5][3]=0;MatrixPrasanna[5][4]=0;MatrixPrasanna[5][5]=0;MatrixPrasanna[5][6]=0;MatrixPrasanna[5][7]=0;MatrixPrasanna[5][8]=0;MatrixPrasanna[5][9]=0;MatrixPrasanna[5][10]=0;MatrixPrasanna[5][11]=0;MatrixPrasanna[5][12]=0;MatrixPrasanna[5][13]=0;MatrixPrasanna[5][14]=0;MatrixPrasanna[5][15]=0;MatrixPrasanna[5][16]=0;MatrixPrasanna[5][17]=0;MatrixPrasanna[5][18]=0;MatrixPrasanna[5][19]=0;MatrixPrasanna[5][20]=0;MatrixPrasanna[5][21]=0;MatrixPrasanna[5][22]=1;MatrixPrasanna[5][23]=0;
	MatrixPrasanna[6][0]=0;MatrixPrasanna[6][1]=0;MatrixPrasanna[6][2]=0;MatrixPrasanna[6][3]=0;MatrixPrasanna[6][4]=0;MatrixPrasanna[6][5]=0;MatrixPrasanna[6][6]=0;MatrixPrasanna[6][7]=0;MatrixPrasanna[6][8]=0;MatrixPrasanna[6][9]=0;MatrixPrasanna[6][10]=0;MatrixPrasanna[6][11]=0;MatrixPrasanna[6][12]=0;MatrixPrasanna[6][13]=0;MatrixPrasanna[6][14]=0;MatrixPrasanna[6][15]=0;MatrixPrasanna[6][16]=0;MatrixPrasanna[6][17]=0;MatrixPrasanna[6][18]=0;MatrixPrasanna[6][19]=1;MatrixPrasanna[6][20]=1;MatrixPrasanna[6][21]=0;MatrixPrasanna[6][22]=1;MatrixPrasanna[6][23]=1;
	MatrixPrasanna[7][0]=0;MatrixPrasanna[7][1]=0;MatrixPrasanna[7][2]=0;MatrixPrasanna[7][3]=0;MatrixPrasanna[7][4]=0;MatrixPrasanna[7][5]=0;MatrixPrasanna[7][6]=0;MatrixPrasanna[7][7]=0;MatrixPrasanna[7][8]=0;MatrixPrasanna[7][9]=0;MatrixPrasanna[7][10]=0;MatrixPrasanna[7][11]=0;MatrixPrasanna[7][12]=0;MatrixPrasanna[7][13]=0;MatrixPrasanna[7][14]=0;MatrixPrasanna[7][15]=0;MatrixPrasanna[7][16]=0;MatrixPrasanna[7][17]=1;MatrixPrasanna[7][18]=0;MatrixPrasanna[7][19]=0;MatrixPrasanna[7][20]=0;MatrixPrasanna[7][21]=1;MatrixPrasanna[7][22]=0;MatrixPrasanna[7][23]=0;
	MatrixPrasanna[8][0]=0;MatrixPrasanna[8][1]=0;MatrixPrasanna[8][2]=0;MatrixPrasanna[8][3]=0;MatrixPrasanna[8][4]=0;MatrixPrasanna[8][5]=0;MatrixPrasanna[8][6]=0;MatrixPrasanna[8][7]=0;MatrixPrasanna[8][8]=0;MatrixPrasanna[8][9]=0;MatrixPrasanna[8][10]=0;MatrixPrasanna[8][11]=0;MatrixPrasanna[8][12]=0;MatrixPrasanna[8][13]=0;MatrixPrasanna[8][14]=0;MatrixPrasanna[8][15]=0;MatrixPrasanna[8][16]=0;MatrixPrasanna[8][17]=0;MatrixPrasanna[8][18]=0;MatrixPrasanna[8][19]=1;MatrixPrasanna[8][20]=1;MatrixPrasanna[8][21]=1;MatrixPrasanna[8][22]=0;MatrixPrasanna[8][23]=1;
	MatrixPrasanna[9][0]=0;MatrixPrasanna[9][1]=0;MatrixPrasanna[9][2]=0;MatrixPrasanna[9][3]=0;MatrixPrasanna[9][4]=0;MatrixPrasanna[9][5]=0;MatrixPrasanna[9][6]=0;MatrixPrasanna[9][7]=0;MatrixPrasanna[9][8]=0;MatrixPrasanna[9][9]=0;MatrixPrasanna[9][10]=0;MatrixPrasanna[9][11]=0;MatrixPrasanna[9][12]=0;MatrixPrasanna[9][13]=0;MatrixPrasanna[9][14]=0;MatrixPrasanna[9][15]=0;MatrixPrasanna[9][16]=0;MatrixPrasanna[9][17]=1;MatrixPrasanna[9][18]=0;MatrixPrasanna[9][19]=0;MatrixPrasanna[9][20]=1;MatrixPrasanna[9][21]=1;MatrixPrasanna[9][22]=0;MatrixPrasanna[9][23]=1;
	MatrixPrasanna[10][0]=0;MatrixPrasanna[10][1]=0;MatrixPrasanna[10][2]=0;MatrixPrasanna[10][3]=0;MatrixPrasanna[10][4]=0;MatrixPrasanna[10][5]=0;MatrixPrasanna[10][6]=0;MatrixPrasanna[10][7]=0;MatrixPrasanna[10][8]=0;MatrixPrasanna[10][9]=0;MatrixPrasanna[10][10]=0;MatrixPrasanna[10][11]=0;MatrixPrasanna[10][12]=0;MatrixPrasanna[10][13]=0;MatrixPrasanna[10][14]=0;MatrixPrasanna[10][15]=0;MatrixPrasanna[10][16]=1;MatrixPrasanna[10][17]=1;MatrixPrasanna[10][18]=0;MatrixPrasanna[10][19]=0;MatrixPrasanna[10][20]=0;MatrixPrasanna[10][21]=1;MatrixPrasanna[10][22]=1;MatrixPrasanna[10][23]=0;
	MatrixPrasanna[11][0]=0;MatrixPrasanna[11][1]=0;MatrixPrasanna[11][2]=0;MatrixPrasanna[11][3]=0;MatrixPrasanna[11][4]=0;MatrixPrasanna[11][5]=0;MatrixPrasanna[11][6]=0;MatrixPrasanna[11][7]=0;MatrixPrasanna[11][8]=0;MatrixPrasanna[11][9]=0;MatrixPrasanna[11][10]=0;MatrixPrasanna[11][11]=0;MatrixPrasanna[11][12]=0;MatrixPrasanna[11][13]=0;MatrixPrasanna[11][14]=0;MatrixPrasanna[11][15]=0;MatrixPrasanna[11][16]=1;MatrixPrasanna[11][17]=1;MatrixPrasanna[11][18]=0;MatrixPrasanna[11][19]=0;MatrixPrasanna[11][20]=0;MatrixPrasanna[11][21]=0;MatrixPrasanna[11][22]=0;MatrixPrasanna[11][23]=0;
	MatrixPrasanna[12][0]=0;MatrixPrasanna[12][1]=0;MatrixPrasanna[12][2]=0;MatrixPrasanna[12][3]=0;MatrixPrasanna[12][4]=0;MatrixPrasanna[12][5]=0;MatrixPrasanna[12][6]=0;MatrixPrasanna[12][7]=0;MatrixPrasanna[12][8]=0;MatrixPrasanna[12][9]=0;MatrixPrasanna[12][10]=0;MatrixPrasanna[12][11]=0;MatrixPrasanna[12][12]=0;MatrixPrasanna[12][13]=0;MatrixPrasanna[12][14]=0;MatrixPrasanna[12][15]=0;MatrixPrasanna[12][16]=1;MatrixPrasanna[12][17]=1;MatrixPrasanna[12][18]=0;MatrixPrasanna[12][19]=0;MatrixPrasanna[12][20]=1;MatrixPrasanna[12][21]=0;MatrixPrasanna[12][22]=1;MatrixPrasanna[12][23]=1;
	MatrixPrasanna[13][0]=0;MatrixPrasanna[13][1]=0;MatrixPrasanna[13][2]=0;MatrixPrasanna[13][3]=0;MatrixPrasanna[13][4]=0;MatrixPrasanna[13][5]=0;MatrixPrasanna[13][6]=0;MatrixPrasanna[13][7]=0;MatrixPrasanna[13][8]=0;MatrixPrasanna[13][9]=0;MatrixPrasanna[13][10]=0;MatrixPrasanna[13][11]=0;MatrixPrasanna[13][12]=0;MatrixPrasanna[13][13]=0;MatrixPrasanna[13][14]=0;MatrixPrasanna[13][15]=0;MatrixPrasanna[13][16]=1;MatrixPrasanna[13][17]=1;MatrixPrasanna[13][18]=1;MatrixPrasanna[13][19]=0;MatrixPrasanna[13][20]=0;MatrixPrasanna[13][21]=1;MatrixPrasanna[13][22]=0;MatrixPrasanna[13][23]=0;
	MatrixPrasanna[14][0]=0;MatrixPrasanna[14][1]=0;MatrixPrasanna[14][2]=0;MatrixPrasanna[14][3]=0;MatrixPrasanna[14][4]=0;MatrixPrasanna[14][5]=0;MatrixPrasanna[14][6]=0;MatrixPrasanna[14][7]=0;MatrixPrasanna[14][8]=0;MatrixPrasanna[14][9]=0;MatrixPrasanna[14][10]=0;MatrixPrasanna[14][11]=0;MatrixPrasanna[14][12]=0;MatrixPrasanna[14][13]=0;MatrixPrasanna[14][14]=0;MatrixPrasanna[14][15]=0;MatrixPrasanna[14][16]=0;MatrixPrasanna[14][17]=1;MatrixPrasanna[14][18]=0;MatrixPrasanna[14][19]=1;MatrixPrasanna[14][20]=0;MatrixPrasanna[14][21]=1;MatrixPrasanna[14][22]=1;MatrixPrasanna[14][23]=0;
	MatrixPrasanna[15][0]=0;MatrixPrasanna[15][1]=0;MatrixPrasanna[15][2]=0;MatrixPrasanna[15][3]=0;MatrixPrasanna[15][4]=0;MatrixPrasanna[15][5]=0;MatrixPrasanna[15][6]=0;MatrixPrasanna[15][7]=0;MatrixPrasanna[15][8]=0;MatrixPrasanna[15][9]=0;MatrixPrasanna[15][10]=0;MatrixPrasanna[15][11]=0;MatrixPrasanna[15][12]=0;MatrixPrasanna[15][13]=0;MatrixPrasanna[15][14]=0;MatrixPrasanna[15][15]=0;MatrixPrasanna[15][16]=0;MatrixPrasanna[15][17]=0;MatrixPrasanna[15][18]=0;MatrixPrasanna[15][19]=0;MatrixPrasanna[15][20]=1;MatrixPrasanna[15][21]=0;MatrixPrasanna[15][22]=1;MatrixPrasanna[15][23]=0;
	MatrixPrasanna[16][0]=0;MatrixPrasanna[16][1]=0;MatrixPrasanna[16][2]=0;MatrixPrasanna[16][3]=0;MatrixPrasanna[16][4]=0;MatrixPrasanna[16][5]=0;MatrixPrasanna[16][6]=0;MatrixPrasanna[16][7]=0;MatrixPrasanna[16][8]=0;MatrixPrasanna[16][9]=0;MatrixPrasanna[16][10]=0;MatrixPrasanna[16][11]=0;MatrixPrasanna[16][12]=0;MatrixPrasanna[16][13]=0;MatrixPrasanna[16][14]=0;MatrixPrasanna[16][15]=0;MatrixPrasanna[16][16]=-4;MatrixPrasanna[16][17]=0;MatrixPrasanna[16][18]=1;MatrixPrasanna[16][19]=1;MatrixPrasanna[16][20]=0;MatrixPrasanna[16][21]=0;MatrixPrasanna[16][22]=1;MatrixPrasanna[16][23]=1;
	MatrixPrasanna[17][0]=0;MatrixPrasanna[17][1]=0;MatrixPrasanna[17][2]=0;MatrixPrasanna[17][3]=0;MatrixPrasanna[17][4]=0;MatrixPrasanna[17][5]=0;MatrixPrasanna[17][6]=0;MatrixPrasanna[17][7]=0;MatrixPrasanna[17][8]=0;MatrixPrasanna[17][9]=0;MatrixPrasanna[17][10]=0;MatrixPrasanna[17][11]=0;MatrixPrasanna[17][12]=0;MatrixPrasanna[17][13]=0;MatrixPrasanna[17][14]=0;MatrixPrasanna[17][15]=0;MatrixPrasanna[17][16]=0;MatrixPrasanna[17][17]=-4;MatrixPrasanna[17][18]=1;MatrixPrasanna[17][19]=0;MatrixPrasanna[17][20]=1;MatrixPrasanna[17][21]=1;MatrixPrasanna[17][22]=0;MatrixPrasanna[17][23]=1;
	MatrixPrasanna[18][0]=0;MatrixPrasanna[18][1]=0;MatrixPrasanna[18][2]=0;MatrixPrasanna[18][3]=0;MatrixPrasanna[18][4]=0;MatrixPrasanna[18][5]=0;MatrixPrasanna[18][6]=0;MatrixPrasanna[18][7]=0;MatrixPrasanna[18][8]=0;MatrixPrasanna[18][9]=0;MatrixPrasanna[18][10]=0;MatrixPrasanna[18][11]=0;MatrixPrasanna[18][12]=0;MatrixPrasanna[18][13]=0;MatrixPrasanna[18][14]=0;MatrixPrasanna[18][15]=0;MatrixPrasanna[18][16]=1;MatrixPrasanna[18][17]=1;MatrixPrasanna[18][18]=-4;MatrixPrasanna[18][19]=1;MatrixPrasanna[18][20]=1;MatrixPrasanna[18][21]=1;MatrixPrasanna[18][22]=0;MatrixPrasanna[18][23]=0;
	MatrixPrasanna[19][0]=0;MatrixPrasanna[19][1]=0;MatrixPrasanna[19][2]=0;MatrixPrasanna[19][3]=0;MatrixPrasanna[19][4]=0;MatrixPrasanna[19][5]=0;MatrixPrasanna[19][6]=0;MatrixPrasanna[19][7]=0;MatrixPrasanna[19][8]=0;MatrixPrasanna[19][9]=0;MatrixPrasanna[19][10]=0;MatrixPrasanna[19][11]=0;MatrixPrasanna[19][12]=0;MatrixPrasanna[19][13]=0;MatrixPrasanna[19][14]=0;MatrixPrasanna[19][15]=0;MatrixPrasanna[19][16]=1;MatrixPrasanna[19][17]=0;MatrixPrasanna[19][18]=1;MatrixPrasanna[19][19]=-4;MatrixPrasanna[19][20]=1;MatrixPrasanna[19][21]=1;MatrixPrasanna[19][22]=1;MatrixPrasanna[19][23]=0;
	MatrixPrasanna[20][0]=0;MatrixPrasanna[20][1]=0;MatrixPrasanna[20][2]=0;MatrixPrasanna[20][3]=0;MatrixPrasanna[20][4]=0;MatrixPrasanna[20][5]=0;MatrixPrasanna[20][6]=0;MatrixPrasanna[20][7]=0;MatrixPrasanna[20][8]=0;MatrixPrasanna[20][9]=0;MatrixPrasanna[20][10]=0;MatrixPrasanna[20][11]=0;MatrixPrasanna[20][12]=0;MatrixPrasanna[20][13]=0;MatrixPrasanna[20][14]=0;MatrixPrasanna[20][15]=0;MatrixPrasanna[20][16]=0;MatrixPrasanna[20][17]=1;MatrixPrasanna[20][18]=1;MatrixPrasanna[20][19]=1;MatrixPrasanna[20][20]=-4;MatrixPrasanna[20][21]=0;MatrixPrasanna[20][22]=0;MatrixPrasanna[20][23]=0;
	MatrixPrasanna[21][0]=0;MatrixPrasanna[21][1]=0;MatrixPrasanna[21][2]=0;MatrixPrasanna[21][3]=0;MatrixPrasanna[21][4]=0;MatrixPrasanna[21][5]=0;MatrixPrasanna[21][6]=0;MatrixPrasanna[21][7]=0;MatrixPrasanna[21][8]=0;MatrixPrasanna[21][9]=0;MatrixPrasanna[21][10]=0;MatrixPrasanna[21][11]=0;MatrixPrasanna[21][12]=0;MatrixPrasanna[21][13]=0;MatrixPrasanna[21][14]=0;MatrixPrasanna[21][15]=0;MatrixPrasanna[21][16]=0;MatrixPrasanna[21][17]=1;MatrixPrasanna[21][18]=1;MatrixPrasanna[21][19]=1;MatrixPrasanna[21][20]=0;MatrixPrasanna[21][21]=-4;MatrixPrasanna[21][22]=0;MatrixPrasanna[21][23]=0;
	MatrixPrasanna[22][0]=0;MatrixPrasanna[22][1]=0;MatrixPrasanna[22][2]=0;MatrixPrasanna[22][3]=0;MatrixPrasanna[22][4]=0;MatrixPrasanna[22][5]=0;MatrixPrasanna[22][6]=0;MatrixPrasanna[22][7]=0;MatrixPrasanna[22][8]=0;MatrixPrasanna[22][9]=0;MatrixPrasanna[22][10]=0;MatrixPrasanna[22][11]=0;MatrixPrasanna[22][12]=0;MatrixPrasanna[22][13]=0;MatrixPrasanna[22][14]=0;MatrixPrasanna[22][15]=0;MatrixPrasanna[22][16]=1;MatrixPrasanna[22][17]=0;MatrixPrasanna[22][18]=0;MatrixPrasanna[22][19]=1;MatrixPrasanna[22][20]=0;MatrixPrasanna[22][21]=0;MatrixPrasanna[22][22]=-4;MatrixPrasanna[22][23]=0;
	MatrixPrasanna[23][0]=0;MatrixPrasanna[23][1]=0;MatrixPrasanna[23][2]=0;MatrixPrasanna[23][3]=0;MatrixPrasanna[23][4]=0;MatrixPrasanna[23][5]=0;MatrixPrasanna[23][6]=0;MatrixPrasanna[23][7]=0;MatrixPrasanna[23][8]=0;MatrixPrasanna[23][9]=0;MatrixPrasanna[23][10]=0;MatrixPrasanna[23][11]=0;MatrixPrasanna[23][12]=0;MatrixPrasanna[23][13]=0;MatrixPrasanna[23][14]=0;MatrixPrasanna[23][15]=0;MatrixPrasanna[23][16]=1;MatrixPrasanna[23][17]=1;MatrixPrasanna[23][18]=0;MatrixPrasanna[23][19]=0;MatrixPrasanna[23][20]=0;MatrixPrasanna[23][21]=0;MatrixPrasanna[23][22]=0;MatrixPrasanna[23][23]=-4;
*/
}

void Robot::countingArrivals()
{
	numberArrivals+=1;
	if(this->flag_Nodes_already_Received==false)
	{
		if(numberArrivals==2*(NPROC-1))
		{
//			imprimirMensaje("Reconoce que llegaron todos los estados... debe ir a la siguiente fase");
			numberArrivals=0;
			this->flag_Nodes_already_Received=true;
			emit go_NextPhase();
		}
	}else{
		if(numberArrivals==(NPROC-1))
		{
//			imprimirMensaje("Reconoce que llegaron todos los estados... debe ir a la siguiente fase");
			numberArrivals=0;
			emit go_NextPhase();
		}
	}
}

void Robot::nextPhase()
{
//	TODO: The sequence of if below must be replaced for something more efficient
	if(this->RobotID==1)ArrangeOutput(this->d.size(),this->otherProcessors[this->RobotID].States.size(),this->otherProcessors[(this->RobotID)+1].States.size(),this->d.data(),this->otherProcessors[(this->RobotID)].States.data(),this->otherProcessors[(this->RobotID)+1].States.data(),this->IniNodesV.data(),this->otherProcessors[this->RobotID].Nodes.data(),this->otherProcessors[(this->RobotID)+1].Nodes.data() , this->results.data());
	if(this->RobotID==2)ArrangeOutput(this->d.size(),this->otherProcessors[(this->RobotID)-2].States.size(),this->otherProcessors[(this->RobotID)].States.size(),this->d.data(),this->otherProcessors[(this->RobotID)-2].States.data(),this->otherProcessors[(this->RobotID)].States.data(),this->IniNodesV.data(),this->otherProcessors[(this->RobotID)-2].Nodes.data(),this->otherProcessors[(this->RobotID)].Nodes.data() , this->results.data());
	if(this->RobotID==3)ArrangeOutput(this->d.size(),this->otherProcessors[(this->RobotID)-3].States.size(),this->otherProcessors[(this->RobotID)-2].States.size(),this->d.data(),this->otherProcessors[(this->RobotID)-3].States.data(),this->otherProcessors[(this->RobotID)-2].States.data(),this->IniNodesV.data(),this->otherProcessors[(this->RobotID)-3].Nodes.data(),this->otherProcessors[(this->RobotID)-2].Nodes.data() , this->results.data());
	this->iterator_Varsha+=1;
	fprintf(this->OutputStates,"%d	",this->iterator_Varsha);

	int j;
	ComputeNextState( this->IniStatesV.size(), this->IniStatesV.data(),this->IniNodesV.data(),this->results.data(),this->q.data());
	this->IniStatesV=this->q;
//	imprimirMensaje("Nuevo Estado");
	for(j=0;j<(this->IniStatesV.size());j++)
	{
		if(j<((this->IniStatesV.size())-1))
		{
			fprintf(this->OutputStates,"%6.4f		",this->IniStatesV[j]);
		}else{
			fprintf(this->OutputStates,"%6.4f;\n",this->IniStatesV[j]);
		}
	}
	printf("\n");
	this->ComputeOutput(IniStatesV.size(),IniStatesV.data(),d.data());

//	imprimirMensaje("De aqui a volver a empezar");
	if(this->iterator_Varsha<=1000)
	{
		imprimirMensaje("iterator_Varsha= "+QString::number(this->iterator_Varsha,10));
		emit signal_MakeSending();
	}else
	{
		this->iterator_Varsha=0;
		fprintf(this->OutputStates,"];");
		fclose(this->OutputStates);
	}
}

void Robot::hacerEnvio()
{
	int aa;

	for(aa=1;aa<=NPROC;aa++)
	{
		if(aa==RobotID)continue;
		while(this->arregloDeSockets[aa-1]->enviarSolicitud(d.data(),d.size())==-1 ) ;
	}
	if (this->flag_Nodes_already_Sent==false)
	{
		for(aa=1;aa<=NPROC;aa++)
		{
			if(aa==RobotID)continue;
			while(this->arregloDeSockets[aa-1]->enviarSolicitud(IniNodesV.data(),IniNodesV.size()) ==-1 ) ;
		}
		this->flag_Nodes_already_Sent=true;
	}
}
Robot::~Robot()
{
	delete Server;
}


void Robot::ComputeOutput( int n, double *RobState, double *d)
{
	state= RobState;
	int i ;
	for ( i=0 ; i<n ; i++ )
	{
		d[i] = 0.5*(fabs(state[i]+1)-fabs(state[i]-1));
	}

}
void Robot::ComputeNextState( int p, double *RobState, int* RobNodeNumber,double *results, double* q)
{
	state = RobState;
  	node_number=RobNodeNumber;
  	int a; int i;
  	double weight_term;

  	for(a=0;a<p;a++)
 	{
 		weight_term=0;
 		for(i=0;i<NSIZE;i++)
 		{
			weight_term=weight_term + Weight_Matrix[RobNodeNumber[a]][i]*results[i];
 		}
		q[a]=RobState[a]+hHH*weight_term + (exp(hHH)-1)*(exp(-hHH))*Bias_vector[RobNodeNumber[a]];
	}
}
void Robot::ArrangeOutput(int l, int m, int n,  double *d1, double *d2,double *d3,int *Rob1NodeNumber, int *Rob2NodeNumber,int *Rob3NodeNumber, double *result)
{
	int a;int b;int c;
	for(a=0;a<l;a++)result[Rob1NodeNumber[a]]=d1[a];
	for(b=0;b<m;b++)result[Rob2NodeNumber[b]]=d2[b];
	for(c=0;c<n;c++)result[Rob3NodeNumber[c]]=d3[c];
}

void Robot::arrivalNodes()
{
	emit ack_GoNextPhase();
}
void Robot::arrivalStates()
{
	emit ack_GoNextPhase();
}


void Robot::SendWord()
{
	while(this->arregloDeSockets[0]->enviarSolicitud("JacSec_Over")==-1)
	{
		imprimirMensaje("intentando enviar se~Nal JacSec_Over ");
	}
}
void Robot::SendAck()
{
	int pp;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("GotAllVs")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal GotAllVs");
		}
	}
}
void Robot::SendEndJacobiI()
{
	int pp;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("signalEndJacobiI")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal signalEndJacobiI");
		}
	}
}
void Robot::SendEndJacobiII()
{
	int pp;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("signalEndJacobiII")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal signalEndJacobiII");
		}
	}
}
void Robot::SendSpark()
{
	int pp;
	imprimirMensaje("ESTA ENVIANDO INIT_VARSHA_SEC ");
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("Init_VarshaAlg")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal Init_VarshaAlg");
		}
	}
}
void Robot::SendSpark_Jacobi()
{
	int pp;
	imprimirMensaje("ESTA ENVIANDO INIT_JACSEC ");
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("Init_JacSec")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal Init_JacSec ");
		}
	}
}
void Robot::SendPMidJM()
{
	int pp;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud("ProcMidJM")==-1)
		{
			imprimirMensaje("intentando enviar se~Nal ProcMidJM ");
		}
	}
}
void Robot::JacobiMethod()
{
	this->mJMI.lock();
	int b=0;
	int p=0,q=0,pb=0,qb=0;
	double co,si;
	double RotMat[2][2];
	double TempMatrix[2][2];
	double TempMatrix2[2][2];
	double MatrizL[NSIZE][2];
	double MatrizR[NSIZE][2];

//	imprimirMensaje("JacobiMethod ");

	if((this->top[iteradorColumnasJacobi])>(this->botton[iteradorColumnasJacobi]))
	{
		p=this->botton[iteradorColumnasJacobi];
		q=this->top[iteradorColumnasJacobi];
	}else{
		q=this->botton[iteradorColumnasJacobi];
		p=this->top[iteradorColumnasJacobi];
	}
	valSineCosine(MatrixPrasanna,p,q,&co,&si);

	RotMat[0][0]=co;
	RotMat[0][1]=si;
	RotMat[1][0]=-si;
	RotMat[1][1]=co;

	this->co=co;
	this->si=si;
	this->cp=p;
	this->cq=q;

	SelCols(MatrixPrasanna,p,q,MatrizL);
	multiplicarMatrices(MatrizL,RotMat,MatrizR,NSIZE);
	DevCols(MatrizR,p,q,MatrixPrasanna);

	RotMat[0][0]=co;
	RotMat[0][1]=-si;
	RotMat[1][0]=si;
	RotMat[1][1]=co;

	for(b=(NSIZE/(2*NPROC))*(this->RobotID - 1);b<(NSIZE/(2*NPROC))*(this->RobotID);b++)
	{
		if((this->top[b])>(this->botton[b]))
		{
			pb=this->botton[b];
			qb=this->top[b];
		}else{
			qb=this->botton[b];
			pb=this->top[b];
		}
		SelCols(MatrixPrasanna,pb,qb,MatrizR);
		hacerCasting_RelatedParts(MatrizR,TempMatrix,p,q);
		multiplicarMatrices(RotMat,TempMatrix,TempMatrix2,2);
		devolverResultado_VSmallT(MatrizR,TempMatrix2,p,q);
		DevCols(MatrizR,pb,qb,MatrixPrasanna);

	}
//	imprimirMensaje("Final JacobiMethod I");
	verificationFinJacI(this->RobotID);
	emit signalEndJacobiI();

	this->mJMI.unlock();
}

void Robot::JacobiMethodII()
{
	QVector<int> TempA(NSIZE/(2*NPROC));
	QVector<int> TempB(NSIZE/(2*NPROC));
	this->mJMII.lock();
//	imprimirMensaje("JacobiMethodII ");

	int b=0,c=0,d=0;
	int pb=0,qb=0;

	double RotMat[2][2];
	double TempMatrix[2][2];
	double TempMatrix2[2][2];
	double MatrizR[NSIZE][2];

	for(c=1;c<=NPROC;c++)
	{
		if(c==this->RobotID)
		{
			continue;
		}

		RotMat[0][0]=(this->Buffer[c-1].Cosine);
		RotMat[0][1]=-(this->Buffer[c-1].Sine);
		RotMat[1][0]=(this->Buffer[c-1].Sine);
		RotMat[1][1]=(this->Buffer[c-1].Cosine);

		for(b=(NSIZE/(2*NPROC))*(this->RobotID - 1);b<(NSIZE/(2*NPROC))*(this->RobotID);b++)
		{

			if((this->top[b])>(this->botton[b]))
			{
				pb=this->botton[b];
				qb=this->top[b];
			}else{
				qb=this->botton[b];
				pb=this->top[b];
			}
			SelCols(MatrixPrasanna,pb,qb,MatrizR);
			hacerCasting_RelatedParts(MatrizR,TempMatrix,this->Buffer[c-1].p,this->Buffer[c-1].q);
			multiplicarMatrices(RotMat,TempMatrix,TempMatrix2,2);
			devolverResultado_VSmallT(MatrizR,TempMatrix2,this->Buffer[c-1].p,this->Buffer[c-1].q);
			DevCols(MatrizR,pb,qb,MatrixPrasanna);
		}

		this->Buffer[c-1].libre=true;
//		bufferLleno=0;
	}
//	imprimirMensaje("Trabajando en JacobiMethodII ... iteradorColumnasJacobi = "+QString::number(iteradorColumnasJacobi,10)+"LIMITE "+QString::number((((NSIZE/(2*NPROC))*(this->RobotID))-1),10));

	if(iteradorColumnasJacobi==((NSIZE/(2*NPROC))*(this->RobotID))-1)
	{
		int ColsToSend;
		parallelOrdering(this->top.data(),this->botton.data(),this->Ntop.data(),this->Nbotton.data(),NSIZE);
		this->top=this->Ntop;
		this->botton=this->Nbotton;

		for(d=(NSIZE/(2*NPROC))*(this->RobotID - 1);d<(NSIZE/(2*NPROC))*(this->RobotID);d++)
		{
			TempA[d-(NSIZE/(2*NPROC))*(this->RobotID - 1)]=this->top[d];
			TempB[d-(NSIZE/(2*NPROC))*(this->RobotID - 1)]=this->botton[d];
		}

		if(this->RobotID == 1)
		{
//			imprimirMensaje("DERECHA Columna a enviar = "+QString::number(this->top[(NSIZE/(2*NPROC))*this->RobotID],10));
			ColsToSend=(this->top[(NSIZE/(2*NPROC))*this->RobotID]);
			emit signalsRight(ColsToSend);
			//ENVIAR A LA DERECHA

		}else if (this->RobotID == NPROC)
		{
//			imprimirMensaje("IZQUIERDA Columna a enviar = "+QString::number(this->botton[((NSIZE/(2*NPROC))*(this->RobotID - 1))-1],10));
			ColsToSend=(this->botton[((NSIZE/(2*NPROC))*(this->RobotID - 1))-1]);
			emit signalsLeft(ColsToSend);
			//ENVIAR A LA IZQUIERDA

		}else
		{
//			imprimirMensaje("DERECHA Columna a enviar = "+QString::number(this->top[(NSIZE/(2*NPROC))*this->RobotID],10));
			ColsToSend=(this->top[(NSIZE/(2*NPROC))*this->RobotID]);
			emit signalsRight(ColsToSend);
			//ENVIAR A LA DERECHA

//			imprimirMensaje("IZQUIERDA Columna a enviar = "+QString::number(this->botton[((NSIZE/(2*NPROC))*(this->RobotID - 1))-1],10));
			ColsToSend=(this->botton[((NSIZE/(2*NPROC))*(this->RobotID - 1))-1]);
			emit signalsLeft(ColsToSend);
			//ENVIAR A LA IZQUIERDA
		}
		iteradorColumnasJacobi=(NSIZE/(2*NPROC))*(this->RobotID - 1);
		finalstage_JacobiII=true;
		emit signalPosColandENDCycle();;
	}else{
		iteradorColumnasJacobi++;
		emit finalJacobiII();
	}

/*
	if((this->RobotID!=1)&&(this->RobotID!=NPROC))
	{
		if(bothcolumns_received==true)imprimirMensaje("bothcolumns_received = true");
		if(finalstage_JacobiII==true)imprimirMensaje("finalstage_JacobiII = true");
		if(bothcolumns_received==false)imprimirMensaje("bothcolumns_received = false");
		if(finalstage_JacobiII==false)imprimirMensaje("finalstage_JacobiII = false");
	}else{
		if(this->RobotID==1)
		{
			if(this->bufferColumnsArrival[1].newColumn==true)imprimirMensaje("bothcolumns_received = true");
			if(this->finalstage_JacobiII==true)imprimirMensaje("finalstage_JacobiII = true");
			if(this->bufferColumnsArrival[1].newColumn==false)imprimirMensaje("bufferColumnsArrival[1] = false");
			if(this->finalstage_JacobiII==false)imprimirMensaje("finalstage_JacobiII = false");
		}
		if(this->RobotID==NPROC)
		{
			if(this->bufferColumnsArrival[0].newColumn==true)imprimirMensaje("bufferColumnsArrival[0] = true");
			if(this->finalstage_JacobiII==true)imprimirMensaje("finalstage_JacobiII = true");
			if(this->bufferColumnsArrival[0].newColumn==false)imprimirMensaje("bufferColumnsArrival[0] = false");
			if(this->finalstage_JacobiII==false)imprimirMensaje("finalstage_JacobiII = false");
		}
	}
*/

//	imprimirMensaje("Final Jacobi II");
	this->mJMII.unlock();
}

void Robot::setJacobiOption()
{
	this->JacobiOption=true;
	this->VarshaOption=false;

}
void Robot::setVarshaOption()
{
	this->JacobiOption=false;
	this->VarshaOption=true;

}
void Robot::beginSelectedAlgorithm()
{
	if((this->JacobiOption==true)&&(this->VarshaOption==false))
	{
		SendSpark_Jacobi();
		JacobiMethod();
		imprimirMensaje("Do Jacobi Algorithm");
	}
	if((this->JacobiOption==false)&&(this->VarshaOption==true))
	{
		imprimirMensaje("Do Varsha Algorithm");
		SendSpark();
		hacerEnvio();
	}
}
void Robot::arrivalVsMatrixSiCos(double s,double c,int p,int q,int id)
{
	this->witnessVsmallArrivals=(this->witnessVsmallArrivals) +1;
	if(this->witnessVsmallArrivals <= NPROC)
	{
		this->Buffer[id-1].Cosine=c;
		this->Buffer[id-1].Sine=s;
		this->Buffer[id-1].p=p;
		this->Buffer[id-1].q=q;
		this->Buffer[id-1].libre=false;

		if(this->witnessVsmallArrivals==NPROC)
		{
			this->witnessVsmallArrivals=1;
			this->verificationStageIIJM(this->RobotID);
			emit GotAllVs();
		}
	}
}
void Robot::SendSinCosine(double s,double c,int p,int q)
{
	int pp;
	for(pp=1;pp<=NPROC;pp++)
	{
		if(pp==RobotID)continue;
		while(this->arregloDeSockets[pp-1]->enviarSolicitud(s,c,p,q)==-1)
		{
			imprimirMensaje("Seno Coseno a "+QString::number(pp,10)+" p = "+QString::number(p,10)+" q = "+QString::number(q,10)+" s = "+QString::number(s,'g',6)+" c = "+QString::number(c,'g',6));
		}
	}
}
void Robot::verificationFinJacI(int id)
{
//	this->mutex1.lock();
	this->AllfinnishJacobiI[id-1]=true;
//	imprimirMensaje("proc "+QString::number(id,10)+" ya acabo con JacobiI");
	int temp=0,temp1=0;
	for(temp=0;temp<NPROC;temp++)
	{
		if(this->AllfinnishJacobiI[temp]==false)
		{
			break;
		}else{
			witnessJacobiIEnd++;
			if(witnessJacobiIEnd==NPROC)
			{
//				imprimirMensaje("Listo para mandar la matriz de rotacion");
				witnessJacobiIEnd=0;
				for(temp1=0;temp1<NPROC;temp1++)this->AllfinnishJacobiI[temp1]=false;
				emit signalSinCos(this->si,this->co,this->cp,this->cq);
			}
		}
	}
//	this->mutex1.unlock();
}
void Robot::verificationFinJacII(int id)
{
//	this->mutex4.lock();
	this->AllfinnishJacobiII[id-1]=true;
//	imprimirMensaje("proc "+QString::number(id,10)+" JacobiII");
	int temp=0,temp1=0;
	for(temp=0;temp<NPROC;temp++)
	{
		if(this->AllfinnishJacobiII[temp]==false)
		{
			break;
		}else{
			witnessJacobiIIEnd++;
//			imprimirMensaje("witnessJacobiIIEnd = "+QString::number(witnessJacobiIIEnd,10));
			if(witnessJacobiIIEnd==NPROC)
			{
				witnessJacobiIIEnd=0; //debe ser limpiado en otro refere
				for(temp1=0;temp1<NPROC;temp1++)this->AllfinnishJacobiII[temp1]=false;
				//debe emitir la sen~al
				FlagAllProc=true;
//				imprimirMensaje("Todos acabaron con Jacobi method II");

				if((FlagRefere==true)&&(FlagAllProc==true))
				{
//					imprimirMensaje("FlagRefere=true FlagAllProc=true en verificationFinJacII(int id)");
					emit signalCheckRefere();
				}
			}
		}
	}
//	this->mutex4.unlock();
}
void Robot::verificationStageIIJM(int id)
{
//	this->mutex2.lock();
	this->AllgotVsMatrices[id-1]=true;
//	imprimirMensaje("entra a desde verificationStageIIJM ID  = "+QString::number(id,10));
	int temp=0,temp1=0;
	for(temp=0;temp<NPROC;temp++)
	{
		if(this->AllgotVsMatrices[temp]==false)
		{
			break;
		}else{
			witnessStageBefJacobiII++;
//			imprimirMensaje("counter "+QString::number(witnessStageBefJacobiII,10));
			if(witnessStageBefJacobiII==NPROC)//talvez deba cambiar por temp
			{
//				imprimirMensaje("Listo para hacer la siguiente parte de Jacobi method temp = "+QString::number(temp,10));
				witnessStageBefJacobiII=0;
				for(temp1=0;temp1<NPROC;temp1++)this->AllgotVsMatrices[temp1]=false;
				emit go_JacobiII();
			}
		}
	}
//	this->mutex2.unlock();
}
void Robot::positionColumn()
{
	int temp=0;

	if((this->RobotID!=1)&&(this->RobotID!=NPROC))
	{
		if((this->bufferColumnsArrival[0].newColumn==true)&&(this->bufferColumnsArrival[1].newColumn==true))
		{
//			this->mutex3.lock();
			for(temp=0;temp<NSIZE;temp++)
			{
				this->MatrixPrasanna[temp][this->bufferColumnsArrival[0].NColumn]=this->bufferColumnsArrival[0].Columna[temp];
				this->MatrixPrasanna[temp][this->bufferColumnsArrival[1].NColumn]=this->bufferColumnsArrival[1].Columna[temp];
			}
			bothcolumns_received=true;
//			imprimirMensaje("columna reemplazada en "+QString::number(this->bufferColumnsArrival[0].NColumn,10)+" y en "+QString::number(this->bufferColumnsArrival[1].NColumn,10));
			emit signalPosColandENDCycle();;
//			this->mutex3.unlock();
		}
	}else{
		if(this->RobotID==1)
		{
//			this->mutex3.lock();
			for(temp=0;temp<NSIZE;temp++)
			{
				this->MatrixPrasanna[temp][this->bufferColumnsArrival[1].NColumn]=this->bufferColumnsArrival[1].Columna[temp];
			}
//			imprimirMensaje("columna reemplazada en "+QString::number(this->bufferColumnsArrival[1].NColumn,10));
//			verificationFinJacII(this->RobotID);
			emit signalPosColandENDCycle();;
//			this->mutex3.unlock();
		}
		if(this->RobotID==NPROC)
		{
//			this->mutex3.lock();
			for(temp=0;temp<NSIZE;temp++)
			{
				this->MatrixPrasanna[temp][this->bufferColumnsArrival[0].NColumn]=this->bufferColumnsArrival[0].Columna[temp];
			}
//			imprimirMensaje("columna reemplazada en "+QString::number(this->bufferColumnsArrival[0].NColumn,10));
//			verificationFinJacII(this->RobotID);
			emit signalPosColandENDCycle();;
//			this->mutex3.unlock();
		}
	}
}
void Robot::SendLeft(int columnsP)
{
	while(this->arregloDeSockets[RobotID-2]->enviarSolicitud(this->MatrixPrasanna,columnsP)==-1)
	{
		imprimirMensaje("intentando enviar Left ");
	}
}
void Robot::SendRight(int columnsP)
{
	while(this->arregloDeSockets[RobotID]->enviarSolicitud(this->MatrixPrasanna,columnsP)==-1)
	{
		imprimirMensaje("intentando enviar Right ");
	}
}
void Robot::imprimirMP()
{
	 imprimirMatriz(this->MatrixPrasanna);
}
void Robot::PosColandENDCycle()
{
//	imprimirMensaje("ENTRADA AL REFERE que mira posicionamiento de columna y final de jacII");
	if((this->RobotID!=1)&&(this->RobotID!=NPROC))
	{
		if((bothcolumns_received==true)&&(finalstage_JacobiII==true))
		{

			this->bufferColumnsArrival[0].newColumn=false;
			this->bufferColumnsArrival[1].newColumn=false;
			this->finalstage_JacobiII=false;
			bothcolumns_received=false;
			FlagRefere=true;
//			imprimirMensaje("FUNCION DE ARBITRAJE PARA GESTIONAR EL FINAL DE JACII Y LLEGADA DE COLUMNAS");

			emit signalEndJacobiII();
			verificationFinJacII(this->RobotID);//Deberiamos evitar entrar a esta funcion desde el mismo procesador... sera mejor hacer de cuenta que la bandera comienza en 1?

			if((FlagRefere==true)&&(FlagAllProc==true))
			{
				imprimirMensaje("FlagRefere=true FlagAllProc=true en PosColandENDCycle()");
				emit signalCheckRefere();
			}
		}
	}else{
		if(this->RobotID==1)
		{
			if((this->bufferColumnsArrival[1].newColumn==true)&&(this->finalstage_JacobiII==true))
			{

				this->bufferColumnsArrival[1].newColumn=false;
				this->finalstage_JacobiII=false;
				FlagRefere=true;
//				imprimirMensaje("FUNCION DE ARBITRAJE PARA GESTIONAR EL FINAL DE JACII Y LLEGADA DE COLUMNAS");

				emit signalEndJacobiII();
				verificationFinJacII(this->RobotID);

				if((FlagRefere==true)&&(FlagAllProc==true))
				{
					imprimirMensaje("FlagRefere=true FlagAllProc=true en PosColandENDCycle()");
					emit signalCheckRefere();
				}
			}
		}
		if(this->RobotID==NPROC)
		{
			if((this->bufferColumnsArrival[0].newColumn==true)&&(this->finalstage_JacobiII==true))
			{
				this->bufferColumnsArrival[0].newColumn=false;
				this->finalstage_JacobiII=false;
				FlagRefere=true;
//				imprimirMensaje("FUNCION DE ARBITRAJE PARA GESTIONAR EL FINAL DE JACII Y LLEGADA DE COLUMNAS");

				emit signalEndJacobiII();
				verificationFinJacII(this->RobotID);

				if((FlagRefere==true)&&(FlagAllProc==true))
				{
					imprimirMensaje("FlagRefere=true FlagAllProc=true en PosColandENDCycle()");
					emit signalCheckRefere();
				}
			}
		}
	}

}
void Robot::final_Stage_Refere()
{	//deberia verificarse que todos los procesadores ya acabaron con jacobiII
	FlagRefere=false;
	FlagAllProc=false;
	iterator_Jacobi++;
	imprimirMensaje("iterator Jacobi = "+QString::number(iterator_Jacobi,10));
//	imprimirMensaje("DESDE final_Stage_Refere() SE DEBE EMPEZAR OTRA VEZ NO DEBERIA HABER NADA ESCRITO DEBAJO DE ESTO");
	imprimirMensaje(" ");
	if(this->JacobiOption==true)
	{
		if(this->iterator_Jacobi<10501)
		{
//			SendSpark_Jacobi();
//			JacobiMethod();
		}
	}
}
