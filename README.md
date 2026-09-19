# Algoritmo Genético Binário em C

Implementação de um algoritmo genético de representação binária em C, com suporte a múltiplas funções de fitness, métodos de seleção, cruzamento, mutação e mapeamento de domínio contínuo. Totalmente configurável via argumentos de linha de comando ou arquivo INI. Inclui pipeline de experimento estatístico para comparação de cenários via Teste de Friedman.

---

## Funcionalidades

- Seleção por roleta ou torneio (com tamanho de torneio configurável)
- Cruzamento de ponto único com taxa de corte configurável
- Mutação por inversão de bit com taxa configurável
- Mapeamento de domínio contínuo via decodificação linear
- Minimização ou maximização da função de fitness
- Configuração por arquivo INI, argumentos CLI, ou ambos (CLI tem precedência)
- Execução interativa passo a passo (`--interactive`)
- Exportação do resultado de cada execução para CSV (`--out`)
- Exibição do melhor indivíduo da última geração e do melhor geral ao fim da execução

---

## Compilação

```bash
make
```

---

## Uso

```
Uso: ./ga [OPCOES]

Opções:
  -F, --config            ARQ    Caminho para arquivo de configuracao INI
  -p, --pop-size          INT    Tamanho da populacao (min: 2)            [padrao: 15]
  -i, --ind-size          INT    Tamanho do individuo (min: 2)            [padrao: 22]
  -g, --generations       INT    Numero de geracoes (min: 1)              [padrao: 30]
  -k, --tournament-size   INT    Individuos por torneio (min: 2)          [padrao: 2]
  -c, --cut-point         FLOAT  Razao do ponto de corte (0.0,1.0)        [padrao: 0.6]
  -r, --mutation-rate     FLOAT  Taxa de mutacao [0.0,1.0)                [padrao: 0.01]
  -d, --direction         STR    minimize | maximize                      [padrao: minimize]
  -f, --fitness           STR    quadratic                                [padrao: quadratic]
  -s, --selection         STR    roulette | tournament                    [padrao: roulette]
  -x, --crossover         STR    single-point                             [padrao: single-point]
  -m, --domain-min        FLOAT  Valor minimo do dominio                  [padrao: 0.0]
  -M, --domain-max        FLOAT  Valor maximo do dominio                  [padrao: 2^ind-size - 1]
  -o, --out               ARQ    Arquivo CSV para registro do resultado
  -I, --interactive              Executa passo a passo com pausas
  -h, --help                     Exibe esta mensagem e encerra
```

**Exemplo:** minimizar x² no domínio [-1, 1] com seleção por torneio e execução interativa:

```bash
./ga --direction minimize --domain-min -1.0 --domain-max 1.0 --selection tournament --interactive
```

---

## Arquivo de configuração

Todos os parâmetros podem ser definidos em um arquivo INI e carregados com `-F`. Argumentos passados diretamente na linha de comando sempre sobrescrevem os valores do arquivo.

```ini
# Exemplo de configuracao
pop_size        = 20
ind_size        = 22
generations     = 50
direction       = minimize
selection       = tournament
tournament_size = 3
mutation_rate   = 0.01
domain_min      = -1.0
domain_max      = 1.0
```

```bash
./ga --config run.ini
```

Chaves podem ser escritas com hífen ou sublinhado (`pop-size` e `pop_size` são equivalentes). Linhas iniciadas com `#` são ignoradas.

---

## Ambiente de desenvolvimento

O repositório inclui um flake Nix com todas as dependências do projeto — toolchain C e pacotes R. Para entrar no ambiente:

```bash
nix develop
```

Se [direnv](https://direnv.net) estiver instalado, o ambiente é ativado automaticamente ao entrar no diretório:

```bash
direnv allow
```

O uso de Nix e direnv é opcional. Para compilar e executar o binário basta ter `gcc` e `make` disponíveis. Para rodar o script de análise, basta ter R com o pacote `PMCMRplus` instalado.

---

## Experimento Estatístico

O pipeline de experimento permite comparar cenários de configuração do AG de forma reproduzível, gerando dados para análise via Teste de Friedman.

### Definindo os tratamentos

Os cenários a comparar são definidos em `treatments.txt`, um por linha, no formato de argumentos CLI. Linhas em branco e linhas iniciadas com `#` são ignoradas.

```
# Exemplo
-s tournament -p 15  -g 30
-s roulette   -p 15  -g 30
-s tournament -p 150 -g 30
```

Qualquer parâmetro suportado pelo binário pode ser usado. Os valores não especificados são herdados do arquivo de configuração base passado ao script.

### Executando as rodadas

```bash
chmod +x run.sh
./run.sh [--runs N] [--config ARQ] [--out ARQ] [--treatments ARQ]
```

| Opção | Padrão |
|---|---|
| `--runs` | 30 |
| `--config` | `example.ini` |
| `--out` | `results.csv` |
| `--treatments` | `treatments.txt` |

O script executa cada tratamento `--runs` vezes, registrando o fitness do melhor indivíduo da última geração em um CSV. O cabeçalho do arquivo é escrito automaticamente.

### Analisando os resultados

Com `results.csv` gerado na raiz do projeto:

```bash
Rscript analysis.R
```

O script imprime no console o resultado do Teste de Friedman e o pós-teste de Nemenyi par a par, e salva o boxplot comparativo em `boxplot.png`.
