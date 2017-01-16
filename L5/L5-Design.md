# L5-Design

## Inhaltsverzeichnis
- [Vorüberlegung](#vorüberlegung)
	- [Auswahl des Schedulers](#auswahl-des-schedulers)
- [Funktionsweise](#funktionsweise)
	- [Grundlegende Datenstrukturen](#vorüberlegung)
	- [Implementierung](#implementierung)
		- [Berechnung der Prioritäten](#berechnung-der-prioritäten)
		- [Funktionsweise eines Threads](#funktionsweise-eines-threads)
- [Kritik](#kritik)
- [Weitere Quellen](#weitere-quellen)


## Vorüberlegung

### Auswahl des Schedulers:
In unsere Implementierung haben wir für ein non-preemptive Round-Robin Scheduling ausgewählt, zudem mit Prioritäten arbeitet. 
Durch das Round-Robin System wird jegliche Möglichkeit einer Starvation eliminiert, da jedes Tier irgendwann drankommt.
Es werden Tiere priorisiert, die eine niedrige satisfied-time haben, sprich wenn sie schnell wieder essen wollen, kriegen sie eine höhere Priorität. Dadurch müssen solche Threads nicht zu lange warten.
Haben Tiere dieselbe satisfied-time, so wir das letzte genommen, da wir in einer Schleife das Minimum der satisfied-time berechnen und den Index dieses Tieres speichern.

Der Vorteil hieraus ergibt sich das die Prioritäten ausbalanciert werden und jedes Tier dran kommt.  

### Implementierung

#### Grundlegende Datenstrukturen

Bevor der Scheduler überhaupt implementiert werden kann, müssen die Grundbausteine implementiert werden.
In diesem Falle sind es dementsprechend die Tiergruppen und die Food-area.
Eine Tiergruppe ist ein struct und hat eine Anzahl, die es essen möchte `num_eat` (int), eine Zeit, die es braucht, um zu essen `eating_time` (int), eine Zeit, die es verbringt, satt zu sein `satisfied_time` (int), einen Typ `animal_type` (char*) und eine `id` (int). 
Letzteres brauchen wir, um innerhalb der Gruppen zwischen jedem Tier, also jedem Thread, zu unterscheiden. Die Zeiten jeweils sind alle in Sekunden.

Die Food-area ist ebenfalls ein struct und besteht aus einer Anzahl `bowles` (int), einem Status `status` (char*) und einer Anzahl, wieviele Tiere momentan am Essen sind `num_eaten` (int).

Zudem sind in der API noch weitere Macros definiert, die den Code insgesamt verständlicher machen. Beispielsweise wird ein boolean-Typ definiert, wie man ihn normal kennt und es werden Katze, Hund, Maus, Anzahl Gruppen und die jeweiligen Farben definiert.

//fehlt noch -> prio_queue_t
// was ist mit den Hilfsmatrizen wie synchronize, threadDone?

#### Funktionsweise

Zu Anfang werden die Parameter eingelesen, die Variablen dementsprechend zugeordnet und Speicher allokiert. Hervorzuheben ist hier, dass zum Speichern der Prioritäten ein struct definiert wird, dass Daten wichtig für die Priorisierung der Tiere und Tiergruppen beinhaltet. 
Dann werden sämtliche Threads gestartet. 
Wichtig hierbei ist, dass der Scheduler-Thread vor den Tier-Threads startet, da dieser ja entscheidet welcher Thread wann drankommt. 

//Welche Prioritäten? Es gibt 2. Welche Structs? -> prio_queue_t

Die Tier-Threads werden gestartet, inkrementieren die Anzahl der Tiere, die in der Gruppe bereit sind (`isReady[][]`) und gehen dann in den Suspend-Zustand, bis der Scheduler sie aufweckt. 

//Warum ist isReady so wichtig?

Hierfür kriegt jede Tiergruppe eine eigene Condition-Variable, auf die sie schläft, welche durch einen Mutex jeweils geschützt wird.

Wenn der Scheduler startet, so ruft er die `nextGroup()` Funktion auf, die den Index der Tiergruppe mit der wichtigsten Priorität zurückliefert. Der Scheduler verweilt dann so lange im aktiven Warten, bis einer der folgenden Fälle zutrifft:
- Es gibt insgesamt mehr oder gleich viele Tiere innerhalb der Tiergruppe als Futternäpfe __und__ es sind mindestens gleich viele Tiere zum Essen bereit, wie es Futternäpfe gibt
- Es gibt weniger oder gleich viele Tiere innerhalb der Tiergruppe als Futternäpfe __und__ es sind _genau_ so viele Tiere bereit wie es insgesamt Tiere innerhalb der Tiergruppe gibt

//Die beiden Fälle haben sich geändert, der Scheduler wartet lediglich bis die alle Threads einer Gruppe bereit sind.

Erster Fall ist wichtig, wenn wir mehr oder gleich viele Tiere, sprich Threads, als Futternäpfe haben und letzter Fall ist wichtig, wenn wir weniger Threads haben als Futternäpfe. 
Ohne dieses aktive Warten würde er entweder mehr Tiere an die Näpfe lassen, als verfügbar sind oder ewig auf ein Tier warten, dass den Futterschüssel füllt, obwohl es nicht genug gibt.

//muss noch angepasst werden

Daraufhin sendet der Scheduler das Signal an das nächste Tier, das drankommen soll und wartet dann aktiv, bis das Tier sich in die `synchronize` Matrix eingetragen hat. 
Dieser Schritt ist notwendig, denn sonst, da der Scheduler zu schnell arbeitet, würde er zu viele Signale auf einmal senden und somit zu viele Tier-Threads auf einmal aufwecken. 
Im schlimmsten Fall würde er hiermit mehr Tiere aufwecken, als Näpfe verfügbar sind. Mit der Hilfsmatrix `synchronize[][]` wird immer nur ein Tier auf einmal geweckt.

//das Signal an das nächste Tier wird mit Hilfe der nextAnimal Funktion gemacht.

Die `synchronize[][]` Matrix ist ein zweidimensionales Array, wobei die erste Dimension die Tiergruppe (wobei 0 für Katzen, 1 für Hund und 2 für Maus steht) darstellt und die zweite für das jeweilige Tier innerhalb der Gruppe.
Ist ein Tier momentan am Essen, so setzt es an seinem eigenen Index (der derselbe ist wie die ID des Tieres) den Wert true. Ist es fertig mit Essen, so setzt das Tier den Wert auf false. Initialisiert wird die Matrix mit dem undefiniertem Wert -1.

Wenn wir am letzten Tier angekommen sind, warten wir erneut bis alle Tiere fertig sind mit essen. Ansonsten würden mehrere verschieden Tiergruppen auf einmal essen. Auch hier kommt die `synchronize[][]` Matrix ins Spiel. 
Es wird mit der `checkIfEmpty(animal)`-Funktion überprüft, ob alle Einträge an der Stelle der Tiergruppe auf false gesetzt sind, sprich ob alle Tiere fertig sind mit Essen.


//Vielleicht wäre es noch nice wenn wir erklären wieviele Synchronisationselemente wir haben. Also in aller erster Linie haben wir
3 mal aktiv warten. Diese werden über synchronize und  isReady gesteuert. Hauptaugenmerk muss auf den Scheduler.

##### Berechnung der Prioritäten

Wenn das letzte Tier fertig mit Essen ist, so berechnet er die Gruppenpriorität mit `calcGroupPriorities(animal)` neu und schaltet der Scheduler auf die nächste Gruppe. 
Die Funktion macht nichts anderes als die Priorität der Gruppe, die gerade dran war, oder eine Gruppe, die fertig mit dem Essen ist, zu inkrementieren und die der anderen Gruppen zu dekrementieren. 
Durch das Dekrementieren wird verhindert, dass eine Tiergruppe verhungert, denn irgendwann ist die Priorität so wichtig, dass sie auf jeden Fall irgendwann drankommt.
Um zu überprüfen, ob eine Gruppe fertig mit dem Essen ist, wird eine `groupIsDone()`-Funktion verwendet. Diese Funktion schaut in der `threadIsDone`-Matrix, ob alle Threads terminiert sind.

Der Unterschied zur `synchronize[][]`-Matrix ist hierbei, dass die `threadIsDone[][]`-Matrix überprüft ob die Threads terminiert sind, sprich wenn alle Tiere genug gegessen haben. 
Die `synchronize[][]`-Matrix hingegen speichert, ob ein Tier gerade gegessen hat oder am Essen ist.

Wichtig zu beachten hierbei ist, dass eine "höhere" Priorität __unwichtiger__ ist und analog eine "niedrige" Priorität __wichtiger__ ist!

//letztes Tier bzw Gruppe. Und es wird nicht sofort die Gruppenprio berechnet sondern erst wird am Ende gewartet bis alle Threads fertig sind -> checkifEmpty(..). Danach sind wir ja am Schleifende und berechnen erst dann die pries neu.

##### Funktionsweise eines Threads
Nachdem der Scheduler also ein Tier aufgeweckt hat, wird zuerst überprüft ob das Tier schon genug gegessen hat.
Ist dies der Fall, springt er zurück zum Schleifenkopf, die Bedingung wird nicht erfüllt und er geht aus der Schleife raus.
Ansonsten wird die nächste freie Schale mit `nextBowle()` gesucht. Diese Funktion überprüft den Status an der Stelle i. 
Ist dieser '-', was als "frei" definiert ist, so wird der Index von dieser Schüssel zurückgeliefert und das Tier besetzt daraufhin die Schüssel und die Anzahl wie oft das Tier noch essen möchte, wird dekrementiert.

Der Thread wartet daraufhin solange aktiv, bis eine Schüssel frei ist, d.h. der Index *grösser* als -1 ist.
Er setzt dann seinen Wert in `synchonize[][]` auf *true*, d.h. er fängt jetzt an zu essen. 
Essen in diesem Falle heisst, sie schlafen passiv ihre `eating_time`. Da das Tier nun gegessen, ist es nicht mehr "bereit" um zu essen, daher wird `isReady[][]` von der Tiergruppe dekrementiert.
Ausserdem wird der Status der Schüssel zurück auf '-' zurückgesetzt und der Eintrag im `synchronize[][]` Array an der jeweiligen Stelle wird zurück auf *false* gesetzt. Das Tier ist also nicht mehr am Essen.

Dies wiederholt er, wie oben beschrieben, so lange bis das Tier genug gegessen hat. Wenn er aus der Schleife rausspringt, setzt er seinen Status im `threadisDone[][]` Array auf true, da der Thread nun terminiert.

Der Scheduler terminiert, wenn `nextGroup()` -1 zurückliefert, also wenn alle Gruppen genug gegessen haben.

// vllt Chronologischer Ablauf und bessere Trennung von scheduler und thread Funktionalität
// 1. Suspend und isReady 2. Aufgeweckt, schale füllen, aktiv warten bis eine weitere schale verfügbar ist, 3. scheduler wecken 
// und fressen, 4. weg von der Schüssel und synchronnize auf false, da der Thread mehrere male drann kommen kann 5. schritt 1.

//Scheudler Funktion
//1. scheduler holt sich die nächste Gruppe mit der wichtigsten prio, 2. warten bis alle Threads einer Gruppe bereit sind, 3. starte den nächsten priorisieren thread einer Gruppe und warte bis der thread bescheid gibt (Synchronize) damit der nächste Thread nacheinander gestartet wird, 5. Am ende wird geprüft ob die Threads auch tatsächlich alle gegessen haben, 6. repeat.

// Weitere Punkte die noch erwähnenswert sind.
//- wann werden die die Wartezeiten pro thread gespeichert, wie werten wir diese aus
//

## Kritik

## Weitere Quellen
