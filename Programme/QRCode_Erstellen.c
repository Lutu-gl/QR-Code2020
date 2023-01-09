/* CC Alex, Christof, Luca */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "QRCodeByte.h"


/*Dieser Code erzeugt den QR Code von Version 1-6 Fehlerkorrekturlevel H*/


int main(){
	char eingabe[1000] = {0}, nameDatei[100] = {0};										//Die Benutzer Eingabe
	int binear[50000] = {0}, spezial1[8] = {1,1,1,0,1,1,0,0}, spezial2[8] = {0,0,0,1,0,0,0,1};		//binear=bineare Code. spezial1 und spezial2 sind zum auffüllen da, falls die Kapazität noch nicht erreicht wurde.
	int laenge=0, kapazitaet = 0, korrekturwoerter=0, version=0 , gerade=1, zahl=0;						//kapazitaet für level 1 und Fehlerkorrekturlevel H | gerade ist dafür da ob jetzt spezial1 oder spezial2 dran ist | zahl ist der Rückgabewert von der Funktion die Umwandelt. Also die von Binär -> Dez rechnet
	FILE *datei;															//FILE pointer, falls man eine Datei öffnen muss
	char zeichen;														//Wenn man aus einer Datei ließt, dass man die Zeichen bekommt.
	int zahlen[100], n = 0;												//zahlen[] --> array für die übergabe von zahl auf die funktion Polynom(); n --> zähler für zahlen[]
	
	
	
	printf("\nWelche Groese/Version soll der QR-Code haben (0-6) (0 = Programm passt die Gr\224\341e automatisch an): ");	//Eingabe benutzer
	scanf("%i",&version);
	fflush(stdin);
	
	if((version < 0) || (version > 6)){
		printf("Falsche Eingabe\n");
		exit(0);
	}
	
	printf("Gib einen String oder den Dateinamen an: ");
	fgets(nameDatei, 1000, stdin);
	chomp(nameDatei);
	datei = fopen(nameDatei, "r");
	if(datei == NULL){
		strcpy(eingabe, nameDatei);														//Die Datei wurde nicht gefunden, darum nimmt man einfach den String den der Benutzer eingegeben hat her.
		printf("Datei wurde nicht gefunden, darum wandle ich den eingegebenen String um! \"%s\"\n",eingabe);
	}else{
		for(int i=0; ((zeichen = getc(datei)) != EOF); i++) eingabe[i] = zeichen;		//Die Datei wurde gefunden. Darum nimmt man den Inhalt der Datei her.
		printf("Datei wurde gefunden! \"%s\"\n", eingabe);
	}
	laenge = (strlen(eingabe) * 8);												//mal 8 wegen Byte
	
	if(version == 0){						//Wenn der Benutzer Version 0 ausgewählt hat (also programm nimmt die passene groeße) dann rechnet es hier die Version aus.
		if(laenge+12 <= 72) version = 1;	// +12 weil man noch den Modus und die Länge hinzufügen muss
		else if(laenge+12 <= 128) version = 2;
		else if(laenge+12 <= 208) version = 3;
		else if(laenge+12 <= 288) version = 4;
		else if(laenge+12 <= 368) version = 5;
		else if(laenge+12 <= 480) version = 6;
		else{ printf("Der angegebene String ist zu gro\341 und passt nicht in Version 6 hinein!\n"); exit(0);}
	}
	
	switch(version){															//Größe der Kapazität und Anzahl der Korrekturwoerter in abhängigkeit der Größe bestimmen
		case 1: kapazitaet = 72, korrekturwoerter = 17; break;						
		case 2: kapazitaet = 128, korrekturwoerter = 28; break;
		case 3: kapazitaet = 208, korrekturwoerter = 22; break;
		case 4: kapazitaet = 288, korrekturwoerter = 16; break;
		case 5: kapazitaet = 368, korrekturwoerter = 22; break;
		case 6: kapazitaet = 480, korrekturwoerter = 28; break;
	}

	binear[0] = 0;			//Den Modus hinzufügen. Also byte: 0100
	binear[1] = 1;
	binear[2] = 0;
	binear[3] = 0;
	
	LaengeHinzufuegen(laenge/8, binear, 4);			//Eigene Funktion  um die Länge des Textes hinzuzufügen, da sie sich in mehreren Punkten von der CharZuBinear Funktion unterscheidet. 4 ist der startpunkt
	laenge= laenge + 12;							//Die Länge wurde um 13 erhöht. Wegen modus + länge
	StringZuBinear(eingabe, binear);				//Die Eingabe vom Benutzer hinzufügen
	
	if(laenge > kapazitaet){						//Schauen ob die Größe überhaput passt.
		printf("Der eingegebende String passt nicht in diesen QR-Code hinein!\n");
		exit(0);
	}
	
	for(int i=0; i < 4; i++){						//Auffüllen mit dem Terminator.  (4 Nullen oder bis Kapazität fertig): 0000, (das muss man machen, obwohl man mit 0 initialisiert hat, damit man auch die laenge des strings mitkontrolliert)
		if(laenge > kapazitaet) break;
		binear[laenge++] = 0;	
	}
	
	//In 8-er Blöcke aufteilen. Wenn es nicht fertig ist, den letzten 8er Block noch mit 0en auffüllen.
	while(laenge%8 != 0){
		binear[laenge++] = 0;
	}
	
	while(laenge < kapazitaet){		//Falls Kapazität noch nicht erreicht ist wird mit 2 speziellen Wörtern abwechselnd aufgefüllt.  Spezial1 : 11101100  Spezial2 : 00010001
		for(int i=0; i<8; i++){
			if(gerade) binear[laenge++] = spezial1[i];
			else binear[laenge++] = spezial2[i];
		}
		gerade = gerade ^ 1;
	}
	
	
	/*Die 8er Blöcke in integer umwandeln und sie einzeln der Funktion geben, bei dem dann die Verschiedenen Fehlerkorrekturwerte bipapo berechnet werden und hinzugefügt werden */
	for(int i=0; i < laenge; i=i+8){
		zahl = BinearZuDez(binear, i);		//i ist der startpunkt
		
		zahlen[n] = zahl;
		n++;
	}	
	
	Polynom(zahlen, binear, laenge, korrekturwoerter, version);			//Fügt die Fehlerkorrekturwerte hinzu. | Berechnet den String der dann in den QR-Code eingefügt wird.
	InDateiSchreiben(binear, version-2);								//Erstellt den QR-Code, also als .pbm
	
	printf("QR-Code wurde erstellt!\n");
	return 0;
}

