Plik README.md powstal juz po oddaniu projektu.


Projekt ma celu zaproponowanie użytkownikiowi, by zakupił sobie jakieś smakołyki,
jeśli prognoza pogody wskazuje, że w najbliżsych dniach będzie 
gorsza pogoda.

Program pobiera dane ze strony openwaethermap.org.

Dane są to pobierane do niewiecej 5 dni w przyszłość 
(ale użytkownik może zmniejszyć tą wartość w ustawieniach)
ze stałym 3-godzinowym odstępem
(zaczynając od północy).

Najwcześniejzą pogdoą pokazywaną przez program, jest ta prognoza z przszłości,
ktora była najbliżej chwili obecnej. Najpóźniejsza zaś ta, co jest
o dokłądnie 5 dni późniejsza.

Za gorszą pogodę uznaje się deszcz i burzę zgodnie z ozanczennaimi w
openweather.org.

Program dokładnie wylicza liczbę takich dni.
Za dzień z gorszą pogdą uznaje się dzień, w którym wystąpi conajmiej raz gorsza pogoda.
Nie uwzględniane są jednak godziny nocne
(0:00 oraz 3:00, są to jedyne godziy nocne dla których są dostępne dane).

Program aktualizuje pogodę co 5 minut.

Program podzielony na wątki. Co ma zapenić jego sprawne działanie.

Najpierw program otwiera następni graficzny interfejs.
Po czym dzieli program na 2 wątki. Jeden odpowiada za interferjs graficzny,
a drugi aktualizuje pogodę.

Prognoza pogody jest pokazywana po lewej stonie ekarnu.
Żeby pokazać prognozę pogody na pożniejszy czas. Należy użyć paska scrollbar.

Na środku ekranu pokazywana jest liczba dni, w których wystąpi gorsza pogoda.

Poniżej jest pole do wpisywania nowych produktów. Dodanie nowego
produktu zapisuje sie klikajac przycisk safe.
Prczycisk clear wymazuje aktualnie napisany przez uzytkownika
tekst, który jeszcze nie został zatwierdzony.

Na prawo od niego znajduje się lista wszystkich produktów.
Po prawej stronie każego z produktów znajduje się czerwony krzyżyk, 
po jego wciśnięciu produkt jest całkowiczie usuwany z listy.
Można również kliknąć po prawej od czerwonego krzyżyka na checkbox. 

Lista jest zawsze posortowana. Na górzez są te z zaznczonym checkboxem,
drugim prioryttem sortowania jest kolejność ortograficzna.

W dolym prawym rogu jest przycisk 'close', który zamyka ekran.

Po lewj lest ikonka symbolizująca ustawienia. Po kliknięciu na nią Otwiera się nowe okno.

Widać na nim możliwoć ustawienia liczby dni. Można ustawić dowolną watość 
od 1 do 5. Przycisk save zatwierdza. 
Podanie innej wartości wyświetli komunikat o błędzie.

Poniżej jest możliwość podanaia lokalizacji. Przycisk save zatwierdza.
Jeśli podanej lokalizacji nie ma na stronie openweather.org, 
to wyświetlany jest komunikat o błędzie.

Zmaina liczby dni lub lokalizacji powoduje natychmiastowe zaktualizowanie się pogody.

Przyciskiem w prawym donlym rogu z napisem 'close' zamyka się
okienko z ustawieniami.


