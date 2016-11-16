# BSYS L1

## Code files

### C
* genstacklib.c
	* GenstackNew(...) Initialisiert Stack mit Startwerten und allokiert Speicher für den Stack Zeiger.
	* GenstackPush(...) Setzt einen Wert eines beliebigen Typs auf den Kopf des Stacks.
	* GenstackPop(...) Entfernt den Kopf des Stacks und liefert das Ergebnis in einem Zeiger zurück.
	* GenstackDispose(...) Gibt den allokierten Speicher an das Betriebsystem frei.
	* GenstackEmpty(...) Gibt zurück, ob der Stack leer ist.
* forksqrt.c
	* Konfiguriert die inih library für die sqrt-Config-Datei. Struktur referenziert https://github.com/benhoyt/inih
	* Erstellt Pipes, forkt und schreibt im Elternprozess die von der Config-File eingelesenen Parameter, die der Kindprozess benötigt. Der Kindprozess ist das Pythonscript forksqrt.py. Gibt das Ergebnis vom Script auf die Konsole aus.
	* Nimmt über getopt zusätzliche Parameter, die Hilfe zeigen, eine andere Config-File einlesen oder Debug-Informationen anzeigen.

### Python
* forksqrt.py
	* Berechnet mit dem Heron-Verfahren die Wurzel einer Zahl und kann bei Bedarf Debug-Ausgaben auf die Konsole ausgeben
	* Bekommt die Zahlen und Konfigurationen durch eine Pipe vom Elternprozess.
	* Öffnet eine eigene Pipe und sendet das Ergebnis zurück zum Elternprozess.
* procenv.py
	* Gibt je nach eingegebenem Kommandozeilenparameter Systeminfo aus.
	* Kann Informationen über die Environment des Systems ausgeben.
	* Kann eine Prozesshierarchie bis zum init Prozess darstellen.

### Zusätzliche Dateien
* ini.c / ini.h
	* C Library zum einlesen von Config-Files
* forksqrt.cfg
	* Standard Konfigurationen für die sqrt2 Python Funktion.
* forksqrtmax.cfg
	* Konfigurationen für maximale Genauigkeit bevor das System zusammenbricht.

### Weitere Referenzen
* http://stackoverflow.com/questions/23635576/send-data-from-c-program-to-python-program-using-pipe