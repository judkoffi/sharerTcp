<strong>Sharer</strong><br>
Cette application permet d'envoyer des fichiers entre deux machines dans le reseau avec le protocole TCP.<br>
Celui qui veut recevoir ce mets en mode serveur avec l'option "-s", et celui qui veut envoyer le fichier en mode client avec l'option "-c".<br>
un exemple de mode serveur: <br>
&nbsp;<em>./share -s 9000 received.dat</em><br>
Un exemple de mode client: <br>
&nbsp;<em>./share -c 172.16.1.1 9000 send.dat</em><br>
<strong>NB:</strong><em> Comme c'est du C, pensez à recompiler :)</em>
