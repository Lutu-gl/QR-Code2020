/* CC Alex, Christof, Luca */
#ifndef QRCodeByte_h
#define	QRCodeByte_h

void LaengeHinzufuegen(int laenge, int binear[], int startpunkt);				//Fügt den binearen String die Länge hinzu
void StringZuBinear(char eingabe[], int binear[]);								//Rechnet die Eingabe des Benutzer binär
void CharZuBinear(int array[], char buchstabe, int startpunkt);					//Die Funktion um einen Char als binär umzuwandeln
int BinearZuDez(int binear[], int startpunkt);									//Rechnet Binär zu Dezimal
void chomp(char *str);															//Entfernt das \n von der Eingabe mit fgets();
void Polynom(int zahlen[], int binear[], int laenge, int korrekturwoerter, int version);	//Fügt die Fehlerkorrekturwerte hinzu. | Berechnet den String der dann in den QR-Code eingefügt wird.
void EinlesenEingabe(int korrekturwoerter, int startpunkt, int block[]);					//Bei der Funktion kann der Benutzer die Ergebnisse von der Webseite eintragen. Hier wird die Eingabe zu den Blöcken hinzugefügt
void Benutzeranweisung(int zahlen[], int korrekturwoerter, int dataCodewords, int startpunkt, int modus); //Bei dieser Funktion wird der Benutzer aufgefordert in die Webseite die angegebenen Zahlen einzugeben. 
void DezZuBin(int zahl , int binear[], int laenge);											//Rechnet Dezimal zu Binär
void InDateiSchreiben(int binear[], int version);											//Erstellt den QR-Code, also als .pbm

void printQR(int binear[],int laenge){
	for(int i=0; i < laenge; i++){
		printf("%i",binear[i]);
		if(i%8==7){
			printf("");
		}
	}
	printf("\n");
}


void LaengeHinzufuegen(int laenge, int binear[],int startpunkt){
	int n = 7, potenz=0;								 //Hochzahl ist automatisch schon 8, weil es 9 Stellen sein müssen.
	
	for(;n >= 0; n--){				//Solange wie n größer gleich null ist
		potenz = pow(2,n);			//Für die if Abfrage
		
		if((laenge & potenz) == potenz){		//Bit -Opteratoren. Es schaut ob bei der stelle von der Zahl eine 1 steht. schau oben. !
			binear[startpunkt] = 1;				//Wenn stimmt dann schreibt es eine 1 hin, sonst nichts (weil es mit 0 aufgefüllt wurde)
		}
	startpunkt++;
	}
}

void StringZuBinear(char eingabe[], int binear[]){
	
	for(int i=0; eingabe[i] != '\0'; i++){
		CharZuBinear(binear, eingabe[i], i*8+12);			//Der Funktion übergibt man immer einen Block. Dieser Block wird Codiert und hinzugefügt. Das 8*i+13 isch der Startpunkt. +13 weil man den Modus und die Länge schon hinzugefügt hat. 4+9=13
	}	
}

void CharZuBinear(int array[], char buchstabe, int startpunkt){
	long long int zahl = 0, potenz = 0;
	int n = 7;								 //Hochzahl ist automatisch schon 7, weil es 8 Stellen sein müssen. 
	
	zahl = (int)buchstabe;
		
	
	for(;n >= 0; n--){				//Solange wie n größer gleich null ist
		potenz = pow(2,n);			//Für die if Abfrage
		
		if((zahl & potenz) == potenz){		//Bit -Opteratoren. Es schaut ob bei der stelle von der Zahl eine 1 steht. schau oben. !
			array[startpunkt] = 1;				//Wenn stimmt dann schreibt es eine 1 hin, sonst nichts (weil es mit 0 aufgefüllt wurde)
		}
	startpunkt++;
	}
}

int BinearZuDez(int binear[], int startpunkt){
	int zahl=0;
	
	for(int n=7; n >= 0; n--){
		if(binear[abs(n-7)+startpunkt] == 1){
			zahl = zahl + pow(2, n);
		}
	}
	return zahl;
}
	
void Polynom(int zahlen[], int binear[], int laenge, int korrekturwoerter, int version){
	int zahl=0;
	int startpunkt=0;					//Der Startpunkt wo die Funktion DezZuBin hinschreiben soll.
	int block_1[5000] = {0};			//Die Einteilung in Blöcken die man für Version 3-6 machen musss						
	int block_2[5000] = {0};
	int block_3[5000] = {0};
	int block_4[5000] = {0};
	int dataCodewords_1 = 0;			//Für die Einteilung von Blöcken für die Versionen
	int dataCodewords_2 = 0;
	int block_1E[5000] = {0};
	int block_2E[5000] = {0};
	int block_3E[5000] = {0};
	int block_4E[5000] = {0};
	
	if(version < 3){					//Version 1/2 Es ist leicht da man nicht aufteilen muss wie bei den höheren leveln
		if(version == 1) dataCodewords_1 = 9;
		else dataCodewords_1 = 16;
	
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, 0, 1);	//dataCodeworts ist wielange es ausgeben soll, bzw. wie viele datenCodewörter es braucht, 0 ist der startpunkt.  Diese Funktion gibt einfach aus was der Benutzer eingeben muss
		EinlesenEingabe(korrekturwoerter, laenge, binear);					//Benutzer kann das ausgerechnete polynom eingeben
	}else if(version == 3){													//version 3 ist komplizierter als 1 und 2
		dataCodewords_1 = 13;
	
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, 0, 1);	//Startpunkt ist null | Benutzer wird aufgefordert einzugeben in die Webseite
		EinlesenEingabe(korrekturwoerter, 0, block_1);						//Das Ergebnis von der Webseite soll eingegeben werden
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 1, 0);	//Startpunkt ist 13, also dataCodewords * 1
		EinlesenEingabe(korrekturwoerter, 0, block_2);						//Das Ergebnis von der Webseite soll eingegeben werden
		
		for(int i=0; i < dataCodewords_1*8; i++){						//Die Blöcke der Eingabe des Benutzers generieren
			block_1E[i] = binear[i];
			block_2E[i] = binear[i+dataCodewords_1*8];
		}

		int j=0;
		for(int i=0; i < dataCodewords_1; i++){										//Die Verschiedenen Blöcke Richtig hinzuweisen
			for(int n=0; n < 8; n++) binear[j*8+n]=block_1E[i*8+n];					//Zuerst die Blöcke der Eingabe werden zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_2E[i*8+n];
			j++;
		}
		j=0;
		for(int i=0; i < korrekturwoerter; i++){
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2]=block_1[i*8+n];	//Danach werden die Blöcke der Fehlerkorrektur zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2]=block_2[i*8+n];
			j++;
		}
	}else if(version == 4){
		dataCodewords_1 = 9;
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, 0, 1);	//Startpunkt ist null
		EinlesenEingabe(korrekturwoerter, 0, block_1);
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 1, 0);	//Startpunkt ist 9, also dataCodewords * 1
		EinlesenEingabe(korrekturwoerter, 0, block_2);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 2, 0);	//Startpunkt ist 18, also dataCodewords * 2
		EinlesenEingabe(korrekturwoerter, 0, block_3);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 3, 0);	//Startpunkt ist 27, also dataCodewords * 3
		EinlesenEingabe(korrekturwoerter, 0, block_4);	
		
		for(int i=0; i < dataCodewords_1*8; i++){						//Die Blöcke der Eingabe des Benutzers generieren
			block_1E[i] = binear[i];
			block_2E[i] = binear[i+dataCodewords_1*8];
			block_3E[i] = binear[i+dataCodewords_1*8*2];
			block_4E[i] = binear[i+dataCodewords_1*8*3];
		}

		int j=0;
		for(int i=0; i < dataCodewords_1; i++){										//Die Verschiedenen Blöcke Richtig hinzuweisen
			for(int n=0; n < 8; n++) binear[j*8+n]=block_1E[i*8+n];					//Zuerst die Blöcke der Eingabe werden zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_2E[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_3E[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_4E[i*8+n];
			j++;
		}
		j=0;
		for(int i=0; i < korrekturwoerter; i++){
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_1[i*8+n];	//Danach werden die Blöcke der Fehlerkorrektur zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_2[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_3[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_4[i*8+n];
			j++;
		}	
	}else if(version == 5){					//Bei Version 5 ist es auch noch speziell, da man hier 4 Blöcke hat, aber 2 mit 11 Wörtern und 2 mit 12 Wörtern
		dataCodewords_1 = 11;
		dataCodewords_2 = 12;
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, 0, 1);	//Startpunkt ist null
		EinlesenEingabe(korrekturwoerter, 0, block_1);
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 1, 0);	//Startpunkt ist 11, also dataCodewords * 1
		EinlesenEingabe(korrekturwoerter, 0, block_2);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_2, dataCodewords_1 * 2, 0);	//Startpunkt ist 22, also dataCodewords * 2
		EinlesenEingabe(korrekturwoerter, 0, block_3);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_2, dataCodewords_1 * 2 + dataCodewords_2 * 1, 0);	//Startpunkt ist 34, also dataCodewords * 2 + dataCodewords2 * 1
		EinlesenEingabe(korrekturwoerter, 0, block_4);		
		
		for(int i=0; i < dataCodewords_1*8; i++){						//Die Blöcke der Eingabe des Benutzers generieren
			block_1E[i] = binear[i];
			block_2E[i] = binear[i+dataCodewords_1*8];
		}
		for(int i=0; i < dataCodewords_2*8;i++){
			block_3E[i] = binear[i+dataCodewords_1*8*2];
			block_4E[i] = binear[i+dataCodewords_1*8*2 + dataCodewords_2*8];
		}
		int j=0;
		for(int i=0; i < dataCodewords_1; i++){										//Die Verschiedenen Blöcke richtig hinzuweisen
			for(int n=0; n < 8; n++) binear[j*8+n]=block_1E[i*8+n];					//Zuerst die Blöcke der Eingabe werden zugewiesen
			j++;	
			for(int n=0; n < 8; n++) binear[j*8+n]=block_2E[i*8+n];
			j=j+3;																	//Weil man 2 Blöcke überspringen muss
		}
		j=2;
		for(int i=0; i < dataCodewords_2; i++){										//Da die Version 5 speziell ist muss man hier wieder aufteilen in Codeword1/2.
			for(int n=0; n < 8; n++) binear[j*8+n]=block_3E[i*8+n];			
			j++;	
			for(int n=0; n < 8; n++) binear[j*8+n]=block_4E[i*8+n];
			j=j+3;																	//Weil man 2 Blöcke überspringen muss
		}
		j=0;
		for(int i=0; i < korrekturwoerter; i++){
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2+dataCodewords_2*8*2]=block_1[i*8+n];	//Danach werden die Blöcke der Fehlerkorrektur zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2+dataCodewords_2*8*2]=block_2[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2+dataCodewords_2*8*2]=block_3[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*2+dataCodewords_2*8*2]=block_4[i*8+n];
			j++;
		}		
	}else if(version == 6){
		dataCodewords_1 = 15;
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, 0, 1);	//Startpunkt ist null
		EinlesenEingabe(korrekturwoerter, 0, block_1);
		
		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 1, 0);	//Startpunkt ist 15, also dataCodewords * 1
		EinlesenEingabe(korrekturwoerter, 0, block_2);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 2, 0);	//Startpunkt ist 30, also dataCodewords * 2
		EinlesenEingabe(korrekturwoerter, 0, block_3);

		Benutzeranweisung(zahlen, korrekturwoerter, dataCodewords_1, dataCodewords_1 * 3, 0);	//Startpunkt ist 45, also dataCodewords * 3
		EinlesenEingabe(korrekturwoerter, 0, block_4);
		
		for(int i=0; i < dataCodewords_1*8; i++){						//Die Blöcke der Eingabe des Benutzers generieren
			block_1E[i] = binear[i];
			block_2E[i] = binear[i+dataCodewords_1*8];
			block_3E[i] = binear[i+dataCodewords_1*8*2];
			block_4E[i] = binear[i+dataCodewords_1*8*3];
		}
		
		int j=0;
		for(int i=0; i < dataCodewords_1; i++){										//Die Verschiedenen Blöcke Richtig hinzuweisen
			for(int n=0; n < 8; n++) binear[j*8+n]=block_1E[i*8+n];					//Zuerst die Blöcke der Eingabe werden zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_2E[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_3E[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n]=block_4E[i*8+n];
			j++;
		}
		j=0;
		for(int i=0; i < korrekturwoerter; i++){
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_1[i*8+n];	//Danach werden die Blöcke der Fehlerkorrektur zugewiesen
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_2[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_3[i*8+n];
			j++;
			for(int n=0; n < 8; n++) binear[j*8+n+dataCodewords_1*8*4]=block_4[i*8+n];
			j++;
		}		
		
		
	}
}

void EinlesenEingabe(int korrekturwoerter, int startpunkt, int block[]){				//Bei der Funktion kann der Benutzer die Ergebnisse von der Webseite eintragen. Hier wird die Eingabe zu den Blöcken hinzugefügt
	int zahl=0;
	for(int n=0; n < korrekturwoerter; n++){
		scanf("%i", &zahl);
		DezZuBin(zahl, block, startpunkt);									//Bei block hinzufügen
		startpunkt=startpunkt+8;
	}
}


void Benutzeranweisung(int zahlen[], int korrekturwoerter, int dataCodewords, int startpunkt, int modus){		//Bei dieser Funktion wird der Benutzer aufgefordert in die Webseite die angegebenen Zahlen einzugeben. 
	if(modus){																									//Modus = 1, ist beim 1.ten aufruf, damit der Benutzer versteht was er machen muss");
		printf("\nGeben Sie die angegebenen Zahlen (mit Kommas) in die erste Eingabenzeile in der angegebene Website ein ( https://www.thonky.com/qr-code-tutorial/show-division-steps ).\n\n");
		printf("Zahlen: ");
		
		for(int i=0; (zahlen[i + startpunkt] != '\0') && (i < dataCodewords); i++){
			printf("%i", zahlen[i+startpunkt]);
			if((zahlen[i+1 +startpunkt] != '\0') &&(i+1 < dataCodewords)) printf(",");
		}
		
		printf("\nIn der zweiten Eingabenzeile geben Sie bitte folgende Zahl ein: %i\n\n", korrekturwoerter);
		printf("Nachdem Sie das gemacht haben, klicken Sie bitte auf \"Perform Division\"\n");
		printf("Nun scrollen Sie ganz nach unten zum Ende der Website, dort steht nun das Ergebnis der Polynom-Division\n");
		printf("\nF\201gen Sie diese Zahlen hier ein: ");
	}else{
		printf("Wiederholen Sie das ganze mit der selben Zahl in der 2. Zeile, jedoch mit diesen Zahlen in der 1. Zeile: ");
		for(int i=0; (zahlen[i + startpunkt] != '\0') && (i < dataCodewords); i++){
			printf("%i", zahlen[i+startpunkt]);
			if((zahlen[i+1 +startpunkt] != '\0') &&(i+1 < dataCodewords)) printf(",");
		}
		printf("\nF\201gen Sie die von der Seite berechneten Zahlen hier ein: ");
	}
}




void DezZuBin(int zahl , int binear[], int laenge){					//Dezimalzahl wird in Binär umgerechnet. Für die Fehlerkorrekturzahlen
    long long int potenz = 0;
	int n=7;							//Es muss 7 sein, weil 8 Bit
	
    for(;n >= 0; n--){                //Solange wie n größer gleich null ist
        potenz = pow(2,n);            //Für die if Abfrage

        if((zahl & potenz) == potenz){    //Bit -Opteratoren. Es schaut ob bei der stelle von der Zahl eine 1 steht. schau oben. !
            binear[laenge++] = 1;                //Wenn stimmt dann schreibt es eine 1 hin, sonst nicht
        }else{
			binear[laenge++] = 0;			
		}
    }
}

void chomp(char *str){				//entfernt das \n von der eingabe mit fgets();
   size_t p=strlen(str);
   /* '\n' mit '\0' überschreiben */
   str[p-1]='\0';
}




void InDateiSchreiben(int binear[], int version){ 						//Schreibt die ganze Sache in eine Datei
 int matrix[8][8]={
            {1,1,1,1,1,1,1,0},
            {1,0,0,0,0,0,1,0},
            {1,0,1,1,1,0,1,0},
            {1,0,1,1,1,0,1,0},
            {1,0,1,1,1,0,1,0},
            {1,0,0,0,0,0,1,0},
            {1,1,1,1,1,1,1,0},
            {0,0,0,0,0,0,0,0}
    };
	int alignemt [5][5]={
		{1,1,1,1,1},
		{1,0,0,0,1},
		{1,0,1,0,1},
		{1,0,0,0,1},
		{1,1,1,1,1},
	};
	int size=24+(4*version);
	int locations[4][2]={
		{3,3},
		{3,15+(version*4)},
		{15+(version*4),4},
		{15+(version*4),15+(version*4)},
	};
    int typinfo[]={0,0,1,0,1,1,0,1,0,0,0,1,0,0,1};
    int code[41][41];
    for(int i=0;i<41;i++){
        for (int b=0;b<41;b++)
        {
            code[i][b]=-1;
        }
    }


    int c=0;
	//erkennungfelder
    for(int i=0;i<8;i++){

        for(int b=0;b<8;b++){

            code[i][b]= matrix[i][b];
            code[size-i][b]= matrix[i][b];
            code[i][size-b]= matrix[i][b];
        }
    }
	code[size-7][8]=1;
		for (int i=0;i<size+1;i++){
		if(code[6][i]==-1){
			if((i+6)%2==0){
				code[i][6]=1;
				code[6][i]=1;
			}
			else{
				code[i][6]=0;
				code[6][i]=0;
			}
		}
	}
	
	
	//typinfo vertikal
    for(int i=(size);i>=0;i--){

        if(i==(size-8)){i=8;}
        if(code[i][8]==-1){
            code[i][8]=typinfo[c];
            c++;}
    }
    c=0;
	//typinfo waagrecht
    for(int b=0;b<=size;b++){

        if(b==8){b=size-7;}
        if(code[8][b]==-1){
            code[8][b]=typinfo[c];
            c++;}
    }
	if(version>=0){
	for(int z=0;z<4;z++){
		if (code[locations[z][0]][locations[z][1]]!=-1 || code[locations[z][0]+5][locations[z][1]]!=-1||code[locations[z][0]][locations[z][1]+5]!=-1){
			continue;
		}
	for(int i=0;i<5;i++){

        for(int b=0;b<5;b++){
            code[locations[z][0]+i][locations[z][1]+b]= alignemt[i][b];
        }
    }
	}
	}
	
	
	//richtige auffüllen
    int k=0,max=size,counter=0;
	int d=0;
while(max>=0){
	if(max==6){
		max=5;
	}
	if(counter%2==0){
    for (int i=size;i>=0;i--)
    {
        if(code[i][max]==-1){
            if(((i+1)+(max+1))%2!=0){
                code[i][max]=binear[k];
            }
            else{
                if(binear[k]==0){
                    code[i][max]=1;
                }else{
                    code[i][max]=0;
                }

            }
			k++;
		}	
		
		if(code[i][max-1]==-1){
           
                if(((i+1)+(max))%2!=0){
                    code[i][max-1]=binear[k];
                }
                else{
                    if(binear[k]==0){
                        code[i][max-1]=1;
                    }else{
                        code[i][max-1]=0;
                    }

                }
           k++;     
        }
  

	}}
	else{
		for (int i=0;i<=size;i++)
    {
		
        if(code[i][max]==-1){
            if(((i+1)+(max+1))%2!=0){
                code[i][max]=binear[k];
            }
            else{
                if(binear[k]==0){
                    code[i][max]=1;
                }else{
                    code[i][max]=0;
                }

            }
		k++;
		}
            if(code[i][max-1]==-1){
                if(((i+1)+(max))%2!=0){
                    code[i][max-1]=binear[k];
                }
                else{
                    if(binear[k]==0){
                        code[i][max-1]=1;
                    }else{
                        code[i][max-1]=0;
                    }

                }
                
        k++;
		}
		
    //k=k+2;
	}	
	}

	counter++;
	max=max-2;
}	



    FILE *f;
    f = fopen("QRCode.pbm", "w+");
    fprintf(f,"P1\n");
    fprintf(f,"%d %d\n",size+1,size+1);
    for(int i=0;i<size+1;i++){
        for (int b=0;b<size+1;b++)
        {
            fprintf(f,"%d ",code[i][b]);
        }
        fprintf(f,"\n");
    }

    fclose(f);
}


#endif