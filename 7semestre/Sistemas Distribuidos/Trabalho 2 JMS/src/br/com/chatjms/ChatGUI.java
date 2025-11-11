package br.com.chatjms;

import javax.jms.*;
import javax.swing.*;
import java.awt.*;

public class ChatGUI extends JFrame implements MessageListener {
    private ChatClient client;
    private JTextArea chatArea;
    private JTextField inputField;
    private JTextField privateUserField;

    public ChatGUI(String username) {
        super("Chat JMS - Usuário: " + username);

        setSize(500, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Área de mensagens
        chatArea = new JTextArea();
        chatArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(chatArea);

        // Campo para digitar mensagens
        inputField = new JTextField();
        JButton publicBtn = new JButton("Enviar Público");
        JButton privateBtn = new JButton("Enviar Privado");

        // Campo para nome do usuário destino (mensagem privada)
        privateUserField = new JTextField(10);

        JPanel bottomPanel = new JPanel(new BorderLayout());
        JPanel buttonPanel = new JPanel();
        buttonPanel.add(publicBtn);
        buttonPanel.add(new JLabel("Privado para:"));
        buttonPanel.add(privateUserField);
        buttonPanel.add(privateBtn);

        bottomPanel.add(inputField, BorderLayout.CENTER);
        bottomPanel.add(buttonPanel, BorderLayout.EAST);

        add(scrollPane, BorderLayout.CENTER);
        add(bottomPanel, BorderLayout.SOUTH);

        try {
            client = new ChatClient(username, this);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Erro ao conectar: " + e.getMessage());
            System.exit(1);
        }

        // Ações dos botões
        publicBtn.addActionListener(e -> {
            try {
                String text = inputField.getText();
                if (!text.isEmpty()) {
                    client.sendPublicMessage(text);
                    inputField.setText("");
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        });

        privateBtn.addActionListener(e -> {
            try {
                String text = inputField.getText();
                String toUser = privateUserField.getText();
                if (!text.isEmpty() && !toUser.isEmpty()) {
                    client.sendPrivateMessage(toUser, text);
                    inputField.setText("");
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        });
    }

    // Quando chega uma mensagem (pública ou privada)
    @Override
    public void onMessage(Message message) {
        if (message instanceof TextMessage) {
            try {
                String text = ((TextMessage) message).getText();
                chatArea.append(text + "\n");
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
