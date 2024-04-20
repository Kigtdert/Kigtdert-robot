# Kigtdert-robot
ESP32/arduino projekt robota. //////
Plastové části jsou navržené ve Fusion 360 a vytištěné na 3D tiskárně. Gumové pásy se otáčí na dvou kolech každý. Každé kolo má ložiska z obou stran, což zajišťuje plynulý chod bez zadrhávání. Celá konstrukce je v celku robustní, nejslabší místo je v uchycení věže. Ta drží jen na samotném servu. Fyzický vypínač se nachází na zadní straně spodní části robota. na vrchní části se nachází krytka konektoru napájení a OLED display s kapacitní tlačítkem, schovaným pod tenkou vrstvou plastu. na přední straně se nachází indikační dioda. Zeleně svítí pokud je robot zapnutý, pokud problikává rychle červená, je vybitá baterie, pokud pomaleji, některá z kontrolovaných součástek se přehřívá a je třeba počkat, nejlépe robota vypnout. Na levém boku je schovaný DHT11 senzor teploty a vlhkosti.
Vpravo jsou čtyři konektory, sloužící nahrání programu do arduina bez nutnosti rozdělávat tělo robota. Ke stejnému účelu, akorát pro ESP32 slouží 5 konektorů na pravé straně věže. na stejné straně je i modul laseru. Nalevo je WIFI anténa ESP32. Zepředu je kamera ESP32 a bílá LED pro osvětlení.

Custom PCB je navrženo v KiCadu. Je osazeno dutinkovymi lištami a JST XH konektory pro nazení komponent. Přímo je připájené jen piezo, Step down(připojené díry), potřebné rezistory a termistory(počítá se s jejich odstraněním). Baterie je s PCB spojená přes vypínač a dvoupinový JST XH konektor, nabíjí se pomocí konektoru pod krytkou na vrchní straně robota. Dva čtyřpinové konektory, každý na jiné straně PCB, slouží k připojení motorů. Tří pinový konektor vpravo dole slouží pro 433Mhz vysílač. Čtyřpinový konektor ve střední části PCB je pro připojení komunikace a napájení věže. Vedlejší třípinový konektor je pro DHT11. V levé dolní části je jeden pětipinový konektor pro OLED a kapacitní tlačítko a jeden šestipinový pro vyvedený bootovací systém a indikační LED. Ovládání H bridge se připojí pomocí jumper kabelů, napájení a motory dostatečně silným drátem. Je pro něj vyhrazené místo, ale nebude k PCB přidělaný napevno, maximálně oboustrannou lepící páskou. Baterie je dvoučlánková 18650 s napětím 7,4V. Kapacita té konkrétní, kterou momentálně používám je 1800mAh. Maximální vybíjecí  proud by měl být do 4A. Step down XL4005 převádí 7,4V na 5V. V ESP32 převádí interní převodník 5V na 3,3V. Spotřeba celé vrchní části se pohybuje od 0,4A do 0,65A při 5V. Ve spodní části tvoří drtivou většinu spotřeby motory. Spotřeba arduina, OLED displaye a ostatní součástek je téměř zanedbatelná. Maximálně 0,4A. Každý motor může dosáhnout spotřeby až 1,2A (stall). Při normálních podmínkách je to okolo 0,6A na 1 motor. Takže asi 2,5A celkově. Takže spotřeba při využívání všech částí je asi 3,5-4A při 5V. Přes step down se to dostane asi na 3A odebírané z baterie. Výdrž při těchto podmínkách je lehce přes půl hodiny. Do budoucna se počítá s vlastní baterií, která bude mít kapacitu větší a proudlouží tak výdrž.

První kód je pro arduino nano ve spodní části robota, druhý je pro ESP32 CAM, které je umístěné ve věži. Jsou mezi sebou propojeny pomocí sériové linky skrz převodník logických úrovní. Na ESP32 se spustí webserver, pomocí něhož se ovládají funkce robota, je tam vysílán obraz z kamery ESP32 a jsou na něm zobrazovány hodnoty ze senzoru DHT11 a napětí baterie. Spustí se ihned po zapnutí.
Po startu dvakrát pípne piezo a zabliká bílá LEDka na ESP32. Také se rozsvítí zelená indikační LED. Samotné ESP32 ovládá jen laser, servomotor SG90 otáčející věží a vestavěnou bílou LEDku. Zbytek příkazů odesílá do arduina které je zpracuje a na jejich základě podnikne akce, např pohyb dopředu. Arduino ovládá H bridge L298 s čtyřmi motory GA12-N20, OLED display SSD1306, kapacitní tlačítko pro přepínání zobrazení displaye, DHT11 senzor teploty a vlhkosti, 433Mhz vysílač s anténou, piezo, zjištuje stav napětí baterie (dokáže odhadnout když je vybitá a dát to audiovizuálně najevo), indikační 3mm LED a 3 termistory pro zjišťování teploty součástek. To má zabránit přehřátí H-bridge, step downu a baterie. Později se počítá s jejich odstraněním, pokud nebudou žádné přehřívací problémy nebo přesunutím na jiné součástky.

*bootovani Arduina
Musí se vytáhnout RST propojka hned po doběhnutí progressbaru

Ceny součástek:

Arduino nano 
168kč // 68kc

esp32 cam
348kč  //160kc

custom PCB
50kč

sg90 servo
62kč //24kc

laser
12kč //8kc

anténa+kabel
98+38 kč  //65+23kc

step down XL4005
48kč  //22kc

H bridge l298
58kč  //43kc

piezo
4-6kč

logický převodník
16kč //4kc

vypínač
8kč

baterie
+-500kč  //200kč

LEDky
2kč

rezistory
3.20kč

konektory 
5.10kč

dutinkové lišty
13kč

kabely
81kč  //50kč

DHT11
55kč //28kc

OLED SSD1306
98kč  //34kč

modul pro 433Mhz komunikaci + přijímač
98kč  //45kc

kapacitní tlačítko
6kč

motory
420kč //165kc


ložiska
+-200kč  //50kč

3D tištěné díly(hmotnost+odhad)
300g - 100kč materiál + 100kč energie

pásy(odhad)
150kč

CELKEM: 2671,7kč bez dopravy (většina z laskakitu)
ALIRXPRESS: 1240kč + 200kč tisknuté díly

