library(ggplot2)
x = read.csv("/Users/derekjones2025/OneDrive/CS463/finalproject/unixABC/unixABC/abcalgodata.csv")


qplot(x$Colony.Size, x$Error..distance.from.zero) + geom_line(linetype = "dotted")+ geom_point(size = 3, colour ="darkred") + labs(x = "Colony Size", y =" Distance from optimal solution") + ggtitle("Colony Size vs Solution Quality")

ggsave("abcOutput.jpg")