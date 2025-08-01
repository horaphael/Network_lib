/*
** EPITECH PROJECT, 2025
** main
** File description:
** main
*/

#include "include/net.h"
#include "include/log.h"

void handle_client_connect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("New client connected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_disconnect(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("Client disconnected: fd=%d", client->fd);
    LOG_INFO("CMD: %s", cmd);
}

void handle_client_data(net_client_t *client, net_server_t *server, void *args)
{
    char *cmd = (char *)args;

    (void)server;
    LOG_INFO("Data from fd=%d: %s", client->fd, client->buffer);
    LOG_INFO("CMD: %s", cmd);
}

int main(void)
{
    net_server_t *server = net_server_create("127.0.0.1", 4243, 1024);
    int timeout = 0;
    char data_command[256] = "Commande reçu";
    char data_disconnect_command[256] = "Commande disconnection reçu";
    char data_connect_command[256] = "Commande connection reçu";

    if (!server)
        return 84;
    set_handle_connection(server, handle_client_connect, data_connect_command);
    set_handle_disconnection(server, handle_client_disconnect, data_disconnect_command);
    set_handle_data(server, handle_client_data, data_command);

    // set_handle_connection(server, NULL);
    // set_handle_disconnection(server, NULL);
    // set_handle_data(server, NULL);
    if (!net_server_start(server))
        return 84;
    while (server->running) {
        net_server_poll(server, timeout);
    }
    net_server_destroy(server);
    return 0;
}
