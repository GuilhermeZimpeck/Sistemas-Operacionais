# wgrep

O programa **wgrep** e é uma variação simplificada do comando UNIX **grep**. Esse programa lê um arquivo linha por linha buscando por um padrão(palavra) fornecido pelo usuário. Somente as linhas que contiverem o padrão serão exibidas. 

Esse é um exemplo de como o usuário poderia utilizar o **wgrep** para imprimir linhas que contenham a palavra **"foo"** no arquivo **teste.txt**:

```
prompt> ./wgrep foo teste.txt
this line has foo in it
so does this foolish line; do you see where?
even this line, which has barfood in it, will be printed.
```
##
**Detalhes**

* Os argumentos de linha de comando para o programa **wgrep** são um termo de busca e zero ou mais arquivos, onde o termo será buscado (ou seja, é possível buscar em mais de um arquivo). O processamento será feito linha a linha 
  para cada cada arquivo passado como argumento via linha de comando, onde a linha será impressa somente se ela contiver o termo de busca;
* A comparação do termo de busca com cada linha considera a diferença entre maiúsculas e minúsculas (case sensitive). Assim, ao buscar por **foo**, linhas contendo **Foo** não resultarão em uma combinação;
* Linhas podem ser arbitrariamente longas (ou seja, você pode encontrar caracteres muitos caracteres antes de encontrar uma nova linha, \\n). O **wgrep** deve funcionar como esperado mesmo na presença de linhas muito longas;
* Se nenhum argumento de linha de comando for passado para o **wgrep**, ele deve imprimir a mensagem "wgrep: searchterm [file ...]", e em seguida, deve finalizar;
* Se o **wgrep** encontrar um arquivo que não puder ser aberto, ele deve imprimir a mensagem "wgrep: cannot open file";
* Se um termo de busca for fornecido como argumento de linha de comando, mas nenhum arquivo for especificado, o **wgrep** irá ler os dados a partir do *standard input*;
* Por simplicidade, se o usuário passar uma string vazia como o argumento do termo de busca, **wgrep** pode considerar que todas as linhas têm uma correspondência ou nenhuma delas.
* É necessario que todos os arquivos passados por linha de comando estejam no mesmo diretório que o programa **wgrep**;
* O programa **wgrep** precisa ser obrigatoriamente executado no diretório que armazena o executavel **wgrep**, para entrar no diretório utilize o comando **cd** seguido pelo PATH (o PATH pode ser copiado na pasta do arquivo)
```
prompt> cd /home/"usuario"/"PATH"
...
```
