
#define m 10

typedef struct ArvB{ 
	long int chave[2*m-1];
	int num_chave; 
	struct ArvB* filho[2*m];
	int folha; 
}ArvB;
ArvB* RemoveIntermediario(ArvB* pagina, int posicao); 
ArvB* CriaPagina(int EhFolha){ 
	ArvB* pag = malloc(sizeof(ArvB)); 
	pag->num_chave = 0;
	pag->folha = EhFolha;
	return pag;
}
ArvB* QuebraPagina(ArvB* pagina,int QualFilho, ArvB* filho ){ 
	ArvB* novo = CriaPagina(filho->folha); 
	novo->num_chave = m-1; 
	int i;
	for(i = 0;i<m-1;i++){
		novo->chave[i] = filho->chave[i+m];
	}
	if(filho->folha == 0){ 
		for(i=0;i<m;i++){
			novo->filho[i] = filho->filho[i+m];
		}
	}
	filho->num_chave = m-1;
	for( i = pagina->num_chave; i >= QualFilho+1; i--){
		pagina->filho[i+1] = pagina->filho[i];
	}
	pagina->filho[QualFilho+1] = novo;
	for(i = pagina->num_chave-1;i >= QualFilho; i--){ 
		pagina->chave[i+1] = pagina->chave[i];
	}
	pagina->chave[QualFilho] = filho->chave[m-1];
	pagina->num_chave++; 
	return pagina;
}
ArvB* InsereSecundaria(ArvB* pagina, long int chave){
	if(pagina->folha == 1){
		int i = pagina->num_chave-1;
		while(i>=0 && pagina->chave[i] > chave){ 
			pagina->chave[i+1] = pagina->chave[i];
			i--;
		}
		pagina->chave[i+1] = chave;
		pagina->num_chave++;
		return pagina;
	}
	else{ 
		int i = pagina->num_chave-1;
		while(i >= 0 && pagina->chave[i] > chave){
			i--;
		}
		if(pagina->filho[i+1]->num_chave == 2*m-1){
			pagina = QuebraPagina(pagina,i+1,pagina->filho[i+1]);
			if(pagina->chave[i+1]<chave){
				i++;
			}
		}
		pagina->filho[i+1] = InsereSecundaria(pagina->filho[i+1],chave);
		return pagina;
	}
}
ArvB* Insere(ArvB* pagina,long int chave){
	if(pagina == NULL){
		ArvB* raiz = CriaPagina(1);
		raiz->chave[0] = chave;
		raiz->num_chave = 1; 
		return raiz;
	}
	else{
		if(pagina->num_chave == 2*m-1){ 
			ArvB* NovaRaiz = CriaPagina(0);
			NovaRaiz->filho[0] = pagina;
			NovaRaiz = QuebraPagina(NovaRaiz,0,pagina);
			int i = 0;
			if(NovaRaiz->chave[0]<chave){ 
				i++;
			}
			NovaRaiz->filho[i] = InsereSecundaria(NovaRaiz->filho[i],chave); 
			return NovaRaiz;
		}
		else{
			pagina = InsereSecundaria(pagina,chave); 
			return pagina;
		}
	}
}
ArvB* RemoveFolha(ArvB* pagina, int posicao){
	int i;
	for(i=posicao+1;i<pagina->num_chave;i++){
		pagina->chave[i-1]= pagina->chave[i];
	}
	pagina->num_chave--; 
	return pagina;
}
long int MaiorDosMenores(ArvB* pagina, int posicao){
	ArvB* caminhador = pagina->filho[posicao]; 
	while(caminhador->folha==0){
		caminhador = caminhador->filho[caminhador->num_chave];
	}
	int i = caminhador->num_chave;
	long int MDM = caminhador->chave[i-1];
	return MDM;
}
long int MenorDosMaiores(ArvB* pagina, int posicao){
	ArvB* caminhador = pagina->filho[posicao+1];
	while(caminhador->folha==0){
		caminhador = caminhador->filho[0];
	}
	long int MDM = caminhador->chave[0]; 
	return MDM;
}
ArvB* PegaFilhoRicoEsquerdo(ArvB* pagina,int posicao){ 
	ArvB* irmaorico = pagina->filho[posicao-1];
	ArvB* filhopobre = pagina->filho[posicao]; 
	int i;
	for(i=(filhopobre->num_chave)-1;i>=0;i--){ 
		filhopobre->chave[i+1] = filhopobre->chave[i];
	}
	if(filhopobre->folha==0){ 
		for(i=filhopobre->num_chave;i>=0;i--){
			filhopobre->filho[i+1] = filhopobre->filho[i]; 
		}
	}
	filhopobre->chave[0] = pagina->chave[posicao-1];
	filhopobre->num_chave++;
	if(filhopobre->folha == 0){  
		filhopobre->filho[0] = irmaorico->filho[irmaorico->num_chave];
	}
	pagina->chave[posicao-1] = irmaorico->chave[irmaorico->num_chave-1]; 
	irmaorico->num_chave--;
	return pagina;
}
ArvB* PegaFilhoRicoDireito(ArvB* pagina,int posicao){
	ArvB* irmaorico = pagina->filho[posicao+1];
	ArvB* filhopobre = pagina->filho[posicao];
	filhopobre->chave[filhopobre->num_chave] = pagina->chave[posicao];
	if(filhopobre->folha == 0){
		filhopobre->filho[(filhopobre->num_chave)+1] = irmaorico->filho[0];
	}
	pagina->chave[posicao] = irmaorico->chave[0];
	int i;
	for(i=1;i<irmaorico->num_chave;i++){
		irmaorico->chave[i-1] = irmaorico->chave[i];
	}
	if(irmaorico->folha==0){
		for(i=1;i<=irmaorico->num_chave;i++){
			irmaorico->filho[i-1]=irmaorico->filho[i];
		}
	}
	filhopobre->num_chave += 1;
	irmaorico->num_chave -= 1;
	return pagina;
}
ArvB* Concatena(ArvB* pagina, int posicao){
	ArvB* irmao = pagina->filho[posicao+1];
	ArvB* filhoB = pagina->filho[posicao]; 
	filhoB->chave[m-1]= pagina->chave[posicao];
	int i;
	for (i=0;i<irmao->num_chave;i++){
		filhoB->chave[i+m] = irmao->chave[i];
	}
	if(filhoB->folha==0){ 
		for(i=0;i<irmao->num_chave;i++){
			filhoB->filho[i+m] = irmao->filho[i];
		}
	}
	for(i=posicao+1;i<pagina->num_chave;i++){ 
		pagina->chave[i-1]= pagina->chave[i];
	}
	for(i=posicao+2;i<=pagina->num_chave;i++){
		pagina->filho[i-1] = pagina->filho[i];
	}
	filhoB->num_chave += (irmao->num_chave)+1;
	pagina->num_chave--; 
	free(irmao); 
	return pagina;
}
ArvB* Rotacao(ArvB* pagina,int posicao){ 
	if(posicao!=0 && pagina->filho[posicao-1]->num_chave >= m){
        pagina = PegaFilhoRicoEsquerdo(pagina,posicao);
        return pagina;
	}
	else if(posicao!=pagina->num_chave && pagina->filho[posicao+1]->num_chave >= m){ 
		pagina = PegaFilhoRicoDireito(pagina,posicao);
		return pagina;
	}
	else{ 
		if(posicao!=pagina->num_chave){ 
			pagina = Concatena(pagina,posicao);
		}
		else{
			pagina = Concatena(pagina,posicao-1);
		}
		return pagina;
	}
}

ArvB* RemoveGeral(ArvB* pagina, long int chave){
	int i=0;
	while(i<pagina->num_chave && pagina->chave[i]<chave){
		i++;
	}
	if(i<pagina->num_chave && pagina->chave[i]==chave){ 
		if(pagina->folha == 1){
			pagina = RemoveFolha(pagina,i);
		}
		else{ 
            pagina = RemoveIntermediario(pagina,i);
		}
	}
	else{
		if(pagina->folha == 1){
			return pagina;
		}
		int flag = 0;
		if(i == pagina->num_chave){
			flag = 1;
		}
		if(pagina->filho[i]->num_chave < m){ 
			pagina = Rotacao(pagina,i);
		}
		if(flag == 1 && i > pagina->num_chave){
			pagina->filho[i-1] = RemoveGeral(pagina->filho[i-1],chave); 
		}
		else{
			pagina->filho[i] = RemoveGeral(pagina->filho[i],chave);
		}
	}
	return pagina;
}
ArvB* RemoveIntermediario(ArvB* pagina, int posicao){
	int i = pagina->chave[posicao];
	if(pagina->filho[posicao]->num_chave >m-1){ 
		long int MDM = MaiorDosMenores(pagina,posicao);
		pagina->chave[posicao] = MDM;
		pagina->filho[posicao] = RemoveGeral(pagina->filho[posicao],MDM);
	}
	else if(pagina->filho[posicao+1]->num_chave > m-1){ 
		long int MDM = MenorDosMaiores(pagina,posicao);
		pagina->chave[posicao] = MDM;
		pagina->filho[posicao+1] = RemoveGeral(pagina->filho[posicao+1],MDM);
	}
	else{ 
		pagina = Concatena(pagina,posicao);
		pagina->filho[posicao] = RemoveGeral(pagina->filho[posicao],i);
	}
	return pagina;
}
ArvB* RemoveRaiz(ArvB* raiz, long int chave){
	if(raiz == NULL){ 
		printf("arvore vazia\n");
		return raiz;
	}
	raiz = RemoveGeral(raiz,chave); 
	if(raiz->num_chave == 0){ 
		ArvB* pagina = raiz;
		if(raiz->folha == 1){ 
			raiz = NULL;
		}
		else{ 
			raiz = raiz->filho[0];
		}
		free(pagina); 
	}
	return raiz;
}
void PrintaTudo(ArvB* pagina){ 
	int i;
	if(pagina!=NULL){
        for(i=0;i<pagina->num_chave;i++){
            if(pagina->folha==0){
                PrintaTudo(pagina->filho[i]);
            }
            printf("%ld\n",pagina->chave[i]);
        }
        if(pagina->folha==0){
            PrintaTudo(pagina->filho[i]);
        }
    }
}

ArvB* VerificaInsere(ArvB* Antigo,ArvB* pagina,long int chave){ 
	int p=0; 
	if(pagina!=NULL){ 
	for(p=0;p<pagina->num_chave;p++){ 
		if(pagina->folha==0){ 
			Antigo = VerificaInsere(Antigo,pagina->filho[p],chave);
		}
		if((pagina->chave[p] - pagina->chave[p]%10000000) <= (chave - chave%10000000)){ 
			if(pagina->chave[p]%100 < chave%100){  
				Antigo = Insere(Antigo,pagina->chave[p]); 
			}
		}
	}
	if(pagina->folha==0){ 
		Antigo = VerificaInsere(Antigo,pagina->filho[p],chave);
		}
	}
	return Antigo; 
}
ArvB* VerificaRemove(ArvB* raiz,ArvB* pagina,long int chave){
	int p=0;
	if(pagina!=NULL){
        while(p<pagina->num_chave){
            if(pagina->folha==0){
            raiz = VerificaRemove(raiz,pagina->filho[p],chave);
            }
            if((pagina->chave[p] - pagina->chave[p]%10000000) <= (chave - chave%10000000)){
                if(pagina->chave[p]%100 < chave%100){
                    raiz = RemoveRaiz(raiz,pagina->chave[p]);
                    p--; 
                }
            }
            p++;
        }
            if(pagina->folha==0){
                raiz = VerificaRemove(raiz,pagina->filho[p],chave);
        }
	}
	return raiz; 
}
int Busca01(ArvB* pagina,long int chave){
	int i = 0;
	int j = 0;
	if(pagina!=NULL){
        for(i=0;i<pagina->num_chave;i++){
            if(pagina->folha==0){
                j+= Busca01(pagina->filho[i],chave); 
            }
            if(pagina->chave[i] - pagina->chave[i]%10000000 == chave - chave%10000000){ 
                if(pagina->chave[i]%100 == chave%100){ 
                    return 1;

                }
            }
    	}
    	if(pagina->folha==0){
            j+= Busca01(pagina->filho[i],chave); 
        }
	}
	
		return j;

}
int BuscaElemento(ArvB* pagina,long int chave,int ano ){ 
	int i = 0;
	int caminhador = 0; 
	if(pagina!=NULL){ 
        for(i=0;i<pagina->num_chave;i++){ 
            if(pagina->folha==0){
                caminhador+= BuscaElemento(pagina->filho[i],chave,ano); 
            }
            if(pagina->chave[i] - pagina->chave[i]%10000000 == chave*10000000){ 
                if(pagina->chave[i]%100 == ano){ 
                    return (pagina->chave[i]%10000000 - pagina->chave[i]%100)/100;
                }
            }
        }
        if(pagina->folha==0){
            caminhador+= BuscaElemento(pagina->filho[i],chave,ano);
        }
	}
	return caminhador;
}
ArvB* Incrementa(ArvB* pagina,long int chave){ 
	int i = 0;
	while(i<pagina->num_chave && chave - chave%10000000 > pagina->chave[i] - pagina->chave[i]%10000000){
		i++;
	}
	if(pagina->chave[i] - pagina->chave[i]%10000000 == chave - chave%10000000){
        if(pagina->chave[i]%100 == chave%100){
            pagina->chave[i] += (chave%10000000 - chave%100); 
            return pagina; 
        }
	}
	else if (pagina->folha != 1){
		pagina->filho[i] = Incrementa(pagina->filho[i],chave);
	}
	else{
		return pagina;
	}
}
ArvB* InsereVista(ArvB* atual,long int chave){

    int flag = 0; 
    atual = VerificaRemove(atual,atual,chave);
    flag = Busca01(atual,chave); 
    if(flag == 0){ 
        atual = Insere(atual,chave); 
    }
    else{ 
        atual = Incrementa(atual,chave);
    }

    return atual; 
}
long int VerificaParcela(ArvB* antiga,long int chave, int parcela){ 
    long int Parcelado = ((chave%10000000 - chave%100)/100)/parcela; 
	int ano = chave%100; 
	long int chaveParcelada = (chave - chave%10000000) + (Parcelado*100) + ano; 
	int i ; 
	int mes = (chave%1000000000 - chave%10000000)/10000000; 
	for(i=1;i<parcela;i++){
		if(i+mes > 12){ 
			chaveParcelada = chaveParcelada - 110000000; 
			chaveParcelada++; 
			mes = 1; 
		}
		else{
            chaveParcelada = chaveParcelada+10000000; 

		}
	}
	return chaveParcelada; 
}
ArvB* InsereParcelado(ArvB* atual,long int chave, int parcela){
	long int Parcelado = ((chave%10000000 - chave%100)/100)/parcela;
	int ano = chave%100; 
	long int chaveParcelada = (chave - chave%10000000) + (Parcelado*100) + ano;
	int i ;
	int mes = (chave%1000000000 - chave%10000000)/10000000; 
	atual = InsereVista(atual,chaveParcelada); 
	for(i=1;i<parcela;i++){
		if(i+mes > 12){
			chaveParcelada = chaveParcelada - 110000000;
			chaveParcelada++; 
			mes = 1; 
			atual = InsereVista(atual,chaveParcelada ); 
		}
		else{ 
            chaveParcelada = chaveParcelada+10000000;
			atual = InsereVista(atual,chaveParcelada); 
		}
	}
	return atual; 
}
int BuscaMes(ArvB* pagina,int mes,int ano){
	long int chave = mes + 100; 
	int i;
	int soma = 0; 
	for(i=0;i<30;i++){
		soma+= BuscaElemento(pagina,chave,ano); 
		chave+= 100; 
	}
	return soma; 
}
int BuscaPeriodo(ArvB* pagina, long int chave,int dias,int ano){ 
	int i;
	int soma = 0; 
	for(i=0;i<dias;i++){
		if(chave - chave%100 > 3000){ 
			chave++;
			if(chave%100>12){
				ano++; 
				chave = chave - 12; 
			}
			chave = chave - 3000;
		}
		soma+= BuscaElemento(pagina,chave,ano);
		chave+=100;
	}
	return soma;
}
int main(){ 
   
	ArvB *antigaP;
	ArvB *atualP; 
	ArvB *antigaR;
	ArvB *atualR; 
	antigaR = NULL;
	atualR = NULL;
	antigaP = NULL;
	atualP = NULL;
	long int ultimaParcela;
	long int chave;
	int i;
	int OP;
	int OP2;
	int OP3;
	int OP4;
	while(OP!=5){ 
        printf("DIGITE A OPERACAO DESEJADA\n");
        printf("1)PAGAMENTO\n");
        printf("2)RECEBIMENTO\n");
        printf("3)CONSULTA ANO ATUAL\n");
        printf("4)CONSULTA ANOS ANTERIORES\n");
        printf("5)FINALIZAR OPERACOES\n");
        scanf("%d",&OP);
        if(OP == 1){
            printf("1)PARCELADO\n");
            printf("2)A VISTA\n");
            scanf("%d",&OP2);
            if(OP2 == 1){
                printf("ESCOLHA A PARCELA, ATE 6 PARCELAS\n");
                scanf("%d",&OP2);
					 while(OP2<1 || OP2>6){
						 printf("Escolha um numero entre 1 e 6\n");
						 scanf("%d",&OP2);
					 }
                printf("INSIRA DIA MES 5 DIGITOS DE PAGAMENTO E ANO NO MESMO NUMERO, EXEMPLO: 1010000100\n");
                scanf("%ld",&chave);
                ultimaParcela = VerificaParcela(antigaP,chave,OP2);
					 antigaP = VerificaInsere(antigaP,atualP,ultimaParcela);
                atualP = InsereParcelado(atualP,chave,OP2);
            }
            else if(OP2 == 2){
                printf("INSIRA MES ANO 5 DIGITOS DE PAGAMENTO E ANO NO MESMO NUMERO, EXEMPLO: 1010000100\n");
                scanf("%ld",&chave);
                antigaP = VerificaInsere(antigaP,atualP,chave);
                atualP = InsereVista(atualP,chave);
            }
        }
        else if(OP == 2){
            printf("1)PARCELADO\n");
            printf("2)A VISTA\n ");
            scanf("%d",&OP2);
            if(OP2 == 1){
                printf("ESCOLHA A PARCELA, ATE 6 PARCELAS\n");
                scanf("%d",&OP2);
					while(OP2<1 || OP2>6){
						 printf("Escolha um numero entre 1 e 6\n");
						 scanf("%d",&OP2);
					 }
                printf("INSIRA DIA MES 5 DIGITOS DE PAGAMENTO E ANO NO MESMO NUMERO, EXEMPLO: 1010000100\n");
                scanf("%ld",&chave);
                ultimaParcela = VerificaParcela(antigaR,chave,OP2);
					 antigaR = VerificaInsere(antigaR,atualR,ultimaParcela);
                atualR = InsereParcelado(atualR,chave,OP2);
            }
            else if(OP2 == 2){
                printf("INSIRA MES ANO 5 DIGITOS DE PAGAMENTO E ANO NO MESMO NUMERO, EXEMPLO: 1010000100\n");
                scanf("%ld",&chave);
                antigaR = VerificaInsere(antigaR,atualR,chave);
                atualR = InsereVista(atualR,chave);
            }
        }
        else if(OP == 3){
            printf("1) DIA\n");
            printf("2) MES\n");
            printf("3) PERIODO\n");
            scanf("%d",&OP2);
            if(OP2 == 1){
                printf("INSIRA O DIA E MES DESEJADO EXEMPLO: 2104 \n");
                scanf("%d",&OP2);
					while(OP2<101 || (OP2-OP2%100) > 3000 || OP2%100 > 12){
						printf("Dia e Mes Invalido, por favor digite um numero valido\n");
						scanf("%d",&OP2);
					}
                printf("INSIRA O ANO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaElemento(atualR,OP2,OP3),BuscaElemento(atualP,OP2,OP3),(BuscaElemento(atualR,OP2,OP3)-BuscaElemento(atualP,OP2,OP3)));
            }
            else if(OP2 == 2){
                printf("INSIRA O MES DESEJADO\n");
                scanf("%d",&OP2);
					 while(OP2<1 || OP2 > 12){
						 printf("Digite um mes entre 1 e 12\n");
						 scanf("%d",&OP2);
					 }
                printf("INSIRA O ANO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaMes(atualR,OP2,OP3),BuscaMes(atualP,OP2,OP3),(BuscaMes(atualR,OP2,OP3)-BuscaMes(atualP,OP2,OP3)));
            }
            else if(OP2 == 3){
                printf("INSIRA A DATA INICIAL PARA BUSCAR EXEMPLO : 2607\n");
                scanf("%d",&OP2);
					 while(OP2<101 || (OP2-OP2%100) > 3000 || OP2%100 > 12){
						printf("Dia e Mes Invalido, por favor digite um numero valido\n");
						scanf("%d",&OP2);
					}
                printf("INSIRA O ANO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("INSIRA O PERIODO EM DIAS QUE GOSTARIA DE BUSCAR\n");
                scanf("%d",&OP4);
					 while(OP4<1){
					 	printf("Digite um periodo maior que 0\n");
						scanf("%d",&OP4);
					 }
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaPeriodo(atualR,OP2,OP4,OP3),BuscaPeriodo(atualP,OP2,OP4,OP3),BuscaPeriodo(atualR,OP2,OP4,OP3)-BuscaPeriodo(atualP,OP2,OP4,OP3));
            }
        }
        else if(OP == 4){
            printf("1) DIA\n");
            printf("2) MES\n");
            printf("3) PERIODO\n");
            scanf("%d",&OP2);
            if(OP2 == 1){
                printf("INSIRA O DIA E MES DESEJADO EXEMPLO: 2104 \n");
                scanf("%d",&OP2);
					 while(OP2<101 || (OP2-OP2%100) > 3000 || OP2%100 > 12){
						printf("Dia e Mes Invalido, por favor digite um numero valido\n");
						scanf("%d",&OP2);
					}
                printf("INSIRA O ANO DESEJADO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaElemento(antigaR,OP2,OP3),BuscaElemento(antigaP,OP2,OP3),(BuscaElemento(antigaR,OP2,OP3)-BuscaElemento(antigaP,OP2,OP3)));
            }
            else if(OP2 == 2){
                printf("INSIRA O MES DESEJADO\n");
                scanf("%d",&OP2);
					while(OP2<1 || OP2 > 12){
						 printf("Digite um mes entre 1 e 12\n");
						 scanf("%d",&OP2);
					 }
                printf("INSIRA O ANO DESEJADO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaMes(antigaR,OP2,OP3),BuscaMes(antigaP,OP2,OP3),(BuscaMes(antigaR,OP2,OP3)-BuscaMes(antigaP,OP2,OP3)));
            }
            else if(OP2 == 3){
                printf("INSIRA A DATA INICIAL PARA BUSCAR EXEMPLO : 2607\n");
                scanf("%d",&OP2);
					while(OP2<101 || (OP2-OP2%100) > 3000 || OP2%100 > 12){
						printf("Dia e Mes Invalido, por favor digite um numero valido\n");
						scanf("%d",&OP2);
					}
                printf("INSIRA O ANO DESEJADO\n");
                scanf("%d",&OP3);
					 while(OP3<0 || OP3>99){
						printf("Por favor digite um ano entre 0 e 99\n");
						scanf("%d",&OP3);
					}
                printf("INSIRA O PERIODO EM DIAS QUE GOSTARIA DE BUSCAR\n");
                scanf("%d",&OP4);
					 while(OP4<1){
					 	printf("Digite um periodo maior que 0\n");
						scanf("%d",&OP4);
					 }
                printf("RECEBEU: %d PAGOU: %d SALDO: %d\n",BuscaPeriodo(antigaR,OP2,OP4,OP3),BuscaPeriodo(antigaP,OP2,OP4,OP3),BuscaPeriodo(antigaR,OP2,OP4,OP3)-BuscaPeriodo(antigaP,OP2,OP4,OP3));
            }
        }
    }
}
