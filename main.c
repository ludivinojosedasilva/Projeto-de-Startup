#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_EVENTOS 100
#define MAX_PARTICIPANTES 500

// Estruturas para Evento e Participante
typedef struct {
    int codigo;
    char nome[100];
    char data[20];
    char local[100];
    char categoria[50];
    int vagas;
    int vagasDisponiveis;
} Evento;

typedef struct {
    int codigo;
    char nome[100];
    char email[100];
    char instituicao[100];
    int eventoCodigo;
} Participante;

// Variáveis globais para armazenamento temporário
Evento eventos[MAX_EVENTOS];
Participante participantes[MAX_PARTICIPANTES];
int numEventos = 0;
int numParticipantes = 0;
int ultimoCodigoEvento = 0;        // Armazena o último código gerado para evento
int ultimoCodigoParticipante = 0;  // Armazena o último código gerado para participante

// Funções para manipulação de eventos
void cadastrarEvento();
void listarEventos();
void editarEvento();
void excluirEvento();
void consultarEvento();

// Funções para manipulação de participantes
void cadastrarParticipante();
void listarParticipantes();
void cancelarInscricao();
void consultarInscricao();

// Funções para geração de relatórios
void gerarRelatorioEventosPorCategoria();
void gerarRelatorioParticipantesPorInstituicao();
void gerarRelatorioEventosMaisInscritos();

// Funções de persistência de dados
void salvarEventos();
void carregarEventos();
void salvarParticipantes();
void carregarParticipantes();

// Funções utilitárias
int encontrarEventoPorCodigo(int codigo);
int encontrarParticipantePorCodigo(int codigo);
void menu();

// Implementação das funções principais
int main() {
    carregarEventos();
    carregarParticipantes();
    menu();
    return 0;
}

void gerarRelatorioEventosCSV();
void gerarRelatorioParticipantesCSV();

// Adicionando novas opções ao menu
void menu() {
    int opcao;
    do {
        printf("\n--- Sistema de Gerenciamento de Eventos Tecnológicos ---\n");
        printf("1. Cadastrar Evento\n");
        printf("2. Listar Eventos\n");
        printf("3. Editar Evento\n");
        printf("4. Excluir Evento\n");
        printf("5. Cadastrar Participante\n");
        printf("6. Listar Participantes\n");
        printf("7. Cancelar Inscrição\n");
        printf("8. Consultar Evento\n");
        printf("9. Relatórios\n");
        printf("10. Exportar Relatório de Eventos (CSV)\n");
        printf("11. Exportar Relatório de Participantes (CSV)\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarEvento(); break;
            case 2: listarEventos(); break;
            case 3: editarEvento(); break;
            case 4: excluirEvento(); break;
            case 5: cadastrarParticipante(); break;
            case 6: listarParticipantes(); break;
            case 7: cancelarInscricao(); break;
            case 8: consultarEvento(); break;
            case 9: gerarRelatorioEventosPorCategoria(); break;
            case 10: gerarRelatorioEventosCSV(); break;
            case 11: gerarRelatorioParticipantesCSV(); break;
            case 0: printf("Saindo do sistema.\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

// Função para exportar eventos em formato CSV
void gerarRelatorioEventosCSV() {
    FILE *file = fopen("eventos.csv", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo CSV de eventos.\n");
        return;
    }

    fprintf(file, "Código,Nome,Data,Local,Categoria,Vagas,Vagas Disponíveis\n");
    for (int i = 0; i < numEventos; i++) {
        fprintf(file, "%d,%s,%s,%s,%s,%d,%d\n",
                eventos[i].codigo,
                eventos[i].nome,
                eventos[i].data,
                eventos[i].local,
                eventos[i].categoria,
                eventos[i].vagas,
                eventos[i].vagasDisponiveis);
    }

    fclose(file);
    printf("Relatório de eventos exportado para 'eventos.csv' com sucesso.\n");
}

// Função para exportar participantes em formato CSV
void gerarRelatorioParticipantesCSV() {
    FILE *file = fopen("participantes.csv", "w");
    if (file == NULL) {
        printf("Erro ao criar o arquivo CSV de participantes.\n");
        return;
    }

    fprintf(file, "Código,Nome,Email,Instituição,Código do Evento\n");
    for (int i = 0; i < numParticipantes; i++) {
        fprintf(file, "%d,%s,%s,%s,%d\n",
                participantes[i].codigo,
                participantes[i].nome,
                participantes[i].email,
                participantes[i].instituicao,
                participantes[i].eventoCodigo);
    }

    fclose(file);
    printf("Relatório de participantes exportado para 'participantes.csv' com sucesso.\n");
}

// Função para cadastrar um novo evento
void cadastrarEvento() {
    if (numEventos >= MAX_EVENTOS) {
        printf("Limite de eventos atingido.\n");
        return;
    }
    Evento e;
    e.codigo = ++ultimoCodigoEvento; // Gerar código único para o evento
    printf("Nome do Evento: ");
    scanf(" %[^\n]", e.nome);
    printf("Data (DD/MM/AAAA): ");
    scanf(" %[^\n]", e.data);
    printf("Local: ");
    scanf(" %[^\n]", e.local);
    printf("Categoria: ");
    scanf(" %[^\n]", e.categoria);
    printf("Número de Vagas: ");
    scanf("%d", &e.vagas);
    e.vagasDisponiveis = e.vagas;
    
    eventos[numEventos++] = e;
    salvarEventos();
    printf("Evento cadastrado com sucesso! Código do evento: %d\n", e.codigo);
}

void listarEventos() {
    printf("\n--- Lista de Eventos ---\n");
    for (int i = 0; i < numEventos; i++) {
        printf("Código: %d\n", eventos[i].codigo);
        printf("Nome: %s\n", eventos[i].nome);
        printf("Data: %s\n", eventos[i].data);
        printf("Local: %s\n", eventos[i].local);
        printf("Categoria: %s\n", eventos[i].categoria);
        printf("Vagas Disponíveis: %d\n", eventos[i].vagasDisponiveis);
        printf("---------------------------\n");
    }
}

void editarEvento() {
    int codigo;
    printf("Digite o código do evento a ser editado: ");
    scanf("%d", &codigo);
    int index = encontrarEventoPorCodigo(codigo);

    if (index == -1) {
        printf("Evento não encontrado.\n");
        return;
    }

    printf("Editando evento %d\n", codigo);
    printf("Novo nome (atual: %s): ", eventos[index].nome);
    scanf(" %[^\n]", eventos[index].nome);
    printf("Nova data (atual: %s): ", eventos[index].data);
    scanf(" %[^\n]", eventos[index].data);
    printf("Novo local (atual: %s): ", eventos[index].local);
    scanf(" %[^\n]", eventos[index].local);
    printf("Nova categoria (atual: %s): ", eventos[index].categoria);
    scanf(" %[^\n]", eventos[index].categoria);
    printf("Novo número de vagas (atual: %d): ", eventos[index].vagas);
    scanf("%d", &eventos[index].vagas);

    // Atualiza o número de vagas disponíveis de acordo com as novas vagas
    eventos[index].vagasDisponiveis = eventos[index].vagas;
    salvarEventos();
    printf("Evento atualizado com sucesso!\n");
}

void excluirEvento() {
    int codigo;
    printf("Digite o código do evento a ser excluído: ");
    scanf("%d", &codigo);
    int index = encontrarEventoPorCodigo(codigo);

    if (index == -1) {
        printf("Evento não encontrado.\n");
        return;
    }

    for (int i = index; i < numEventos - 1; i++) {
        eventos[i] = eventos[i + 1];
    }
    numEventos--;
    salvarEventos();
    printf("Evento excluído com sucesso!\n");
}

void consultarEvento() {
    int codigo;
    printf("Digite o código do evento para consulta: ");
    scanf("%d", &codigo);
    int index = encontrarEventoPorCodigo(codigo);

    if (index == -1) {
        printf("Evento não encontrado.\n");
        return;
    }

    printf("Código: %d\n", eventos[index].codigo);
    printf("Nome: %s\n", eventos[index].nome);
    printf("Data: %s\n", eventos[index].data);
    printf("Local: %s\n", eventos[index].local);
    printf("Categoria: %s\n", eventos[index].categoria);
    printf("Vagas Disponíveis: %d\n", eventos[index].vagasDisponiveis);
    printf("---------------------------\n");
}

// Declaração da função de validação de e-mail
bool validarEmail(const char *email);

// Função para cadastrar participante com validação de e-mail
void cadastrarParticipante() {
    if (numParticipantes >= MAX_PARTICIPANTES) {
        printf("Limite de participantes atingido.\n");
        return;
    }

    Participante p;
    p.codigo = ++ultimoCodigoParticipante; // Gerar código único para o participante

    printf("Nome do Participante: ");
    scanf(" %[^\n]", p.nome);

    // Solicitar e validar e-mail
    do {
        printf("Email: ");
        scanf(" %[^\n]", p.email);
        if (!validarEmail(p.email)) {
            printf("E-mail inválido. Tente novamente.\n");
        }
    } while (!validarEmail(p.email));

    printf("Instituição: ");
    scanf(" %[^\n]", p.instituicao);

    printf("Código do Evento: ");
    scanf("%d", &p.eventoCodigo);

    int eventoIndex = encontrarEventoPorCodigo(p.eventoCodigo);
    if (eventoIndex == -1) {
        printf("Evento não encontrado.\n");
        return;
    }

    if (eventos[eventoIndex].vagasDisponiveis <= 0) {
        printf("Não há vagas disponíveis para este evento.\n");
        return;
    }

    eventos[eventoIndex].vagasDisponiveis--;
    participantes[numParticipantes++] = p;
    salvarParticipantes();
    salvarEventos();
    printf("Participante cadastrado com sucesso! Código do participante: %d\n", p.codigo);
}

// Função para validar o formato do e-mail
bool validarEmail(const char *email) {
    const char *arroba = strchr(email, '@');
    const char *ponto = strrchr(email, '.');

    // Verifica se há '@' e '.' e se estão em posições válidas
    if (!arroba || !ponto || arroba == email || ponto < arroba || ponto == arroba + 1 || *(ponto + 1) == '\0') {
        return false;
    }
    return true;
}


void listarParticipantes() {
    printf("\n--- Lista de Participantes ---\n");
    for (int i = 0; i < numParticipantes; i++) {
        printf("Código: %d\n", participantes[i].codigo);
        printf("Nome: %s\n", participantes[i].nome);
        printf("Email: %s\n", participantes[i].email);
        printf("Instituição: %s\n", participantes[i].instituicao);
        printf("Código do Evento: %d\n", participantes[i].eventoCodigo);
        printf("---------------------------\n");
    }
}

void cancelarInscricao() {
    int codigo;
    printf("Digite o código do participante para cancelar inscrição: ");
    scanf("%d", &codigo);
    int index = encontrarParticipantePorCodigo(codigo);

    if (index == -1) {
        printf("Participante não encontrado.\n");
        return;
    }

    int eventoIndex = encontrarEventoPorCodigo(participantes[index].eventoCodigo);
    if (eventoIndex != -1) {
        eventos[eventoIndex].vagasDisponiveis++;
    }

    for (int i = index; i < numParticipantes - 1; i++) {
        participantes[i] = participantes[i + 1];
    }
    numParticipantes--;
    salvarParticipantes();
    salvarEventos();
    printf("Inscrição cancelada com sucesso!\n");
}

void salvarEventos() {
    FILE *file = fopen("eventos.dat", "wb");
    if (file == NULL) {
        printf("Erro ao salvar eventos.\n");
        return;
    }
    fwrite(&numEventos, sizeof(int), 1, file);
    fwrite(&ultimoCodigoEvento, sizeof(int), 1, file); // Salvar o último código gerado
    fwrite(eventos, sizeof(Evento), numEventos, file);
    fclose(file);
}

void carregarEventos() {
    FILE *file = fopen("eventos.dat", "rb");
    if (file == NULL) {
        return; // Arquivo ainda não existe
    }
    fread(&numEventos, sizeof(int), 1, file);
    fread(&ultimoCodigoEvento, sizeof(int), 1, file); // Carregar o último código gerado
    fread(eventos, sizeof(Evento), numEventos, file);
    fclose(file);
}

void salvarParticipantes() {
    FILE *file = fopen("participantes.dat", "wb");
    if (file == NULL) {
        printf("Erro ao salvar participantes.\n");
        return;
    }
    fwrite(&numParticipantes, sizeof(int), 1, file);
    fwrite(&ultimoCodigoParticipante, sizeof(int), 1, file); // Salvar o último código gerado
    fwrite(participantes, sizeof(Participante), numParticipantes, file);
    fclose(file);
}

void carregarParticipantes() {
    FILE *file = fopen("participantes.dat", "rb");
    if (file == NULL) {
        return; // Arquivo ainda não existe
    }
    fread(&numParticipantes, sizeof(int), 1, file);
    fread(&ultimoCodigoParticipante, sizeof(int), 1, file); // Carregar o último código gerado
    fread(participantes, sizeof(Participante), numParticipantes, file);
    fclose(file);
}

int encontrarEventoPorCodigo(int codigo) {
    for (int i = 0; i < numEventos; i++) {
        if (eventos[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

int encontrarParticipantePorCodigo(int codigo) {
    for (int i = 0; i < numParticipantes; i++) {
        if (participantes[i].codigo == codigo) {
            return i;
        }
    }
    return -1;
}

void gerarRelatorioEventosPorCategoria() {
    printf("\n--- Relatório de Eventos por Categoria ---\n");
    for (int i = 0; i < numEventos; i++) {
        printf("Categoria: %s\n", eventos[i].categoria);
        printf("Evento: %s\n", eventos[i].nome);
        printf("---------------------------\n");
    }
}

