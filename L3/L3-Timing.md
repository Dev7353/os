
Matrix
	Vorüberlegung
	Ziel der Aufgabe ist es, eine Matrixmultiplikation in einem Singlethread und wahlweise
	in Multithreads zu laufen. Dabei soll die Performance Unterschiede ersichtlich werden.
	Die Multiplikation Operation wird in diesem Rahmen ausschließlich mit NxN Matrizen ausgeführt.
	Das Python Programm, welches die Matrizen generiert hat die Möglichkeit NxM Matrizen zu erzeugen.
	
	Somit generiert ein Python-Skript die notwendigen Matrizen und ein C-Programm verrechnet diese dann.
	Für Python gibt es hinsichtlich von Mathematischen Operationen usw. eine Umfangreiche Bibliothek: ‚Numpy‘.
	Über diese wird dann eine Matrix erzeugt und als Datei abgespeichert. Ein kleiner Mechanismus soll darauf achten
	das die älteren Daten nicht überschrieben werden im Hinblick auf die Performance-Messung. 

	Die API für das C-Programm gibt eine Funktion zum einlesen, multiplizieren und eine weitere Hilfsfunktion.
	Den eigentlichen Algorithmus für die Verrechnung der Matrizen wird hierbei aus dem Internet (Siehe Quellen) entnommen, da die Multiplikation 	nicht das primäre Lernziel ist. 
	Die Multithreading Variante nutzt die POSIX pthreads. Hierbei ist auf eine korrekte Parallelisierung zu achten.
	Bei Fehlern könnte z.B. ein Thread die Werte eines anderen Threads überschreiben.
	Um das zu verhindern müssen die Zellen mit unabhängigen Werten verrechnet werden. Ein Wechselseitiger Ausschluss
	wäre z.B. möglich. In diesem Beispiel jedoch, bietet es sich an, jeden Thread über einen Bestimmten Wertebereich rechnen zu lassen.
	
	
	Funktionsweise
	
		Python
		Die Numpy 
		
		C
		Der Client bzw. Test enthält Argumente die es erlauben jede Matrix Datei einzulesen und wahlweise im Single- oder Multithreading 		zu starten. 

		Über die Funktion readmatrix(…) wird eine Datei ausgelesen und zeilenweise in eine Matrixstruktur gespeichert. 
		Für die Matrixstruktur muss zuvor genügen Speicher allokiert werden. Das wäre im Prinzip Speicher für die Gesamtstruktur sowie 
		für die eigentliche Matrix von Datentyp ‚double‘. Die Matrix allokiert zuerst einen Zeiger auf die Matrix und anschließend
		in einer Schleife die Zeiger auf die Zahlen. Das Zeilenweise auslesen und umkopieren der Matrix-Datei wird über ein Dateiobjekt 		realisiert. Über die Datei Funktionen wie fopen(…) usw. werden Zeilenweise die Daten über den Filedeskriptor kopiert. Um die Daten 		nun in die Matrix zu speichern wird Funktion fscanf(…) verwendet. Um dieAnzahl der Zeilen zu berechnen wird über eine Schleife jedes 		Zeichen gelesen und eine Zählvariable inkrementiert wenn ein ‚\n‘ (Zeilenende) erscheint. Allerdings muss, falls man den 			Filedeskriptor nochmals verwenden will, der Deskriptor geschloßen und
		erneut geöffnet werden, da andernfalls Valgrind Speicherfehler meldet. Wenn die Daten nun vollständig geladen wurden wird die
		gefüllte Matrix zurückgeliefert.  

		Die Funktion multiplyMatrix(…) werden zwei Matrizen multipliziert, jedoch bedarf es davor noch eine Vorbereitung. Analog wie bei 
		Readmatrix, muss zuerst Speicher für die Ergebnismatrix allokiert werden. Da der Multiplikations-Algorithmus davon ausgeht
		das die Ergebnismatrix für jede Zelle mit null initialisiert ist, wird hier die Funktion calloc verwendet. 
	
	A better way is to set threads with specific slices.
	That means we create for example 2 threads for a 3x3 Matrix. There are 9 Elements,
	so each thread get a slice of 4 and 5 Elements to solve.
	Then again you can only create n threads on a nxn Matrix.
	But for that usecase we decide to take this solution.
	
	 
- Auslagerung der Funktion in Thread Methode
- Explizite verarbeitung von  Spalten in Threads
- Kreeirung von Struktur mit allen notwendigen Objekten
- problem mit dem join 
- !!! Zerlge Matrix in anzahl Threads Mengen und berechne jedes Feld  innerhalb des jeweiligen threads.
- jeder Thread erhält ein eigenes Attribut !!! sonst überschreiben usw.
