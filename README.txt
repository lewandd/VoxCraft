-------------------------------------------------------------------------------------------------------------
Praca inżynierska
-------------------------------------------------------------------------------------------------------------

Praca inżynierska autorstwa Damiana Lewandowskiego - gra z proceduralnie generowanym otwartym światem. Świat składa się z modyfikowalnych wokseli.
Gra napisana jest w C++ z użyciem OpenGL'a.

-------------------------------------------------------------------------------------------------------------
Instrukcja konfiguracji projektu:
-------------------------------------------------------------------------------------------------------------

Do uruchomienia pojektu wymagane jest środowisko Visual Studio.
Pliki potrzebne do działania API OpenGL'a znajdują się w configuration_files.zip.

Wymagany plik:
 configuration_files.zip (zawierający folder configuration_files)

configuration_files
 |- Include (folder z plikami źródłowymi)
 |- Libs (folder z bibliotekami)
 |- glad.c (plik do dodania do projektu)

Instrukcja:

1. Rozpakuj configuration_files.zip
2. Otwórz projekt w Visual Studio
3. Konfiguracja (ustawienie ścieżek do configuration_files)
	3.1 Projekt > Właściwości
	3.2 Katalogi VC++ > Katalogi plików nagłówkowych > dodaj ścieżkę 
		.../configuration_files/Include
	3.3 Katalogi VC++ > Katalogi bibliotek > dodaj ścieżkę
		.../configuration_files/Libs
4. Konfiguracja (dodanie plików)
	4.1 Projekt > Właściwości	
	4.2 Konsolidator > Dane wejściowe > Dodatkowe zależności > dodaj pliki
		 opengl32.lib glfw3.lib
	4.3 Skopiuj plik glad.c do folderu projektu
	4.4 Dodaj glad.c do projektu
		4.4.1 Prawy przycisk myszy na nazwę projektu w panelu po prawej stronie
		4.4.2 Dodaj > Istniejący element... > glad.c
