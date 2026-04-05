# Simulador da Microarquitetura Mic-1

## Descrição

Este repositório contém a implementação de um simulador em linguagem **C** para uma versão modificada da microarquitetura **Mic-1**.

O objetivo do projeto é compreender, por meio de simulação em software, o funcionamento interno de componentes fundamentais de uma arquitetura de computadores, como **ULA, registradores, barramentos e memória**.

O simulador interpreta instruções e executa operações que representam o comportamento da máquina Mic-1 estudada em sala de aula.

---

## Disciplina

**Arquitetura de Computadores II**
Centro de Informática – UFPB

Professora: **Sarah Pontes Madruga**

---

## Objetivo do Projeto

O projeto tem como objetivo implementar, em uma linguagem de alto nível, uma simulação do funcionamento da arquitetura Mic-1 modificada, permitindo observar como instruções são executadas internamente pela máquina.

Durante o desenvolvimento serão simulados diferentes componentes da arquitetura, permitindo acompanhar passo a passo as mudanças em registradores, memória e na ULA.

---

## Estrutura do Projeto

```text
mic1-simulador
│
├── README.md
├── .gitignore
│
├── etapa1
│   ├── src
│   ├── input
│   └── output
│
├── etapa2
│   ├── tarrefa 1
│   └── tarefa 2
│
├── etapa3
│  ├── tarrefa 1
└── relatorio
```

Cada pasta corresponde a uma etapa do projeto proposta na disciplina.

---

## Etapas do Projeto

### Etapa 1 – Simulação da ULA

Nesta etapa será implementada uma simulação da **ULA (Unidade Lógica e Aritmética)** da arquitetura Mic-1.

O programa será capaz de:

* Ler instruções de 6 bits a partir de um arquivo `.txt`
* Interpretar os sinais de controle da ULA
* Executar operações lógicas e aritméticas
* Registrar os resultados em um arquivo de log

---

### Etapa 2 – Caminho de Dados da Mic-1

Nesta etapa será implementado o **caminho de dados da arquitetura**, incluindo:

* registradores
* barramentos
* integração com a ULA

---

### Etapa 3 – Acesso à Memória e Execução de Instruções

A última etapa adiciona suporte para:

* acesso à memória
* manipulação de pilha
* execução de instruções da linguagem **IJVM**

---

## Linguagem Utilizada

O simulador será implementado em:

**C**

---

## Equipe

Projeto desenvolvido pelos estudantes:

* **Miracle Temitope Hazzan**
* **Lucas Leite**
* **Tiago Varelo Silva**
*  **Antonio Carlos**

---

## Instituição

Universidade Federal da Paraíba (UFPB)
Centro de Informática
Curso de Ciência da Computação
