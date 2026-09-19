#!/usr/bin/env bash
set -euo pipefail

# ---------------------------------------------------------------------------
# run.sh — executa os tratamentos definidos em treatments.txt e gera o CSV
#
# Uso: ./run.sh [--runs N] [--config ARQ] [--out ARQ] [--treatments ARQ]
# ---------------------------------------------------------------------------

RUNS=30
CONFIG="example.ini"
OUT="results.csv"
TREATMENTS="treatments.txt"
GA="./agt"

while [[ $# -gt 0 ]]; do
  case $1 in
  --runs)
    RUNS=$2
    shift 2
    ;;
  --config)
    CONFIG=$2
    shift 2
    ;;
  --out)
    OUT=$2
    shift 2
    ;;
  --treatments)
    TREATMENTS=$2
    shift 2
    ;;
  *)
    echo "Argumento desconhecido: $1" >&2
    exit 1
    ;;
  esac
done

[[ ! -x "$GA" ]] && {
  echo "Erro: binario '$GA' nao encontrado. Execute 'make' primeiro." >&2
  exit 1
}
[[ ! -f "$CONFIG" ]] && {
  echo "Erro: '$CONFIG' nao encontrado." >&2
  exit 1
}
[[ ! -f "$TREATMENTS" ]] && {
  echo "Erro: '$TREATMENTS' nao encontrado." >&2
  exit 1
}

echo "selection,pop_size,generations,best_fitness" >"$OUT"

n=0
while IFS= read -r line || [[ -n "$line" ]]; do
  # Skip blank lines and comment lines
  [[ "$line" =~ ^[[:space:]]*(#|$) ]] && continue

  n=$((n + 1))
  echo "Tratamento $n: $line"

  for i in $(seq 1 "$RUNS"); do
    # shellcheck disable=SC2086  # word-split of $line is intentional
    "$GA" -F "$CONFIG" $line -o "$OUT" >/dev/null
  done

  echo "  $RUNS rodadas concluidas"
done <"$TREATMENTS"

echo ""
echo "Concluido: $n tratamento(s) x $RUNS rodadas -> '$OUT'"
