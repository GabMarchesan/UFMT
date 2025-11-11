package com.techsolutions;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import java.util.List;

public class TarefaDAO {

    private EntityManager em;

    public TarefaDAO(EntityManager em) {
        this.em = em;
    }

    // Método para salvar uma nova tarefa ou atualizar uma existente
    public void salvar(Tarefa tarefa) {
        EntityTransaction transaction = em.getTransaction();
        transaction.begin();
        em.merge(tarefa);
        transaction.commit();
    }

    // Método para buscar uma tarefa pelo seu ID
    public Tarefa buscarPorId(Long id) {
        return em.find(Tarefa.class, id);
    }

    // Método para listar todas as tarefas
    public List<Tarefa> listarTodas() {
        // Usa JPQL para buscar todas as tarefas
        return em.createQuery("SELECT t FROM Tarefa t", Tarefa.class).getResultList();
    }

    // Método para remover uma tarefa
    public void remover(Long id) {
        Tarefa tarefa = buscarPorId(id);
        if (tarefa != null) {
            EntityTransaction transaction = em.getTransaction();
            transaction.begin();
            em.remove(tarefa);
            transaction.commit();
        }
    }
}