
Matrix
	Prolog
	In this part the task is about multiplying two matrices.
	The Specification is that the program has to run in single thread mode
	and in a multithreading mode.
	
	The algotihm for solving a matrix multiplication is found by searching it in google. (see sources)
	One of the biggest problems is, how the program can multiply the matrix with threads.
	The first idea was to calculate every row in number of threads. So if we take a 3x3 matrix
	then a possible multithreaded solution can look like 3 threads solving each row. 
	But then, is only one specific number which calculates the matrix. In this case: 3. Or 1
	if we write a default routine without threads. Till this point its not possible
	to calculate the matrix with a random number of threads.
	
	A better way is to set threads with specific slices.
	That means we create for example 2 threads for a 3x3 Matrix. There are 9 Elements,
	so each thread get a slice of 4 and 5 Elements to solve.
	Then again you can only create n threads on a nxn Matrix.
	But for that usecase we decide to take this solution.
	
	 
- normaler single thread algotighmus //name?
- Auslagerung der Funktion in Thread Methode
- Explizite verarbeitung von  Spalten in Threads
- Kreeirung von Struktur mit allen notwendigen Objekten
- problem mit dem join 
- !!! Zerlge Matrix in anzahl Threads Mengen und berechne jedes Feld  innerhalb des jeweiligen threads.
