# Trabalho Árvores AVL, Rubro-Negra e B

Este repositório contém os códigos utilizados no trabalho de Estruturas de Dados II, com o objetivo de analisar o esforço computacional das operações de adição e remoção em diferentes tipos de árvores balanceadas.

As estruturas analisadas foram:

- Árvore AVL
- Árvore Rubro-Negra
- Árvore B de ordem 1
- Árvore B de ordem 5
- Árvore B de ordem 10

## Objetivo

O objetivo do trabalho foi comparar, de forma experimental, o comportamento das operações de inserção e remoção de nós, considerando também os ajustes de balanceamento realizados por cada estrutura.

Para isso, foram gerados conjuntos de chaves aleatórias com tamanhos de até 10000 elementos. Os testes foram executados utilizando 10 amostras, e os resultados finais foram calculados a partir da média dos valores obtidos.

## Metodologia

Os experimentos foram realizados a partir de códigos implementados em linguagem C. Para cada tamanho de conjunto de dados, foram executadas operações de adição e remoção nas estruturas AVL, rubro-negra e árvore B.

No caso da árvore B, foram testadas três ordens diferentes: 1, 5 e 10. Na operação de adição, foi considerada a quantidade de divisões de nós, chamadas de splits. Já na operação de remoção, foram consideradas as fusões e os empréstimos entre nós, pois essas operações fazem parte dos ajustes necessários para manter a estrutura da árvore.

Os resultados foram salvos em um arquivo CSV e, posteriormente, utilizados em um script Python para gerar os gráficos de linha.

## Arquivos do projeto

- `trabalho_final.c`: arquivo principal responsável pela execução dos testes.
- `resultados_arvores.csv`: arquivo com os resultados médios obtidos nos experimentos.
- `gerar_graficos.py`: script em Python utilizado para gerar os gráficos a partir do CSV.
- `graficos/`: pasta contendo os gráficos gerados.
- `arvores/`: pasta com implementações das árvores.
- `includes/`: pasta com os arquivos de cabeçalho.
- `utilitarios/`: pasta com funções auxiliares utilizadas no projeto.

## Geração dos gráficos

Para gerar os gráficos, é necessário ter o Python instalado e instalar as bibliotecas `pandas` e `matplotlib`.

No terminal, dentro da pasta do projeto, execute:

```bash
py -m pip install pandas matplotlib
