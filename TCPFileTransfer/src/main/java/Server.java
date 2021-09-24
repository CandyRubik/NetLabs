import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class Server {

    private static final int HOST_IP_INDEX = 0;
    private static List<ClientHandler> clientList;
    public static void main(String[] args) {
        if(args.length < 1) {
            System.err.println("Too few arguments, hint [first] - host ip");
        }

        try(ServerSocket server = new ServerSocket(Integer.parseInt(args[HOST_IP_INDEX]))
            ) {
            clientList = new ArrayList<>();
            while (true) {
                ClientHandler clientHandler = new ClientHandler(server.accept());
                clientList.add(clientHandler);
                clientHandler.start();
                clientList.stream().filter(ClientHandler::isWorkEnd).forEach(ClientHandler::close);
                clientList.removeIf(ClientHandler::isWorkEnd);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}