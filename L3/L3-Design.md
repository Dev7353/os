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
    
    Bei der Nutzung des Parameters -h wird nicht nur alle möglichen Parameter aufgelistet, sondern auch die ID des aktuellen HEAD vom Branch auf dem
    Entwickelt wird. Die Information wurde nicht, wie empfohlen, in einer String Variable gespeichert, sondern direkt über ein shell Kommando agerufen.
    In C bietet die Standardbibliothek die Funkion system() an, welche Strings (Kommandos) direkt ausführt. Der Git Befehl ist 'git rev-parse HEAD'.
    Somit ist die Ausgabe der Funktion auf der Shell und nicht über eine printf() Ausgabe.
    
    Der Parameter -v setzt eine boolsche Variable auf true und ermöglich die Ausgabe der Errno Variable.
    
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
    
    Die obere und untere Grenze bei der Berechnung der Zufallszahl war statisch auf 10 bzw. 0 gesetzt. Dies soll nun dynamisch durch die Argumente
    -r und -R realisiert werden. Das Problem hierbei ist, das die Generierung der Zufallszahl in der PrintHello Methode erfolgt. Allerdings
    erwartet PrintHello nur einen Parameter. Um mehrere Parameter zu übergeben wird eine Struktur angelegt. Die Struktur arg enthält die Nummer des Threads,
    also die Laufvariable i, die ober- sowie untere Grenze. So können die Parameter in eine Struktur Instanz gespeichert werden, wobei die Nummer bzw. 
    Laufvariable nach jedem Schleifendurchgang übergeben werden muss, da diese sich stetig ändert.
    Da die Threads fast gleichzeitig starten entsteht das Problem, dass die Laufvariable mehrfach verwendet wird eher sie inkrementiert wird. Das führt zu falschen
    Ausgaben der Thread Nummer. Um das zu vermeiden werden dem Thread nun nichtmehr eine Struktur Instanz als Parameter übergeben, sondern eine Array in Anzahl Threads.
    Somit hat jeder Thread seine eigene Struktur mit Argumenten bzw. seine eigene Thread Nummer. Hierzu wird in derselben Schleife bei dem Thread Start
    eine arg Struktur Instanz aus dem Array übergeben. 
    
    Nachdem alle Threads erzeugt wurden, muss die Funktion pthread_join() für jeden Thread aufgerufen werden. Die Problematik hierbei ist,
    dass, wenn diese Funktion nicht aufgerufen wird, terminieren die Threads, sobald der Mainthread terminiert. Der Mainthread ist in diesem Fall
    das Hauptprogramm und terminiert schneller als die Threads. Das führt dazu, dass die Threads quasie abgrebochen werden und somit nicht vollständig den Algorithmus ablaufen.
    Die Join Funktion verhindert dies in dem das Hauptprogramm auf die Terminierung der Threads wartet. 
    
### Kritik

    weiter Punkte:
    - Kritik? 

   
