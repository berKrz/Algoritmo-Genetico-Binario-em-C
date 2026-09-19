# Análise comparativa de cenários do Algoritmo Genético Binário
# Teste de Friedman + pós-teste de Nemenyi par a par

library(PMCMRplus)

data <- read.csv("results.csv")

data$label <- paste0(data$selection, "/", data$pop_size, "/", data$generations)

scenarios <- unique(data$label)
n_runs    <- sum(data$label == scenarios[1])
mat       <- matrix(NA, nrow = n_runs, ncol = length(scenarios),
                    dimnames = list(NULL, scenarios))

for (s in scenarios) {
  mat[, s] <- data$best_fitness[data$label == s]
}

# Friedman test
ft <- friedman.test(mat)
print(ft)

# Nemenyi post-hoc
y      <- as.vector(mat)
groups <- factor(rep(colnames(mat), each = nrow(mat)))
blocks <- factor(rep(seq_len(nrow(mat)), times = ncol(mat)))
ph     <- frdAllPairsNemenyiTest(y, groups = groups, blocks = blocks)
print(ph)

# Boxplot saved to PNG for use in the document
png("boxplot.png", width = 800, height = 500)
boxplot(mat,
        main = "Distribuição do Fitness por Cenário",
        xlab = "Cenário",
        ylab = "Fitness (última geração)",
        col  = "steelblue")
dev.off()

cat("\nBoxplot salvo em boxplot.png\n")
