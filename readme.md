# SK - Projekt zaliczeniowy
## Podjęty temat - Komunikator internetowy typu GG

Należy sklonować repozytorium i skompilować za pomocą cmake'a następnie uruchomić.
server otwiera się na porcie 9090.

### Klient - ts+react
Klient napisany w technologii `javascript` rozszerzony o `typescript` przy użyciu framework'u `react` wraz z kilkoma paczkami wspierającymi jak `lodash`, `axios`, `vite`. 
Klienci na porcie 3000+.
Klient otwiera łączę WebSocket o nazwie `server` zapewniając kontekts danych przetrzymywanych przez serwer.
Klient otwiera łączę WebSocket o własnej nazwie zapewniając kontekst wysyłania wiadomości do odiborców z podanej, dynamicznie odświeżanej, listy użytkowników.
Klient podczas otwierania połączenia typu websocket wysyła żądanie w protokole `http` proszące serwer o potwierdzenia zajścia połączenia przy wykorzystaniu protokołu typu `handshake`. Serwer na prośbę klienta zwraca wymuszenie 'ulepszenia' protokołu komunikacji na `websocket`, od tej chwili jest otwarte połączeni dwukierunkowe. 

### Serwer - c w standardzie C23
Serwer napisany w technologii `c` w standardzie `c23` przy wykorzystaniu zewnętrznych bibliotek `pthread` do obsługi wątków oraz `OpenSSL` do kryptograficznych funkcji niezbędnych do obłsugi protokołu przesyłu danych `websocket`.
Serwer prowadzi na wydzielonym wątku ciągłe oczekiwanie na wydarzenia oparte na mechanizmie monitorującym epoll.
Serwer prowadzi na oddzielnym wątku statystyki dotyczące aktualnych użytkowników oraz liczbę wywołań poszczególnych wydarzeń (hangup/error/write/read), liczbę połączeń aktualnych jak i przez całość działania serwera.
Serwer prowadzi ciągłe oczekiwanie na zakończenie pracy wątka epoll by bezpiecznie zamknąć wątek statystyki i obsłużyć bezpieczne wyjście programu.


###### Autorzy:
 - Daniel Zdancewicz
 - Monika Zielińska 
