# Algoritmo Genetico Binario em C

Implementação de um algoritmo genético de representacao binária em C, com suporte a múltiplas funções de fitness, métodos de seleção, cruzamento, mutação e mapeamento de domínio contínuo. Totalmente configurável via argumentos de linha de comando ou arquivo INI.

---

## Funcionalidades

- Seleção por roleta ou torneio (com tamanho de torneio configurável)
- Cruzamento de ponto único com taxa de corte configurável
- Mutação por inversão de bit com taxa configurável
- Mapeamento de domínio contínuo via decodificação linear
- Minimização ou maximização da função de fitness
- Configuração por arquivo INI, argumentos CLI, ou ambos (CLI tem precedência)
- Execução interativa passo a passo (`--interactive`)
- Exibição do melhor indíviduo da última geração e do melhor geral ao fim da execução

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
pop_size      = 20
ind_size      = 22
generations   = 50
direction     = minimize
selection     = tournament
tournament_size = 3
mutation_rate = 0.01
domain_min    = -1.0
domain_max    = 1.0
```

```bash
./ga --config run.ini
```

Chaves podem ser escritas com hífen ou sublinhado (`pop-size` e `pop_size` são equivalentes). Linhas iniciadas com `#` sao ignoradas.
