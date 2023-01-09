#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "QRCodeByte.h"
void QRreader(int feld[41][41],int size){
	
	int buchstaben[1000];
	char binear[1000];
	for(int i=0;i<1000;i++){
	
	buchstaben[i]=0;
}
	int matrix[8][8]={
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1,-1,-1,-1}
    };
	int alignemt [5][5]={
		{-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1},
	};
	int version=(size-24)/4;
	int locations[4][2]={
		{3,3},
		{3,15+(version*4)},
		{15+(version*4),4},
		{15+(version*4),15+(version*4)},
	};
    int typinfo[]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int code[41][41];
    int c=0;
	//erkennungfelder
    for(int i=0;i<8;i++){

        for(int b=0;b<8;b++){

            feld[i][b]= matrix[i][b];
            feld[size-i][b]= matrix[i][b];
            feld[i][size-b]= matrix[i][b];
        }
    }
	feld[size-7][8]=-1;
		for (int i=0;i<size+1;i++){
		
				feld[i][6]=-1;
				feld[6][i]=-1;
	}
	
	
	//typinfo vertikal
    for(int i=(size);i>=0;i--){

        if(i==(size-8)){i=8;}
       
            feld[i][8]=-1;}
    
    c=0;
	//typinfo waagrecht
    for(int b=0;b<=size;b++){

        if(b==8){b=size-7;}
            feld[8][b]=-1;
          }
    
	if(version>=0){
	for(int z=0;z<4;z++){
		if (feld[locations[z][0]][locations[z][1]]==-1 || feld[locations[z][0]+5][locations[z][1]]==-1||feld[locations[z][0]][locations[z][1]+5]==-1){
			continue;
		}
	for(int i=0;i<5;i++){
        for(int b=0;b<5;b++){
            feld[locations[z][0]+i][locations[z][1]+b]= alignemt[i][b];
        }
    }
	}
	}
    int k=0,max=size,counter=0;
	int d=0;
while(max>=0){
	if(max==6){
		max=5;
	}
	if(counter%2==0){
    for (int i=size;i>=0;i--)
    {
        if(feld[i][max]!=-1){
            if(((i+1)+(max+1))%2!=0){
                buchstaben[k]=feld[i][max];
            }
            else{
                if(feld[i][max]==0){
                    buchstaben[k]=1;
                }else{
                    buchstaben[k]=0;
                }

            }
			k++;
		}	
		
		if(feld[i][max-1]!=-1){
           
                if(((i+1)+(max))%2!=0){
                    buchstaben[k]=feld[i][max-1];
                }
                else{
                    if(feld[i][max-1]==0){
                        buchstaben[k]=1;
                    }else{
                        buchstaben[k]=0;
                    }

                }
           k++;     
        }
  

	}}
	else{
		for (int i=0;i<=size;i++)
    {
		
        if(feld[i][max]!=-1){
            if(((i+1)+(max+1))%2!=0){
                buchstaben[k]=feld[i][max];
            }
            else{
                if(feld[i][max]==0){
                    buchstaben[k]=1;
                }else{
                    buchstaben[k]=0;
                }

            }
		k++;
		}
            if(feld[i][max-1]!=-1){
                if(((i+1)+(max))%2!=0){
                   buchstaben[k]= feld[i][max-1];
                }
                else{
                    if(feld[i][max-1]==0){
                        buchstaben[k]=1;
                    }else{
                        buchstaben[k]=0;
                    }

                }
                
        k++;
		}
	}	
	}

	counter++;
	max=max-2;
	
}
int zahl=  BinearZuDez(buchstaben, 4);
	
	int a=0;
	char fertig[1000];
	char bitstream[1000];
	//binär zu aschii converter
	for(int o=12;o<=zahl*8+11;o=o+8){
		int n=0;
		for (int i = o; i < o+8; i++) {
			n += sprintf (&bitstream[n], "%d", buchstaben[i]);
		}
		char c = strtol(bitstream, 0, 2);
		fertig[a]=c;
		a++;
	}
	printf("%s\n",fertig);	
	
}

int main(){
	int eingabe[10000] = {0};
	char nameDatei[100];
	int feld[41][41]={0};
	char zeichen;
	char test[100];
	FILE *datei;
	int c=0;
	
	printf("Gib den Dateinamen des QR-Codes (.pbm) an: ");
	fgets(nameDatei, 1000, stdin);
	strtok(nameDatei,"\n");
	datei = fopen(nameDatei, "r");
	if(datei == NULL){
		printf("Datei wurde nicht gefunden!\n");
	}else{
		for(int i=0; ((zeichen = getc(datei)) != EOF); i++) {
				if(zeichen=='\n'){
					i--;
					continue;
				}
				else if(zeichen==' '){
					i--;
					continue;
				}
				else{
					eingabe[i] = atoi(&zeichen);
				}	
		}			
		
	}
	c=5;
	int size=eingabe[2]*10+eingabe[3];
	for(int b=0;b<size;b++){
	for(int i=0;i<size;i++){
		c++;
		feld[b][i]=eingabe[c];
	}
	}
	QRreader(feld,size-1);

}