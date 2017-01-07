#L5-Design
##Vorüberlegun
Auswahl des Schedulers:
Round Robin ist starvation frei aber es stellt das problem dar, das es 
präemtptiv ist, sobald das Zeitquantum so stark runterskaliert wird, dass 
der Thread nicht schnell genug fertig ist verletzten wir bereits eines der 
Anforderungen, nämlich nicht präemtives scheduling. 
