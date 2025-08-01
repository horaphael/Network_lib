/*
** EPITECH PROJECT, 2025
** network_lib
** File description:
** net
*/

#ifndef NET_H_
    #define NET_H_

    #define MAX_CLIENTS 128
    #define BUFFER_SIZE 1024

    #include <unistd.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <errno.h>
    #include <poll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>



            /* ================== STRUCT ================== */

/**
 * @brief Représente un client réseau connecté au serveur.
 */
typedef struct net_client_s {
    int fd;                         // Descripteur de socket du client
    bool active;                    // Indique si le client est actif
    char *buffer;       // Tampon de réception des données
} net_client_t;

/**
 * @brief Formatted packet to send from server to client.
 */
typedef struct msg_packet_s {
    void *message;              //message the user want to send
    size_t message_size;        //the size of the message
} msg_packet_t;

/**
 * @brief Contient les informations et états du serveur.
 */
typedef struct net_server_s {
    int listen_fd; // Socket d'écoute
    size_t buffer_size; // Size for client's buffer
    unsigned int port; // Port TCP utilisé par le serveur
    struct pollfd pfds[MAX_CLIENTS]; // Tableau de pollfd pour la surveillance
    net_client_t clients[MAX_CLIENTS]; // Tableau des clients connectés
    bool running; // Indique si le serveur est en cours d’exécution
    void (*on_data)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle incomming data = Null by default
    void (*on_disconnect)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle disconnection of clients = Null by default
    void (*on_connect)(net_client_t *client, struct net_server_s *server, void *args); // Function to handle connection of clients = Null by default
    void *data_args;
    void *data_connection;
    void *data_disconnection;
} net_server_t;


            /* ================== CORE ================== */

/**
 * @brief Crée une instance de serveur réseau.
 *
 * @param port Le port TCP sur lequel le serveur doit écouter.
 * @return Un pointeur vers une structure net_server_t initialisée
 * , ou NULL en cas d’échec.
 */
net_server_t *net_server_create(const char *host, unsigned int port, size_t buffer_size);

/**
 * @brief Démarre le serveur (bind + listen).
 *
 * @param server Un pointeur vers le serveur initialisé.
 * @return true si le démarrage a réussi, false sinon.
 */
bool net_server_start(net_server_t *server);

/**
 * @brief Lance la boucle de traitement des événements (poll).
 *
 * @param server Le serveur dont on veut traiter les événements.
 */
bool net_server_poll(net_server_t *server, int poll_timeout);

/**
 * @brief Arrête proprement le serveur (arrête la boucle).
 *
 * @param server Le serveur à arrêter.
 */
void net_server_stop(net_server_t *server);

/**
 * @brief Libère les ressources allouées pour le serveur.
 *
 * @param server Le serveur à détruire.
 */
void net_server_destroy(net_server_t *server);

/**
 * @brief allow the user to send message to a group of client
 *
 * @param server The server
 * @param on_data Function the user wants to define
 */
void net_set_handle_data(net_server_t *server, void (*on_data)
        (net_client_t *client, struct net_server_s *server, void *args), void *args);

void net_set_handle_connection(net_server_t *server, void (*on_connect)
        (net_client_t *client, struct net_server_s *server, void *args), void *args);

void net_set_handle_disconnection(net_server_t *server, void (*on_connect)
        (net_client_t *client, struct net_server_s *server, void *args), void *args);

void net_send_to_specified_clients(net_server_t *server, void *fd, msg_packet_t packet, bool list_mode);

void net_send_all(net_server_t *server, msg_packet_t packet);

            /* ================== UTILS ================== */

/**
 * @brief Envoie un message via un socket.
 *
 * @param fd Descripteur de fichier du socket.
 * @param msg Chaîne de caractères à envoyer.
 */
// void net_send(int fd, const char *msg);
void net_send(int fd, void *msg, size_t msg_size);

/**
 * @brief Ferme proprement un socket avec message d’erreur.
 *
 * @param msg Message d’erreur à afficher.
 * @param socket Descripteur du socket à fermer.
 */
void close_socket(char *msg, int socket);



            /* ================== CLIENTS ================== */

/**
 * @brief Initialise tous les clients comme inactifs.
 *
 * @param server Le serveur dont on initialise les clients.
 */
void init_clients_array(net_server_t *server);

/**
 * @brief Ferme la connexion d’un client donné.
 *
 * @param server Le serveur auquel appartient le client.
 * @param fd Le descripteur de fichier du client à fermer.
 */
void net_close_client(net_server_t *server, int fd);

/**
 * @brief Ferme toutes les connexions clients actives.
 *
 * @param server Le serveur dont on veut fermer tous les clients.
 */
void net_close_all_clients(net_server_t *server);



            /* ================== POLL ================== */

/**
 * @brief Initialise le tableau de pollfd pour surveiller les sockets.
 *
 * @param server Le serveur dont on initialise les pollfds.
 */
void init_poll_fds(net_server_t *server);



            /* ================== HANDLERS ================== */

/**
 * @brief Gère la déconnexion d’un client.
 *
 * @param fd Le descripteur du client déconnecté.
 */
void handle_disconnect(int fd);

/**
 * @brief Gère une nouvelle connexion entrante.
 *
 * @param fd Le descripteur du client connecté.
 */
void handle_connect(int fd);

/**
 * @brief Gère les données reçues d’un client.
 *
 * @param fd Le descripteur du client.
 * @param data Les données reçues.
 */
void handle_data(int fd, char *data);

#endif /* NET_H_ */
