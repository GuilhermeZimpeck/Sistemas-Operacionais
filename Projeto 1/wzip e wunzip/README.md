## wzip e wunzip

A próxima ferramenta é um par de programas:

* **wzip**: realiza a compressão de um ou mais arquivos especificados como argumentos de linha de comando;
* **wunzip**: realiza a descompressão de um ou mais arquivos especificados como argumentos de linha de comando e que foram comprimidos pelo programa **wzip**. 

O tipo de compressão utilizado aqui é uma forma simplificada de um algoritmo de compressão chamado de *run-Length encoding* (*RLE*). O funcionamento do *RLE* é bastante simples: quando uma sequência de **n** caracteres iguais é encontrada em uma linha de um arquivo, essa sequência é escrita na saída padrão como o valor de **n** seguido de uma instância do caracter repetido.

Assim, se temos um arquivo com o seguinte conteúdo:
```
aaaaaaaaaabbbb
```
a ferramenta irá gerar o seguinte resultado:
```
10a4b
```

Entretanto, note que o formato exato do arquivo comprimido é bastante importante. Para cada sequência de caracteres repetidos encontrada, o **wzip** irá escrever um inteiro de 4 bytes (int) em sua representação binária, seguido do valor ASCII (1 byte) do caracter repetido. Ou seja, o arquivo comprimido será composto por **k** entradas de 5 bytes cada, sendo cada entrada contendo um inteiro de 4 bytes (representando o número de repetições do caractere) e 1 byte representando o caracter em si. 

Observe que o uso típico do programa **wzip** irá utilizar o redirecionamento de saída para gerar um arquivo de saída comprimido. Por exemplo, para comprimir o arquivo **file.txt** de forma a obter o arquivo comprimido **file.z**, você deve executar o programa **wzip** da seguinte forma:

```
prompt> ./wzip file.txt > file.z
```

O sinal "maior que" é utilizado para redirecionar os dados escritos na saída parão pelo programa da esquerda para o arquivo especificado do lado direito. Caso esse redirecionamento não seja feito, os dados gerados por **wzip** serão impressos na tela. Nós veremos como os redirecionamentos são implementados mais a frente no curso.

O programa **wunzip** simplesmente executa o processo contrário do **wzip**. Ele recebe como argumento um arquivo comprimido pelo **wzip**, faz a descompressão e imprime o resultado também na saída padrão. Por exemplo, para ver o conteúdo original do arquivo **file.txt** a partir do arquivo **file.z**, execute o seguinte comando:

```
prompt> ./wunzip file.z
```
##
**Detalhes**

* Para a correta invocação dos programas **wzip** e **wunzip**, é necessário passar um ou mais arquivos via linha de comando para serem comprimidos;
* Se nenhum arquivo for fornecido, o programa deve imprimir a mensagem  "wzip: file1 [file2 ...]" ou "wunzip: file1 [file2 ...]";
* Note que se múltiplos arquivos forem passados como argumentos para **wzip**, eles serão comprimidos em uma única saída. Por esse motivo, quando o arquivo comprimido por descomprimido por **wunzip**, o resultado será fluxo único de caracteres (ou seja, a informação de que originalmente múltiplos arquivos fonte deram origem ao arquivo comprimido será perdida). O mesmo pode ser dito sobre o **wunzip**;
* É necessario que todos os arquivos passados por linha de comando estejam no mesmo diretório que o programa **wzip** e **wunzip**;
* O programa **wzip** e **wunzip** precisa ser obrigatoriamente executado no diretório que armazena os executaveis, para entrar no diretório utilize o comando **cd** seguido pelo PATH (o PATH pode ser copiado na pasta do arquivo)
```
prompt> cd /home/"usuario"/"PATH"
...
```
