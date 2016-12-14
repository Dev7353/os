# L4-Design

## Prodcon

### Vorüberlegung
Ziel dieser Aufgabe ist es das Consumer/Producer Problem für n,m Threads zu lösen.
Ein wichtiger Punkt hierbei ist der Umgang mit einem Buffer worin die Daten
gespeichert bzw. woraus gelesen werden sollen. Eine Möglichkeit wäre eine simple globale Matrix
aus char Typen zu erstellen, sodass die Threads abwechselnd darauf zugreifen.
Ein Hindernis ist jedoch, dass die Zugriffsoperationen in jedem Thread definiert erden müssen, da jeder Thread 
über seine interne ID zugreift. Eine Lösung hierfür wäre eine FIFO Queue zu realisieren die mit eifnachen init, destroy,
push und pop Operationen die Daten modifiziert. Die Queue ist demnach lediglich ein Bestandteil des Buffers. Der Buffer
ist eine Struktur in dem Anzahl Elemente, nächstes Element, die eigentliche Queue sowie Inforamationen darüber,
ob die Queue leer bzw. voll ist. Zu diesem Zeitpunkt erscheinen diese Parameter als Optional, da die vollständige Implementierung
der Threads nicht ausdefiniert war. Falls diese Parameter nicht benötigt werden, werden sie entfernt.
Der Buffer enthält somit die Queue sowie zusätzliche Variablen die den Status des Buffers sehr schnell ermitteln lässt.
Eine zusätzliche API implementiert die Buffer Operationen.

Die Anforderung von L4 setzt weiterhin vorraus, dass die Eingabedaten von einer Datei oder von der Standardeingabe eingelesen werden können.
Da das parallele erstellen von Threads und einlesen der Daten für unser Verständnis seperiert werden sollen, wird ein weiterer Buffer verwendet.
Dieser ist ebenfalls global, jedoch nur vom Producer bedient, dh kein mutex. Die Daten werden also formatiert in einen InputBuffer geschrieben
und anschließen von den Producern in den originalen Buffer kopiert. 

Der mit Abstand wichtigste Punkt ist die Thread-Synchronisierung. Das Synchronisationsverfahren für einen Producer bzw. Consumer
ist bereits in der Vorbereitungsaufgabe erstellt und kann daher als Basis verwendet werden. 
Die Consumer bzw. Producer müssen nicht notwendigerweise abwechselnd die Daten verarbeiten. Hierbei kann der Producer pro Thread
mehrere Daten (Strings) in den Buffer schreiben. Wenn der Thread terminiert wird ein Signal an einen Consumer geschickt der so lange 
die Daten  verarbeitet bis er fertig ist. Der wichtigste Punkt hierbei ist der Wechselseitige Ausschluß der Threads.
Damit der Consumer weis, dass er nun arbeiten darf, muss er benachrichtigt werden. Somit könnte der producer nach jeder Operation
auf die Daten zugreifen.
Wenn 5 Consumer 6 Datensätze verarbeiten, kann es passieren, dass der letzte Consumer niemals terminiert, da er nicht weis, wann er dies machen soll.
Für diesen fall ist es wichtig sich zu merken, wieviele Consumer nach Beendigung der Arbeit noch aktiv sind. Eine globale Zugriffsmatrix wird daher definiert. 

Die Prodcuer Threads haben eine besondere Eigenschaft. Jeder Producer muss unterschiedlich viele Daten verarbeiten. Bei 5 Daten und 2 Producern
müssen alle Daten von den Producern verarbeitet werden. Analog wie beim Consumer, bietet es sich ebenfalls an eine globale Zugriffsmatrix zu definieren.
Somit wissen die Producer wieviele Threads bereits gearbeitet haben. Falls ale bereits aktiv waren, es jedoch noch Daten im InputBuffer gibt, müssen die Producer wieder in einen ready zustand gebracht werden, sodass sie erneut arbeiten können. Wenn nun alle Daten verarbeitet wurden und 
es noch aktive Producer im Suspend zustand gibt, dann müssen diese ebenfalls terminiert werden.

Das passive und aktive Warten wird trivialerweiser wie bereits aus L3 bekannt, implementiert.

### Funktionsweise

#### Datenfluss
Der gesamte Synchronisationsmechanismus besteht aus n Consumer, m Producer sowie einem Producer Observer und Consumer Observer.
Die Steuerung erfolgt über jede abgeschlossene Operation. Die Reihenfolge der Threads ist hierbei sehr wichtig. Der Consumer Observer (co) startet
zuerst und geht in den Suspend Zustand. Anschließend werden nacheinander alle producer und alle Consumer Threads gestartet welche 
ebenfalls im Zustand Suspend sind. 
Der Producer Observer(po)
wird am Ende gestartet, denn dieser löst eine Kettenreaktion aus. Der po startet den ersten producer Thread und geht in den Suspend Zustand. 
Grundsätzlich wird jeder Thread mit einer ID initialisiert. Bei m Producer wird per Zugriffsmatrix der erste Thread geweckt. 
Jeder Thread hat eine eigene Condition Variable. Der Erste Producer legt n Datensätze in den Buffer, weckt den co und geht in den Suspend Zustand.
Der co weckt analog zum po per Zugriffsmatrix den ersten bzw. nächsten consumer Thread. 
Der Consumer holt über die pop Funktion das Kopfelement der Queue und wandelt dieses Element entsprechend in Großbuchstaben. 
Anschließend wird po geweckt und in den Suspend Zustand gewechselt. Das Verfahren erfolgt erneut, solange bis alle Daten verarbeitet wurden.

Jedes mal wenn der po geweckt wird, wird geprüft ob noch aktive Producer existieren wobei die Arbeit beendet ist. Falls ja, werden diese ebenfalls 
terminiert. Ähnlich verfährt der co.

Im Falle das die Arbeit nicht beendet ist, jedoch alle Producer bzw. Consumer im Suspend Zustand sind, werde diese wieder geweckt. 

#### Interne Verarbeitung

### Kritik
- Instabilität
- rowsPerCosl Bug
-
