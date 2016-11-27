
# L3-Timing Design
## Matrix
### Vorüberlegung
	Ziel der Aufgabe ist es, eine Matrixmultiplikation in einem Singlethread und wahlweise
	in Multithreads laufen zu lassen. Dabei soll die Performanceunterschiede ersichtlich werden.
	Die Multiplikation wird in diesem Rahmen ausschließlich mit NxN Matrizen ausgeführt.
	Bei Multiplikation von NxM Matrizen werden wird eine Benachrichtigung ausgeben und die Parameter auf NxN begradigt. 
	
	Die Matrizen werden mit einem Pythonskript generiert und von einem C Programm verrechnet. 
	Für Python gibt es hinsichtlich von Mathematischen Operationen eine Umfangreiche Bibliothek: Numpy.
	Über diese wird dann eine Matrix erzeugt und als Datei abgespeichert. Ein kleiner Mechanismus soll darauf achten
	das die älteren Daten nicht überschrieben werden, sodass mehrere Matrizen erzeugt werden können. 

	Die API für das C-Programm gibt eine Funktion zum einlesen und multiplizieren an. Wobei noch eine zusätzliche Hilfsmethode vorgeschrieben ist.
	Den eigentlichen Algorithmus für die Verrechnung der Matrizen wird hierbei aus dem Internet (Siehe Quellen) entnommen.
	
	Die Multithreading Variante nutzt die POSIX pthreads. Hierbei ist auf eine korrekte Parallelisierung zu achten.
	Bei Fehlern könnte z.B. ein Thread die Werte eines anderen Threads überschreiben.
	Um das zu verhindern müssen die Zellen mit unabhängigen Werten verrechnet werden. Ein Wechselseitiger Ausschluss
	wäre z.B. möglich. In diesem Beispiel jedoch, bietet es sich an, jeden Thread über einen Bestimmten Wertebereich rechnen zu lassen, da 
	eine mutex eine gewisse Anzahl Prozessorzeit in Anspruch nimmt.
	
	Zusätzliche müssen noch Fehlerbehandlungen für eine negativer Anzahl an Threads erfolgen.
	
	
### Funktionsweise
	
#### Python
	Die Numpy Bibliothek ist vorallem im Umgang mit Matrizen sehr mächtig. So Kann die Funktion numpy.random.random_integers(...) 
	in einem speziellen Wertebereich und einer definierten Größe eine Matrix generieren.

	Bei der Speicherung der Matrix kam ein besonderer Fehler auf. Der erste Lösungsversuch war, über die Matrix zu iterieren und jede Zeile als String in eine Datei mit der Methode write() zu speichern. Allerdings funktionierte dieses Verfahren lediglich für kleinere Matrizen. Für Größen im Bereich von
	über 1000 Zeilen wurden die Strings runtergebrochen und in der Datei fälschlich dargestellt. 
	
	Als bessere Alternative bietet Numpy eine einfache Methode an, den gesmten Inhalt einer Matrix abzuspeichern: numpy.savetext().
	
	Wenn jetzt mehrere unterschiedlich große Matrizen generiert werden, zwecks der Performancemessung darf die Matrixdatei nicht überschrieben werden.
	Daher muss jede Matrix in eine neue Datei gespeichert werden. Jede Datei enthält einen Posfix der von 0-9 geht dh. können auch nur 10 Matrizen erzeugt werden. 
	Um die Datein im makefile ebenfalls zu löschen sieht eine Matrixdatei wie folgend aus: 'matr.0'.
	Das Pythonskript prüft vorher ob es bereits eine Datei mit dem selben Namen gibt, falls doch, wird der Postfix inkrementiert und die Matrix darin abgespeichert. 
	Falls nicht, wird der Postfix nicht inkrementiert und der erste Dateiname verwendet. 
	
	Negative Matrixgrößen werden als Fehler abgefangen. 
	NxM Matrizen werden nach NxN umgestellt und anschließend weiterverfahren. 

		
#### C
	Der Client bzw. Test enthält Argumente die es erlauben jede Matrix Datei einzulesen und wahlweise im Single- oder Multithreading zu starten. 

	Über die Funktion readmatrix(…) wird eine Datei ausgelesen und zeilenweise in eine Matrixstruktur gespeichert. 
	Für die Matrixstruktur muss zuvor genügen Speicher allokiert werden. Das wäre im Prinzip Speicher für die Gesamtstruktur sowie 
	für die eigentliche Matrix von Datentyp ‚double‘. Die Matrix allokiert zuerst einen Zeiger auf die Matrix und anschließend
	in einer Schleife die Zeiger auf die Zahlen. Das Zeilenweise auslesen und umkopieren der Matrix-Datei wird über ein Dateiobjekt realisiert. Über die Datei Funktion wie fopen(…). werden Zeilenweise die Daten über den Filedeskriptor kopiert. Um die Daten nun zeilenweise in die Matrix zu speichern wird Funktion fscanf(…) verwendet. Um die Anzahl der Zeilen zu berechnen wird der Vorteil einer NxN Matrix genutzt. Über eine Schleife jedes jedes Zeilenende '\n' gezählt. Da Zeilen = Spalten muss diese Routine nur einmal erfolgen.
	Allerdings muss, falls man den Filedeskriptor nochmals verwenden will, der Deskriptor geschloßen und erneut geöffnet werden, da andernfalls Valgrind Speicherfehler meldet. Wenn die Daten nun vollständig geladen wurden wird die gefüllte Matrix zurückgeliefert.  

	Die Funktion multiplyMatrix(…) multipliziert zwei Matrizen, jedoch bedarf es davor noch eine Vorbereitung. Analog wie bei readmatrix(...), 
	muss zuerst Speicher für die Ergebnismatrix allokiert werden. Da der Multiplikations-Algorithmus davon ausgeht
	das die Ergebnismatrix für jede Zelle mit null initialisiert ist, wird hier die Funktion calloc verwendet. 
	Die Berechnung erfolgt in zwei Schritten. Der erste Schritt ist, die Zellen zu lokalisieren, dies geschieht über zwei verschachtelte
	For-Schleifen. Der zweite Schritt ist die eigentliche Berechnung, also die Summe über die Produkte der zwei Eingangsmatrizen. Dieser
	Berechnungsteil wird in die multiplyRowColumn(…) Funktion verlagert. 
		
	Um die Matrizen in einem bzw. mehreren Threads zu verrechnen werden die POSIX pthreads verwendet. Hierbei wird ein Array von
	Threads angelegt. Die Multiplikationsschritte werden dann in einer Hilfs-Methode realisiert. Ein Thread erwartet bei seiner Erzeugung vier Parameter, wobei zwei davon optional sind und daher mit NULL angegeben werden können. Die Thread Variable, also ein Thread aus dem Thread Array, ein Optionales Thread Attribut, die Methode die parallelisiert ausgeführt wird, sowie ein Argument an die Methode falls sie dies erwartet. Da die Threads parallelisiert laufen, ist es wichtig, das keine Daten überschrieben werden.

	Im ersten Versuch die Threads korrekt zu parallelisieren war das Problem, das jeder Thread die gesamte Ausgangsmatrix berechnet hat.
	Somit lag auch die Laufzeit in einem sehr hohen Bereich, da allein eine komplette Berechnung von einem Thread eine Laufzeit von O(n^3) hat. Die Lösung ist, dass jeder Thread nur einen bestimmten Teil der Matrix berechnet. Es ist wichtig, das die Teilmatrizen und analog die Threads sich nicht überlappen, sondern komplett unabhängig von einander sind.

	Um eine Teilmatrize zu berechnen wird ein bestimmter Bereich definiert. Jeder Thread läuft in der ersten Schleife von einem Endpunkt zum nächsten. Die erste Schleife umfasst die Zeilen und bietet sich daher sehr gut an. Der Startwert hängt von der Anzahl Threads ab. Die Threads werden nacheinander in einer Schleife initialisiert. Zu dem Zeitpunkt wo die Threads mit der Funktion pthread_create(…) gestartet werden, muss als Parameter die Position des Threads an die Funktion übergeben werden. Da es zwei Positionen gibt (Start, Ende) wobei die pthread_create Funktion ein Argument zulässt	muss eine Struktur erzeugt werden um mehrere Argumente zu übergeben. Die Struktur speichert weiterhin nicht nur die Indices, sondern auch die eigentlichen Matrizen, da diese nicht global definiert sind. Die Struktur ‚args’ enthält somit drei Matrizen Referenzen (A, B und Ergebnis) sowie die Integer Indices für jeden Thread.
	Die Indices für Start und Ende werden in Abhängigkeit der Thread Nummer berechnet. 
	So wären die Indices bei einer  Matrix [Start = i * (Anzahl_Zeilen / Anzahl_Threads)] und 
	[Ende = (i+1) i * (Anzahl_Zeilen / Anzahl_Threads)]. Exemplarisch wäre hier für i=0, 10 Threads und einer 100x100 Matrix die Start und End Indices 0 und 10. Der erste Thread würde somit die Teilmatrize von 0 bis 9 berechnen. Der zweite Thread dann 10 bis 19 usw. 
	Allerdings schlagen alle Berechnung fehl aufgrund eines Fehlers in der Parallelisierung.

	Aufgrund dessen, das die Parameter für die Funktion in einer Struktur gespeichert sind hat jeder Thread zugriff auf die ein und selbe Struktur. Die führt zu ungewollten Überschreibung der Indice Variablen, da die Threads fast gleich starten und die Indices nacheinander verändert werden, rechnet jeder Thread in verschiedenen unzulässigen Bereichen.
	Dieses Problem löst sich in dem jeder Thread seine eigene Struktur Instanz bekommt. Daher wird zusätzlich noch ein Array von Strukturen angelegt und während der Thread Erzeugung als Parameter übergeben.
	
	Nachdem sämtliche Threads erzeugt wurden ruft das Programm im Anschluss für jeden Thread die pthread_join(...) Methode auf. Die Methode bewirkt, dass der Mainthread,
	also das Hauptprogramm, nicht vorzeitig terminiert, sondern auf die Beendigung jeden Threads wartet. Falls die join Methode nicht aufgerufen wird und der
	Mainthread terminiert, werden alle noch laufenden Threads ebenfalls terminiert. Das führt dazu das die Multiplikation nicht vollständig abläuft, dh. kein korrektes
	Ergebnis liefert. 
	
	Da die multiplyMatrix Funktion eine Ergebnismatrix zurückliefert, kann der allokierte Speicher für die Matrix nicht innerhalb der Funktion freigegeben werden.
	Die free Funktionen müssen daher im Client bzw. matrix.c aufgerufen werden. Der allokierte Speicher wird im Client am Ende freigegeben, jedoch ist es wichtig
	auf die Reihenfolge zu achten. Zuerst mussten die inneren Zeiger der double Matrix freigegeben werden und dann erst der eigentliche Zeiger auf die Matrix, da
	die inneren Referenzen sonst erhalten bleiben. Im Anschluss werden dann die Zeiger auf die Strukturen freigegeben.  
	
	Der Client Matrix.c enthält, abgesehen von der Speicherverwaltung, Mechanismen für Argumente. So können verschiedene Matrixdateien über zwei Parameter 
	(-a -b) eingelesen werden. Die Singlethread routine ist die Standardeinstellung und kann über den Parameter -p auf die Multithreading Routine umgeschalten werden.
	Möchte man die Anzahl Threads angeben in denen die Matrizen verrechnet werden soll, muss zusätzlich der Parameter -t und die dazugehörige Anzahl angeben.
	Falsche Datein, negative Anzahl an Threads werden als Fehler abgefangen und in gültige Werte gesetzt. So würde -9 Threads in 9 Threads umgewandelt werden.

	Die Ergebnismatrix wird im Client über zwei verschachtelte For-Schleifen in eine Datei kopiert. Hierbei wurden auch Tabulatoren und Zeilenwechsel berücksichtigt.
	Da die Zahlen vom Typ double sind, müssen sie vorher in einen String gecastet werden, um anschließend den String über die fputs(...)  Methode in die Datei zu kopieren. Die Tabs und Zeilenwechsel werden als zeichen '\t' und '\n' über fputc(...) kopiert.

	Im Client werden zusätzlich die Anzahl nullen in der Ergebnismatrix gezählt. Anhand dessen lässt sich schnell feststellen, ob die Matrizen überhaupt multipliziert werden. Eine niedrige Anzahl an nullen ist normal und können während der Generierung der Zufallszahlen auftreten. Liegt dieser Wert
	jedoch in einem sehr hohen Bereich kann man vorab vorraussagen, ob der Fehler im Multiplikationsschritt stattfand.
	
	Um das testen des Programms zu beschleunigen, wude im Makefile ein zusätzliche Ziel angelegt, welches zwei 1000x1000 große Matrizen generiert und anschließend 
	im Single und Multithreading berechnet. Um zu prüfen, ob die Mutlithread Variante korrekt rechnet wird über das Kommando diff die Singlethread und Multithread Ergebnismatrizen verglichen. 
	
#### Quellen
	https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

