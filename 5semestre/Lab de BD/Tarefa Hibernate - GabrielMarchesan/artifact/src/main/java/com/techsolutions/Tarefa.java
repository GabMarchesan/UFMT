package com.techsolutions;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;
import java.time.LocalDate;

@Entity
@Table(name = "tarefas") // Especifica o nome da tabela no banco de dados
public class Tarefa {

    @Id // Indica a chave primária da tabela
    @GeneratedValue(strategy = GenerationType.IDENTITY) // Configura a geração de valor para a chave primária
    private Long id;

    private String descricao;
    private String status;
    private LocalDate prazo;

    // Getters e Setters
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getDescricao() {
        return descricao;
    }

    public void setDescricao(String descricao) {
        this.descricao = descricao;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public LocalDate getPrazo() {
        return prazo;
    }

    public void setPrazo(LocalDate prazo) {
        this.prazo = prazo;
    }

    @Override
    public String toString() {
        return "Tarefa [id=" + id + ", descricao=" + descricao + ", status=" + status + ", prazo=" + prazo + "]";
    }
}