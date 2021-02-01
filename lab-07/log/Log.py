class Logger:    
    def __init__(self, buffer_size):
        self.buffer_size = buffer_size

    def BlockedConsumer(self, remaining_capacity):
        '''
        Verifica se a thread consumidora deveria se bloquar.
        A condição para bloqueio é que o buffer esteja vazio, 
        ou seja, não há elementos para consumir
        '''
        if remaining_capacity == self.buffer_size :
            print("ERRO: Consumidor bloqueado com elementos no buffer ")

    def BlockedProducer(self, remaining_capacity):       
        '''
        Verifica se a thread produtora deveria se bloquear.
        A condiçao para bloqueio é que o buffer esteja cheio,
        ou seja, não é possível alocar mais elementos no buffer
        '''
        if remaining_capacity > 0:
            print("ERRO: Produtor bloqueado, mas há espaço disponível no buffer")

    def ProcessConsumer(self, remaining_capacity):
        '''
        Verifica se o consegue remover um elemento do buffer
        Condições: 
            - Buffer precisa possuir ao menos um elemento
        '''
        if remaining_capacity == self.buffer_size:
            print("ERRO: Consumidor não tem o que consumir")
        

    def ProcessProducer(self, remaining_capacity):
        '''
        Verifica se o consegue consumir elemento ao buffer
        Condições: 
            - Buffer precisa ao menos uma posição livre
        '''
        if remaining_capacity == 0:
            print("ERRO: Não é possível alocar novo elemento, buffer está cheio")

