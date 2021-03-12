***NUME Mandru, PRENUME Cosmina***
****GRUPA 313CB, SERIA CB**********
 
							Readme Tema 3

		Tratarea cazurilor de eroare: In toate functiile ce au trebuit
	implementate au existat si situatii in care nu erau indeplinite conditiile
	de realizare a unei comenzi. Astfel am creat functii auxiliare ce intorceau
	NULL daca un nod din cuvantul cautat nu exista sau chiar nodul daca acesta
	era prezent in arbore. 
		De asemeni, in toate functiile pentru comenzi au existat cazurile in 
	care ca parametru era dat numele directorului cu sau fara "/". In functie
	de acest aspect se crea cuvantul ce apoi era trimis spre inserare.
		Am creat un pointer in care retineam mereu numele directorului curent.
		Comenzile mkdir si touch au fost implementate in aceasi functie 
	mkdir_touch, diferenta consta in faptul ca atunci cand se insrera un fisier
	variabila file avea valoarea 1 pentru a seta campul istouch din fiecare nod 
	cu true. Sunt tratate cazurile speciale, erorile, iar pentru inserarea 
	propriu zisa este apelata functia de Inserare, Ins_Arb. Aceasta cuprinde 2
	cazuri unul atunci cand arborele este gol si celalalt in caz contrar. In 
	prima situatie este creat nodul radacina cu "/" si apoi sunt inserate pe 
	rand caracterele din numele fisierului(mai exact toata calea) setandu-se
	si campurile din nod in functie de crietrii. In cel de al doilea caz 
	fiecare litera este cautat in arbore si doar in cazul in care nu a fost
	gasit este creat iar restul literelor sunt inserate in "josul" acestuia.
		Comanda cd este implemenata in 2 functii: una se ocupa cu cd - ul de
	"mers innapoi" iar una pentru inaintarea in sistemul de fisiere. Practic
	aceasta functie manipuleaza un sir de caractere, de arbore folosindu - se
	doar ca sa verifice existenta folderului in care se doreste inaintarea si 
	daca acesta este intr-adevar folder. Functia cd_back eliminaul cuvantul de
	dupa ultimul "/". La sfarsit variabila curen_directory este actualizata.
		Comenzile mv si cp sunt asemantoare, diferenta consta in faptul ca la
	mv ce a fost mutat trebuie sters. Sunt tratate cazurile de neexistenta a 
	folderelor si sau fisierelor cu ajutorul functiilor auxiliare(search, 
	exist_node, find_node) comentate in cod. Daca comenzile se pot efectua
	numele fisierului dat care a fost recreat in functie daca incepea sau nu 
	cu "/" este reinserat in arbore si sters in cazul functie mv.
		Comenzile rm si rmdir, (asemanatoare si acestea) sterg literele din 
	din arbore ce fac parte din numele dat ca parametru dar care nu intra in
	alcatuirea altor cuvinte. Sun tratate, ca si la celelalte comenzi, cazurile
	de neexistenta sau cele in care folderele nu sun goale.
		Comanda pwd afiseaza variabila curent_directoru.
		Comanda ls este implementata cu ajutorul unor functii auxiliare.
	Initial sunt tratate cazurile exceptate. Se cauta nodul de la care se 
	incepe afisarea. Functia DFS_all_branches_ls returneaza vectorul ce 
	contine numele fisierelor sau folderelor ce vor fi afisate. Aceasta
	functie se foloseste de functia recursiva ce parcurge toate subramurile
	ramurilor principale ale nodului radacina.