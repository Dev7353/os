# L3-Timing Ergebnisse
## Matrix
	
	- make test_matrix erzeugt und führt einen vollständigen Test aus.
	
	[28.11.2016 13:24] 
	Messung 1000x1000 Matrixmultiplikation
	
	FILE A 		 	matr.0
	FILE B 		 	matr.1
	MULTITHREADING 	aus
	THREADS 	 	1
	Time : 			13235 [msec]
	
	
	FILE A 			matr.0
	FILE B 			matr.1
	MULTITHREADING 	an
	THREADS 	 	100
	Time : 			1178 [msec]

## Mthread
	
    * Wieviele Threads können Sie maximal erstellen? Woher kommt dieses Limit?
		- ab über 1000 Threads ist die Ausfürhung des Programm sehr instabil. So werden Segmentationfault Fehlermeldungen bei unterschiedlich 
		  großer Anzahl Threads (> 1000) geworfen. Diese Limit hängt von verschiedenen Faktoren ab. Da jeder Thread ebenfalls Speicher belegt liegt es nahe, das 
		  der Speicherplatz der limitierender Faktor ist. 
    * Untersuchen Sie mit time das Zeitverhalten. Zu welchem Ergebnis kommen Sie?
		- Über time wird die tatsächliche Ausführungszeit des Programms gemessen. In diesem Fall liegt der Unterschied bei ca. 3 msec. Dieser Unterschied ist
		  von der jeweiligen Implementierung abhängig.
    * Was passiert, wenn Sie die ''pthread_join()'' Aufrufe nicht machen?
		- Dieser Punkt ist bereits im Design beschrieben. Wenn der join Aufruf nicht gemacht wird dann terminiert der Hauptthread vor den Threads und bricht quasie die laufenden 
		  Threads ab.

## Race-Condition

## C in Python
