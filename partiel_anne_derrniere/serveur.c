//programme du client
int main()
{
//TODO
	struct requete_client_serveur {
	pid_t client_pid; //PID du Client
	char instrument[5]; // code l instrument
	char variable[5]; // nom de la var mesuree
	char value[4]; //valeur de la var mesuree
	};
	char path,reponse;
	strcpy(path,"/temp/serveur_fifo")
	int serveur,client;

//creation du tube nomme pour ce process
	mkfifo(path);
while(1)
{
//TODO
//lecture d une requete sur le tube du serveur
	client = fopen(path,"r");
	fread(reponse,	sizeof(struct requete_client_serveur),1,client);
//TODO
// utilisation possible d un fork pour traiter la requete
	popen()
//TODO
//Preparez la requete Shell pour sauvegarder dans la BDD
//TODO
//Tube process pour la sauvegarde
//TODO
//Tube process pour la sauvegarde avec lecture du resultat
//TODO
//Envoyer le resultat de l ACK au client sur son tube d ecoute
}
}