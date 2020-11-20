/*
 * plentyfunctions.cpp
 *
 *  Created on: Jul 13, 2010
 *      Author: Luis Ariel Valbuena Reyes
 */

#include "plentyfunctions.h"

double estimator_JacMethod(double Mat[][NSIZE],int *indColumns,int sizeC)
{
	QVector<int> columnsIt(sizeC);
	int a=0,b=0;
	double d=0;
	for(a=0;a<sizeC;a++)
	{
		columnsIt[a]=indColumns[a];
	}
	for(a=0;a<NSIZE;a++)
	{
		for(b=0;b<sizeC;b++)
		{
			if(a!=columnsIt[b])
			{
				d=d+pow(Mat[a][columnsIt[b]],2);
			}
		}
	}
	return d;
}

void printOrdering(int *a,int *b,int size)
{
	QString Arriba=" ";
	QString Abajo=" ";

	int f=0;
	for (f=0;f<size;f++)
	{
		Arriba+=QString::number(a[f],10)+" ";
		Abajo+=QString::number(b[f],10)+" ";
	}
	imprimirMensaje("Arriba."+Arriba);
	imprimirMensaje("Abajo."+Abajo);
}

void parallelOrdering(int *top,int *bot,int *newtop,int *newbot,int Nsize)
{
	int k=0,m=Nsize/2;
	for(k=0;k<m;k++)
	{
		if(k==0)
		{
			newtop[k]=0;
		}else if(k==1)
		{
			newtop[k]=bot[0];
		}else if(k>1)
		{
			newtop[k]=top[k-1];
		}
		if (k==(m-1))
		{
			newbot[k]=top[k];
		}else{
			newbot[k]=bot[k+1];
		}
	}
}

void OrdenarVector(int *Input,int *Output,int N)
{
	int ii=0,menor=Input[0],mayor=Input[N-1],indice=0,j=0;
	QVector<int> Temp(8);
	for(ii=0;ii<N;ii++)
	{
		Temp[ii]=Input[ii];
		if(Input[ii]<menor)menor=Input[ii];
		if(Input[ii]>mayor)mayor=Input[ii];
	}

	for(ii=menor;ii<=mayor;ii++)
	{
		indice=Temp.indexOf(ii);
		if(indice!=-1)
		{
			Output[j]=Temp[indice];
			j++;
		}
	}
}

void imprimirMensaje(QString st)
{
		QTextStream out(stdout);
		out << qPrintable(st)<< endl;
}

void imprimirMatriz(double Mat[][NSIZE])
{
	QString Number,Linea,espacio="	";
	int m=0,n=0;
	for(m=0;m<NSIZE;m++)
	{
		Linea.clear();
		for(n=0;n<NSIZE;n++)
		{
			Number=QString::number(Mat[m][n],'g',6);
			Number+=espacio;
			Linea=Linea+Number;
		}

		QTextStream out(stdout);
		out << qPrintable(Linea)<< endl;
	}
}

void imprimirDiagonal(double Mat[][NSIZE])
{
	QString Number,Linea,espacio="	";
	int m=0;
	for(m=0;m<NSIZE;m++)
	{
		Number=QString::number(Mat[m][m],'g',6);
		Number+=espacio;
		Linea=Linea+Number;
	}

	QTextStream out(stdout);
	out << qPrintable(Linea)<< endl;
}

void reShape(double * apuntadorVector,double MatrizR[][NSIZE],int *Ind,int NElems)
{
	int i=0,j=0;
	for(i=0;i<NElems;i++)
	{
		for(j=0;j<NSIZE;j++)
		{
			MatrizR[j][Ind[i]]=apuntadorVector[(NSIZE*Ind[i])+j];
		}
		//imprimirMensaje("en el reshape las columnas ... "+QString::number(Ind[i],10));
	}
}

void clearMatrix(double Mat[][2],int n)
{
	int r=0;
	for(r=0;r<n;r++)
	{
		Mat[r][0]=0;
		Mat[r][1]=0;
	}
}
void clearColumn(double Mat[][NSIZE],int index)
{
	int ff=0;
	for(ff=0;ff<NSIZE;ff++)
	{
		Mat[ff][index]=0;
	}
}

void devolverResultado_VSmallT(double Al[][2],double InPut[][2],int p,int q)
{
	Al[p][0]=InPut[0][0];
	Al[p][1]=InPut[0][1];
	Al[q][0]=InPut[1][0];
	Al[q][1]=InPut[1][1];

}

void hacerCasting_RelatedParts(double Al[][2],double OutPut[][2],int p,int q)
{
	OutPut[0][0]=Al[p][0];
	OutPut[0][1]=Al[p][1];
	OutPut[1][0]=Al[q][0];
	OutPut[1][1]=Al[q][1];
}

void multiplicarMatrices(double Al[][2],double Vlocal[][2],double Resultado[][2],int n)
{
	int i=0,j=0,m=0;
	double temp=0;
	for(i=0;i<n;i++)
	{
		for(m=0;m<(2);m++)
		{
			for(j=0;j<(2);j++)
			{
				temp=temp+((Al[i][j])*(Vlocal[j][m]));//Assign to the Resultado Matrix
			}
			Resultado[i][m]=temp;
			temp=0;
		}
	}
}

void valSineCosine(double MatrizA[][NSIZE],int p, int q,double *c,double *s)
{
	double t=0;
	double Tao=(MatrizA[q][q]-MatrizA[p][p])/(2*MatrizA[p][q]);
	double t1=-Tao+sqrt(1+(pow(Tao,2)));
	double t2=-Tao-sqrt(1+(pow(Tao,2)));
	if(MatrizA[p][q]!=0)
	{
		if(t1<t2)
		{
			t=t1;
		}else{
			t=t2;
		}
		*c=1/sqrt(1+(pow(t,2)));
		*s=t*( *c);
	}else{
		*c=1;
		*s=0;
	}
}

void SelCols(double MatrizA[][NSIZE],int p,int q,double SelCols[][2])
{
	int iter=0;
	for(iter=0;iter<NSIZE;iter++)
	{
		SelCols[iter][0]=MatrizA[iter][p];
		SelCols[iter][1]=MatrizA[iter][q];
	}
}

void DevCols(double SelCols[][2],int p,int q,double MatrizA[][NSIZE])
{
	int iter=0;
	for(iter=0;iter<NSIZE;iter++)
	{
		MatrizA[iter][p]=SelCols[iter][0];
		MatrizA[iter][q]=SelCols[iter][1];
	}
}
double Cadena2double(char *Cad,int Tamano)
{
	int Signo=1,Potencia=0,a=0;
	double Resultado=0;

	char *aaa;
	aaa=(char*)memchr(Cad,'.',Tamano);
	if(Cad[0]=='-')
	{
		Signo=-1;
		if(aaa == NULL)
		{
			Potencia=Tamano-2;
		}else{
			Potencia=aaa-Cad-2;
		}
		a=1;
	}else
	{
		Signo=1;
		if(aaa == NULL)
		{
			Potencia=Tamano-1;
		}else{
			Potencia=aaa-Cad-1;
		}
		a=0;
	}
	while(Cad[a]!='\0')
	{
		if(Cad[a]=='.')
		{
			a++;
			continue;
		}
		Resultado=Resultado+((Cad[a]-48)*pow(10.0,Potencia));
		Potencia--;
		a++;
	}
	Resultado=Resultado*Signo;
	return Resultado;
}
void ArrangeOutput(int l, int m, int n,  double *d1, double *d2,double *d3,int *Rob1NodeNumber, int *Rob2NodeNumber,int *Rob3NodeNumber, double (&d)[24])
{
	int i;
	int a;int b;int c;
	double result;

	for(i=0;i<24;i++)
	{
		for(a=0;a<l;a++)
		{
			if(Rob1NodeNumber[a]==i)
			result=*(d1+a);
		}
		for(b=0;b<m;b++)
		{
			if(Rob2NodeNumber[b]==i)
			result=*(d2+b);
		}
		for(c=0;c<n;c++)
		{
			if(Rob3NodeNumber[c]==i)
			result=*(d3+c);
		}

		*(d+i)=result;
	}
}
