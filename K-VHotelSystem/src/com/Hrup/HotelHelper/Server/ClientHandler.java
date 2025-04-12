package com.Hrup.HotelHelper.Server;

import com.Hrup.HotelHelper.Common.Command;
import com.Hrup.HotelHelper.Common.CommandParser;
import com.Hrup.HotelHelper.Config.Logger;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientHandler implements Runnable {
    private final Socket socket;
    private final Database database;

    public ClientHandler(Socket socket, Database database) {
        this.socket = socket;
        this.database = database;
    }
    @Override
    public void run() {
        Logger.info("Client connected: " + socket.getInetAddress());
        try (BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
            String inputLine;
            while ((inputLine = in.readLine()) != null) {
                Command command = CommandParser.parse(inputLine);
                String response = database.executeCommand(command);
                out.println(response);
            }
        } catch (IOException e) {
            Logger.error("Error handling client: " + e.getMessage(), e);
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                Logger.error("Error closing socket: " + e.getMessage(), e);
            }
        }
    }
}
