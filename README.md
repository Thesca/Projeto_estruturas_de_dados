# Estrutura de dados avançadas
O IMDB disponibiliza periodicamente arquivos com informações sobre a indústria cinematográfica estruturados em formato TSV (tab separated values). Dentre esses arquivos estão o name.basics.tsv.gz e o title.basics.tsv.gz (que podem ser encontrados em datasets.imdbws.com). Segue um trecho do primeiro:

nconst primaryName birthYear deathYear primaryProfession knownForTitles

nm0000001 Fred Astaire 1899 1987 soundtrack,actor,miscellaneous tt0072308,tt0045537,tt0050419,tt0053137

nm0000002 Lauren Bacall 1924 2014 actress,soundtrack tt0037382,tt0117057,tt0038355,tt0075213

Embora aparentem ser espaços, os valores de cada linha estão separados pelo caracter tab (\t). A primeira linha é o cabeçalho. A partir da segunda linha, o valor da primeira coluna (nconst) contem o ID de um artista e o valor da última coluna (knownForTitles) contem uma sequência separada por vírgulas dos IDs dos filmes de destaque deste artista. O segundo arquivo está organizado de maneira análoga, contendo os IDs, nomes e outras informações a respeito dos filmes.

O objetivo deste trabalho é implementar em linguagem C (utilizando apenas os cabeçalhos padrão) um programa que identifique quais filmes possuem artistas em comum, utilizando para tanto as estruturas de dados vistas no curso até então. Mais especificamente, o programa deve gerar um grafo (representado como uma lista de adjacências) tal que os vértices sejam correspondam aos filmes e as arestas correspondam a filmes que tiveram artistas em comum no elenco. Segue um delineamento do processo:
