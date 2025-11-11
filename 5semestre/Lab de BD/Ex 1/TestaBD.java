package testeconexao;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.ResultSet;
import java.util.Scanner; // Importar Scanner

public class TestaBD {

    // Detalhes da conexão com o banco de dados
    private static final String URL = "jdbc:mysql://localhost:3306/teste"; // Substitua pelo nome do seu DB
    private static final String USUARIO = "root"; // Substitua pelo seu usuário do MySQL
    private static final String SENHA = "";     // Substitua pela sua senha do MySQL

    public static void main(String[] args) {
        System.out.println("Iniciando aplicação de teste de banco de dados...");

        try {
            // Carregar o driver JDBC para MySQL
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("Driver JDBC do MySQL carregado.");

            // Usar try-with-resources para garantir que a conexão seja fechada automaticamente
            try (Connection conexao = DriverManager.getConnection(URL, USUARIO, SENHA);
                 Scanner scanner = new Scanner(System.in)) { // Scanner também é AutoCloseable
                System.out.println("Conexão com o MySQL estabelecida com sucesso!");

                // Verificar a validade da conexão
                if (!conexao.isValid(5)) { // 5 segundos de timeout
                    System.err.println("A conexão não é válida ou expirou. Abortando operações.");
                    return;
                }

                int opcao = -1;
                while (opcao != 0) {
                    exibirMenu();
                    System.out.print("Escolha uma opção: ");
                    try {
                        opcao = Integer.parseInt(scanner.nextLine());

                        switch (opcao) {
                            case 1:
                                criarTabelaSaudacao(conexao);
                                break;
                            case 2:
                                System.out.print("Digite a mensagem para inserir: ");
                                String mensagem = scanner.nextLine();
                                inserirSaudacao(conexao, mensagem);
                                break;
                            case 3:
                                consultarSaudacao(conexao);
                                break;
                            case 4:
                                deletarTabelaSaudacao(conexao);
                                break;
                            case 0:
                                System.out.println("Saindo da aplicação. Adeus!");
                                break;
                            default:
                                System.out.println("Opção inválida. Por favor, tente novamente.");
                        }
                    } catch (NumberFormatException e) {
                        System.out.println("Entrada inválida. Por favor, digite um número.");
                    }
                    System.out.println("\nPressione Enter para continuar...");
                    scanner.nextLine(); // Consumir a nova linha após a opção
                }

            } // conexao.close() e scanner.close() são chamados automaticamente aqui
            System.out.println("\nRecursos do banco de dados e scanner foram fechados automaticamente.");

        } catch (ClassNotFoundException e) {
            System.err.println("Erro: Driver JDBC do MySQL não encontrado. Verifique se o JAR está no CLASSPATH.");
            e.printStackTrace();
        } catch (SQLException e) {
            System.err.println("Erro de SQL: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /**
     * Exibe o menu de opções para o usuário.
     */
    private static void exibirMenu() {
        System.out.println("\n--- MENU DE OPERAÇÕES NO BANCO DE DADOS ---");
        System.out.println("1. Criar Tabela 'Saudacao'");
        System.out.println("2. Inserir Mensagem na Tabela 'Saudacao'");
        System.out.println("3. Consultar Mensagens na Tabela 'Saudacao'");
        System.out.println("4. Deletar Tabela 'Saudacao'");
        System.out.println("0. Sair");
        System.out.println("------------------------------------------");
    }

    /**
     * Cria a tabela 'Saudacao' se ela não existir.
     * @param conexao A conexão JDBC com o banco de dados.
     * @throws SQLException Se ocorrer um erro SQL.
     */
    private static void criarTabelaSaudacao(Connection conexao) throws SQLException {
        try (Statement stmt = conexao.createStatement()) {
            String sql = "CREATE TABLE IF NOT EXISTS Saudacao (Mensagem VARCHAR(255) NOT NULL)";
            stmt.executeUpdate(sql);
            System.out.println("\nTabela 'Saudacao' criada ou já existente.");
        }
    }

    /**
     * Insere uma nova tupla na tabela 'Saudacao'.
     * @param conexao A conexão JDBC com o banco de dados.
     * @param mensagem O texto da saudação a ser inserido.
     * @throws SQLException Se ocorrer um erro SQL.
     */
    private static void inserirSaudacao(Connection conexao, String mensagem) throws SQLException {
        // Usando PreparedStatement para segurança contra SQL Injection
        // e melhor performance em inserções repetidas.
        String sql = "INSERT INTO Saudacao (Mensagem) VALUES (?)";
        try (java.sql.PreparedStatement pstmt = conexao.prepareStatement(sql)) {
            pstmt.setString(1, mensagem);
            int linhasAfetadas = pstmt.executeUpdate();
            System.out.println("Inserido: " + linhasAfetadas + " tupla(s) na tabela 'Saudacao'.");
        }
    }

    /**
     * Consulta todas as tuplas da tabela 'Saudacao' e exibe-as no terminal.
     * @param conexao A conexão JDBC com o banco de dados.
     * @throws SQLException Se ocorrer um erro SQL.
     */
    private static void consultarSaudacao(Connection conexao) throws SQLException {
        try (Statement stmt = conexao.createStatement();
             ResultSet rs = stmt.executeQuery("SELECT Mensagem FROM Saudacao")) {
            System.out.println("\nConteúdo da tabela 'Saudacao':");
            if (!rs.isBeforeFirst()) {
                System.out.println("Nenhuma tupla encontrada.");
            } else {
                while (rs.next()) {
                    String mensagem = rs.getString("Mensagem");
                    System.out.println("Mensagem: " + mensagem);
                }
            }
        }
    }

    /**
     * Deleta a tabela 'Saudacao' do banco de dados.
     * @param conexao A conexão JDBC com o banco de dados.
     * @throws SQLException Se ocorrer um erro SQL.
     */
    private static void deletarTabelaSaudacao(Connection conexao) throws SQLException {
        try (Statement stmt = conexao.createStatement()) {
            String sql = "DROP TABLE IF EXISTS Saudacao";
            stmt.executeUpdate(sql);
            System.out.println("\nTabela 'Saudacao' deletada com sucesso.");
        }
    }
}