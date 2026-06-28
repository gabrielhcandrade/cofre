# 🔐 Cofre de Senhas Local 

Este projeto é uma implementação em **C** de um **Cofre de Senhas Local**, desenvolvido para demonstrar na prática os conceitos de **segurança de dados em repouso (*at-rest data encryption*)**. O programa permite adicionar e listar senhas de forma segura, garantindo que os dados nunca sejam gravados em disco em texto plano.

---

# 🛡️ Conceitos de Segurança Aplicados

O desenvolvimento do projeto foi baseado em quatro pilares fundamentais de segurança.

1. **Autenticação ("O que você sabe")**
   - O acesso ao cofre depende de uma senha mestra fornecida pelo usuário.
   - Essa senha funciona como a chave principal para proteger os dados armazenados.

2. **Hashing e Salting**
   - A senha mestra não é armazenada diretamente.
   - O sistema simula a derivação de uma chave utilizando a senha combinada com um *salt*, reduzindo a eficácia de ataques de dicionário.

3. **Criptografia Simétrica (Dados em Repouso)**
   - Antes de serem gravados no arquivo `cofre_seguro.bin`, todos os dados passam por uma cifra simétrica utilizando **XOR**.
   - Os dados permanecem descriptografados apenas enquanto estão na memória RAM durante a execução do programa.

4. **Princípio do Menor Privilégio (Segurança de Memória)**
   - Informações sensíveis, como a senha mestra e as chaves derivadas, permanecem na memória apenas pelo tempo necessário.
   - A função `limpar_memoria()` sobrescreve esses dados com zeros antes do encerramento do programa.

---

# 🏗️ Arquitetura do Sistema

O fluxo de funcionamento do sistema pode ser representado da seguinte forma:

```text
                Senha Mestra
                     │
                     ▼
              Derivação de Chave
             (Hash + Salt Simulado)
                     │
                     ▼
        Dados em Memória (RAM)
                     │
             Criptografia XOR
                     │
                     ▼
             cofre_seguro.bin
```

Durante a leitura dos dados, o processo ocorre no sentido inverso: os dados criptografados são carregados do arquivo, descriptografados temporariamente na memória e exibidos ao usuário.

---

# 🚀 Como Compilar

### Linux / Windows (GCC)

```bash
gcc -Wall -o cofre cofre.c
```

---

# ▶️ Como Executar

### Windows (PowerShell)

```powershell
.\cofre.exe
```

### Linux

```bash
./cofre
```

Ao iniciar, o programa solicitará a criação ou autenticação utilizando a senha mestra antes de liberar o acesso ao cofre.

---

# 📋 Como Utilizar

Após informar a senha mestra, será apresentado o menu principal com as seguintes opções.

### Listar Senhas

Exibe todas as credenciais armazenadas.

Os dados são descriptografados apenas durante a leitura e permanecem em memória somente enquanto necessários.

---

### Adicionar Nova Senha

Permite cadastrar:

- Site
- Usuário
- Senha

Após o cadastro, todas as informações são imediatamente criptografadas e gravadas no arquivo:

```text
cofre_seguro.bin
```

---

### Sair

Ao encerrar o programa:

- a senha mestra é removida da memória;
- as chaves derivadas são sobrescritas;
- buffers sensíveis são limpos através da função `limpar_memoria()`.

---

# 🔍 Verificando a Persistência

Após adicionar uma ou mais credenciais, será criado o arquivo:

```text
cofre_seguro.bin
```

Esse arquivo contém todas as entradas do cofre armazenadas de forma criptografada.

Ao executar novamente o programa utilizando a mesma senha mestra, as credenciais poderão ser recuperadas e exibidas corretamente.

---

## Inspeção Direta do Arquivo

É possível verificar que os dados não estão armazenados em texto plano.

### Windows (PowerShell)

```powershell
Select-String -Path .\cofre_seguro.bin -Pattern "gmail"
```

ou

```powershell
Select-String -Path .\cofre_seguro.bin -Pattern "senha"
```

Nenhuma informação legível deverá ser encontrada.

### Linux

```bash
strings cofre_seguro.bin
```

A saída deverá conter apenas caracteres sem significado ou dados aparentemente aleatórios, demonstrando que as credenciais foram armazenadas de forma criptografada.

---

# 📂 Estrutura do Projeto

```text
.
├── cofre.c              # Código-fonte principal
├── cofre_seguro.bin     # Arquivo contendo as credenciais criptografadas
└── README.md
```

---

# 🛠️ Funcionalidades Implementadas

- ✅ Autenticação por senha mestra
- ✅ Derivação de chave utilizando Hash + Salt (simulado)
- ✅ Criptografia simétrica utilizando XOR
- ✅ Armazenamento seguro em arquivo binário
- ✅ Leitura e descriptografia das credenciais
- ✅ Limpeza de memória de informações sensíveis
- ✅ Persistência dos dados entre execuções

---

# 📚 Objetivo

Este projeto foi desenvolvido com fins didáticos para demonstrar conceitos fundamentais de **Segurança da Informação**, incluindo:

- Segurança de dados em repouso (*At-Rest Data Encryption*);
- Autenticação por senha;
- Hashing e Salting;
- Criptografia simétrica;
- Gerenciamento seguro de memória;
- Persistência segura de credenciais em arquivos binários.
