package br.com.chatjms;

import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        String username = JOptionPane.showInputDialog("Digite seu nome de usuário:");
        if (username == null || username.trim().isEmpty()) {
            System.exit(0);
        }

        SwingUtilities.invokeLater(() -> {
            ChatGUI gui = new ChatGUI(username);
            gui.setVisible(true);
        });
    }
}
