package com.Hrup.HotelHelper.Server;

import com.Hrup.HotelHelper.Common.CommandParser;
import com.Hrup.HotelHelper.Common.Protocol;
import com.Hrup.HotelHelper.Config.ConfigLoader;
import com.Hrup.HotelHelper.Config.Logger;
import com.Hrup.HotelHelper.Datastructures.DoublyLinkedList;
import com.Hrup.HotelHelper.Common.Command;

import java.io.*;
import java.util.HashMap;
import java.util.Map;


public class Database {
    private final Map<String, String> stringData = new HashMap<>();
    private final Map<String, DoublyLinkedList> listData = new HashMap<>();
    private final Map<String, Map<String, String>> hashData = new HashMap<>();
    private final String persistenceFile;

    public Database() {
        ConfigLoader configLoader = new ConfigLoader();
        persistenceFile = configLoader.getProperty("persistence.file");
        loadData();
    }

    private void loadData() {
        try (BufferedReader reader = new BufferedReader(new FileReader(persistenceFile))) {
            String line;
            while ((line = reader.readLine()) != null) {
                Command command = CommandParser.parse(line);
                executeCommand(command);
            }
        } catch (IOException e) {
            Logger.error("Failed to load data from persistence file: " + e.getMessage());
        }
    }

    private void persistCommand(Command command) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(persistenceFile, true))) {
            StringBuilder sb = new StringBuilder(command.getName());
            for (String arg : command.getArgs()) {
                sb.append(" ").append(arg);
            }
            writer.write(sb.toString());
            writer.newLine();
        } catch (IOException e) {
            Logger.error("Failed to persist command: " + e.getMessage());
        }
    }

    public String executeCommand(Command command) {
        String name = command.getName();
        String[] args = command.getArgs();
        switch (name) {
            case "set":
                stringData.put(args[0], args[1]);
                persistCommand(command);
                return Protocol.OK;
            case "get":
                return stringData.getOrDefault(args[0], Protocol.NULL);
            case "del":
                stringData.remove(args[0]);
                persistCommand(command);
                return Protocol.OK;
            case "lpush":
                listData.computeIfAbsent(args[0], k -> new DoublyLinkedList()).lpush(args[1]);
                persistCommand(command);
                return Protocol.OK;
            case "rpush":
                listData.computeIfAbsent(args[0], k -> new DoublyLinkedList()).rpush(args[1]);
                persistCommand(command);
                return Protocol.OK;
            case "range":
                DoublyLinkedList list = listData.get(args[0]);
                if (list == null) {
                    return "[]";
                }
                int start = Integer.parseInt(args[1]);
                int end = Integer.parseInt(args[2]);
                return list.range(start, end).toString();
            case "len":
                return String.valueOf(listData.getOrDefault(args[0], new DoublyLinkedList()).len());
            case "lpop":
                DoublyLinkedList lpopList = listData.get(args[0]);
                if (lpopList == null) {
                    return Protocol.NULL;
                }
                String lpopValue = lpopList.lpop();
                if (lpopValue != null) {
                    persistCommand(command);
                }
                return lpopValue;
            case "rpop":
                DoublyLinkedList rpopList = listData.get(args[0]);
                if (rpopList == null) {
                    return Protocol.NULL;
                }
                String rpopValue = rpopList.rpop();
                if (rpopValue != null) {
                    persistCommand(command);
                }
                return rpopValue;
            case "ldel":
                listData.remove(args[0]);
                persistCommand(command);
                return Protocol.OK;
            case "hset":
                hashData.computeIfAbsent(args[0], k -> new HashMap<>()).put(args[1], args[2]);
                persistCommand(command);
                return Protocol.OK;
            case "hget":
                Map<String, String> hgetMap = hashData.get(args[0]);
                if (hgetMap == null) {
                    return Protocol.NULL;
                }
                return hgetMap.getOrDefault(args[1], Protocol.NULL);
            case "hdel":
                if (args.length == 2) {
                    Map<String, String> hdelMap = hashData.get(args[0]);
                    if (hdelMap != null) {
                        hdelMap.remove(args[1]);
                        persistCommand(command);
                    }
                } else {
                    hashData.remove(args[0]);
                    persistCommand(command);
                }
                return Protocol.OK;
            case "ping":
                return Protocol.PONG;
            case "help":
                if (args.length == 0) {
                    try (BufferedReader reader = new BufferedReader(new FileReader("src/com/Hrup/Resources/Command_Help.txt"))) {
                        StringBuilder helpText = new StringBuilder();
                        String line;
                        while ((line = reader.readLine()) != null) {
                            helpText.append(line).append("\n");
                        }
                        return helpText.toString();
                    } catch (IOException e) {
                        Logger.error("Failed to read help text: " + e.getMessage());
                        return "无法读取帮助信息";
                    }
                } else {
                    return "暂未实现该指令帮助";
                }
            default:
                return Protocol.ERROR_COMMAND_NOT_FOUND;
        }
    }
}
