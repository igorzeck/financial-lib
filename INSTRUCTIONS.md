# Teste Técnico – Estágio em Desenvolvimento C

## 📌 Objetivo

O objetivo deste teste é avaliar seus conhecimentos em:

* Lógica de programação
* Linguagem C
* Organização de código
* Criação de bibliotecas
* Cálculos matemáticos e financeiros
* Manipulação de structs
* Boas práticas com funções e validações

---

## 🚀 Desafio

Você deverá desenvolver uma **biblioteca em C para cálculos financeiros**.

A biblioteca deverá permitir calcular operações financeiras básicas, como juros simples, juros compostos, parcelas e amortização.

---

## 🧰 Tecnologias esperadas

* Linguagem C
* GCC ou Clang
* Makefile
* Organização em arquivos `.h` e `.c`

---

## 📦 Estrutura esperada

Organize o projeto de forma semelhante a:

```txt
financial-lib/
├── include/
│   └── finance.h
├── src/
│   └── finance.c
├── tests/
│   └── main.c
├── Makefile
└── README.md
```

---

## 📚 Funcionalidades obrigatórias

A biblioteca deve implementar as seguintes funções:

### 1. Juros Simples

```c
double simple_interest(double principal, double rate, int periods);
```

Fórmula:

```txt
J = P * i * n
```

Onde:

* `P` = valor inicial
* `i` = taxa de juros
* `n` = número de períodos

---

### 2. Montante com Juros Simples

```c
double simple_interest_amount(double principal, double rate, int periods);
```

Fórmula:

```txt
M = P * (1 + i * n)
```

Onde:

* `M` = montante final
* `P` = valor principal
* `i` = taxa de juros
* `n` = número de períodos

---

### 3. Montante com Juros Compostos

```c
double compound_interest_amount(double principal, double rate, int periods);
```

Fórmula:

```txt
M = P * (1 + i)^n
```

Onde:

* `M` = montante final
* `P` = valor principal
* `i` = taxa de juros por período
* `n` = número de períodos

---

### 4. Juros Compostos

```c
double compound_interest(double principal, double rate, int periods);
```

Fórmula:

```txt
J = M - P
```

Substituindo o montante:

```txt
J = P * ((1 + i)^n - 1)
```

Onde:

* `J` = juros acumulados
* `P` = capital inicial
* `i` = taxa de juros por período
* `n` = número de períodos

---

### 5. Cálculo de Parcela Fixa

```c
double fixed_installment(double principal, double monthly_rate, int months);
```

Fórmula:

```txt
PMT = P * [ i * (1 + i)^n ] / [ (1 + i)^n - 1 ]
```

Onde:

* `PMT` = valor da parcela fixa
* `P` = valor financiado
* `i` = taxa de juros mensal
* `n` = número de parcelas

#### Observação

Caso a taxa de juros seja igual a zero, a função deve apenas dividir o valor principal pela quantidade de meses:

```txt
PMT = P / n
```

---

## ⚙️ Regras de Negócio

A aplicação deve respeitar as seguintes regras:

1. Não permitir valores negativos para principal, taxa ou períodos
2. Não permitir período igual ou menor que zero
3. Caso a taxa seja zero, o cálculo de parcela deve dividir o valor igualmente pelos meses
4. Os cálculos devem retornar valores com precisão adequada usando `double`
5. A biblioteca não deve imprimir valores diretamente nas funções principais
6. A entrada e saída de dados deve ficar separada da lógica de cálculo

---

## 🧪 Programa de Teste

Crie um arquivo `tests/main.c` demonstrando o uso da biblioteca.

Exemplo esperado:

```c
#include <stdio.h>
#include "finance.h"

int main() {
    double principal = 1000.0;
    double rate = 0.05;
    int periods = 12;

    printf("Juros simples: %.2f\n", simple_interest(principal, rate, periods));
    printf("Montante simples: %.2f\n", simple_interest_amount(principal, rate, periods));
    printf("Montante composto: %.2f\n", compound_interest_amount(principal, rate, periods));
    printf("Juros compostos: %.2f\n", compound_interest(principal, rate, periods));

    return 0;
}
```

---

## 📤 Exemplos de Resultado

Para:

```txt
principal = 1000.00
taxa = 0.05
periodos = 12
```

Resultado aproximado:

```txt
Juros simples: 600.00
Montante simples: 1600.00
Montante composto: 1795.86
Juros compostos: 795.86
```

---

## ⚠️ Possíveis Erros Esperados

### ❌ Valor principal inválido

```txt
Erro: valor principal não pode ser negativo
```

### ❌ Período inválido

```txt
Erro: período deve ser maior que zero
```

### ❌ Taxa inválida

```txt
Erro: taxa não pode ser negativa
```

---

## 📌 Requisitos

* Utilizar linguagem C
* Separar declaração e implementação usando `.h` e `.c`
* Criar um `Makefile`
* Criar exemplos de uso
* Validar entradas inválidas
* Código deve ser legível e bem nomeado
* Evitar repetição desnecessária de código

---

## 📎 Diferenciais não obrigatórios

* Criar testes automatizados simples
* Implementar tabela de amortização
* Implementar cálculo de valor futuro
* Implementar cálculo de valor presente
* Implementar TIR ou VPL
* Usar enum para representar códigos de erro
* Documentar as funções no arquivo `.h`
* Criar biblioteca estática `.a`

---

## 🧮 Desafio Extra: Amortização

Implemente uma função que gere uma simulação de amortização usando parcelas fixas.

### Struct sugerida

```c
typedef struct {
    int month;
    double installment;
    double interest;
    double amortization;
    double remaining_balance;
} AmortizationRow;
```

### Função sugerida

```c
int generate_amortization_schedule(
    double principal,
    double monthly_rate,
    int months,
    AmortizationRow schedule[]
);
```

A função deve preencher o array recebido com os dados de cada mês.

#### Observação
A função deve retornar 0 em caso de sucesso e um código diferente de zero em caso de erro.

---

## 📦 Entrega

Você deve entregar:

* Repositório no GitHub
* README com instruções para compilar e executar
* Código-fonte organizado
* Exemplos de uso da biblioteca

---

## ▶️ Como rodar esperado

Explique no README:

1. Como compilar o projeto usando `make`
2. Como executar o programa de exemplo
3. Como rodar os testes, caso existam

Exemplo:

```bash
make
./bin/app
```

---

## 💬 Observações

* Não é necessário usar banco de dados
* Não é necessário criar interface gráfica
* Não é necessário usar bibliotecas externas
* Foque em clareza, organização e funcionamento
* O uso de IA é permitido, mas o candidato deve conseguir explicar o código

---

Boa sorte! 🚀
