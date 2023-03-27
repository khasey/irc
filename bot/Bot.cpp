#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Vérifier les arguments de ligne de commande
    if (argc < 6) {
        std::cerr << "Usage :<prog> <server_ip> <server_port> <channel> <nickname> <password>" << std::endl;
        return EXIT_FAILURE;
    }
    const char *server_ip = argv[1];
    const int server_port = std::stoi(argv[2]);
    const char *channel = argv[3];
    const char *nickname = argv[4];
    const char *password = argv[5];
    // Créer une socket TCP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur : impossible de créer la socket." << std::endl;
        return EXIT_FAILURE;
    }

    // Résoudre l'adresse IP du serveur
    struct hostent *server = gethostbyname(server_ip);
    if (server == NULL) {
        std::cerr << "Erreur : impossible de résoudre l'adresse IP du serveur." << std::endl;
        return EXIT_FAILURE;
    }

    // Initialiser l'adresse du serveur
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(server_port);

    // Connecter la socket au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erreur : impossible de se connecter au serveur." << std::endl;
        return EXIT_FAILURE;
    }

    // Envoyer les informations d'identification au serveur
    std::ostringstream oss;
    oss << "PASS " << password << "\r\n";
    std::string pass_message = oss.str();
    if (send(sockfd, pass_message.c_str(), pass_message.length(), 0) < 0) {
        std::cerr << "Erreur : impossible d'envoyer le mot de passe." << std::endl;
        return EXIT_FAILURE;
    }

    oss.str("");
    oss << "NICK " << nickname << "\r\n";
    std::string nick_message = oss.str();
    if (send(sockfd, nick_message.c_str(), nick_message.length(), 0) < 0) {
        std::cerr << "Erreur : impossible d'envoyer le pseudonyme." << std::endl;
        return EXIT_FAILURE;
    }

    oss.str("");
    oss << "JOIN " << channel << "\r\n";
    std::string join_message = oss.str();
    if (send(sockfd, join_message.c_str(), join_message.length(), 0) < 0) {
        std::cerr << "Erreur : impossible de rejoindre le canal." << std::endl;
        return EXIT_FAILURE;
    }

    // Boucle principale du bot
    while (true) {
    // Lire la commande entrée par l'utilisateur depuis la console
    std::cout << "=================SUCESS=================" << std::endl;
    std::cout << "NICK : " << nickname << std::endl;
    std::cout << "Waiting for a command......." << std::endl;
    std::string command;
    std::getline(std::cin, command);

    // Vérifier si l'utilisateur a entré une commande
    if (!command.empty()) {
        // Vérifier si la commande est "PING"
        if (command == "PING") {
            // Convertir le pointeur server en une chaîne de caractères
            std::string server_address = inet_ntoa(*(struct in_addr *)server->h_addr);
            // Envoyer la commande PING au serveur
            std::string ping_message = "PONG 127.0.0.1\n";
            if (send(sockfd, ping_message.c_str(), ping_message.length(), 0) < 0) {
                std::cerr << "Erreur : impossible d'envoyer la commande PING." << std::endl;
                return EXIT_FAILURE;
            }
        }
        // Vérifier si la commande est "PRIVMSG"
        else if (command.substr(0, 7) == "PRIVMSG") {
            // Extraire le nom de la personne à qui envoyer le message
            std::size_t first_space_pos = command.find(' ');
            std::size_t second_space_pos = command.find(' ', first_space_pos + 1);
            if (second_space_pos == std::string::npos) {
                std::cerr << "Erreur : la commande PRIVMSG doit avoir deux arguments." << std::endl;
                continue;
            }
            std::string recipient = command.substr(first_space_pos + 1, second_space_pos - first_space_pos - 1);

            // Extraire le message à envoyer
            std::string message = command.substr(second_space_pos + 1);

            // Ajouter le préfixe PRIVMSG et le message de commande à envoyer au serveur
            std::ostringstream oss;
            oss << "PRIVMSG " << recipient << " :" << message << "\r\n";
            std::string full_message = oss.str();

            // Envoyer la commande au serveur
            if (send(sockfd, full_message.c_str(), full_message.length(), 0) < 0) {
                std::cerr << "Erreur : impossible d'envoyer la commande PRIVMSG." << std::endl;
                return EXIT_FAILURE;
            }
        }
        // Si la commande n'est ni "PING" ni "PRIVMSG", envoyer la commande au serveur telle quelle
        else {
            std::string full_command = command + "\r\n";
            if (send(sockfd, full_command.c_str(), full_command.length(), 0) < 0) {
                std::cerr << "Erreur : impossible d'envoyer la commande." << std::endl;
                return EXIT_FAILURE;
            }
        }
    }

    // Lire les messages entrants du serveur
    char buffer[1024];
    int num_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (num_bytes < 0) {
        std::cerr << "Erreur : impossible de recevoir les données du serveur." << std::endl;
        return EXIT_FAILURE;
    }
    buffer[num_bytes] = '\0';

    // Afficher les messages entrants dans la console
    std::cout << buffer;

    // Vérifier si le message entrant est une commande PING
    std::string incoming_message(buffer);
    std::size_t ping_pos = incoming_message.find("PING");
    if (ping_pos != std::string::npos) {
        // Extraire les détails du serveur à partir du message PING entrant
    std::string server_details = incoming_message.substr(ping_pos + 5);
    std::string pong_message = "PONG " + server_details;
    
    // Envoyer la réponse PONG au serveur
    if (send(sockfd, pong_message.c_str(), pong_message.length(), 0) < 0) {
        std::cerr << "Erreur : impossible d'envoyer la réponse PONG." << std::endl;
        return EXIT_FAILURE;
    }
    }
}
}