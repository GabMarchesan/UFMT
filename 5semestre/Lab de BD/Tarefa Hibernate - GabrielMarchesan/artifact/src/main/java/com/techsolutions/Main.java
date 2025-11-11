package com.techsolutions;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.time.format.DateTimeParseException;
import java.util.List;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        EntityManagerFactory emf = Persistence.createEntityManagerFactory("techsolutions-pu");
        EntityManager em = emf.createEntityManager();

        // Inicializa nosso DAO com o EntityManager
        TarefaDAO tarefaDAO = new TarefaDAO(em);
        Scanner scanner = new Scanner(System.in);
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy");

        int opcao = -1;

        while (opcao != 0) {
            System.out.println("\n--- MENU DE TAREFAS ---");
            System.out.println("1. Adicionar Nova Tarefa");
            System.out.println("2. Listar Todas as Tarefas");
            System.out.println("3. Atualizar Status da Tarefa");
            System.out.println("4. Remover Tarefa");
            System.out.println("0. Sair");
            System.out.print("Escolha uma opção: ");

            try {
                opcao = Integer.parseInt(scanner.nextLine());
            } catch (NumberFormatException e) {
                System.out.println("Opção inválida. Por favor, digite um número.");
                continue;
            }

            switch (opcao) {
                case 1:
                    // --- Adicionar Tarefa ---
                    System.out.print("Digite a descrição da tarefa: ");
                    String descricao = scanner.nextLine();

                    System.out.print("Digite o status (ex: Pendente): ");
                    String status = scanner.nextLine();

                    LocalDate prazo = null;
                    while (prazo == null) {
                        System.out.print("Digite o prazo (formato dd/MM/yyyy): ");
                        String prazoStr = scanner.nextLine();
                        try {
                            prazo = LocalDate.parse(prazoStr, formatter);
                        } catch (DateTimeParseException e) {
                            System.out.println("Formato de data inválido. Tente novamente.");
                        }
                    }

                    Tarefa novaTarefa = new Tarefa();
                    novaTarefa.setDescricao(descricao);
                    novaTarefa.setStatus(status);
                    novaTarefa.setPrazo(prazo);
                    tarefaDAO.salvar(novaTarefa);
                    System.out.println("Tarefa adicionada com sucesso! ID: " + novaTarefa.getId());
                    break;

                case 2:
                    // --- Listar Tarefas ---
                    System.out.println("\n--- LISTA DE TAREFAS ---");
                    List<Tarefa> tarefas = tarefaDAO.listarTodas();
                    if (tarefas.isEmpty()) {
                        System.out.println("Nenhuma tarefa encontrada.");
                    } else {
                        for (Tarefa t : tarefas) {
                            System.out.println(t);
                        }
                    }
                    break;

                case 3:
                    // --- Atualizar Tarefa ---
                    System.out.print("Digite o ID da tarefa que deseja atualizar: ");
                    try {
                        Long idUpdate = Long.parseLong(scanner.nextLine());
                        Tarefa tarefaUpdate = tarefaDAO.buscarPorId(idUpdate);
                        if (tarefaUpdate != null) {
                            System.out.print("Digite o novo status da tarefa: ");
                            String novoStatus = scanner.nextLine();
                            tarefaUpdate.setStatus(novoStatus);
                            tarefaDAO.salvar(tarefaUpdate);
                            System.out.println("Tarefa atualizada com sucesso!");
                        } else {
                            System.out.println("Tarefa com ID " + idUpdate + " não encontrada.");
                        }
                    } catch (NumberFormatException e) {
                        System.out.println("ID inválido. Por favor, digite um número.");
                    }
                    break;

                case 4:
                    // --- Remover Tarefa ---
                    System.out.print("Digite o ID da tarefa que deseja remover: ");
                    try {
                        Long idRemove = Long.parseLong(scanner.nextLine());
                        tarefaDAO.remover(idRemove);
                        System.out.println("Tarefa removida com sucesso (se existia).");
                    } catch (NumberFormatException e) {
                        System.out.println("ID inválido. Por favor, digite um número.");
                    }
                    break;

                case 0:
                    System.out.println("Saindo do programa...");
                    break;

                default:
                    System.out.println("Opção inválida. Tente novamente.");
            }
        }

        // Fecha os recursos
        scanner.close();
        em.close();
        emf.close();
    }
}