#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRIES 50
#define ARQUIVO_COFRE "cofre_seguro.bin"
#define SALT_FIXO "s@lt_d1d4t1c0"

typedef struct {
    char site[50];
    char username[50];
    char password[50];
} Entry;

// Função para limpar dados sensíveis da RAM (Princípio do Menor Privilégio)
void limpar_memoria(void *ptr, size_t tamanho) {
    volatile char *p = (volatile char *)ptr;
    while (tamanho--) {
        *p++ = 0;
    }
}

// 1. Módulo de Autenticação: Simulação de Hashing com Salt
void hash_master_password(const char* pass, const char* salt, char* output_hash) {
    snprintf(output_hash, 128, "%s%s_hashed", pass, salt);
}

// 2. Integração Criptográfica: Simulação de Criptografia (Dados em Repouso)
void aes_encrypt_decrypt_file(Entry* ram_buffer, int count, const char* key, int encrypt) {
    size_t key_len = strlen(key);
    if (key_len == 0) return;

    for (int i = 0; i < count; i++) {
        char* entry_bytes = (char*)&ram_buffer[i];
        for (size_t j = 0; j < sizeof(Entry); j++) {
            entry_bytes[j] ^= key[j % key_len]; 
        }
    }
}

// Salvar no disco
void salvar_cofre(Entry* dados, int count, const char* chave_derivada) {
    FILE *f = fopen(ARQUIVO_COFRE, "wb");
    if (!f) return;

    fwrite(&count, sizeof(int), 1, f); // Salva a quantidade de entradas
    
    // Criptografa o buffer ANTES de salvar no disco
    aes_encrypt_decrypt_file(dados, count, chave_derivada, 1);
    fwrite(dados, sizeof(Entry), count, f);
    
    // Descriptografa de volta para a RAM continuar operando
    aes_encrypt_decrypt_file(dados, count, chave_derivada, 0); 
    fclose(f);
}

// Carregar do disco
int carregar_cofre(Entry* dados, const char* chave_derivada) {
    FILE *f = fopen(ARQUIVO_COFRE, "rb");
    if (!f) return 0; // Arquivo não existe ainda

    int count = 0;
    fread(&count, sizeof(int), 1, f);
    fread(dados, sizeof(Entry), count, f);
    fclose(f);

    // Decifra o buffer na RAM
    aes_encrypt_decrypt_file(dados, count, chave_derivada, 0);
    return count;
}

int main() {
    char master_pass[50];
    char chave_derivada[128];
    Entry buffer_ram[MAX_ENTRIES];
    int total_entradas = 0;
    int opcao;

    printf("=== Cofre de Senhas Local ===\n");
    printf("Digite a Senha Mestra: ");
    scanf("%49s", master_pass);

    // 1. Derivação de Chave via Hash/Salt
    hash_master_password(master_pass, SALT_FIXO, chave_derivada);
    
    // Limpa a senha mestra imediatamente (Princípio do Menor Privilégio)
    limpar_memoria(master_pass, sizeof(master_pass));

    // 2. Carregar arquivo e decifrar em RAM
    total_entradas = carregar_cofre(buffer_ram, chave_derivada);
    printf("Cofre carregado com sucesso. %d entrada(s).\n", total_entradas);

    // 3. Mostrar menu de senhas
    do {
        printf("\n1 - Listar Senhas");
        printf("\n2 - Adicionar Nova Senha");
        printf("\n0 - Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("\n--- Suas Senhas ---\n");
            for (int i = 0; i < total_entradas; i++) {
                printf("[%d] Site: %s | User: %s | Pass: %s\n", 
                    i+1, buffer_ram[i].site, buffer_ram[i].username, buffer_ram[i].password);
            }
        } 
        else if (opcao == 2) {
            if (total_entradas < MAX_ENTRIES) {
                printf("Site: "); scanf("%49s", buffer_ram[total_entradas].site);
                printf("Usuario: "); scanf("%49s", buffer_ram[total_entradas].username);
                printf("Senha: "); scanf("%49s", buffer_ram[total_entradas].password);
                total_entradas++;
                salvar_cofre(buffer_ram, total_entradas, chave_derivada);
                printf("Entrada salva de forma criptografada no disco!\n");
            } else {
                printf("Cofre cheio!\n");
            }
        }
    } while (opcao != 0);

    // 4. Limpar RAM antes de sair
    limpar_memoria(buffer_ram, sizeof(buffer_ram));
    limpar_memoria(chave_derivada, sizeof(chave_derivada));
    
    printf("\nRAM limpa com sucesso. Encerrando programa.\n");
    return 0;
}