mateus tree
mateus man
vini
salvatp
mateus

int ValidaJogo(FILE *arq, TpJogo elem){
    TpJogo reg;
    while(!feof(arq) & !(stricmp(reg.login) == 0 && stricmp(reg.palavra,elem.palavra) == 0))
        fread(&reg,sizeof(TpJogo),1,arq);

    if(!feof(arq))
        return 1;
    return -1;
}