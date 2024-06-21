Plik README.md powstal juz po oddaniu projektu.


Projekt ma celu zaproponowanie użytkownikiowi, by zakupił sobie jakieś smakołyki,
jeśli prognoza pogody wskazuje, że w najbliżsych dniach będzie 
gorsza pogoda.

Program pobiera dane ze strony openwaethermap.org.

Dane są to pobierane do 5 dni w przyszłość ze stałym 3-godzinowym odstępem
(zaczynając od północy).

Najwcześniejzą pogdoą pokazywaną przez program, jest ta prognoza z przszłości,
ktora była najbliżej chwili obecnej. Najpóżźniejsza zaś ta, co jest
o dokłądnie 5 dni późniejsza.

Za gorszą pogodę uznaje się deszcz i burzę zgodnie z ozanczennaimi w
openweather.org.

Program dokładnie wylicza liczbę takich dni.
Za dzień z gorszą pogdą uznaje się dzień, w którym wystąpi conajmiej raz gorsza pogoda.
Nie uwzględniane są jednak godziny nocne
(0:00 oraz 3:00, są to jedyne godziy nocne dla których są dostępne dane).

Program aktualizuje pogodę co 5 minut.

Program podzielony na wątki. Co ma zapenić jego sprawne działanie.

Najpierw program pobiera pogodę i otwiera następni graficzny interfejs.

Prognoza pogody jest pokazywana po lewej stonie ekarnu.
Żeby pokazać prognozę pogody na pożniejszy czas. Należy użyć paska scrollbar.

Na środku ekranu pokazywana jest liczba dni, w których wystąpi gorsza pogoda.

W dolym prawym rogu jest przycisk 'close', który zamyka ekran.

Po lewj lest ikonka symbolizująca ustawienia. Po kliknięciu na nią Otwiera się nowe okno.

Na razie jednak jedyną opcją w nim jest zamnięcie tego okienka przyciskiem
w prawym donlym rogu z napisem 'close'.


