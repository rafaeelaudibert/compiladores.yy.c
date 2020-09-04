# UFRGS – Informática - INF01147 - Compiladores  - 2020/1

## Turma A - Prof. Marcelo Johann

### Trabalho Prático - Especificação da Etapa 1: Análise Léxica e Inicialização de Tabela de Símbolos

#### Resumo

O trabalho consiste na implementação de um compilador funcional. Esta primeira etapa do trabalho consiste em fazer um analisador léxico utilizando a ferramenta de geração de reconhecedores `lex` (ou `flex`) e inicializar uma tabela de símbolos encontrados.

#### Funcionalidades necessárias

A sua análise léxica deve fazer as seguintes tarefas:

a.reconhecer as expressões regulares que descrevem cada tipo de lexema;
b.classificar os lexemas reconhecidos em tokens retornando as constantes definidas no arquivo `tokens.h` fornecido ou códigos ascii para caracteres simples;
c.incluir os identificadores e os literais (inteiros, caracteres e strings) em uma tabela de símbolos implementada com estrutura hash;
d.controlar o número de linha do arquivo fonte, e fornecer uma função declarada como `int getLineNumber(void)` a ser usada nos testes e pela futura análise sintática;
e.ignorar comentários de única linha e múltiplas linhas;
f.informar erro léxico ao encontrar caracteres inválidos na entrada, retornando o token de erro;
g.definir e atualizar uma variável global e uma função `int isRunning(void)`, que mantém e retorna valor `true` (diferente de `0`) durante a análise e muda para `false` (igual a `0`) ao encontrar a marca de fim de arquivo;

#### Descrição dos tokens

Existem tokens que correspondem a caracteres particulares, como vírgula, ponto-e-vírgula, parênteses, para os quais  é mais conveniente usar seu próprio código ascii, convertido para inteiro, como valor de retorno que os identifica. Para os tokens compostos, como palavras reservadas e identificadores, cria-se uma constante  (`#define` em C ANSI) com um código maior do que `255` para representá-los.

Os tokens representam algumas categorias diferentes, como palavras reservadas, operadores de mais de um caractere e literais, e as constantes definidas no código do trabalho são precedidas por um prefixo para melhor identificar sua função, separando-as de outras constantes que serão usadas no compilador.

#### Palavras reservadas

As palavras reservadas da linguagem neste semestre são: `char`, `int`, `float`, `bool`, `if`, `then`, `else`, `while`,  `loop`, `read`, `print` e `return`. Para cada uma deve ser retornado o token correspondente.

#### Caracteres especiais

Os caracteres simples especiais empregados pela linguagem são listados abaixo  (estão separados apenas por espaços), e devem ser retornados com o próprio código ascii convertido para inteiro. Você pode fazer isso em uma  única regra léxica que retorna `yytext[0]`. São eles:

    , ; : ( ) [ ] { } + - * / < > = | ^ ~ & $ #

#### Operadores Compostos

A linguagem possui, além dos operadores representados por alguns dos caracteres acima, operadores compostos, que necessitam mais de um caractere (na verdade, somente dois) para serem representados no código fonte. São somente quatro operadores relacionais, conforme a tabela abaixo:

| Source Representation | Returned Token |
|:---------------------:|:--------------:|
|           <=          |   OPERATOR_LE  |
|           >=          |   OPERATOR_GE  |
|           ==          |   OPERATOR_EQ  |
|           !=          |  OPERATOR_DIF  |

#### Identificadores

Os identificadores da linguagem são usados para designar variáveis, vetores e nomes de funções, são formados por uma sequência de um ou mais caracteres alfanuméricos, incluindo `underline` (`_`), arroba, ou  "at"  (`@`), e permitindo dígitos  (`0`-`9`) a partir da segunda posição.

#### Literais

Literais são formas de descrever constantes no código fonte. A linguagem desse semestre usará o sistema hexadecimal, e os literais inteiros são representados como repetições de um ou mais dígitos decimais ou letras maiúsculas de `A` a `F`, ou seja `{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F}`. Literais reais são formados por duas sequências de um ou mais dígitos hexadecimais, contendo um ponto decimal entre elas, sem espaços, sendo que a primeira sequência pode ser vazia. Ou seja, `.0`  é um literal real, mas  `0.` não  é um literal real válido. Literais do tipo caracter são representados por um  único caractere entre aspas simples, como por exemplo: `'a'`, `'X'`, `'-'`. Literais do tipo string são qualquer sequência de caracteres entre aspas duplas, como por exemplo `"meu nome"` ou `"Mensagem!"`, e servem apenas para imprimir mensagens com o comando `print`. Strings consecutivas não podem ser consideradas como apenas uma, o que significa que o caractere de aspas duplas não pode fazer parte de uma string. Para incluir o caracter de aspas duplas e final de linha, devem ser usadas sequências de escape, como  `"\""` e  `"\n"`. A linguagem inclui também os literais booleanos constantes, `TRUE` e `FALSE`, escritos assim, em caracteres maiúsculos.

#### Comentários

Comentários de uma  única linha começam em qualquer ponto com a sequência  `//` e terminam na próxima marca de final de linha, representada pelo caractere `\n`. Comentários de múltiplas linhas iniciam pela sequência  `/*` e terminam pela sequência  `*/`, sendo que podem conter quaisquer caracteres, que serão todos ignorados, incluindo uma ou mais quebras de linha, as quais, entretanto, devem ser contabilizadas para controle do número de linha.

### Controle e organização do seu código fonte

Você deve manter o arquivo `tokens.h` intacto, e separar a sua função main em um arquivo especial chamado `main.c`, já que a função `main` não pode estar contida no código de `scanner.l`. Isso é necessário para facilitar a automação dos testes, que utilizará uma função main especial escrita pelo professor, substituindo a que você escreveu para teste e desenvolvimento. Você deve usar essa estrutura de organização, manter os nomes `tokens.h` e `scanner.l`. Instruções mais detalhadas sobre o formato de submissão do trabalho e cuidados com detalhes específicos estão em outro documento separado.
