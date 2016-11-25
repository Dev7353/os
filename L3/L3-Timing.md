
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
	
	
	Funktionsweise & Probleme
	
		Python
		Die Numpy 
		
		C
		Der Client bzw. Test enthält Argumente die es erlauben jede Matrix Datei einzulesen und wahlweise im Single- oder Multithreading 		zu starten. 

		Über die Funktion readmatrix(…) wird eine Datei ausgelesen und zeilenweise in eine Matrixstruktur gespeichert. 
		Für die Matrixstruktur muss zuvor genügen Speicher allokiert werden. Das wäre im Prinzip Speicher für die Gesamtstruktur sowie 
		für die eigentliche Matrix von Datentyp ‚double‘. Die Matrix allokiert zuerst einen Zeiger auf die Matrix und anschließend
		in einer Schleife die Zeiger auf die Zahlen. Das Zeilenweise auslesen und umkopieren der Matrix-Datei wird über ein Dateiobjekt 		realisiert. Über die Datei Funktion wie fopen(…). werden Zeilenweise die Daten über den Filedeskriptor kopiert. Um die Daten 			nun zeilenweise in die Matrix zu speichern wird Funktion fscanf(…) verwendet. Um dieAnzahl der Zeilen zu berechnen wird über eine 		Schleife jedes 	Zeichen gelesen und eine Zählvariable inkrementiert wenn ein ‚\n‘ (Zeilenende) erscheint. Allerdings muss, falls man 		den Filedeskriptor nochmals verwenden will, der Deskriptor geschloßen und
		erneut geöffnet werden, da andernfalls Valgrind Speicherfehler meldet. Wenn die Daten nun vollständig geladen wurden wird die
		gefüllte Matrix zurückgeliefert.  

		Die Funktion multiplyMatrix(…) multipliziert zwei Matrizen, jedoch bedarf es davor noch eine Vorbereitung. Analog wie bei 
		Readmatrix, muss zuerst Speicher für die Ergebnismatrix allokiert werden. Da der Multiplikations-Algorithmus davon ausgeht
		das die Ergebnismatrix für jede Zelle mit null initialisiert ist, wird hier die Funktion calloc verwendet. 
		Die Berechnung erfolgt in zwei Schritten. Der erste Schritt ist die Zellen zu lokalisieren, dies geschieht über zwei verschachtelte
		For-Schleifen. Der zweite Schritt ist die eigentliche Berechnung, also die Summe über die Produkte der zwei Eingangsmatrizen. Diese
		Berechnungsteil wird in die multiplyRowColumn(…) Funktion verlagert. 
		
		Um die Matrizen in einem bzw. mehreren Threads zu verrechnen werden die POSIX pthreads verwendet. Hierbei wird ein Array von
		Threads angelegt. Die Multiplikationsschritte werden dann in einer Hilfs-Methode realisiert. Ein Thread erwartet bei seiner 			Erzeugung vier Parameter, wobei zwei davon optional sind. Die Thread Variable, also ein Thread aus dem Thread Array, ein Optionales		Thread Attribut, die Methode die parallelisiert ausgeführt wird, sowie optionale Argumente an die Methode falls sie welche 			erwartet. Da die Threads parallelisiert laufen, ist es wichtig, das keine Daten überschrieben werden.

		Versuch 1:
		Im ersten Versuch die Threads korrekt zu parallelisieren war das Problem, das jeder Thread die gesamte Ausgangsmatrix berechnet hat.
		Somit lag auch die Laufzeit in einem sehr hohen Bereich, da allein eine komplette Berechnung von einem Thread eine Laufzeit von 		O(n^3) hat. Die Lösung ist, dass jeder Thread nur einen bestimmten Teil der Matrix berechnet. Es ist wichtig, das die Teilmatrizen
		sich nicht überlappen, sondern komplett unabhängig von einander sind.

		Versuch 2:
		Um eine Teilmatrize zu berechnen wurde ein bestimmter Bereich definiert. Jeder Thread läuft in der ersten Schleife von einem 			Endpunkt zum nächsten. Die erste Schleife umfasst die Zeilen und bietet sich daher an. Der Startwert hängt von der Anzahl Threads 		ab. Die Threads werden nacheinander in einer Schleife initialisiert. Zu dem Zeitpunkt wo die Threads mit der Funktion 				pthread_create(…) gestartet werden muss als Parameter die Position des Threads
		an die Funktion übergeben werden. Da es zwei Positionen gibt (Start, Ende) jedoch die pthread_create Funktion einen Parameter 			zulässt	muss eine Struktur erzeugt werden um mehrere Parameter zu übergeben. Die Struktur speichert weiterhin nicht nur die Indices, 		sondern auch die eigentlichen Matrizen, da sie nicht global definiert sind. Die Struktur ‚args’ enthält somit drei 				Matrizen Referenzen (A, B und Ergebnis) sowie die Integer Indices für jeden Thread.
		Die Indices für Start und Ende werden in Abhängigkeit der Thread Nummer berechnet. 
		So wären z.B. die Indices bei einer 4x4 Matrix Start = i * (Anzahl_Zeilen / Anzahl_Threads) und 
		Ende = (i+1) i * (Anzahl_Zeilen / Anzahl_Threads). Exemplarisch wäre hier für i=0, 10 Threads und einer 100x100 Matrix die Start und 		End Indices 0 und 10. Der erste Thread würde somit die Teilmatrize von 0 bis 9 berechnen. 
		Allerdings schlagen alle Berechnung fehl.
	
	 
- Auslagerung der Funktion in Thread Methode
- Explizite verarbeitung von  Spalten in Threads
- Kreeirung von Struktur mit allen notwendigen Objekten
- problem mit dem join 
- !!! Zerlge Matrix in anzahl Threads Mengen und berechne jedes Feld  innerhalb des jeweiligen threads.
- jeder Thread erhält ein eigenes Attribut !!! sonst überschreiben usw.