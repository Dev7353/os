# L3-Design

## Mthread

### Vorüberlegung
    Ziel dieser Aufgabe ist es ein Multithreading taugliches Programm zu schreiben, welches
    im Anschluss mit einer geeigneten Random Funktion erweitert wird. Weiterhin soll das Verständnis
    um Mutlithreading sowie der POSIX pthreads vertieft werden. 
    Die API gibt die Methode PrintHello vor welche später parallelisiert aufgerufen wird. 
    Zusätzlich erhält das Programm  Parameter  wie -t für Anzahl Threads, -v für zusätzliche Debug Informationen
    , -h als Auflistung aller Parameter sowie -r und -R für die Grenzen der Pseudozufallszahlen. 
    
    
### Funktionsweise
    
    Über getopt Funktionalität werden die Argumente des Benutzers ausgewertet. Hierbei 
    wird jedes Argument in einer switch case Kontrollstruktur zu einer internen Variable verwiesen.
    Wenn ein Argument ein ':' vorne stehen hat, wird signalisiert, dass das Argument einen Input erwartet. Im Falle der Threads wäre der Input z.B.
    -t 20, also 20 Threads die parallel ausgeführt werden sollen. Hierführ bietet getopt eine Variable optarg an die es erlaubt
    den aktuellen Input auszlesen. Der Input wird, je nach einsatzzewck, in eine Zahl gecastet. In diesem Fall wird optarg über die Funktion
    atoi nach int gecastet. Anschließend können weitere Fehlerbehandlungen prüfen ob der input legal ist wie z.B. nicht negative Anzahl an Threads
    oder die minimale Grenze darf nicht größer sein wie die maximale.
    
    Im nächsten Schritt des Programms werden die Threads gestartet. Da die Anzahl der Threads dynamisch ist, wird ein einfaches Array vom Typ pthread_t
    erstellt. In einer eifnachen For-Schleife welche in Anzahl Threads iteriert, werden die threads über die Funktion 
    pthread_create(...) gestartet. Die Funktion erwartet stets vier Parameter, wobei zwei davon optional sein können.
    Der erste Parameter ist ein Zeiger auf eine pthread_t Variable, der zweite ist ein pthread attribut was in diesem Fall optional ist und das Verhalten der Threads
    zusätzlich ändert. Hierbei wird außschließlich ein Default Thread behandelt. Der dritte Parameter ist die Funktion die vom Thread aufgerufen wird.
    In diesem Fall die PrintHello Methode. Der vierte und letzte Parameter ist ein optionales Argument an die Funktion die vom Thread gestartet wird.
    
    Die Funktion PrintHello liefert einen void Zeiger zurück und erwartet selbst einen void Zeiger als Parameter. Die Hauptfunktionalität der Funktion 
    ist, eine pseudozufällige Zahl zu generieren und anschließend in Anzahl Sekunden zu schlafen und am Ende eine Nachricht auszugeben, das der Thread fertig ist.
    Die Pseudozufallszahlen wurden zu Beginn mit der rand() Funktion berechnet. Hierbei wird ein Zeitstempel an srand() übergeben, welcher als Bezugspunkt dient
    damit nicht jeder Thread diesselbe Nummer berechnet.
    Die rand() Funktion generiert nun über einen Bereich von zahlen eine Zufallszahl. Das Problem hierbei ist, das die Zahlen vorhersehbar sind und somit nicht
    echt zufällig sind. Die bessere Alterantive ist die random() Funktion. Diese ist ebenfalls teil der Standardbibliothek, allerdings 
    sind die Zahlen weniger berechnbar und somit die bessere Funktion. Über die Methode sleep() wartet der Thread passiv.
    
    weiter Punkte:
    - argumente als Parameter weil -r und -R 
    - Mutex und warum?
    - join warum?
    - Kritik? 

   


Maximal Threads? Woher Limit?
Zeitverhalten? Unterchied liegt in kleinen milisekunden bereich. Vorallem wenn die Anzahl Threads zunimmt.
Was passiert wenn kein join? -> Der Mainthread terminiert so werden auch alle restlichen Threads terminiert unabhängig von der Ausfürhung.
	Somit werden die aktuellen Operation in den Threads abgebrochen. 
