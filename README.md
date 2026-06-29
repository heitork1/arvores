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
```

Depois, execute o script:
```bash
py gerar_graficos.py
```

Após a execução, será criada uma pasta chamada graficos, contendo os arquivos:

- grafico_adicao.png
- grafico_remocao.png

## Resultados

Os gráficos gerados apresentam o esforço computacional médio das operações de adição e remoção.

Cada gráfico possui cinco linhas, representando:

- AVL
- Rubro-Negra
- Árvore B de ordem 1, 5 e 10


Foi utilizada escala logarítmica no eixo Y para facilitar a visualização das cinco estruturas no mesmo gráfico, pois os valores obtidos apresentaram diferenças grandes de magnitude.


## Observação sobre as métricas

É importante destacar que as métricas utilizadas não representam exatamente o mesmo tipo de contagem para todas as estruturas.

Nas árvores AVL e rubro-negra, os valores estão mais relacionados às comparações e aos ajustes realizados durante as operações. Já na árvore B, foram consideradas operações estruturais específicas, como splits, fusões e empréstimos.

Por esse motivo, os resultados devem ser interpretados como uma análise experimental das implementações utilizadas no trabalho, e não como uma comparação absoluta de desempenho entre todas as estruturas.

## Conclusão

A partir dos resultados obtidos, foi possível observar que o esforço computacional aumenta conforme cresce o tamanho do conjunto de dados. Esse comportamento apareceu tanto na operação de adição quanto na operação de remoção. Também foi possível perceber que as árvores B de maior ordem apresentaram menor quantidade de operações estruturais, principalmente por conseguirem armazenar mais chaves em cada nó. Já as árvores AVL e rubro-negra apresentaram valores maiores nas métricas utilizadas, o que pode estar relacionado à forma como o balanceamento e os contadores foram implementados.

De forma geral, o trabalho permitiu visualizar na prática como diferentes árvores balanceadas se comportam durante operações de inserção e remoção.


## Integrantes

- Guilherme Gamaliel
- Heitor Henrique Klein
- João Vitor Rulff
- Otávio Osni
