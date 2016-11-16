# L2-Design

## Generischer Stack

### Vorüberlegung:
	
	Die Informationen über die Größe des Elementes obliegt stets dem Client.
	Wenn der Client einen Datentyp anlegt, muss die Größe beim Erstellen des Stacks als Parameter übergeben werden.
	
	Der Client selbst, arbeitet ausschließlich mit Adressen und Speichergrößen und der damit eingehenden Arithmetik.
	Die Vorüberlegung hier war aus dem Anlass heraus, dass typenlose Adressen keine Arithmetik unterstützen, 
	die Eigenschaft zu nutzen den Zeiger auf die Adresse in einen echten Typen umzuwandeln. Denn dann müssten arithmetische Operationen möglich sein.
	
	Nach dem Umwandeln ergibt sich weiterhin, dass der Stackzeiger nicht einfach auf den Wert des Elementzeigers referenzieren kann,
	da das Element stets überschrieben wird. Also müssen die Daten kopiert werden. 
	Eine weitere Überlegung war, ein Hilfsarray zu nutzen das jedes Char und somit jedes Byte einfach per Zuweisung umkopiert. 
	Jedoch ist die Nutzung der Stringbibliothek sicherer und wurde deshalb auch bevorzugt gewählt. 
	Die Bibliothek bietet zahlreiche Möglichkeiten für das byteweise kopieren eines Strings an.
	
	Falls der Stack gelöscht werden soll und es noch Elemente auf dem Stack gibt, müssen diese auch gelöscht werden bzw. der Speicherbereich
	freigegeben werden. Die Lösch-Methode ist das letzte Statement im Client und soll zuverlässig ohne weiteren Eingriff funktionieren. 
	Damit das möglich ist, muss eine weitere Funktion im Client definiert werden, die alle notwendigen Speicherbereiche des Typs freigibt.
	Diese Funktion muss dann in der API aufgerufen werden. 
	
	Ein Test für einen primitiven Datentyp benötigt keine spezielle Methode die von außen gerufen wird, da für Datentypen dieser Art kein Speicher vom Client allokiert werden muss.
	
	Die Tests für die komplexen Datentypen brauchen so eine Methode, die den Speicherbereich der untergeordneten Datentypen freigibt. 
	Die Methode verfährt genauso wie die Freigabe in der Main Methode.
	


### Funktionsweise:

	Im Gegensatz zum statischen unsigned Long Stack aus Aufgabe L1, verwendet der veränderte Stack keine spezifischen Datentypen mehr.
	Vielmehr muss der Stack die Speicheradressen der Elemente korrekt verwalten.

	Wenn also der Client einen Typen definiert, muss er lediglich die Adresse der Variable, sowie die Speichergröße des Typs übergeben.
	Der Stack wird Standardmäßig also mit vier Speicherbereichen der größe des Typs initialisiert. 
	Allerdings sind diese Speicherbereiche nicht zugreifbar, da sie keinen konkreten Typ besitzen sondern nur dessen Größe. Ebenfalls verbietet C i.d.R 
	Pointerarithmetik auf Void Pointern.

	Der Push Befehl erwartet ebenfalls nur Void Pointer. In L1 wurde einfach per Indexoperator an der nächste Wert abgelegt. Da wir dieses Verfahren nicht anwenden können bietet
	der Void Pointer eine sehr hilfreiche Funktion an: Das Casting. Ein Void Pointer kann jeden belieben Pointertyp annehmen. Wenn der Pointer auf einen Int Pointer gecastet wird, 
	kann nun über Pointerarithmetik simpel in 4 Byte Schritten iteriert werden. 4 Byte sind jedoch viel zu spezifisch und würden die Daten nur verfälschen. Angenommen der Typ im Client hat eine Größe von
	5 chars, also 5 Byte, dann wurde man die ersten 4 Byte auf den Stack kopieren und womöglich noch einen Speicherzugriffsfehler. Es ist also ein Casting Typ zu wählen mit dem jedes
	Byte des Speicherbereichs erreicht wird. Hierbei bietet sich der Typ char an. Das Casting von void * nach char * erlaubt es über den gesamten Speicherbereich in ein-Byte Schritten zu iterieren. 

	Da wir nun über den Speicherbereichs einer Typ Instanz iterieren können, müssen nun die Daten auf den Stack gepusht werden. Analog muss der Stack selbst, ebenfalls nach char * gecastet werden.
	Der Stack, sowie das zu pushende Objekt sind nun vom selben Typ. 
	Die eigentliche Push Operation ist in dem Fall nur das kopieren des Speicherbereichs des Objekts auf den Stack. 
	Damit nicht ungewollte Speicherbreiche ebenfalls umkopiert werden, muss angegeben werden, wieviele Bytes benötigt werden.
	Die Anzahl Bytes sind in dem Fall die Anzahl der Bytes pro Objekt, also die Speichergröße des Typs. 

	Ein weiterer wichtiger Punkt ist, an welcher Stelle des Stacks das Objekt reinkopiert wird. 
	Bis hierher kopiert Push den Inhalt des Objektes in den Stack an der ersten Speicherstelle. 
	Vor dem Umkopieren muss der Stack auf die nächste freie Speicherstelle zeigen. 
	Die erfolgt mit der Pointerarithmetik. Nach dem Casten setzen wir den Pointer auf die Stelle mit (ADRESSE + ANZAHL_GEPUSHT * TYPGROESSE).

	Das Kopieren des Speicherinhalts kann auf verschiedene Weisen geschehen,  da C mehrere Alternativen anbietet wie z.B. die Stringbibliothek mit den Funktionen str(n)cpy(), memmove() usw. 
	Die erste Funktion kopiert eine Anzahl Bytes im String. 
	Falls jedoch die Anzahl kleiner als die Stringlänge ist, entfällt der NULL Charakter, der das Ende des Strings signalisiert. 
	Das führt dazu, dass der kopierte String in einer Ausgabe solange gelesen wird, bis ein null character gefunden wird. Somit verändert sich der Speicherbreich bzw. kann auch in undefinierte Bereiche gelangen. 
	Wenn der Client nun auf einen Speicherbereich zugreifen möchte, kann es u.U. zur Fehlzugriffen kommen. 
	Eine Abhilfe schafft die memmove Funktion. Hierbei werden Bytes anstatt chars kopiert wobei die Eigenschaften von Strings nicht berücksichtigt werden. 
	Alternativ kann auch die memcpy Funktion verwendet werden, die im Gegensatz zu memmove ein undefiniertes Verhalten bei überlappenden Speicherbereichen hat. 
	Diese Eigenschaft spielet jedoch keine Rolle, da über die Adressarithmetik so ein Fall nicht eintreten sollte.

	Wie bereits aus L1 bekannt vergrößert sich der Stack automatisch wenn es keine freien Speicherbereiche der angeforderten Größe gibt. 
	Die Funktion realloc
	kopiert den gesamten Inhalt eines Zeigers in einen vergrößert Zeiger um. 
	Das Verkleinern dieses Speicherbereichs wird nicht unterstützt. 
	
	Möchte man nun das oberste Objekt des Stack mit einer pop Operation entfernen, wird das gleiche Verfahren wie beim Pop angewandt nur mit dem Unterschied, dass die Anzahl gepushter Objekte dekrementiert wird und 
	der Speicherbreich des Stacks anstatt des Objekts umkopiert wird, sodass das gepoppte Objekt zurückgeliefert wird. 

	Ein weiterer wichtiger Aspekt ist das anschließende Aufräumen des Stacks. 
	Der Stack reserviert im Laufe seiner Laufzeit eine Menge Speicher der anschließend wieder an as Betriebssystem freigegeben werden muss. 
	Bei primitiven Datentypen muss lediglich der Stack pointer freigegeben werden und ggfs. auf NULL gesetzt.
	Bei komplexen Datentypen können ebenfalls Pointer enthalten sein, wie z.B. char*. 
	Das Problem ist, dass wenn die Objekte nicht fachgerecht gepoppt werden und es noch Objekte auf dem Stack gibt die Aufräumfunktion dispose() lediglich den Stack Pointer frei gibt, jedoch nicht die enthaltenen Pointer. 
	Dies führt unweigerlich zu Speicherleaks. Damit auch die restlichen Pointer auf dem Stack gepoppt werden,
	muss vor dem Freigeben des Stackpointers geprüft werden ob der Stack noch belegt ist, wenn ja, dann müssen weitere Pop Operationen erfolgen. 
	Da diese Feststellung in der Dispose Funktion stattfindet und Teil des generischen Stacks ist, haben wir keine Informationen über den Typ. 
	Die restlichen Stackelemente werden in einer Funktion im Client (freefn) gepoppt und als Parameter im generischen Stack initialisiert. 
	So kann jedes Element jeglichen Typs entfernt werden.
	
	
	
