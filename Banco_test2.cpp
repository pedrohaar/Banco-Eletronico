#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#define MAX_USERS 10

/////////////////////////////////////////////////////// ESTRUTURAS E OBJETOS
typedef struct {
    char usuario[50];
    char senha[50];
} Usuario;

typedef struct{
	
	int codigo;
	char nome[50];
	char email[50];
	char cpf[20];
	char dataNascimento[20];
	char dataCadastro[20];
	
} Cliente;

Usuario usuarios[MAX_USERS];
int contador_usuarios = 0;

typedef struct{
	
	int numero;
	Cliente cliente;
	float saldo;
	float limite;
	float saldoTotal; //saldo + limite
	
} Conta;

/////////////////////////////////////////////////////// CARREGANDO AS FUNÇÕES
void saveToFile();
void loadFromFile();

void menu();

void loadUsers();
int authenticateUser();
void addUser();
void saveUsers();

void mostrarSaldo();
void easterEgg();
void exibirCreditos();
void infoCliente(Cliente cliente);
void infoConta(Conta conta);
void criarConta();
void efetuarSaque();
void efetuarPagamento();
void efetuarDeposito();
void efetuarTransferencia();
void listarContas();
float atualizaSaldoTotal(Conta conta);
Conta buscarContaPorNumero(int numero);
void sacar(Conta conta, float valor);
void depositar(Conta conta, float valor);
void transferir(Conta conta_origem, Conta conta_destino, float valor);
void pagamento(Conta conta, float valor);
void help();

/////////////////////////////////////////////////////// INSIRINDO VALORES STATICOS PARAS AS VÁRIAVEIS
static Conta contas[50];
static int contador_contas = 0;
static int contador_clientes = 0;

/////////////////////////////////////////////////////// FUNÇÃO PRINCIPAL
int main(){
	int ys, tentativas = 0;
	loadFromFile();
	loadUsers();
	while(1){
        printf("===================================\n");
        printf("==- Seja Bem-Vindo ao PHP Bank! -==\n");
        printf("===================================\n\n");
        printf("Selecione uma opcao\n\n 1 - Logar\n 2 - Cadastre-se\n 3 - Sair\n\n-> ");
        scanf("%d", &ys);

        switch(ys) {
            case 1:
                if(tentativas < 3) {
                    if(authenticateUser() == 1) {
                        system("CLS");
                        menu();
                    }else {
                        printf("Falha na autenticacao. Tente novamente.\n");
                        Sleep(2000);
                        system("CLS");
                        tentativas++;
                    }
                }else {
                    printf("Numero de tentativas excedido. Saindo do programa.\n");
                    saveToFile();
                    Sleep(2000);
                    exit(0);
                }
                break;
            case 2:
                addUser();
                break;
            case 3:
                saveToFile();
                printf("Ate a proxima!\n");
                Sleep(2000);
                exit(0);
            default:
                printf("Opcao invalida!\n");
                Sleep(2000);
                system("CLS");
                break;
        }
    }
    return 0;
}

//////////////////////////////////////////////////// BANCO USUARIOS | Onde salva e carrega as informações
void loadUsers() {
    FILE* arq = fopen("users.txt", "r");
    if (arq == NULL) {
        return;
    }

    while (fscanf(arq, "%s %s\n", usuarios[contador_usuarios].usuario, usuarios[contador_usuarios].senha) == 2 && contador_usuarios < MAX_USERS) {
        contador_usuarios++;
    }

    fclose(arq);
}

void saveUsers() {
    FILE* arq = fopen("users.txt", "w");
    if (arq == NULL) {
        printf("Erro para salvar os usuarios. Saindo do programa...\n");
        Sleep(2000);
        exit(0);
    }

    for (int i = 0; i < contador_usuarios; i++) {
        fprintf(arq, "%s %s\n", usuarios[i].usuario, usuarios[i].senha);
    }

    fclose(arq);
}

void esconderSenha(char* senha){
	int i = 0; // Inicializa uma variável para contar a posição na senha
	while(1){ // Loop infinito para continuar lendo caracteres até que Enter seja pressionado
		char ch = getch();  // Obtém um caractere do usuário
		if(ch == '\r' || ch == '\n'){ // Verifica se o caractere é Enter (retorno de carro) ou Nova Linha
			break; // Se for Enter ou Nova Linha, sai do loop
		}else if(ch == 8 && i > 0){
			// Verifica se o caractere é Backspace (código ASCII 8) e se a posição da senha não é zero
        	// Se verdadeiro, retrocede um caractere, apaga o caractere e decrementa a posição na senha
			printf("\b \b");
			i--;
		}else{
			// Se não for Enter, Nova Linha ou Backspace, adiciona o caractere à senha,
        	// imprime um asterisco para ocultar o caractere real e incrementa a posição na senha
			senha[i++] = ch;
			printf("*");
		}
	}
	// Adiciona o caractere nulo no final da senha para indicar o término da string
	senha[i] = '\0';
	printf("\n");	
}

int authenticateUser() {
    char usuario[50];
    char senha[50];
    system("CLS");
    printf("===================================\n");
	printf("==- Seja Bem-Vindo ao PHP Bank! -==\n");
	printf("===================================\n\n");
    printf("[Usuario]: ");
    scanf("%s", usuario);
    printf("[Senha]: ");
    esconderSenha(senha);

    for (int i = 0; i < contador_usuarios; i++) {
        if (strcmp(usuario, usuarios[i].usuario) == 0 && strcmp(senha, usuarios[i].senha) == 0) {
            printf("\n\t\t\t== Autenticacao bem-sucedida. Seja bem-vindo, %s! ==\n", usuario);
            Sleep(2000);
            return 1; // SUCESSO NA AUTENTICIDADE
        } else{
        	return 0;
		}
    }

//    printf("Autenticacao falhou. Usuario ou senha invalido.\n");
//    Sleep(2000);
//    return 0; // FALHA NA AUTENTICIDADE
}

void addUser() {
    if (contador_usuarios < MAX_USERS) {
    	system("CLS");
    	printf("========================\n");
    	printf("======- Cadastro -======\n");
    	printf("========================\n");
        printf("Insira um usuario: ");
        scanf("%s", usuarios[contador_usuarios].usuario);

        printf("Insira uma senha: ");
        scanf("%s", usuarios[contador_usuarios].senha);

        contador_usuarios++;
        printf("Usuarios adicionado com sucesso.\n");
        saveUsers();
        Sleep(1000);
        system("CLS");
        main();
    } else {
        printf("Não é possível adicionar mais usuarios. O limite máximo foi atingido.\n");
    }
}

/////////////////////////////////////////////////////// BANCO CONTAS
void saveToFile() {
    FILE* arq = fopen("contas.txt", "w");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        printf("Arquivo nao encontrado ou erro ao ler arquivo. Saindo do programa...\n");
        Sleep(2000);
        exit(0);
    }

    for (int i = 0; i < contador_contas; i++) {
        fprintf(arq, "%d,%s,%s,%s,%s,%s,%d,%f,%f,%f\n",
                contas[i].cliente.codigo, contas[i].cliente.nome, contas[i].cliente.email,
                contas[i].cliente.cpf, contas[i].cliente.dataNascimento,
                contas[i].cliente.dataCadastro, contas[i].numero, contas[i].saldo,
                contas[i].limite, contas[i].saldoTotal);
    }

    fclose(arq);
}

void loadFromFile() {
    FILE* arq = fopen("contas.txt", "r");
    if (arq == NULL) {
        printf("Arquivo nao encontrado ou erro ao ler arquivo. Saindo do programa...\n");
        Sleep(2000);
        exit(0);
    }

    while (fscanf(arq, "%d,%49[^,],%49[^,],%19[^,],%19[^,],%19[^,],%d,%f,%f,%f\n",
                  &contas[contador_contas].cliente.codigo, contas[contador_contas].cliente.nome,
                  contas[contador_contas].cliente.email, contas[contador_contas].cliente.cpf,
                  contas[contador_contas].cliente.dataNascimento, contas[contador_contas].cliente.dataCadastro,
                  &contas[contador_contas].numero, &contas[contador_contas].saldo,
                  &contas[contador_contas].limite, &contas[contador_contas].saldoTotal) == 10) {
        contador_contas++;
    }

    fclose(arq);
}
/////////////////////////////////////////////////////// FUNÇÃO MENU
void menu(){
	system("CLS");
	int opcao = 0;
	
	printf("====================================                                                      0\n");
	printf("==============- PHP -===============\n");
	printf("==============- BANK -==============\n");
	printf("====================================\n\n");
	
	printf("Selecione uma opcao no menu:\n");
	printf("1 - Criar conta\n");
	printf("2 - Efetuar saque\n");
	printf("3 - Efetuar deposito\n");
	printf("4 - Efetuar transferencia\n");
	printf("5 - Listar contas\n");
	printf("6 - Efetuar pagamento\n");
	printf("7 - Help\n");
	printf("8 - Creditos do banco\n");
	printf("9 - Mostrar o saldo\n");
	printf("10 - Sair do Sistema\n\n-> ");
	
	
	scanf("%d", &opcao);
	getchar();
	switch(opcao){
		case 0:
			easterEgg();
			exit(0);
		case 1:
			criarConta();
			break;
		case 2:
			efetuarSaque();
			break;
		case 3:
			efetuarDeposito();
			break;
		case 4:
			efetuarTransferencia();
			break;
		case 5:
			listarContas();
			break;
		case 6:
			efetuarPagamento();
			break;
		case 7:
			help();
			break;
		case 8:
			exibirCreditos();
			break;
		case 9:
			mostrarSaldo();
    		break;
		case 10:
			saveToFile();
			printf("Ate a proxima!\n");
			Sleep(2000);
			exit(0);
		default:
			printf("Opcao invalida");
			Sleep(2000);
			system("CLS");
			menu();
			exit(0);
	}
}

/////////////////////////////////////////////////////// FUNÇÃO PARA IMPRIMIR AS INFORMAÇÕES DOS CLIENTES
void infoCliente(Cliente cliente){
	printf("Codigo: %d \nNome: %s \nData de Nascimento: %s \nCadastro: %s\n", 
			cliente.codigo, strtok(cliente.nome, "\n"), strtok(cliente.dataNascimento, "\n"), strtok(cliente.dataCadastro, "\n"));
}

/////////////////////////////////////////////////////// FUNÇÃO PARA INSIRIR AS INFORMAÇÕES DAS CONTAS DOS USUÁRIOS
// Exibe as informações detalhadas de uma conta
void infoConta(Conta conta){
	printf("Numero da Conta: %d \nCliente: %s \nData Nascimento: %s \nData Cadastro: %s \nSaldo Total: %.2f\n", 
			conta.numero, // Numero da conta
			strtok(conta.cliente.nome, "\n"), // Nome do cliente
			strtok(conta.cliente.dataNascimento, "\n"), // Data de Nascimento
			strtok(conta.cliente.dataCadastro, "\n"), // Data de cadastro
			conta.saldoTotal); // Saldo Total da conta
}

/////////////////////////////////////////////////////// FUNÇÃO PARA CRIAR A CONTA
void criarConta(){
	system("CLS");
	printf("=====================================\n");
    printf("==============- Criar -==============\n");
    printf("==============- Conta -==============\n");
    printf("=====================================\n\n");
	Cliente cliente;
	
	//Data de Cadastro
	char dia[3]; //06\0
	char mes[3]; //08
	char ano[5]; //2020
	
	char data_cadastro[20]; // Declaração de uma string para armazenar a data de cadastro com espaço para 20 caracteres.
	time_t t = time(NULL); // Obtém o tempo atual em segundos desde 1 de janeiro de 1970 (Epoch time).
	struct tm tm = *localtime(&t);  // Converte o tempo para a estrutura tm, que contém informações detalhadas sobre a data e hora local.
	
	// DIA
	if(tm.tm_mday < 10){
		sprintf(dia, "0%d", tm.tm_mday); // 1, 2, 3, 4, 5, 6, 7, 8, 9 -> 01/08/2020 CASO O DIA SEJA MENOR DE 10 FAZER COM QUE ACRESCENTE 0 ZERO NA FRENTE
	}else{
		sprintf(dia, "%d", tm.tm_mday);
	}
	
	// MES
	if((tm.tm_mon + 1) < 10){ // 01/8/2020 -> 01/08/2020 | insirindo o zero abaixo do 10
		sprintf(mes, "0%d", tm.tm_mon + 1);
	}else{
		sprintf(mes, "%d", tm.tm_mon + 1);
	}
	
	// ANO
	sprintf(ano, "%d", tm.tm_year + 1900);
	
	// STRING COPY
	strcpy(data_cadastro, "");
	
	// STRING CONCATENA
	strcat(data_cadastro, dia); /* dia */
	strcat(data_cadastro, "/"); /* dia/ */
	strcat(data_cadastro, mes); /* dia/mes */
	strcat(data_cadastro, "/"); /* dia/mes/ */
	strcat(data_cadastro, ano); /* dia/mes/ano */
	strcat(data_cadastro, "\0");
	strcpy(cliente.dataCadastro, data_cadastro);
	
	// CRIAR O CLIENTE	
	printf("Informe os dados do cliente:\n"); // Exibe uma mensagem para informar que o usuário deve inserir os dados do cliente.
	cliente.codigo = contador_clientes + 1; // Atribui um código ao cliente, aparentemente baseado em um contador de clientes.
	
	printf("Nome do cliente: \n# "); 
	fgets(cliente.nome, 50, stdin); // Solicita e armazena o nome do cliente. Usa fgets para evitar problemas com a entrada do usuário.
	
	printf("E-mail do cliente: \n# ");
	fgets(cliente.email, 50, stdin); // Solicita e armazena o e-mail do cliente.
	
	printf("CPF do cliente: \n# ");
	fgets(cliente.cpf, 20, stdin); // Solicita e armazena o CPF do cliente.
	
	printf("Data de nascimento do cliente: \n# ");
	fgets(cliente.dataNascimento, 20, stdin); // Solicita e armazena a data de nascimento do cliente.
	
	contador_clientes++;
	
	// CRIAR A CONTA
	contas[contador_contas].numero = contador_contas + 1; // Atribui um número à nova conta, incrementando o contador de contas.
	contas[contador_contas].cliente = cliente; // Atribui os dados do cliente à nova conta.
	// Define um saldo inicial de 10000.0 e um limite de 2000.0 para a nova conta.
	contas[contador_contas].saldo = 10000.0;
	contas[contador_contas].limite = 2000.0;
	// Calcula e atribui o saldo total da nova conta.
	contas[contador_contas].saldoTotal = atualizaSaldoTotal(contas[contador_contas]);
	
	// Exibe mensagens informando que a conta foi criada com sucesso e imprime os dados da nova conta.
	printf("Conta criado com sucesso!\n\n");
	printf("Dados da conta criada: \n\n");
	infoConta(contas[contador_contas]);
	
	// Incrementa o contador de contas para indicar que uma nova conta foi adicionada ao sistema.
	contador_contas++;
	
	//saveToFile();
	Sleep(4000);
	menu();
	
}

/////////////////////////////////////////////////////// FUNÇÃO PARA ATUALIZAR O SALDO TOTAL
float atualizaSaldoTotal(Conta conta){
	return conta.saldo + conta.limite;
}

/////////////////////////////////////////////////////// FUNÇÃO PARA VALIDAR 
Conta buscarContaPorNumero(int numero){
	Conta c;
	if(contador_contas > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == numero){
				c = contas[i];
			}
		}
	}
	return c;
}

//////////////////////////////////////////////////////// FUNÇÃO LOGICA PARA A FUNÇAO DE SAQUE
void sacar(Conta conta, float valor){
	if(valor > 1500){
		printf("Saque nao autorizado. Limite permitido e de R$ 1500.00.\n");
	} else if(valor > 0 && conta.saldoTotal >= valor){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				if(contas[i].saldo >= valor){
					contas[i].saldo = contas[i].saldo - valor;
					contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
					printf("Saque efetuado com sucesso!\n"); 
				} else{
					float restante = contas[i].saldo - valor;
					contas[i].limite = contas[i].limite + restante;
					contas[i].saldo = 0.0;
					contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
					printf("Saque efetuado com sucesso!\n");
				}
			}
		}
	} else{
		printf("Fundos indisponivel. Tente novamente.");
	}
}

//////////////////////////////////////////////////////// FUNÇÃO LOGICA PARA A FUNÇAO DE DEPOSITO
void depositar(Conta conta, float valor){
	if(valor > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				contas[i].saldo = contas[i].saldo + valor;
				contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
				printf("Deposito efetuado com sucesso!\n");
			}
		}
	}else{
		printf("Erro ao efetuar deposito. Tente novamente.\n");
	}
}

//////////////////////////////////////////////////////// FUNÇÃO LOGICA PARA A FUNÇAO DE TRANSFERENCIA
void transferir(Conta conta_origem, Conta conta_destino, float valor){
	if(valor > 0 && conta_origem.saldoTotal >= valor){
		for(int co = 0; co < contador_contas; co++){
			if(contas[co].numero == conta_origem.numero){
				for(int cd = 0; cd < contador_contas; cd++){
					if(contas[cd].numero == conta_destino.numero){
						if(contas[co].saldo >= valor){
							contas[co].saldo = contas[co].saldo - valor;
							contas[cd].saldo = contas[cd].saldo + valor;
							contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
							contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
							printf("Transferencia realizada com sucesso!\n");
						}else{
							float restante = contas[co].saldo - valor;
							contas[co].limite = contas[co].limite + restante;
							contas[co].saldo = 0.0;
							contas[cd].saldo = contas[cd].saldo + valor;
							contas[co].saldoTotal = atualizaSaldoTotal(contas[co]);
							contas[cd].saldoTotal = atualizaSaldoTotal(contas[cd]);
							printf("Transferencia realizada com sucesso!\n");
						}
					}
				}
			}
		}
	}else{
		printf("Transferencia nao realizada. Tente novamente.\n");
	}
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EFETUAR SAQUE
void efetuarSaque(){
	system("CLS");
	printf("===================================\n");
    printf("=========- Efetuar Saque -=========\n");
    printf("===================================\n");
	if(contador_contas > 0){
		int numero;
		printf("Informe o numero da conta: \n-> ");
		scanf("%d", &numero);
		
		Conta conta = buscarContaPorNumero(numero);
		
		if(conta.numero == numero){
			float valor;
			printf("Informe o valor do saque: \n-> ");
			scanf("%f", &valor);
			
			sacar(conta, valor);
		}else{
			printf("Nao foi encontrada uma conta com o numero %d\n", numero);
		}
	}else{
		printf("Ainda nao existe conta para saque.\n");
	}
	Sleep(2000);
	menu();
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EFETUAR DEPOSITO
void efetuarDeposito(){
	system("CLS");
	printf("===================================\n");
    printf("========- Efetuar Deposito -=======\n");
    printf("===================================\n");
	if(contador_contas > 0){
		int numero;
		printf("Informe o numero da conta: \n-> ");
		scanf("%d", &numero);
		
		Conta conta = buscarContaPorNumero(numero);
		
		if(conta.numero == numero){
			float valor;
			printf("Informe o valor do deposito: \n-> ");
			scanf("%f", &valor);
			
			depositar(conta, valor);
		}else{
			printf("Não foi encontrada uma conta com o número %d\n", numero);
		}
	}else{
		printf("Ainda nao existe conta para deposito.\n");
	}
	Sleep(2000);
	menu();
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EFETUAR PAGAMENTO
void pagamento(Conta conta, float valor){
	if(valor > 0){
		for(int i = 0; i < contador_contas; i++){
			if(contas[i].numero == conta.numero){
				contas[i].saldo = contas[i].saldo + valor;
				contas[i].saldoTotal = atualizaSaldoTotal(contas[i]);
				printf("Pagamento efetuado com sucesso!\n");
			}
		}
	}else{
		printf("Erro ao efetuar deposito. Tente novamente.\n");
	}	
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EFETUAR PAGAMENTO
void efetuarPagamento(){
	system("CLS");
	printf("===================================\n");
    printf("=======- Efetuar Pagamento -=======\n");
    printf("===================================\n");
	if(contador_contas > 0){
		int numero;
		printf("Informe o numero da conta: \n-> ");
		scanf("%d", &numero);
		
		Conta conta = buscarContaPorNumero(numero);
		
		if(conta.numero == numero){
			float valor;
			printf("Informe o valor do pagamento: \n-> ");
			scanf("%f", &valor);
			
			pagamento(conta, valor);
		}else{
			printf("Não foi encontrada uma conta com o número %d\n", numero);
		}
	}else{
		printf("Ainda nao existe conta para deposito.\n");
	}
	Sleep(2000);
	menu();
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EFETUAR TRANSFERENCIA
void efetuarTransferencia(){
	system("CLS");
	printf("===================================\n");
    printf("=====- Efetuar Transferencia -=====\n");
    printf("===================================\n");
	if(contador_contas > 0){
		int numero_o, numero_d;
		printf("Informe o numero da sua conta: \n-> ");
		scanf("%d", &numero_o);
		
		Conta conta_o = buscarContaPorNumero(numero_o);
		
		if(conta_o.numero == numero_o){
			printf("Informe o numero da conta do destino: \n-> ");
			scanf("%d", &numero_d);
			
			Conta conta_d = buscarContaPorNumero(numero_d);
			
			if(conta_d.numero == numero_d){
				float valor;
				printf("Informe o valor para transferencia: \n-> ");
				scanf("%f", &valor);
				
				transferir(conta_o, conta_d, valor);
			}else{
				printf("A conta destino com numero %d não foi encontrada. \n", numero_d);
			}
			
		}else{
			printf("Não foi encontrada uma conta com o número %d\n", numero_o);
		}
	}else{
		printf("Ainda nao existe conta para transferencia.");
	}
	Sleep(2000);
	menu();
}

////////////////////////////////////////////////////////  FUNÇÃO PARA LISTAR AS CONTAS
void listarContas(){
	system("CLS");
	if(contador_contas > 0){
		for(int i = 0; i < contador_contas; i++){
			infoConta(contas[i]);
			printf("\n");
			
		}
	}else{
		printf("Nao existem contas cadastradas ainda. \n");
	}
	printf("\n\nPressione Enter para retornar ao menu principal...\n");
    getchar();
	menu();
}

//////////////////////////////////////////////////////// FUNÇÃO DE AJUDA
void help() {
	system("CLS");
    printf("====================================\n");
    printf("==============- PHP -===============\n");
    printf("==============- BANK -==============\n");
    printf("====================================\n\n");

    printf("Bem-vindo ao sistema de ajuda do PHP Bank.\n");
    printf("Aqui estao algumas opcoes disponiveis no menu:\n\n");

    printf("1 - Criar conta: Permite criar uma nova conta no banco.\n");
    printf("2 - Efetuar saque: Permite realizar um saque em uma conta existente.\n");
    printf("3 - Efetuar deposito: Permite realizar um deposito em uma conta existente.\n");
    printf("4 - Efetuar transferencia: Permite transferir dinheiro entre duas contas.\n");
    printf("5 - Listar contas: Exibe informacoes sobre todas as contas cadastradas.\n");
    printf("6 - Efetuar pagamento: Permite efetuar um pagamento em uma conta existente.\n");
    printf("7 - Ajuda: Exibe informacoes sobre as opcoes disponiveis no menu.\n");
    printf("8 - Creditos do banco: Exibe informacoes sobre o banco.\n");
    printf("9 - Sair do sistema: Encerra a execucao do programa.\n\n");

    printf("Lembre-se de seguir as instrucoes apresentadas durante a execucao do programa.\n");
    printf("Para informacoes mais detalhadas, consulte o manual do usuario.\n");

    printf("\nPressione Enter para retornar ao menu principal...\n");
    getchar();
    system("CLS");
    menu();
}

//////////////////////////////////////////////////////// FUNÇÃO PARA EXIBICAO DE CREDITOS
void exibirCreditos() {
    system("CLS");
    printf("====================================\n");
    printf("==============- PHP -===============\n");
    printf("==============- BANK -==============\n");
    printf("====================================\n\n");

    printf("Desenvolvido por:\n");
    printf("Pedro Haar\n");
    printf("pedro.haar@gmail.com\n\n");

    printf("\nPressione Enter para retornar ao menu principal...\n");
    getchar();
    
    menu();
}

//////////////////////////////////////////////////////// EASTER EGG
void easterEgg() {
    system("CLS");
    printf("====================================\n");
    printf("==============- PHP -===============\n");
    printf("==============- BANK -==============\n");
    printf("====================================\n\n");
    printf("Parabens, voce encontrou o Easter Egg!\n");
    printf("Espero que tenha se divertido :)\n");
    
   	printf(
        "\n"
        "	                    ____\n"
        "                 _.' :  `._\n"
        "             .-.'`.  ;   .'`.-.\n"
        "    __      / : ___\\ ;  /___ ; \\      __\n"
        "  ,'_ \"\"--.:__;\".-.\";: :\".-.\":__;.--\" _`,\n"
        "  :' `.t\"\"--.. '<@.`;_  ',@>` ..--\"\"j.' `;\n"
        "       `:-.._J '-.-'L__ `-- ' L_..-;'\n"
        "         \"-.__ ;  .-\"  \"-.  : __.-\"\n"
        "             L ' /.------.\\ ' J\n"
        "              \"-.   \"--\"   .-\"\n"
        "             __.l\"-:_JL_;-\";.__\n"
        "          .-j/'.;  ;\"\"\"\"  / .'\"-.\n"
        "        .' /:`. \"-.:     .-\" .';  `.\n"
        "     .-\"  / ;  \"-. \"-..-\" .-\"  :    \"-.\n"
        ".+\"-.  : :      \"-.__.-\"      ;-._   \\\n"
        "; \\  `.; ;                    : : \"+. ;\n"
        ":  ;   ; ;                    : ;  : \\:\n"
        ": `.\"-; ;  ;                  :  ;   ,/;\n"
        ";    -; :  :                ;  : .-\"'  :\n"
        ":\\     \\  : ;             : \\.-\"      :\n"
        " ;`.    \\  ; :            ;.'_..--  / ;\n"
        " :  \"-.  \"-:  ;          :/.\"      .'  :\n"
        "   \\       .-`.:\\        /t-\"\"  \":-+.   :\n"
        "    `.  .-\"    `l    __/ /`. :  ; ; \\  ;\n"
        "      \\   .-\" .-\"-.-\"  .' .'j \\  /   ;/\n"
        "       \\ / .-\"   /.     .'.' ;_:'    ;\n"
        "        :-\"\"-.`./-.'     /    `.___.'\n"
        "              \\ `t  ._  /  bug :F_P:\n"
        "               \"-.t-._:'\n"
        "\n" 
    );
    
    printf("Pressione qualquer tecla para continuar...\n");
    getch();  // Aguarda o usuário pressionar uma tecla
}
///////////////////////////////////////////////////////////////////// FUNÇÃO PARA MOSTRAR O SALDO
void mostrarSaldo() {
    system("CLS");
    printf("===================================\n");
    printf("=========== Mostrar Saldo =========\n");
    printf("===================================\n");

    if (contador_contas > 0) {
        int numero;
        printf("Informe o numero da conta: \n-> ");
        scanf("%d", &numero);

        Conta conta = buscarContaPorNumero(numero);

        if (conta.numero == numero) {
        	system("CLS");
    		printf("===================================\n");
   			printf("=============- Saldo -=============\n");
   			printf("===================================\n\n");
    		printf("Numero da Conta: %d\n", conta.numero);
    		printf("Cliente: %s\n", strtok(conta.cliente.nome, "\n"));
    		printf("Saldo Total: %.2f\n\n", conta.saldoTotal);
        } else {
            printf("Nao foi encontrada uma conta com o numero %d\n", numero);
        }
    } else {
        printf("Ainda nao existe conta para mostrar saldo.\n");
    }
    
    printf("\nPressione Enter para retornar ao menu principal...\n");
    getch();
    
    menu();
}
