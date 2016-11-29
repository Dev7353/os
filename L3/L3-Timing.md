# L3-Timing Ergebnisse
## Matrix
make test_matrix erzeugt und führt einen vollständigen Test aus.
	
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
	
#### Wieviele Threads können Sie maximal erstellen? Woher kommt dieses Limit?
Ab über 1000 Threads ist die Ausführung des Programm sehr instabil. So werden Segmentationfault Fehlermeldungen bei unterschiedlich großer Anzahl Threads (> 1000) geworfen. Dieses Limit hängt von verschiedenen Faktoren ab. Da jeder Thread ebenfalls Speicher belegt liegt es nahe, das der Speicherplatz der limitierender Faktor ist. 
#### Untersuchen Sie mit time das Zeitverhalten. Zu welchem Ergebnis kommen Sie?
Über time wird die tatsächliche Ausführungszeit des Programms gemessen. In diesem Fall liegt der Unterschied bei ca. 3 msec. Dieser Unterschied ist von der jeweiligen Implementierung abhängig.
#### Was passiert, wenn Sie die ''pthread_join()'' Aufrufe nicht machen?
Dieser Punkt ist bereits im Design beschrieben. Wenn der join Aufruf nicht gemacht wird dann terminiert der Hauptthread vor den Threads und bricht quasie die laufenden Threads ab.

## Race-Condition
### 10 Loops mit ansteigender Threadanzahl
| Peterson       				| Mutex         				|
| ------------------------------|-------------------------------|
| 10 Threads: 2.514670724980533	| 10 Threads: 0.5906399786472321|
| 20 Threads: 8.552850060164928 | 20 Threads: 1.0985388671979308|
| 30 Threads: 28.398671341128647| 30 Threads: 1.6357121728360653|
| 40 Threads: 42.46896166726947 | 40 Threads: 2.146585589274764	|
| 50 Threads: 63.439611121080816| 50 Threads: 2.680081414990127 |

### 10 Threads mit ansteigender Loopanzahl
| Peterson       				| Mutex         				|
| ------------------------------|-------------------------------|
| 1 Loop: 0.04750830866396427	| 1 Loop: 0.05264761671423912	|
| 10 Loops: 2.000440526753664 	| 10 Loops: 0.5748048387467861	|
| 25 Loops: 5.503908468410373	| 25 Loops: 1.4721656925976276	|
| 50 Loops: 12.728423647582531 	| 50 Loops: 2.917170182801783	|

Der Peterson-Algorithmus ist weitaus langsamer als der Mutex. Je mehr Threads man laufen lässt, desto höher ist der Performanceunterschied zu messen. 
Dies liegt beim Peterson-Algorithmus daran, dass dadurch mehr Level entstehen, die in der Schleife durchlaufen werden müssen und dadurch entstehen mehr Abfragen.
Bei Erhöhung der Loops sollte durch die Implementierung die Laufzeit beider Algorithmen linear ansteigen. 
Beim Peterson-Algorithmus ist somit die Laufzeit nicht so sehr in die Höhe gestiegen als bei Erhöhung der Threads.

## C in Python
[28.11.2016 23:11]
```
---cyforksqrt.sqrtm( 5.0 )---
Result: 2.23606797749979
Execution Time: 0.07845490332692862
---cyforksqrt.sqrt2( 5.0 )---
Result: 2.2360680103302
Execution Time: 0.6536249546334147
---forksqrt.sqrt2( 5.0 )---
Result: 2.23606797749979
Execution Time: 91.63766623847187
---math.sqrt( 5.0 )---
Result: 2.23606797749979
Execution Time: 0.10609805304557085
```

Die C Bibliothek ist am schnellsten. Daraufhin folgt die integrierte Python-Funktion, 
dann die cythonized Heron-Funktion und mit grossem Abstand dann die Heron-Funktion von der L2.
Die sehr schnellen Zeiten von der sqrtm und math.sqrt Methode lassen sich dadurch erklären, dass diese bereits optimiert sind und den besten Algorithmus verwenden. Durch die Cython-Optimierung ist die cyforksqrt.sqrt2 Methode zwar langsamer als die Standardwurzelfunktionen, aber dennoch weitaus schneller als die Python-Funktion aus der L2.

## Weitere Quellen
http://www.csee.wvu.edu/~jdm/classes/cs550/notes/tech/mutex/Peterson2.html
https://en.wikipedia.org/wiki/Peterson%27s_algorithm
https://docs.python.org/3/library/functions.html#any
http://stackoverflow.com/questions/3310049/proper-use-of-mutexes-in-python
https://www.tutorialspoint.com/python/python_multithreading.htm

https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
http://www.programmingsimplified.com/c-program-multiply-matrices
