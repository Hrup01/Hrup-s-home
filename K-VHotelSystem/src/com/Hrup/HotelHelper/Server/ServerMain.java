package com.Hrup.HotelHelper.Server;

import com.Hrup.HotelHelper.Config.ConfigLoader;
import com.Hrup.HotelHelper.Config.Logger;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerMain {
    public static void main(String[] args) {
        ConfigLoader configLoader = new ConfigLoader();
        int port = Integer.parseInt(configLoader.getProperty("server.port"));
        Database database = new Database();

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            Logger.info("Server started on port " + port);
            while (true) {
                Socket socket = serverSocket.accept();
                new Thread(new ClientHandler(socket, database)).start();
            }
        } catch (IOException e) {
            Logger.error("Error starting server: " + e.getMessage(), e);
        }
    }
}
