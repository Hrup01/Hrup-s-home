package com.Hrup.HotelHelper.Client;

import com.Hrup.HotelHelper.Config.ConfigLoader;
import com.Hrup.HotelHelper.Config.Logger;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientMain {
    public static void main(String[] args) {
        ConfigLoader configLoader = new ConfigLoader();
        String host = "localhost";
        int port = Integer.parseInt(configLoader.getProperty("server.port"));

        try (Socket socket = new Socket(host, port);
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader consoleIn = new BufferedReader(new InputStreamReader(System.in))) {
            Logger.info("Connected to server on " + host + ":" + port);
            String inputLine;
            while ((inputLine = consoleIn.readLine()) != null) {
                if ("exit".equalsIgnoreCase(inputLine)) {
                    break;
                }
                out.println(inputLine);
                String response = in.readLine();
                System.out.println(response);
            }
        } catch (IOException e) {
            Logger.error("Error communicating with server: " + e.getMessage(), e);
        }
    }
}
