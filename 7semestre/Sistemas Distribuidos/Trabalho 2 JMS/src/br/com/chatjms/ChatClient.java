package br.com.chatjms;

import javax.jms.*;
import org.apache.activemq.ActiveMQConnectionFactory;

public class ChatClient {
    private String username;
    private Connection connection;
    private Session session;
    private MessageProducer publicProducer;
    private MessageConsumer publicConsumer;
    private MessageConsumer privateConsumer;
    private MessageListener messageListener;

    public ChatClient(String username, MessageListener listener) throws Exception {
        this.username = username;
        this.messageListener = listener;

        // Conecta ao ActiveMQ rodando no Windows
        ConnectionFactory factory = new ActiveMQConnectionFactory("tcp://localhost:61616");
        connection = factory.createConnection();
        connection.setClientID(username); // identifica o usuário
        connection.start();

        // Cria sessão sem transação, com auto-ack
        session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);

        // TOPIC para mensagens públicas
        Topic publicTopic = session.createTopic("chat.topic");
        publicProducer = session.createProducer(publicTopic);
        publicConsumer = session.createConsumer(publicTopic);
        publicConsumer.setMessageListener(messageListener);

        // QUEUE para mensagens privadas (nome do usuário)
        Queue privateQueue = session.createQueue("chat.queue." + username);
        privateConsumer = session.createConsumer(privateQueue);
        privateConsumer.setMessageListener(messageListener);
    }

    // Enviar mensagem pública (todos recebem)
    public void sendPublicMessage(String text) throws JMSException {
        TextMessage msg = session.createTextMessage(username + " (public): " + text);
        publicProducer.send(msg);
    }

    // Enviar mensagem privada (apenas um usuário recebe)
    public void sendPrivateMessage(String toUser, String text) throws JMSException {
        Queue privateQueue = session.createQueue("chat.queue." + toUser);
        MessageProducer privateProducer = session.createProducer(privateQueue);
        TextMessage msg = session.createTextMessage(username + " (private): " + text);
        privateProducer.send(msg);
        privateProducer.close();
    }

    public void close() throws JMSException {
        connection.close();
    }
}
