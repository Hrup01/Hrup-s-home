package com.Hrup.HotelHelper.Datastructures;

import java.util.ArrayList;
import java.util.List;

public class DoublyLinkedList {
    private ListNode head;
    private ListNode tail;
    private int size;

    public void lpush(String value) {
        ListNode newNode = new ListNode(value);
        if (head == null) {
            head = tail = newNode;
        } else {
            newNode.next = head;
            head.prev = newNode;
            head = newNode;
        }
        size++;
    }

    public void rpush(String value) {
        ListNode newNode = new ListNode(value);
        if (tail == null) {
            head = tail = newNode;
        } else {
            newNode.prev = tail;
            tail.next = newNode;
            tail = newNode;
        }
        size++;
    }

    public List<String> range(int start, int end) {
        List<String> result = new ArrayList<>();
        if (start < 0 || end < start || start >= size) {
            return result;
        }
        end = Math.min(end, size - 1);
        ListNode current = head;
        for (int i = 0; i < start; i++) {
            current = current.next;
        }
        for (int i = start; i <= end; i++) {
            result.add(current.value);
            current = current.next;
        }
        return result;
    }

    public int len() {
        return size;
    }

    public String lpop() {
        if (head == null) {
            return null;
        }
        String value = head.value;
        if (head == tail) {
            head = tail = null;
        } else {
            head = head.next;
            head.prev = null;
        }
        size--;
        return value;
    }

    public String rpop() {
        if (tail == null) {
            return null;
        }
        String value = tail.value;
        if (head == tail) {
            head = tail = null;
        } else {
            tail = tail.prev;
            tail.next = null;
        }
        size--;
        return value;
    }
}
