## wcat
O programa **wcat** é bastante simples. Ele recebe como argumento um ou mais arquivos, os quais são lidos e seu conteúdo é exibido na tela. 
Uma utilização típica é mostrada abaixo, onde o usuário deseja exibir o conteúdo do arquivo teste.txt: 

```
prompt> ./wcat teste.txt
<texto do arquivo teste.txt>
...
```

Como mostrado, **wcat** lê o arquivo **teste.txt** e imprime seu conteúdo na tela. O "**./**" antes de **wcat** é uma característica do UNIX (o "**.**" indica o diretório atual e a "**/**" separa do nome do programa). 
##
**Detalhes**

* O programa **wcat** é implementado de forma a permitir um ou mais arquivos como argumento. Se dois ou mais arquivos forem fornecidos, **wcat** deve exibir o conteúdo de cada um deles, um por um;
* Se nenhum arquivo for especificado na linha de comando, **wcat** deverá simplesmente encerrar. Note que esse comportamento é ligeiramente diferente daquele apresentado pelo programa **cat** do UNIX;
* Se a chamada para **fopen()** falhar, **wcat** deverá imprimir exatamnete a seguinte mensagem, seguida por uma nova linha: "wcat: cannot open file". Se múltiplos arquivos forem especificados na linha de comando, os arquivos devem ser impressos em ordem até a última linha do último arquivo ou até que uma falha de abertura de arquivo ocorra (caso onde **wcat** exibe a mensagem de erro e encerra, como explicado anteriormente).
* É necessario que todos os arquivos passados por linha de comando estejam no mesmo diretório que o programa **wcat**;
* O programa **wcat** precisa ser obrigatoriamente executado no diretório que armazena o executavel **wcat**, para entrar no diretório utilize o comando **cd** seguido pelo PATH (o PATH pode ser copiado na pasta do arquivo)
```
prompt> cd /home/"usuario"/"PATH"
...
```
