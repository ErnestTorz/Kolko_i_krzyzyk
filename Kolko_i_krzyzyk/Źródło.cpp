#include <iostream>
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include <ctime>
#include <windows.h>
using namespace std;

#define KOMPUTER 1
#define GRACZ 2
#define MAXGLEMP 6 
#define ZNAKKOMPUTER 'O'
#define GRACZZNAK 'X'
char** plansza;
int WIN = 3;
int BOK = 6;

//Czesc graficzna textury ora pola do wyswietlania
sf::Texture _;
sf::Texture X;
sf::Texture O;
sf::Texture tytul;
sf::Texture kto;
sf::Texture TN;
sf::Texture KWIN;
sf::Texture REMIS;
sf::Texture GWIN;
sf::Texture Grasz;
sf::Texture WYMIAR;
sf::Texture ILE;
sf::Texture by;

sf::RectangleShape shape(sf::Vector2f(95.f, 95.f));
sf::RectangleShape Pytania(sf::Vector2f(95.f * 6, 95.f * 6 / 2));

//funkcja do wyswietlania planszy
void Wyswietl(sf::RenderWindow& Window)
{
	// window.clear();
	for (int i = 0; i < BOK; i++)
	{
		for (int a = 0; a < BOK; a++)
		{
			if (plansza[i][a] == 'X')
			{
				shape.setTexture(&X);
			}
			else if (plansza[i][a] == 'O')
			{
				shape.setTexture(&O);
			}
			else if (plansza[i][a] == ' ')
			{
				shape.setTexture(&_);
			}
			else
			{
				plansza[i][a] = ' ';
				shape.setTexture(&_);
			}

			shape.setPosition(a * 95.f, i * 95.f);

			Window.draw(shape);
		}
	}
	Window.display();
}

//funkcja czyszcz¹ca plansze
void zerowanie(char** plansza)
{

	for (int x = 0; x < BOK; x++)
	{
		for (int y = 0; y < BOK; y++)
			plansza[x][y] = ' ';
	}
}


// funkcja sprawdzajca czy jakis wiersza ma wystrczajaca ilosc znakow do wygranej
bool fullwiersz(char** plansza)
{
	bool flaga = true;

	for (int x = 0; x < BOK; x++)
	{
		for (int y = 0; y < BOK; y++)
			if (y + WIN <= BOK)
			{
				flaga = true;
				for (int b = 0; b < WIN;) {
					if (plansza[x][y] != plansza[x][(y + (b++))] || plansza[x][y] == ' ') {
						flaga = false;
					}

				}
				if (flaga == true) { return true; }
			}
	}



	return (false);
}
// funkcja sprawdza czy jakas kolumna ma wystarczajaca ilosc tych samych zanakow do wygranej
bool fullkolumna(char** plansza)
{
	bool flaga = true;

	for (int x = 0; x < BOK; x++)
	{
		for (int y = 0; y < BOK; y++)
			if (y + WIN <= BOK)
			{
				flaga = true;
				for (int b = 0; b < WIN;) {
					if (plansza[y][x] != plansza[(y + (b++))][x] || plansza[y][x] == ' ') {
						flaga = false;
					}

				}
				if (flaga == true) { return true; }
			}
	}



	return (false);
}

// funkcja sprawdzajaca czy jakis skos ma wystarczajaca ilosc tych samych znakow do wygranej
bool fullskos(char** plansza)
{
	bool flaga = true;
	for (int x = 0; x < BOK; x++)
	{
		for (int y = 0; y < BOK; y++)
		{
			if (y + WIN <= BOK && x + WIN <= BOK)
			{
				flaga = true;
				for (int b = 0, a = 0; b < WIN && a < WIN; a++, b++)
				{
					if (plansza[x][y] != plansza[(x + (b))][(y + (a))] || plansza[x][y] == ' ')
					{
						flaga = false;
					}

				}
				if (flaga == true) { return true; }
				flaga = true;
				for (int b = 0, a = WIN - 1; b < WIN && a >= 0; a--, b++)
				{
					if (plansza[x + WIN - 1][y] != plansza[(x + (a))][(y + (b))] || plansza[x + WIN - 1][y] == ' ')
					{
						flaga = false;
					}

				}
				if (flaga == true) { return true; }

			}

		}
	}



	return (false);
}

// funkcja sprawdzajaca czy plansza jest pelna i nie nastpila wygrana, jesli tak to nastepuje remis
bool remis(char** plansza)
{
	for (int x = 0; x < BOK; x++)
	{
		for (int y = 0; y < BOK; y++)
		{
			if (plansza[x][y] == ' ')
			{
				return (false);
			}
		}
	}
	if (fullwiersz(plansza) || fullkolumna(plansza) || fullskos(plansza))
	{
		return (false);
	}
	else
		return (true);
}

//funkcja sprawdzajaca czy jest koniec gry
bool gameOver(char** plansza)
{
	return (fullwiersz(plansza) || fullkolumna(plansza) || fullskos(plansza) || remis(plansza));
}

// MINMAX algorytm
int minimax(char** plansza, int glem, int alpha, int beta, bool toKomp)
{
	bool loop = true;
	int wynik = 0;
	int najWynik = -10000;
	if (gameOver(plansza) == true)
	{
		if (remis(plansza))
		{
			return +1;
		}
		else
		{
			if (toKomp == true)
				return ((-2 * (MAXGLEMP)) + glem); // Coraz to dalsza przegrana jest lepsza poniewa¿, zwiêksza szanse na pomy³kê gracza w kolejnych ruchach
			if (toKomp == false)
				return 2 + MAXGLEMP - glem; // Coraz dalsze wygrane s¹ gorzej punktowane, szybsza wygrana jest lepsza, skraca czas gry
		}
	}
	else
	{
		if (glem < MAXGLEMP)
		{
			if (toKomp == true)
			{
				najWynik = -10000;
				for (int x = 0; x < BOK && loop; x++)
				{
					for (int y = 0; y < BOK && loop; y++)
					{
						if (plansza[x][y] == ' ')
						{
							plansza[x][y] = ZNAKKOMPUTER;
							wynik = minimax(plansza, glem + 1, alpha, beta, false);
							plansza[x][y] = ' ';
							if (wynik > najWynik)
							{
								najWynik = wynik;
							}
							if (wynik > alpha) { alpha = wynik; }
							if (beta <= alpha) { loop = false; }
						}
					}
				}
				return najWynik;
			}
			else
			{
				najWynik = 10000;
				for (int x = 0; x < BOK && loop; x++)
				{
					for (int y = 0; y < BOK && loop; y++)
					{
						if (plansza[x][y] == ' ')
						{
							plansza[x][y] = GRACZZNAK;
							wynik = minimax(plansza, glem + 1, alpha, beta, true);
							plansza[x][y] = ' ';
							if (wynik < najWynik)
							{
								najWynik = wynik;
							}
							if (wynik < beta) { beta = wynik; }
							if (beta <= alpha) { loop = false; }
						}
					}
				}
				return najWynik;
			}
		}
		else
		{
			return 0;
		}
	}
}
// funkcja sprawdzjaca wszystkie mozliwe ruchy i decydujaca ktory jest najbardziej korzystny
int najRuch(char** plansza, int indeksRuch)
{
	int x = -1, y = -1;
	int wynik = -5, najWynik = -10000;
	for (int i = 0; i < BOK; i++)
	{
		for (int j = 0; j < BOK; j++)
		{
			if (plansza[i][j] == ' ')
			{
				plansza[i][j] = ZNAKKOMPUTER;
				wynik = minimax(plansza, indeksRuch, -100000, 100000, false);
				plansza[i][j] = ' ';
				if (wynik > najWynik)
				{
					najWynik = wynik;
					x = i;
					y = j;
				}
			}
		}
	}

	if (najWynik < 0)
	{
		cout << endl
			<< "Komputer prawdopodobnie przegra ";
	}
	else if (najWynik >= 2)
	{
		cout << endl
			<< "Komputer wygra ";
	}
	else if (najWynik == 1)
	{
		cout << endl
			<< "Komputer prawdopodobnie zremisuje";
	}
	else
	{
		cout << endl;
	}
	return x * BOK + y;
}

// funkcja do uruchomienia gry
void Kolko_i_Krzyzyk(int KogoKolej, sf::RenderWindow& Window)
{

	int x = 0, y = 0;

	zerowanie(plansza);


	while (gameOver(plansza) == false)
	{
		int n;
		if (KogoKolej == KOMPUTER)
		{
			n = najRuch(plansza, 0);
			x = n / BOK;
			y = n % BOK;
			plansza[x][y] = ZNAKKOMPUTER;

			cout << endl
				<< "Komputer postawil " << ZNAKKOMPUTER << ", w polu nr." << n + 1 << endl
				<< endl;
			Wyswietl(Window);
			KogoKolej = GRACZ;
		}

		else if (KogoKolej == GRACZ)
		{

			int flaga;
			flaga = 1;
			Sleep(1000);
			while (flaga == 1)
			{
				sf::Event even;
				while (Window.pollEvent(even))
				{
					if (even.type == sf::Event::Closed)
						Window.close();
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2i mousePos = sf::Mouse::getPosition(Window);
						for (int y = 1; y <= BOK; y++)
						{
							for (int x = 1; x <= BOK; x++)
							{
								if ((mousePos.x <= 95.f * x) && (mousePos.x >= 95.f * (x - 1)) && (mousePos.y <= 95.f * y) && (mousePos.y >= 95.f * (y - 1)))
								{
									if (plansza[y - 1][x - 1] == ' ')
									{
										plansza[y - 1][x - 1] = GRACZZNAK;
										flaga = 0;

									}
								}
							}
						}
					}
				}
			}
			Wyswietl(Window);
			KogoKolej = KOMPUTER;
		}
	}
	//Remis
	if (remis(plansza))
	{

		Wyswietl(Window);
		Sleep(1000);
		Window.clear(sf::Color(255, 255, 255));
		Pytania.setTexture(&REMIS);
		Pytania.setPosition(0, (95.f * BOK / 2) - 95.f * BOK / 4);
		Window.draw(Pytania);
		Window.display();
		Sleep(3000);
	}
	else
	{
		// sprawdzanie kto jest zwyciezca
		if (KogoKolej == KOMPUTER)
			KogoKolej = GRACZ;
		else if (KogoKolej == GRACZ)
			KogoKolej = KOMPUTER;

		Wyswietl(Window);
		Sleep(1000);
		// wyswietlenie odpowiedniego zwyciezcy
		if (KogoKolej == KOMPUTER)
		{

			Window.clear(sf::Color(255, 255, 255));
			Pytania.setTexture(&KWIN);
			Pytania.setPosition(0, (95.f * BOK / 2) - 95.f * BOK / 4);
			Window.draw(Pytania);
			Window.display();
			Sleep(3000);
		}
		else
		{
			Window.clear(sf::Color(255, 255, 255));
			Pytania.setTexture(&GWIN);
			Pytania.setPosition(0, (95.f * BOK / 2) - 95.f * BOK / 4);
			Window.draw(Pytania);
			Window.display();
			Sleep(3000);
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(95.f * 6, 95.f * 6), "Kolko i krzyzyk", sf::Style::Titlebar | sf::Style::Close);
	int flaga = 1;
	int pola = 0;
	int win = 0;


	// za³adowanie teksturek
	tytul.loadFromFile("images/TYTUL.png");
	kto.loadFromFile("images/KTO.png");
	TN.loadFromFile("images/TN.png");
	_.loadFromFile("images/_.png");
	X.loadFromFile("images/X.png");
	O.loadFromFile("images/O.png");
	KWIN.loadFromFile("images/KWIN.png");
	Grasz.loadFromFile("images/Graszdalej.png");
	REMIS.loadFromFile("images/REMIS.png");
	GWIN.loadFromFile("images/GWIN.png");
	WYMIAR.loadFromFile("images/W.png");
	ILE.loadFromFile("images/IZ.png");
	by.loadFromFile("images/by.png");

	while (window.isOpen())
	{
		// wyswietlenie intro
		window.clear(sf::Color(255, 255, 255));
		Pytania.setTexture(&tytul);
		Pytania.setPosition(0, 11);
		window.draw(Pytania);
		Pytania.setTexture(&by);
		Pytania.setPosition(0, 95.f * BOK / 2);
		window.draw(Pytania);
		window.display();

		Sleep(2000);



		// wyswietlenie pytania o wymiar
		window.clear(sf::Color(255, 255, 255));
		Pytania.setTexture(&WYMIAR);
		Pytania.setPosition(0, (95.f * BOK / 2) - 95.f * BOK / 4);
		window.draw(Pytania);
		window.display();



		// odczytanie wymiarów od u¿ytkownika
		while (flaga == 1)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();






				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode < 128)
					{



						if (pola != 0)
						{
							if (isdigit(static_cast<char>(event.text.unicode)) && static_cast<char>(event.text.unicode) != '0' && (static_cast<char>(event.text.unicode) != '1') && (static_cast<char>(event.text.unicode) != '2') && (pola >= static_cast<char>(event.text.unicode) - '0'))
							{

								win = static_cast<char>(event.text.unicode) - '0';
								flaga = 0;

							}
						}
						if (isdigit(static_cast<char>(event.text.unicode)) && (static_cast<char>(event.text.unicode) != '0') && (static_cast<char>(event.text.unicode) != '1') && (static_cast<char>(event.text.unicode) != '2') && pola == 0)
						{
							pola = static_cast<char>(event.text.unicode) - '0';

							window.clear(sf::Color(255, 255, 255));
							Pytania.setTexture(&ILE);
							Pytania.setPosition(0, (95.f * BOK / 2) - 95.f * BOK / 4);
							window.draw(Pytania);
							window.display();
						}

					}
				}


			}

		}
		BOK = pola; // przypisanie danych
		WIN = win;
		window.close();
	}
	//otwarcie nowego okna
	sf::RenderWindow Window(sf::VideoMode(95.f * BOK, 95.f * BOK), "Kolko i krzyzyk", sf::Style::Titlebar | sf::Style::Close);
	while (Window.isOpen()) {
		sf::Event even;
		while (Window.pollEvent(even))
		{
			if (even.type == sf::Event::Closed)
				Window.close();

			Pytania.setSize(sf::Vector2f(95.f * BOK, 95.f * BOK / 2));

			plansza = new char* [BOK]; // utworzenie planszy
			for (int i = 0; i < BOK; ++i)
				plansza[i] = new char[BOK];

			Window.clear(sf::Color(255, 255, 255)); //Wyswietlenie pytania kto zaczyna
			Pytania.setTexture(&kto);
			Pytania.setPosition(0, 11);
			Window.draw(Pytania);
			Pytania.setTexture(&TN);
			Pytania.setPosition(0, 95.f * BOK / 2);
			Window.draw(Pytania);
			Window.display();

		}
		char wybor = 'x';
		flaga = 1;
		// odczytanie odpowiedzi gracza
		while (flaga == 1)
		{
			while (Window.pollEvent(even))
			{
				if (even.type == sf::Event::Closed)
					Window.close();

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(Window);

					if ((mousePos.x <= (95.f * BOK) / 2) && (mousePos.x >= 0) && (mousePos.y <= 95.f * BOK) && (mousePos.y >= 95.f * BOK / 2))
					{

						wybor = 'T';
						flaga = 0;
					}

					if ((mousePos.x >= (95.f * BOK) / 2) && (mousePos.x <= 95.f * BOK) && (mousePos.y <= 95.f * BOK) && (mousePos.y >= 95.f * BOK / 2))
					{

						wybor = 'N';
						flaga = 0;
					}
				}

			}
		}
		// zerowanie i wyswietlanie planszy
		zerowanie(plansza);
		Wyswietl(Window);
		// Rozpoczêcie gry z zale¿noœci od koleji
		if (wybor == 'N')
		{
			// system("cls");
			Kolko_i_Krzyzyk(KOMPUTER, Window);
		}
		else if (wybor == 'T')
		{
			// system("cls");
			Sleep(1000);
			Kolko_i_Krzyzyk(GRACZ, Window);
		}
		// Zadanie pytania o nastêpn¹ partiê
		Window.clear(sf::Color(255, 255, 255));
		Pytania.setTexture(&Grasz);
		Pytania.setPosition(0, 11);
		Window.draw(Pytania);
		Pytania.setTexture(&TN);
		Pytania.setPosition(0, 95.f * BOK / 2);
		Window.draw(Pytania);
		Window.display();
		wybor = 'x';
		flaga = 1;
		//odczytanie odpowiedzi
		while (flaga == 1)
		{
			while (Window.pollEvent(even))
			{
				if (even.type == sf::Event::Closed)
					Window.close();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					sf::Vector2i mousePos = sf::Mouse::getPosition(Window);

					if ((mousePos.x <= (95.f * BOK) / 2) && (mousePos.x >= 0) && (mousePos.y <= 95.f * BOK) && (mousePos.y >= 95.f * BOK / 2))
					{
						flaga = 0;
					}

					if ((mousePos.x >= (95.f * BOK) / 2) && (mousePos.x <= 95.f * BOK) && (mousePos.y <= 95.f * BOK) && (mousePos.y >= 95.f * BOK / 2))
					{
						Window.close(); // zakañcza dzia³anie programu poprzez zamkniêcie okna
						flaga = 0;
					}
				}

			}
		}
		Sleep(700);
	}

	return (0);
}