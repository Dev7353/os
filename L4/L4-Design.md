# L4-Design

## Prodcon

### Vorüberlegung
Ziel dieser Aufgabe ist es das Consumer/Producer Problem für n,m Threads zu lösen.
Ein wichtiger Punkt hierbei ist der Umgang mit einem Buffer dh. worin die Daten gespeichert bzw. woraus gelesen werden soll. 
Eine Möglichkeit wäre eine simple globale Matrix aus char Typen zu erstellen, sodass die Threads abwechselnd darauf zugreifen.
Ein Hindernis ist jedoch, dass die Zugriffsoperationen in jedem Thread definiert werden müssen, da jeder Thread über seine interne ID zugreift. 
Eine Lösung hierfür wäre eine FIFO Queue zu realisieren die mit einfachen init, destroy, push und pop Operationen die Daten modifiziert. 
Die Queue ist demnach lediglich ein Bestandteil des Buffers. Der Buffer ist eine Struktur in dem Anzahl Elemente, nächstes Element, die eigentliche Queue sowie Inforamationen darüber,
ob die Queue leer bzw. voll ist. Zu diesem Zeitpunkt erscheinen diese Parameter als optional, da die vollständige Implementierung der Threads nicht ausdefiniert war. Falls diese Parameter nicht benötigt werden, werden sie entfernt.
Der Buffer enthält somit die Queue sowie zusätzliche Variablen die den Status des Buffers sehr schnell ermitteln lässt.
Eine zusätzliche API implementiert die Buffer Operationen.

Die Anforderung von L4 setzt weiterhin vorraus, dass die Eingabedaten von einer Datei oder von der Standardeingabe eingelesen werden können.
Da das parallele Erstellen von Threads und Einlesen der Daten für unser Verständnis separiert werden sollen, wird ein weiterer Buffer verwendet.
Dieser ist ebenfalls global, jedoch nur vom Producer bedient, d.h. kein Mutex. Die Daten werden also formatiert in einen InputBuffer geschrieben
und anschließend von den Producern in den originalen Buffer kopiert. 

Der mit Abstand wichtigste Punkt ist die Thread-Synchronisierung. Das Synchronisationsverfahren für einen Producer bzw. Consumer ist bereits in der Vorbereitungsaufgabe erstellt und kann daher als Basis verwendet werden. 
Die Consumer bzw. Producer müssen nicht notwendigerweise abwechselnd die Daten verarbeiten. Hierbei kann der Producer pro Thread mehrere Daten (Strings) in den Buffer schreiben. 
Wenn der Thread terminiert wird ein Signal an einen Consumer geschickt der so lange die Daten  verarbeitet bis er fertig ist. Der wichtigste Punkt hierbei ist der wechselseitige Ausschluß der Threads.
Damit der Consumer weiss, dass er nun arbeiten darf, muss er benachrichtigt werden. Somit könnte der Producer nach jeder Operation auf die Daten zugreifen.
Wenn 5 Consumer 6 Datensätze verarbeiten, kann es passieren, dass der letzte Consumer niemals terminiert, da er nicht weiss, wann er dies machen soll.
Für diesen Fall ist es wichtig sich zu merken, wieviele Consumer nach Beendigung der Arbeit noch aktiv sind. Eine globale Zugriffsmatrix wird daher definiert. 

Die Producer Threads haben eine besondere Eigenschaft. Jeder Producer muss unterschiedlich viele Daten verarbeiten. Bei 5 Daten und 2 Producern
müssen alle Daten von den Producern verarbeitet werden. Analog wie beim Consumer, bietet es sich ebenfalls an eine globale Zugriffsmatrix zu definieren.
Somit wissen die Producer wieviele Threads bereits gearbeitet haben. Falls alle bereits aktiv waren, es jedoch noch Daten im InputBuffer gibt, müssen die Producer wieder in einen Ready-Zustand gebracht werden, sodass sie erneut arbeiten können. Wenn nun alle Daten verarbeitet wurden und 
es noch aktive Producer im Suspend-Zustand gibt, dann müssen diese ebenfalls terminiert werden.

Das passive und aktive Warten wird trivialerweiser, wie bereits aus L3 bekannt, implementiert.

### Funktionsweise

#### Datenfluss
Der gesamte Synchronisationsmechanismus besteht aus n Consumer, m Producer sowie einem Producer Observer und Consumer Observer.
Die Steuerung erfolgt über jede abgeschlossene Operation. Die Reihenfolge der Threads ist hierbei sehr wichtig. Der Consumer Observer (*CO*) startet zuerst und geht in den Suspend-Zustand. Anschließend werden nacheinander alle Producer und alle Consumer Threads gestartet welche ebenfalls im Zustand Suspend sind. 
Der Producer Observer(*PO*) wird am Ende gestartet, denn dieser löst eine Kettenreaktion aus. Der *PO* startet den ersten Producer-Thread und geht in den Suspend-Zustand. 
Grundsätzlich wird jeder Thread mit einer ID initialisiert. Bei m Producer wird per Zugriffsmatrix der erste Thread geweckt. 
Jeder Thread hat eine eigene Condition-Variable. Der Erste Producer legt n Datensätze in den Buffer, weckt den *CO* und geht in den Suspend Zustand.
Der *CO* weckt analog zum *PO* per Zugriffsmatrix den ersten bzw. nächsten consumer Thread. 
Der Consumer holt über die pop Funktion das Kopfelement der Queue und wandelt dieses Element entsprechend in Großbuchstaben. 
Anschließend wird der *PO* geweckt und in den Suspend-Zustand gewechselt. Das Verfahren erfolgt erneut, solange bis alle Daten verarbeitet wurden.

Jedes mal wenn der *PO* geweckt wird, wird geprüft ob noch aktive Producer existieren wobei die Arbeit beendet ist. Falls ja, werden diese ebenfalls terminiert. Ähnlich verfährt der *CO*.

Im Falle das die Arbeit nicht beendet ist, jedoch alle Producer bzw. Consumer im Suspend-Zustand sind, werde diese wieder geweckt. 

#### Interne Verarbeitung & Probleme
Der globale Buffer wird über einen Mutex geschützt. Jeder Thread hat eine eigene Condition-Variable. Die Notwendigkeit hierfür war, das die Threads nur intern wissen, wer sie sind (anhand der ID). 
Da die Zugriffsmatrizen außerhalb der Threads von *CO* bzw. *PO* gesteuert werden müssen die Threads ebenfalls von außen bekannt sein. Ein Broadcast bzw. Signal würde nur den nächsten Thread starten ohne das *PO* oder *CO* wissen wer im Moment die Daten verarbeitet. 
Weiterhin ergibt sich das Problem falls Consumer und Producer sich gleichzeitig im Suspend Zustand befinden. 
Um diese Synchronisationshemnisse zu beseitigen werden Arrays von Condition Variablen benötigt. So kann jeder Observer den spezifischen Thread starten welcher im Zustand Suspend ist, falls es noch Arbeit gibt.

Zusätzlich hat jeder Observer jeweils eine Condition-Variable sodass sie über diese aufgeweckt werden können.

Der FIFO Buffer enthält eine init-,  destroy-, add- und pop-Funktion. Die init-Funktion initialisiert und allokiert Speicher. 
Hierbei werden Default-Werte gesetzt und Speicher in Größe der einzulesenden Daten allokiert. Die destroy Funktion gibt den gesamten Speicher wieder frei.
Die add Funktion fügt der Queue an der Schwanzspitze (tail) das nächste Element an, falls die Queue nicht bereits voll ist.
Wenn ein Element erfolgreich angehängt wurde wird die tail-Variable inkrementiert. Der Kopf der Queue (head) ist, wenn zuvor noch nicht gepopt wurde, auf dem Wert 0. 
Somit ist die Queue z.B. [Hello, World, Foo] wobei Head 0 ist und queue[head] Hello ist.

Die pop-Funktion inkrementiert den Head, sodass dieser immer auf das nächste Element in der Queue zeigt. Bei einem pop-Aufruf würde dann beispielsweise 'Hello' zurückgeliefert werden und der Head anschließend auf 1 inkrementiert. Beim nächsten pop-Aufruf dann 'World' und Head auf 2.
Allerdings ist der FIFO Buffer nicht vollständig implementiert, denn die älteren Datensätze bleiben erhalten. 
Für unseren Anwendungsfall reicht diese Implementierung jedoch aus, da die Queue nicht dynamisch wachsen bzw. schrumpfen muss.

Da die pop- und add-Funktionen Strings auf die Queue legen und die Strings über Referenzen übergeben werden, müssen sie kopiert werden, um die Daten in der Queue nicht nachträglich zu verfälschen. Hierbei wird die Funktion memcpy verwendet.

Die Datensätze werden wahlweise über eine Datei (readFile) oder Standardeingabe (readStdin) eingelesen.
Falls eine Datei angegeben ist, werden zeilenweise die Sätze als Strings interpretiert. 
Die Stringlänge ist per Default auf 20 chars limitiert. Falls längere Sätze eingelesen werden sollen
bietet die Option -C eine variable Länge an wobei diese nicht negativ sein darf. Die Anzahl der Zeilen die eingelesen werden können ist ebenfalls auf 20 limitiert und über die Option -L modifizierbar.
Vor dem Einlesen muss die Datei im Lesezugriff geöffnet werden. 
Über die Funktion fgets können dann Zeilenweise die Daten in eine Variable gespeichert werden. Die Variable muss zuvor Speicher allokieren. 
Ein bisher noch nicht gelöstes Problem ist, das die maximale Anzahl an chars die pro Satz in die Variable kopiert werden nicht dynamisch bestimmt werden kann. (ausgehend von der derzeitigen Lösung)

Der gelesene Satz in der Variable wird nun entsprechend der -C Option "zurechtgeschnitten". 
Über memcpy werden nur Anzahl chars von der Variablen in eine andere Variable kopiert, für die ebenfalls zuvor Speicher allokiert worden ist. 
Allerdings ist hier zu beachten, dass für die zweite Variable ein char mehr allokiert werden muss, da memcpy kein String Terminator automatisch anhängt. 
Daher wird die zweite Variable um den Terminator '\0' ergänzt. Die add-Funktion der Buffer API fügt nun den String zur Buffer-Queue an. 
Anschließend werden die Speicherbereiche der Variablen freigegeben und der Filedeskriptor geschlossen.

Das Lesen der Daten über die Standardeingabe erfolgt über die fscanf Funktion. 
Über eine bedingte Schleife werden solange Sätze eingelesen, bis der Buffer voll ist. Die letztere Bedingung wird einfach über die Buffer Variable isFull geprüft. 
Weiteres verfährt der Algorithmus gleich wie bei einer Datei (zurechtschneiden usw.).

Nachdem der InputBuffer nun gefüllt ist, werden die Threads, wie bereits beschrieben nacheinander gestartet.

Der *PO* holt über die Funktion nextThread den Index des nächsten Producer Thread. Der Index ist equivalent zum Index der Zugriffsmatrix für den Producer. 
Somit schläft zu Beginn jeder Producer auf seiner eigenen Condition-Variable. 
Der erste Producer wird daraufhin gestartet.
Die Anzahl der Zeilen, welcher der Producer bearbeitet werden bereits zur Threaderzeugung festgelegt. Die Producer werden über eine Anzahl Threads in einer for-Schleife erzeugt. 
Der Index i ist hierbei der Index für den Producer und für die Zugriffsmatrix.
Die Grenzen der Threads, also Zeilenanfang bis Zeilenende, werden in einer Struktur gespeichert. Jeder Producer hat ebenfalls eine Strukturinstanz.

Die Grenzen werden wie folgt berechnet: 
Der Startpunkt ist der Index des Threads * (Anzahl Zeilen/Anzahl Producer).
Der Endpunkt ist genauso nur mit dem Unterschied, das der Index um 1 inkrementiert ist.
Ein sehr wichtiger Punkt hierbei, war darauf zu achten die richtigen Datentypen zu verwenden. 
Falls Integer geteilt werden, wird der Rest nicht berücksichtigt. Da der Rest jedoch ausschlaggebend ist müssen die Zahlen zuvor nach Double gecastet werden. 
Nun können die Ergebnisse über die Funktion round() gerundet werden und die nächstgrößere Zahl als Start bzw. Endpunkt verwendet werden. 
Bei 5 Zeilen und 2 Producer Threads wären die Grenzen also für Thread 0: 0 bis 3 und für Thread 1: 3 bis 5.

Jeder Producer berechnet im exklusiven Zugriff auf den Buffer sämtliche Datensätze. 
Im kritischen Bereich wird die add-Funktion aufgerufen.
In jedem Schleifendurchlauf des Producers wird jedesmal neuer Speicher für einen String allokiert und freigegeben. 
Der Producer holt mit dem pop-Befehl das nächste Element aus dem inputBuffer und fügt es dem richtigen globalen Buffer hinzu. 
Da die pop Funktion Speicher für einen String allokiert, jedoch nicht freigibt, muss nach jedem pop-Aufruf das Ergebnis freigegeben werden (daher auch der free Befehl in jedem Schleifendurchlauf).

Kurz vor dem Verlassen des kritischen Bereichs  wird in der Zugriffsmatrix an der Stelle des Indexes der Zugriff auf *true* gesetzt. 
Somit wird verhindert, dass der Scheduler nicht nach Zufall den nächsten Producer wählt. 
Der nächste Producer wird anhand der Zugriffsmatrix bestimmt. Falls der Thread einen *false* Eintrag hat, wird dieser als nächstes geweckt.

Nach dem Verlassen des kritischen Bereichs wartet der Thread passiv und weckt den *CO*. Der *CO* verfährt analog und wählt anhand der Zugriffsmatrix den nächsten freien Consumer Thread.

Durch die Zugriffsmatrizen entstehen eine vielzahl neuer Probleme bezüglich der Synchronisierung. 
So sind zu Beginn z.B. die Observer Threads terminiert, sobald alle Producer/Consumer-Threads fertig waren, also die Zugriffsmatrizen alle gefüllt sind. 
Da die Zeilen jedoch unabhängig von der Anzal Producer und Consumer vollständig verarbeitet werden, müssen die Zugriffsmatrizen geleert werden. 
Die Observer Threads dürfen daher nicht terminieren wenn die Zugriffmatrizen voll sind und es noch Arbeit gibt. 
Stattdessen müssen die Matrizen zurückgesetzt werden und die Threads erneut die Arbeit aufnehmen.
Die Consumer Threads haben keine bestimmte Anzahl an Elemente welche sie verarbeiten. Ein Consumer läuft in einer Endlosschleife.
Der Grund hierfür ist, dass weitere Grenzen dem Umgang mit den Consumern stärker verkomplizieren als es ohnehin schon ist.
Somit ergibt sich das Problem, dass ein Consumer wissen muss, wann er terminiert. Diese Aufgabe übernimmt der *CO*. Da jeder Consumer Thread nach jedem Schleifendurchgang in den Suspend-Zustand geht muss der *CO* falls er keine Arbeit mehr gibt, alle Consumer frei geben. 
Jeder Consumer prüft intern ebenfalls ob noch Arbeit vorhanden ist. Falls nein, dann wird die Endlosschleife verlassen. 
Ansonsten würde der *CO* gar nicht erst alle Consumer nacheinander aufrufen, wenn es Arbeit geben würde. 
Die Anzahl an verbliebener Arbeit wird in einer globalen Integer Variable (*operations*) abgespeichert und regelmäßig vom *CO* geprüft.

Ähnlich verfährt auch der *PO*. Allerdings verläuft die Logik in entgegengesetzer Richtung. Es wird ebenfalls in einer globalen Integer Variablen (*additions*) ein Wert abgespeichert. 
Allerdings ist der Wert die Anzahl der Elemente der Queue des inputBuffers. 
Das hat den Grund, das der *PO* ebenfalls eine Bedingung braucht, wann er tatsächlich terminieren darf. 
Und das nur falls genau so viele Elemente auf die Queue gelegt werden wie der InputBuffer
zu Beginn hat. 

Nach der Terminierung der Observer-Threads, sollten keine weiteren Threads mehr aktiv sein. Im Hauptthread wurden derweil sämtliche join-Befehle ausgeführt um die Threads zu schließen. 
Da jeder Thread die üthread_exit Funktion aufruft ist das joinen eigentlich überflüssig, jedoch bestehen wir trotzdem darauf, da die joins zuerst da waren.

Zum Ende werden noch allokierte Speicherbereiche freigegeben. Darunter sind die Condition-Variablen und die Zugriffsmatrizen.


### Kritik
Im Rückblick, und nach Aussprache mit Mitkommilitonen, erscheint die Lösung mit den parallelen Observer-Threads als eine etwas komplizierte.
Es gibt relativ wahrscheinlich eine Möglichkeit ohne die Observer, etwa mit R/W Locks oder ähnlichem.
Da wir jedoch die Vorbereitungsaufgabe ebenfalls mit Observern gelöst haben, lag es nahe, dass wir die Hauptaufgabe ähnlich lösen.
Dennoch, durch unseren Lösungsansatz wurde unser Code um viele Zeilen länger, denn nicht nur mussten wir die Producer und Consumer programmieren, wir mussten ebenso schauen dass die Observer korrekt funktionieren.
Dies führte möglicherweise zu den paar Fehlern, die Helgrind uns ausgibt - Jedoch konnten wir nicht herausfinden was diese bedeuten, noch haben wir eine Beeinträchtigung in der Funktionalität unseres Programms gefunden.

Ein weiterer Punkt ist ein Bug der nicht gefixt werden konnte. An sich hat das Programm in allen Konfigurationen keine Speicherlecks mit Ausnahme der -C Option.
Die Option limitiert lediglich die maximale größe des String Elements eingelesen werden soll.
Allerdings führt dies zu einem Speicherleck der nicht näher beseitigt werden konnte. Das Programm verfährt zwar nach wie vor und liefert das richtige Ergebnis, jedoch wird der Speicher nicht korrekt
verwaltet.

Weiterhin funktioniert das Programm nicht korrekt, wenn mehr Producer und/oder Consumer Threads starten, wie Zeilen zu verarbeiten sind. Das Programm läuft stabiler, wenn die Anzahl an Producer und Consumer
überschaubar bleibt.

Die Untersuchung mti time zeigt, dass die Ausführungszeit mit zunehmender Anzahl Strings steigt. 
