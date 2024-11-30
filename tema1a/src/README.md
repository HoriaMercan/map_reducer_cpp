# Tema 1 Algoritmi Paraleli si Distribuiti - MapReduce

## Horia Mercan - grupa 333CA

In cadrul acestui README voi detalia implementarea realizata din cadrul temei 1
la APD, Map Reduce. 


## Structura generala a implementarii

Pornim M threaduri care vor executa functia <b><em>mapper_run</em></b>
si R threaduri care vor executa functia <b><em>reducer_run</em></b>. Pentru
sincronizarea acestor etape vom folosi o bariera, astfel incat etapa de
reduce sa inceapa dupa ce toti map workers au terminat de analizat fisierele.
Tuturor workerilor le este pasata o resursa partajata, care este exemplificata
in clasa SharedResources si initializata in main.


### Resursele partajate pentru map & reduce workers

Clasa SharedResources inglobeaza toate resursele partajate ce sunt folosite
de map / reduce workers. Printre acestea se numara:
* <em>bariera</em> ce este folosita pentru 
        sincronizarea operatiilor map/reduce
* <em>o coada</em> sincronizata cu un <em>mutex</em> din care map workers
    extrag pe rand numele fisierelor ce trebuie analizate (sub forma de
    structuri de forma queue_element_t).
* <em>containere partajate</em> (incluse in obiectul de forma SharedMapContainers). Un container reprezinta o lista de perechi
{cuvant, ID_fisier} unde toate cuvintele incep cu aceeasi litera
a alfabetului englez. Obiectul SharedMapContainers contine
containere pentru fiecare litera, si posibilitatea de a sincroniza
operatia de a adauga astfel de perechi per litera. 

Organizarea taskurilor pentru map workers se face sub forma unei cozi, din
care workerii preiau numele fisierelor in mod dinamic.

Organizarea taskurilor pentru reduce workers se face sub forma unei cozi,
din care workerii preiau o litera mica a alfabetului englezesc iar apoi
agrega cuvintele din containerul specific literei.


### Organizarea operatiei de Map

Functia `void *mapper_run(void *arg)` reprezinta o structura generala
a unui thread ce se ocupa cu operatii de Map din cadrul sistemului.
Extragerea urmatorului task se face cu ajutorul functiei GetTask().
Aceasta returneaza un element de tipul `optional<queue_element_t>`
(am folosit o implementare self-made a echivalentului clasei std::optional
pentru ca am aflat destul de tarziu ca dockerul este configurat sa 
foloseasca c++11). Logica analizarii unui task are loc in functia
`void map_loop(SharedResources *, std::string filename, unsigned id)`
in care citim cuvintele din fisier, cream perechile {cuvant, ID_fisier}
iar la finalul executiei taskului curent adaugam containerele stocate
local threadului in memoria partajata.


### Organizarea operatiei de Reduce
Logica generala a unui thread ce se va ocupa cu operatii de tip Reduce
este descrisa in functia `void *reducer_run(void *)`. In aceasta functie
se preiau pe rand containerele ce contin toate perechile {cuvant, ID_fisier}
ce trebuie agregate, sortate si apoi scrise in fisier. Operatiile din urma
sunt descrise in functia `void reducer_loop(Container_t &, int)`.

