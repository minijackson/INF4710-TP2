library(ggplot2)

rates <- read.csv("data/rates.csv")

pdf("without_subsampling.pdf", height=6)

ggplot(rates, aes(x=Qualité, y=Taux.de.compression, color=Image)) +
    ylab("Taux de compression") +
    geom_line() + geom_point()

dev.off()
