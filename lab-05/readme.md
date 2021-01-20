# Variávies de condição e mutex

Para executar a aplicação, basta clonar o repositório, ir até a pasta lab-04 e executar em um shell unix

> $ make

A execução ira retornar uma saída semelhante a

```
gcc lab.c -o main -Wall -lpthread
1 - tudo bem?
2 - bom dia!
3 - até mais!
4 - boa tarde!
Fim
```

Onde primeiro imprimimos o conjunto [1,2] e depois o conjunto [3,4]. Os itens de cada conjunto podem trocar de lugar entre si.
