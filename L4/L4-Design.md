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
Der globale Buffer wird über einen Mutex geschützt. Jeder Thread hat eine eigene Condition Variable. Die Notwendigkeit hierfür war,
das die Threads nur intern Wissen, wer sie sind (anhand der id). Da die Zugriffsmatrizen außerhalb der Threads von co bzw. po gesteuert
werden müssen die Threads ebenfalls von außen bekannt sein. Ein Broadcast bzw. Signal würde nur den nächsten Thread starten ohne das po oder co
wissen wer im Moment die Daten verarbeitet. Weiterhin ergibt sich das Problem falls consumer und producer
sich gleichzeitig im Suspend Zustand befinden. Um diese Synchronisationshemnisse zu beseitigen werden Arrays von Condition Variablen benötigt. So 
kann jeder der Observer den spezifischen Thread starten welcher suspend ist und es noch Arbeit gibt.

Zusätzlich hat jeder Observer jeweils eine condition variable sodass sie über diese aufgeweckt werden können.

Der FIFO Buffer enthält eine init,  destroy, add und pop Funktion. Die init Funktion initialisiert und allokiert Speicher. Hierbei werden Default
Werte gesetzt und Speicher in größe der einzulesenden Daten allokiert. Die destroy Funktion gibt den gesamten Speicher wieder frei.
Die add Funktion fügt der Queue an der Schwanzspitze (tail)  das nächste Element an, falls die Queue nicht breits voll ist.
Wenn ein Element erfolgreich angehängt wurde wird die tail Variable inkrementiert. Der Kopf der Queue (head) ist, wenn zuvor noch nicht gepopt wurde,
auf dem Wert 0. Somit ist die Queue z.B. [Hello, World, Foo] wobei Head 0 ist und queue[head] Hello ist.

Die Pop Funktion inkrementiert den Head, sodass dieser immer auf das nächste Element in der Queue zeigt. Bei einem Pop Aufruf würde dann
Beispielsweise 'Hello' zurückgeliefert werden und der Head anschließend auf 1 inkrementiert. Beim nächsten pop dann 'World' und Head auf 2.
Allerdings ist der FIFO Buffer nicht vollständig Implementiert, denn die älteren Datensätze bleiben erhalten. Für unseren Anwendungsfall reicht
diese Impementierung jedoch aus, da die Queue nicht dynamisch wachsen bzw. schrumpfen muss.

Da die Pop und Add Funktionen Strings auf die Queue legen und die Strings über referenzen übergeben werden, müssen sie kopiert werden. Um die Daten
in der Queue nicht nachträglich zu verfälschen. Hierbei wird die Funktion memcpy verwendet.

Die Datensätze werden wahlweise über eine Datei (readFile) oder Standardeingabe (readStdin) eingelesen.
Falls eine Datei angegeben ist, werden zeilenweise
die Sätze als Strings interpretiert. Die Stringlänge ist per Default auf 20 chars limitiert. Falls längere Sätze eingelesen werden sollen
bietet die Option -C eine variable Länge an wobei diese nicht negativ sein darf. Die Anzahl der Zeilen die eingelesen werden können ist ebenfalls
auf 20 limitiert und über die Option -L modifizierbar.
Vor dem einlesen muss die Datei im Lesezugriff geöffnet werden. Über die Funktion fgets können dann Zeilenweise die Daten in eine Variable gespeichert
werden. Die Variable muss zuvor speicher allokieren. Ein bisher noch nciht gelöstes Problem ist, das die maximale Anzahl an chars die pro
Satz in die Variable kopiert werden nicht dynamisch bestimmt werden kann. Ausgehend von der derzeitigen Lösung.

Der gelesene Satz in der Variable wird nun entsprechend der -C Option "zurechtgeschnitten". Über memcpy werden nur Anzahl chars von der Variablen
in eine andere Variable kopiert, für die ebenfalls zuvor Speicher allokiert worden ist. Allerdings ist heir zu beachten, dass für den zweite
Variable ein char mehr allokiert werden muss, da memcpy kein String Terminator automatisch anhängt. Daher wird die zweite Variable
um den Terminator '\0' ergänzt. Die add Funktion der Buffer API fügt nun den String zur Buffer-Queue an. Anschließend werden die Speicherbereiche
der Variablen freigegeben und der filedeskriptor geschlossen.

Das Lesen der Daten über die Standardeingabe erfolgt über die fscanf Funktion. Über eine bedingte Schleife werden solange Sätze eingelesen, bis der
Buffer voll ist. Die letztere Bedingung wird eifnach über die Buffer Variable isFull geprüft. Weiteres verfährt der Algorithmus gleich wie bei 
einer Datei (zurechtschneiden usw.).

Nachdem der InputBuffer nun gefüllt ist, werden die Threads, wie bereits beschrieben nacheinander gestartet.

Der po holt über die Funktion nextThread den Index des nächsten Producer Thread. Der Index ist equivalent zum Index der Zugriffsmatrix für
den Producer. Somit schläft zu Beginn jeder Producer auf seiner eigenen Condition Variable.  



### Kritik
- Instabilität
- rowsPerCosl Bug
-
