#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARQUIVO "pessoas.dat"

struct Pessoa {
    char nome[50];
    char cpf[11];
    int idade;
    char email[100];
};

void inserirInfo(struct Pessoa *s) {
    printf("Nome: ");
    scanf(" %[^\n]", s->nome);
    printf("CPF: ");
    scanf(" %[^\n]", s->cpf);
    printf("Idade: ");
    scanf("%d]", &s->idade);
    printf("E-mail: ");
    scanf(" %[^\n]", s->email);
}

void editarInfo(struct Pessoa *s) {
    printf("Nome: ");
    scanf(" %[^\n]", s->nome);
    printf("Idade: ");
    scanf("%d]", &s->idade);
    printf("E-mail: ");
    scanf(" %[^\n]", s->email);
}

void mostrarInfo(struct Pessoa s) {
    printf("Nome: %s\n", s.nome);
    printf("CPF: %s\n", s.cpf);
    printf("Idade: %d\n", s.idade);
    printf("E-mail: %s\n", s.email);
}

void menu() {
    printf("Para cadastrar uma nova pessoa, digite 1\n");
    printf("Para ver todas as pessoas cadastradas, digite 2\n");
    printf("Para verificar os dados de uma pessoa cadastrada, digite 3\n");
    printf("Para atualizar os dados uma pessoa cadastrada, digite 4\n");
    printf("Para remover uma pessoa cadastrada, digite 5\n");
    printf("Para sair do programa, digite 0\n");
}

int cpfJaCadastrado(struct Pessoa pessoas[], int num_pessoas, const char *cpf) {
    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            return 1;
        }
    }
    return 0;
}

int buscarPessoaPorCpf(struct Pessoa pessoas[], int num_pessoas, const char *cpf) {
    for (int i = 0; i < num_pessoas; i++) {
        if (strcmp(pessoas[i].cpf, cpf) == 0) {
            return i;
        }
    }
    return -1;
}

int carregarDados(struct Pessoa pessoas[]) {
    FILE *f = fopen(ARQUIVO, "rb");
    if (f == NULL) {
        return 0;
    }
    int num_pessoas = 0;
    while (fread(&pessoas[num_pessoas], sizeof(struct Pessoa), 1, f)) {
        num_pessoas++;
    }
    fclose(f);
    return num_pessoas;
}

void salvarDados(struct Pessoa pessoas[], int num_pessoas) {
    FILE *f = fopen(ARQUIVO, "wb");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }
    fwrite(pessoas, sizeof(struct Pessoa), num_pessoas, f);
    fclose(f);
}

int main() {
    struct Pessoa pessoas[100];
    int opcao;
    char cpfBusca[11];
    int num_pessoas = carregarDados(pessoas);

    while (1) {
        menu();
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("Saindo do programa...\n");
            break;

        } else if (opcao == 1) {
                printf("\nCadastro de nova pessoa:\n");
                struct Pessoa novaPessoa;
                inserirInfo(&novaPessoa);
                if (cpfJaCadastrado(pessoas, num_pessoas, novaPessoa.cpf)) {
                    printf("\nNao e possivel cadastrar duas pessoas com o mesmo CPF.\n\n");
                } else {
                    pessoas[num_pessoas] = novaPessoa;
                    num_pessoas++;
                    printf("\nCadastro de %s realizado com sucesso!\n\n", novaPessoa.nome);
                    salvarDados(pessoas, num_pessoas);
                }
        } else if (opcao == 2) {
            if (num_pessoas == 0) {
                printf("Nenhuma pessoa cadastrada ainda.\n");
            } else {
                printf("\nLista de pessoas cadastradas:\n\n");
                for (int i = 0; i < num_pessoas; i++) {
                    mostrarInfo(pessoas[i]);
                    printf("\n");
                }
            }

        } else if (opcao == 3) {
            if (num_pessoas == 0) {
                printf("Nenhuma pessoa cadastrada para verificar.\n");
            } else {
                printf("Digite o CPF da pessoa que deseja verificar: ");
                scanf(" %[^\n]", cpfBusca);
                int encontrado = 0;
                for (int i = 0; i < num_pessoas; i++) {
                    if (strcmp(pessoas[i].cpf, cpfBusca) == 0) {
                        printf("\n");
                        mostrarInfo(pessoas[i]);
                        printf("\n");
                        encontrado = 1;
                        break;
                    }
                }
                if (!encontrado) {
                    printf("\nCPF %s nao encontrado.\n\n", cpfBusca);
                }
            }
        } else if (opcao == 4) {
            if (num_pessoas == 0) {
                printf("Nenhuma pessoa cadastrada para atualizar.\n");
            } else {
                char cpfBusca[11];
                printf("Digite o CPF da pessoa que deseja atualizar: ");
                scanf(" %[^\n]", cpfBusca);
                int i = buscarPessoaPorCpf(pessoas, num_pessoas, cpfBusca);
                if (i != -1) {
                    printf("\n");
                    editarInfo(&pessoas[i]);
                    salvarDados(pessoas, num_pessoas);
                    printf("\nDados atualizados com sucesso!\n\n");
                } else {
                    printf("\nPessoa com CPF %s não encontrada.\n\n", cpfBusca);
                }
            }
        } else if (opcao == 5) {
            if (num_pessoas == 0) {
                printf("Nenhuma pessoa cadastrada para remover.\n");
            } else {
                char cpfBusca[11];
                printf("Digite o CPF da pessoa que deseja remover: ");
                scanf(" %[^\n]", cpfBusca);

                int i = buscarPessoaPorCpf(pessoas, num_pessoas, cpfBusca);
                if (i != -1) {
                    for (int i; i < num_pessoas - 1; i++) {
                        pessoas[i] = pessoas[i + 1];
                    }
                    num_pessoas--;
                    salvarDados(pessoas, num_pessoas);
                    printf("\nCadastro removido com sucesso!\n\n");
                } else {
                    printf("\nCPF nao encontrado.\n\n");
                }
            }
        } else {
            printf("\nOpcao invalida, tente novamente.\n\n");
        }
    }
    return 0;
}
