//code de l instrument est donnee par une var externe
extern char instrument[5];
//nom de la variable mesuree
extern char varName[5];

struct requete_client_serveur {
pid_t client_pid; //PID du Client
char instrument[5]; // code l instrument
char variable[5]; // nom de la var mesuree
char value[4]; //valeur de la var mesuree
};


//programme du client
int main()
{
int serveur,client;
char path, requeste;

pid_t pid = getpid();
sprintf(path,"/temp/client/client_%s_fifo",pid);
mkfifo(path);
//creation du tube nomme pour ce process
//TODO
while(1)
{
sleep(10*60);//dormir 10 minutes
//recuperer la valeur de l instrument par une fonction donnee
int value;
getValue(varName, &value);//fonction existante pour faire mesure
//TODO
//Preparation du message pour le serveur
struct requete_client_serveur requete;
requete->client_pid = pid;
requete->value = value;
requete->variable = varName;
requete->instrument = instrument;
//TODO
// ouvrir le tube du serveur
serveur = fopen("/temp/serveur_fifo", "w");
// envoyer le message au serveur
fwrite(requete,sizeof(struct requete_client_serveur),1,serveur);
// fermer le tube du serveur
fclose(serveur);
//TODO
//lire l ACK du serveur sur son tube de lecture
client = fopen(path, "r");
fread(requeste, 80, 1, client)
//TODO
// Si le message est OK attendre 10 min et passer a la prochaine mesure// Sinon attendre 10 minutes avant de renvoyer le mesure et repeter cela
//jusqu a avoir un ACK
if(strcmp(requeste,"OK") != 0) {
	exit(1);
}
}
}