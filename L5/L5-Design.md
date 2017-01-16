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
In unsere Implementierung haben wir für ein non-preemptive Round-Robin Scheduling ausgewählt, welcher mit Prioritäten arbeitet. 
Durch das Round-Robin System wird jegliche Möglichkeit einer Starvation eliminiert, da jedes Tier irgendwann drankommt.
Es werden Tiere priorisiert, die eine niedrige satisfied-time haben, sprich wenn sie schnell wieder essen wollen, kriegen sie eine höhere Priorität. Dadurch müssen solche Threads nicht zu lange warten.
Haben Tiere dieselbe satisfied-time, so wird das letzte genommen, da wir in einer Schleife das Minimum der satisfied-time berechnen und den Index dieses Tieres speichern.

Die Idee ist, das die Prioritäten ausbalanciert werden und jedes Tier dran kommt. 
Zum Beispiel:
Nehmen wir Default-Einstellung für den Scheduler, wobei die satisfied time für Katzen 15 sec, Hunde 10 sec und Mäuse 1 sec ist.
Die Mäuse beginnen zuerst weil die am schnellsten Hungrig werden. Somit ergibt sich die folgende Ablaufreihenfolge für die Tiere:

Katze 14, 	13, 	12, 	11, 	10 ...

Hunde 9, 	8, 	7,	6, 	5 ...

Mäuse 2, 	3, 	4, 	5, 	6 ...

Nach der fünften Runde ist der Hund an der Reihe.


### Implementierung

#### Grundlegende Datenstrukturen

Bevor der Scheduler überhaupt implementiert werden kann, müssen die Grundbausteine implementiert werden.
In diesem Falle sind es die Tiergruppen, die Food-area und die Prioritäten Queue.

Eine Tiergruppe ist ein struct und hat eine Anzahl, die es essen möchte `num_eat` (int), eine Zeit, die es braucht um zu essen `eating_time` (int), eine Zeit, die es verbringt satt zu sein `satisfied_time` (int), einen Typ `animal_type` (char*) und eine `id` (int). 
Letzteres brauchen wir, um innerhalb der Gruppen zwischen jedem Tier, also jedem Thread, zu unterscheiden. Die Zeiten sind alle in Sekunden.

Die Food-area ist ebenfalls ein struct und besteht aus einer Anzahl `bowles` (int), einem Status `status` (char*) und einer Anzahl, wieviele Tiere momentan am Essen sind `num_eaten` (int).

Eine weitere Datenstruktur die Informationen über die Prioritäten speichert ist 'prio_queue_t'. Hierbei wird für jedes Tier und jede Tiergruppe 
die Prioritäten als int Wert abgespeichert. Die Hilfsfunktionen wie z.B. 'nextGroup()' und 'calcGroupPriorities()' arbeiten mit der Struktur. Ersteres 
traversiert über ein Array in dem die Gruppenprioritäten bestimmt sind und liefert den Index der wichtigsten Gruppe zurück. Die zweite Funktion 
balanciert die Prioritäten aus. Die Aktuelle Gruppe verliert an Priorität, während die wartenden Gruppen wichtiger werden. Dies führt dazu, dass
die Gruppen zwar zu Beginn recht unausgeglichen gewählt werden, jedoch längerfristig alle Gruppen in fairer weise drankommen. 

Weiterhin enthält werden noch Hilfmatrizen genutzt. Die Matrix 'threadDone' speichert ob ein Thread pro Gruppe, ob er vollständig terminiert ist. 
Die 'synchronize' Matrix ist ein wichtiger Bestandteil des Synchronisationsmechanismus des Schedulers. Auf den wir später näher eingehen.

Zudem sind in der API noch weitere Macros definiert, die den Code insgesamt verständlicher machen. Beispielsweise wird ein boolean-Typ definiert, wie man ihn normal kennt und es werden Katze, Hund, Maus, Anzahl Gruppen und die jeweiligen Farben definiert.

Die Tiere werden intern als 0 Katze, 1 Hund und 2 Maus referenziert.


#### Funktionsweise

Zu Anfang werden die Parameter eingelesen, die Variablen dementsprechend zugeordnet und Speicher allokiert. 
Nach den Zuweisungen und Ressourcenbereitstellung, werden die Threads nacheinander pro Gruppe gestartet. 
Wichtig hierbei ist, dass der Scheduler-Thread vor den Tier-Threads startet, da dieser ja entscheidet welcher Thread wann drankommt. 

#### Der Tierthread
Der Tierthread ist generisch deffiniert und nimmt über die Parameter seine Rolle als Tier an.
Wenn ein Thread startet registriert dieser sich in einer Hilfmatrix 'isReady' und geht in den Suspend Zustand, dh. das warten auf eine
Condition Variable. Jeder Thread hat eine eigene Condition Variable, dies ermöglicht es, gezielt die Threads zu wecken.

Wenn ein Tier-Thread nun aufgewacht ist, belegt er eine Futterschüssel aus der Food-Area und frisst. Wenn die 'eating_time' abgelaufen ist, verlässt der Thread
die Futterschüssel, verringert die Laufvariablen und geht erneut in den Suspend Zustand. Der Thread terminiert, sobald er die Anzahl an Fressen erfüllt hat.

Am Ende des Thread, kurz vor der Terminierung, wird in der 'threadDone' Matrix eingetragen, dass der aktuelle Thread beendet ist.

#### Der Scheduler
Der Scheduler Thread ist der erste Thread der gestartet wird. Zu Beginn wird über die Funktion 'nextGroup' die Gruppe mit der wichtigsten Priorität bestimmt.
Anschließend verweilt der Scheduler im aktiven Warten, solange bis die Threads der jeweiligen Gruppe sich in der 'isReady' Matrix registriert haben.

Daraufhin holt sich der Scheduler über die Funktion 'nextAnimal' den Index der Condition Variable des Tieres, mit der wichtigsten Priorität und weckt den Thread über
ein Signal und verweilt dann im aktiven warten, bis der geweckte Thread sich in der 'sychronize' Matrix registriert hat. Über diesen Mechanismus, wird ein serialisierter
Ablauf gewährleistet. Weiterhin verfährt der Scheduler weiter, solange alle Tiere einer Gruppe drann waren. Wenn der Scheduler den letzten Thread weckt, wartet
er erneut aktiv, bis alle Threads auch tatsächlich fertig sind. Die Funktion prüft, ob alle Threads sich aus der 'sychronize' Matrix
abgemeldet haben, denn erst dann darf der Scheduler den aktuellen Durchlauf beenden.

Anschließend werden über die Funktion 'calcGroupPriorities' die Prioritäten neu bestimmt, und der Scheduler verfährt erneut.
Der Scheduler terminiert, wenn die Funktion 'nextGroup' -1 zurückgeliefert, denn dies bedeutet, dass alle Gruppen bereits fertig sind und somit der Scheduler keine
Arbeit mehr zu leisten hat.

#### Zeitmessung
Die Zeit pro Tierthread wird wie folgt gemessen. Jedesmal wenn ein Tierthread hungrig wird wird ein Zeitstempel gesetzt. Wenn dieser Thread durch
den Scheduler aufgerufen wird, wird erneut ein Zeitstempel gesetzt. Die Differenz der beiden Zeitstempel wird eine Matrix 'waitingTimes' eingetragen.
Am Ende des Scheduler Threads, werden die Daten mit Hilfe der Funktionen 'getMin', 'getMax' und 'getAvg' ausgewertet. Durch hinzunahme des Arguments --file
werden alle Wartezeiten der Threads, sowie die ausgewerteten Daten in eine Datei gespeichert. Ein Python Programm analysiert die Wartezeiten und zeichnet
mit Hilfe der Matplotlib Python Bibliothek ein Diagramm. 

Das Diagramm enthält die Threads und deren Wartezeiten, wobei die X-Achse die Thread Nummer beschreibt und die Y-Achse die Wartezeit in Sekunden.
## Kritik

## Weitere Quellen
