/*
Atividade: SGT
Data de conclusão: 13/12/2024
Autor: Alexandre Aires Aleixo
*/

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Status { PENDENTE, EM_ANDAMENTO, CONCLUIDA };
enum Prioridade { BAIXA, MEDIA, ALTA };

struct Tarefa {
  int id;
  string titulo;
  string descricao;
  string dataCriacao;
  Status status;
  Prioridade prioridade;
};

vector<Tarefa> tarefas;

// Funções para manipulação de tarefas
void cadastrarTarefa();
void visualizarTarefas();
void buscarTarefaPorCodigo();
void editarTarefa();
void excluirTarefa();
void ordenarTarefas();
void salvarDadosEmArquivo();
void carregarDadosDoArquivo();
string obterDataAtual();
void exibirMenu();

int main() {
  carregarDadosDoArquivo();
  int opcao;

  do {
    exibirMenu();
    cin >> opcao;

    switch (opcao) {
    case 1:
      cadastrarTarefa();
      break;
    case 2:
      visualizarTarefas();
      break;
    case 3:
      buscarTarefaPorCodigo();
      break;
    case 4:
      editarTarefa();
      break;
    case 5:
      excluirTarefa();
      break;
    case 6:
      ordenarTarefas();
      break;
    case 7:
      salvarDadosEmArquivo();
      break;
    case 0:
      cout << "Saindo do programa..." << endl;
      break;
    default:
      cout << "Opção inválida! Tente novamente." << endl;
      break;
    }
  } while (opcao != 0);

  return 0;
}

void exibirMenu() {
  cout << "\nSistema de Gerenciamento de Tarefas" << endl;
  cout << "1. Cadastrar nova tarefa" << endl;
  cout << "2. Visualizar todas as tarefas" << endl;
  cout << "3. Buscar tarefa por código" << endl;
  cout << "4. Editar tarefa existente" << endl;
  cout << "5. Excluir tarefa" << endl;
  cout << "6. Ordenar tarefas" << endl;
  cout << "7. Salvar dados em arquivo" << endl;
  cout << "0. Sair" << endl;
  cout << "Escolha uma opção: ";
}

void cadastrarTarefa() {
  Tarefa novaTarefa;
  novaTarefa.id = tarefas.size() + 1; // ID único
  cout << "Título da tarefa: ";
  cin.ignore();
  getline(cin, novaTarefa.titulo);
  cout << "Descrição da tarefa: ";
  getline(cin, novaTarefa.descricao);
  novaTarefa.dataCriacao = obterDataAtual();
  novaTarefa.status = PENDENTE;
  int prioridade;
  cout << "Prioridade (0 - Baixa, 1 - Média, 2 - Alta): ";
  cin >> prioridade;
  novaTarefa.prioridade = static_cast<Prioridade>(prioridade);

  tarefas.push_back(novaTarefa);
  cout << "Tarefa cadastrada com sucesso!" << endl;
}

void visualizarTarefas() {
  if (tarefas.empty()) {
    cout << "Nenhuma tarefa cadastrada." << endl;
    return;
  }
  for (const auto &tarefa : tarefas) {
    cout << "ID: " << tarefa.id << ", Título: " << tarefa.titulo
         << ", Descrição: " << tarefa.descricao
         << ", Data de Criação: " << tarefa.dataCriacao << ", Status: "
         << (tarefa.status == PENDENTE       ? "Pendente"
             : tarefa.status == EM_ANDAMENTO ? "Em Andamento"
                                             : "Concluída")
         << ", Prioridade: "
         << (tarefa.prioridade == BAIXA   ? "Baixa"
             : tarefa.prioridade == MEDIA ? "Média"
                                          : "Alta")
         << endl;
  }
}

void buscarTarefaPorCodigo() {
  int codigo;
  cout << "Digite o código da tarefa: ";
  cin >> codigo;

  for (const auto &tarefa : tarefas) {
    if (tarefa.id == codigo) {
      cout << "Tarefa encontrada: " << tarefa.titulo << endl;
      return;
    }
  }
  cout << "Tarefa não encontrada." << endl;
}

void editarTarefa() {
  int codigo;
  cout << "Digite o código da tarefa a ser editada: ";
  cin >> codigo;

  for (auto &tarefa : tarefas) {
    if (tarefa.id == codigo) {
      cout << "Novo título (atual: " << tarefa.titulo << "): ";
      cin.ignore();
      getline(cin, tarefa.titulo);
      cout << "Nova descrição (atual: " << tarefa.descricao << "): ";
      getline(cin, tarefa.descricao);
      int prioridade;
      cout << "Nova prioridade (0 - Baixa, 1 - Média, 2 - Alta): ";
      cin >> prioridade;
      tarefa.prioridade = static_cast<Prioridade>(prioridade);
      cout << "Tarefa editada com sucesso!" << endl;
      return;
    }
  }
  cout << "Tarefa não encontrada." << endl;
}

void excluirTarefa() {
  int codigo;
  cout << "Digite o código da tarefa a ser excluída: ";
  cin >> codigo;

  auto it =
      remove_if(tarefas.begin(), tarefas.end(),
                [codigo](const Tarefa &tarefa) { return tarefa.id == codigo; });

  if (it != tarefas.end()) {
    tarefas.erase(it, tarefas.end());
    cout << "Tarefa excluída com sucesso!" << endl;
  } else {
    cout << "Tarefa não encontrada." << endl;
  }
}

void ordenarTarefas() {
  int opcao;
  cout << "Ordenar por (1 - Código, 2 - Prioridade): ";
  cin >> opcao;

  if (opcao == 1) {
    sort(tarefas.begin(), tarefas.end(),
         [](const Tarefa &a, const Tarefa &b) { return a.id < b.id; });
  } else if (opcao == 2) {
    sort(tarefas.begin(), tarefas.end(), [](const Tarefa &a, const Tarefa &b) {
      return a.prioridade < b.prioridade;
    });
  } else {
    cout << "Opção inválida!" << endl;
  }
}

void salvarDadosEmArquivo() {
  ofstream arquivo("tarefas.txt");
  if (!arquivo) {
    cout << "Erro ao abrir o arquivo para salvar." << endl;
    return;
  }

  for (const auto &tarefa : tarefas) {
    arquivo << tarefa.id << ";" << tarefa.titulo << ";" << tarefa.descricao
            << ";" << tarefa.dataCriacao << ";" << tarefa.status << ";"
            << tarefa.prioridade << endl;
  }
  arquivo.close();
  cout << "Dados salvos com sucesso!" << endl;
}

void carregarDadosDoArquivo() {
  ifstream arquivo("tarefas.txt");
  if (!arquivo) {
    cout << "Arquivo não encontrado. Iniciando com lista vazia." << endl;
    return;
  }

  Tarefa tarefa;
  string statusStr, prioridadeStr;
  while (arquivo.good()) {
    string linha;
    getline(arquivo, linha);
    if (linha.empty())
      continue;

    size_t pos = 0;
    tarefa.id = stoi(linha.substr(pos, linha.find(';', pos) - pos));
    pos = linha.find(';', pos) + 1;
    tarefa.titulo = linha.substr(pos, linha.find(';', pos) - pos);
    pos = linha.find(';', pos) + 1;
    tarefa.descricao = linha.substr(pos, linha.find(';', pos) - pos);
    pos = linha.find(';', pos) + 1;
    tarefa.dataCriacao = linha.substr(pos, linha.find(';', pos) - pos);
    pos = linha.find(';', pos) + 1;
    statusStr = linha.substr(pos, linha.find(';', pos) - pos);
    tarefa.status = static_cast<Status>(stoi(statusStr));
    pos = linha.find(';', pos) + 1;
    prioridadeStr = linha.substr(pos);
    tarefa.prioridade = static_cast<Prioridade>(stoi(prioridadeStr));

    tarefas.push_back(tarefa);
  }
  arquivo.close();
  cout << "Dados carregados com sucesso!" << endl;
}

string obterDataAtual() {
  time_t agora = time(0);
  tm *tlocal = localtime(&agora);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%d/%m/%y ", tlocal);
  return string(buffer);
}